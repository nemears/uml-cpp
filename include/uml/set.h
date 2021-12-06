#ifndef SPECIAL_SEQUENCE_H
#define SPECIAL_SEQUENCE_H

#include "element.h"
#include "umlManager.h"
#include <iostream>

namespace UML {

    namespace {
        const ID placeholderID = ID::fromString("&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
    }

    template <class T, class U> class Set;
    template <class T, class U> class OrderedSet;

    class ID_doesNotExistException2 : public std::exception {
        private:
            std::string m_msg;
        public:
            ID_doesNotExistException2(ID id) : m_msg(id.string() + " was not found within the sequence!"){};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };

    class DuplicateElementInSetException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "Duplicate element added to set!";
            };
    };

    class ReadOnlySetException : public std::exception {
        friend class Element;
        private:
            std::string m_msg;

        public:
            ReadOnlySetException(std::string elID) : m_msg("Tried to mutate read only set for element uuid: " + elID) {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };

    class AbstractOppositeFunctor {
        public:
            virtual void operator()(Element& el) const = 0;
            virtual void operator()(Element& el, bool remove) const = 0;
    };
    
    template <class T = Element, class U = Element> class OppositeFunctor : public AbstractOppositeFunctor {
        private:
            U* m_el;
        public:
            Set<U,T>& (T::*m_signature)() = 0;
            OppositeFunctor(U* el) : m_el(el) {};
            void operator()(Element& el) const override {
                (el.as<T>().*m_signature)().nonOppositeAdd(*m_el);
            };
            void operator()(Element& el, bool remove) const override {
                if (remove) {
                    (el.as<T>().*m_signature)().nonOppositeRemove(m_el->getID());
                } else {
                    (el.as<T>().*m_signature)().nonOppositeAdd(*m_el);
                }
            }
    };

    class AbstractSet {
        template <class T, class U> friend class Set;
        template <class T, class U> friend class OrderedSet;
        protected:
            size_t m_size = 0;
            int m_upper = 0; // this effectively lets us determine the type of the set (1 = singleton, 0 = set, -1 = orderedSet)
            struct SetNode {
                SetNode(Element* el) : m_el(el) {
                    m_id = el->getID();
                };
                SetNode(){};
                ID m_id;
                Element* m_el = 0;
                std::string m_name;
                SetNode* m_parent = 0;
                SetNode* m_left = 0;
                SetNode* m_right = 0;
                size_t m_guard = 0;
            };
            SetNode* m_root = 0;
            size_t m_guard = 0;
            virtual void place(SetNode* node, SetNode* parent) = 0;
            virtual SetNode* search(ID id, SetNode* node) = 0;
            void setName(SetNode* node);
            void instantiateSetNode(SetNode* node);
    };

    class SetFunctor {
        protected:
            Element& m_el;
        public:
            SetFunctor(Element* el) : m_el(*el) {};
            virtual void operator()(Element& el) const = 0;
    };

    template <class T> struct SetIterator;
    template <class T> struct ID_Set;

    class Namespace;
    class NamedElement;
    class Package;
    class PackageableElement;
    class Relationship;
    class DirectedRelationship;
    class PackageMerge;
    class Dependency;
    class Classifier;
    class TypedElement;
    class Feature;
    class Property;
    class StructuredClassifier;
    class Generalization;
    class Class;
    class DataType;
    class InstanceSpecification;
    class Slot;
    class BehavioralFeature;
    class Behavior;
    class MultiplicityElement;
    class Operation;
    class OpaqueBehavior;
    class Parameter;
    class BehavioredClassifier;
    class Association;
    class Enumeration;
    class EnumerationLiteral;
    class Comment;

    /**
     * This container is based around a weighted binary search tree
     **/
    template <class T = Element, class U = Element> class Set : public AbstractSet {
        
        template <class V, class W> friend class Set;
        template <class V, class W> friend class OppositeFunctor;
        friend class Set<U>;
        friend struct SetIterator<U>;
        friend struct ID_Set<T>;
        friend class Element;
        friend class Namespace;
        friend class NamedElement;
        friend class Package;
        friend class PackageableElement;
        friend class Relationship;
        friend class DirectedRelationship;
        friend class PackageMerge;
        friend class Dependency;
        friend class Classifier;
        friend class TypedElement;
        friend class Feature;
        friend class Property;
        friend class StructuredClassifier;
        friend class Generalization;
        friend class Class;
        friend class DataType;
        friend class InstanceSpecification;
        friend class Slot;
        friend class BehavioralFeature;
        friend class Behavior;
        friend class MultiplicityElement;
        friend class Operation;
        friend class OpaqueBehavior;
        friend class Parameter;
        friend class BehavioredClassifier;
        friend class Association;
        friend class Enumeration;
        friend class EnumerationLiteral;
        friend class Comment;

        protected:
            std::vector<AbstractSet*> m_superSets;
            std::vector<AbstractSet*> m_subSets;
            std::vector<AbstractSet*> m_redefines;
            bool m_rootRedefinedSet = true;
            AbstractOppositeFunctor* m_oppositeFunctor = 0;
            bool m_ownsOppositeFunctor = false;
            Element* m_el = 0;
            Set<T,U>& (U::*m_signature)() = 0;
            bool m_readOnly = false;
            std::unordered_set<SetFunctor*> m_addFunctors;
            std::unordered_set<SetFunctor*> m_removeFunctors;

            void place(SetNode* node, SetNode* parent) override {
                if (node->m_id == parent->m_id) {
                    delete node;
                    throw DuplicateElementInSetException();
                }
                if (parent->m_left) {
                    // prefer placement to left
                    if (parent->m_right) {
                        // both children are populated, determine where to place
                        if (node->m_id > parent->m_right->m_id && parent->m_left->m_guard <= m_guard) {
                            // place to left if greater than right
                            place(node, parent->m_left);
                        } else if (parent->m_right->m_guard <= m_guard) {
                            // place to right if less than right
                            place(node, parent->m_right);
                        } else {
                            if (node->m_id > parent->m_right->m_id) {
                                SetNode* temp = parent->m_left;
                                parent->m_left = node;
                                place(parent->m_right, temp);
                                parent->m_right = temp;
                                node->m_parent = parent;
                            }
                        }
                    } else {
                        if (node->m_id > parent->m_left->m_id) {
                            // if node greater than left, swap left to right
                            parent->m_right = parent->m_left;
                            parent->m_left = node;
                        } else {
                            parent->m_right = node;
                        }
                        node->m_parent = parent;
                    }
                } else {
                    parent->m_left = node;
                    node->m_parent = parent;
                }
            };
            SetNode* search(ID id, SetNode* node) override {
                if (node->m_id == id) {
                    // found match
                    return node;
                } else {
                    if (node->m_right) {
                        // if there is a right there is both children filled out
                        if (id > node->m_right->m_id) {
                            // if it is greater than the right it has to be within the left
                            return search(id, node->m_left);
                        } else {
                            // if not it must be in the right
                            return search(id, node->m_right);
                        }
                    } else {
                        if (node->m_left) {
                            // if left exist search it
                            return search(id, node->m_left);
                        } else {
                            // both are null, our search is unfruitful :(
                            return 0;
                        }
                    }
                }
            }
            SetNode* search(std::string name, SetNode* node) {
                if (node->m_name == name) {
                    return node;
                } else {
                    SetNode* temp;
                    if (node->m_right) {
                        if((temp = search(name, node->m_right)) != 0) {
                            return temp;
                        }
                    }
                    if (node->m_left) {
                        if ((temp = search(name, node->m_left)) != 0) {
                            return temp;
                        }
                    }
                }
                return 0;
            };
            void add(SetNode* node) {
                node->m_guard = m_guard;
                if (!m_root) {
                    m_root = node;
                    // handle redefines
                    for (auto& redefined : m_redefines) {
                        if (redefined->m_root) {
                            if (redefined->m_root->m_id != m_root->m_id) {
                                m_root = redefined->m_root;
                                place(node, m_root);
                            }
                        } else {
                            redefined->m_root = node;
                        }
                    }
                    // handle subsets
                    for (auto& subsetOf : m_superSets) {
                        if (subsetOf->m_root) {
                            // determine if we need a placeholder to keep subsets separate
                            bool createPlaceholder = false;
                            int placeHolderGuard = 0;
                            for (auto& set : static_cast<Set*>(subsetOf)->m_subSets) {
                                if (set != this) {
                                    Set* rSet = static_cast<Set*>(set);
                                    if (std::find(rSet->m_subSets.begin(), rSet->m_subSets.end(), this) == rSet->m_subSets.end()) {
                                        createPlaceholder = true;
                                        placeHolderGuard = subsetOf->m_guard;
                                        break;
                                    }
                                }
                            }
                            if (createPlaceholder) {
                                // create a "placeholder" node to balance subsets so they dont overlap in even tree
                                SetNode* temp = subsetOf->m_root;
                                subsetOf->m_root = new SetNode();
                                subsetOf->m_root->m_id = placeholderID;
                                subsetOf->m_root->m_guard = placeHolderGuard;
                                subsetOf->place(temp, subsetOf->m_root);
                            }
                            subsetOf->place(node, subsetOf->m_root);
                        } else {
                            subsetOf->m_root = m_root;
                        }
                    }
                } else {
                    if (m_root->m_guard > m_guard) {
                        // if the root is a subsetted sequence push it under this one
                        SetNode* temp = m_root;
                        m_root = node;
                        place(temp, node);
                    } else {
                        place(node, m_root);
                    }
                }
                m_size++;
                for (auto& subsetOf : m_superSets) {
                    subsetOf->m_size++;
                }
                for (auto& redefined : m_redefines) {
                    redefined->m_size++;
                }
            };
            void innerAdd(T& el) {
                SetNode* node = createNode(el);
                setName(node);
                add(node);
                if (m_el) {
                    if (m_el->m_manager) {
                        m_el->setReference(&el);
                    }
                }
            };
            void nonOppositeAdd(T& el) {
                if (m_upper) {
                    // this is a workaround to a polymorphic add, look at size to determine if singleton or not
                    if (m_upper == 1) { // enforce singleton behavior
                        if (this->m_root) {
                            this->removeReadOnly(this->m_root->m_id);
                        }
                    } else {
                        if (m_size >= m_upper) {
                            throw ManagerStateException("TODO: size > upper");
                        }
                    }
                }
                innerAdd(el);
                if (m_el && m_el->m_node) {
                    if (m_el->m_node->m_managerElementMemory != m_el) {
                        (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().innerAdd(el);
                    }
                    for (auto& copy : m_el->m_node->m_copies) {
                        if (copy != m_el) {
                            (copy->as<U>().*m_signature)().innerAdd(el);
                        }
                    }
                }
                for (auto& func : m_addFunctors) {
                    (*func)(el);
                }
            };
            virtual void deleteNode(SetNode* node) {
                delete node;
            };
            void innerRemove(ID id) {
                SetNode* temp = search(id, m_root);
                if (temp->m_parent) {
                    // has parent
                    bool isLeft = temp->m_parent->m_left ? temp->m_parent->m_left->m_id == temp->m_id ? true : false : false;
                    if (isLeft) {
                        // The removed element is on the left side of its parent
                        if (!temp->m_parent->m_right) {
                            // If the parent has no right side just copy the removed nodes children to the parents children
                            temp->m_parent->m_left = temp->m_left;
                            temp->m_parent->m_right = temp->m_right;
                            if (temp->m_left) {
                                temp->m_left->m_parent = temp->m_parent;
                            }
                            if (temp->m_right) {
                                temp->m_right->m_parent = temp->m_parent;   
                            }
                        } else {
                            if (temp->m_left) {
                                if (temp->m_parent->m_right->m_id > temp->m_left->m_id) {
                                    // If the removed node's left side is less than the right side put the right to the left and left to the right and place the removed nodes right on the left side
                                    temp->m_parent->m_left = temp->m_parent->m_right;
                                    temp->m_parent->m_right = temp->m_left;
                                    temp->m_left->m_parent = temp->m_parent;
                                    if (temp->m_right) {
                                        place(temp->m_right, temp->m_left);
                                    }
                                } else {
                                    // put the removed node's left side to the parent's left and place the right wherever it belongs
                                    temp->m_parent->m_left = temp->m_left;
                                    temp->m_left->m_parent = temp->m_parent;
                                    if (temp->m_right) {
                                        place(temp->m_right, temp->m_parent);
                                    }
                                }
                            } else {
                                // removed node has no children
                                temp->m_parent->m_left = temp->m_parent->m_right;
                                temp->m_parent->m_right = 0;
                                /** TODO: **/
                                // may be of use to rebalance tree here?

                                if (temp->m_parent->m_id == placeholderID) {
                                    for (auto& superSet : m_superSets) {
                                        if (superSet->m_root == temp->m_parent) {
                                            superSet->m_root = temp->m_parent->m_left;
                                            superSet->m_root->m_parent = 0;
                                        }
                                        for (auto& set : static_cast<Set*>(superSet)->m_subSets) {
                                            if (set != this) {
                                                if (set->m_root == temp->m_parent) {
                                                    SetNode* node =  temp->m_parent->m_left;
                                                    if (node->m_parent) {
                                                        node->m_parent = 0;
                                                    }
                                                    while (node && node->m_guard < set->m_guard) {
                                                        node = node->m_left;
                                                    }
                                                    set->m_root = node;
                                                }
                                            }
                                        }
                                    }
                                    delete temp->m_parent;
                                }
                            }
                        }
                    } else {
                        if (!temp->m_left) {
                            // removed node has no children
                            temp->m_parent->m_right = 0;
                            /** TODO: **/
                            // may be of use to rebalance tree here? 
                            if (temp->m_parent->m_id == placeholderID) {
                                for (auto& superSet : m_superSets) {
                                    if (superSet->m_root == temp->m_parent) {
                                        superSet->m_root = temp->m_parent->m_left;
                                        superSet->m_root->m_parent = 0;
                                    }
                                    for (auto& set : static_cast<Set*>(superSet)->m_subSets) {
                                        if (set != this) {
                                            if (set->m_root == temp->m_parent) {
                                                SetNode* node =  temp->m_parent->m_left;
                                                if (node->m_parent) {
                                                    node->m_parent = 0;
                                                }
                                                while (node && node->m_guard < set->m_guard) {
                                                    node = node->m_left;
                                                }
                                                set->m_root = node;
                                            }
                                        }
                                    }
                                }
                                delete temp->m_parent;
                            }
                        } else {
                            if (temp->m_left->m_id > temp->m_parent->m_left->m_id) {
                                temp->m_parent->m_right = temp->m_parent->m_left;
                                temp->m_parent->m_left = temp->m_left;
                                temp->m_left->m_parent = temp->m_parent;
                                if (temp->m_right) {
                                    place(temp->m_right, temp->m_parent);
                                }
                            } else {
                                temp->m_parent->m_right = temp->m_left;
                                temp->m_left->m_parent = temp->m_parent;
                                if (temp->m_right) {
                                    place(temp->m_right, temp->m_parent);
                                }
                            }
                        }
                    }
                } else {
                    // we are removing base root of tree
                    if (temp->m_left) {
                        m_root = temp->m_left;
                        if (temp->m_right) {
                            place(temp->m_right, m_root);
                        }
                        m_root->m_parent = 0;
                    }
                }
                if (temp->m_id == m_root->m_id) {
                    // if we removed the root of the sequence
                    if (temp->m_left) {
                        place(m_root->m_right, m_root->m_left);
                        m_root->m_left->m_parent = m_root->m_parent;
                        m_root = temp->m_left;
                    } else {
                        m_root = 0;
                    }
                    for (auto& redefined : m_redefines) {
                        redefined->m_root = m_root;
                    }
                }
                for (auto subset : m_subSets) {
                    if (temp->m_guard >= subset->m_guard) {
                        subset->m_size--;
                        if (subset->m_root && subset->m_root->m_id == id) {
                            if (subset->m_root->m_left) {
                                place(subset->m_root->m_right, subset->m_root->m_left);
                                subset->m_root->m_left->m_parent = subset->m_root->m_parent;
                                subset->m_root = subset->m_root->m_left;
                            } else {
                                subset->m_root = 0;
                            }
                        }
                    }
                }
                for (auto& subsetOf : m_superSets) {
                    if (subsetOf->m_root->m_id == id) {
                        if (subsetOf->m_root->m_left) {
                            place(subsetOf->m_root->m_right, subsetOf->m_root->m_left);
                            m_root->m_left->m_parent = m_root->m_parent;
                            m_root = subsetOf->m_root->m_left;
                        } else {
                            subsetOf->m_root = 0;
                        }
                    }
                    subsetOf->m_size--;
                }
                for (auto& redefined : m_redefines) {
                    redefined->m_size--;
                }
                for (auto& func : m_removeFunctors) {
                    if (!temp->m_el) {
                        temp->m_el = m_el->m_manager->get(m_el, temp->m_id);
                    }
                    (*func)(*dynamic_cast<T*>(temp->m_el));
                }
                deleteNode(temp);
                m_size--;
            };
            void nonOppositeRemove(ID id) {
                innerRemove(id);
                if (m_el) {
                    if (m_el->m_node->m_managerElementMemory != m_el) {
                        (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().innerRemove(id);
                    }
                    for (auto& copy : m_el->m_node->m_copies) {
                        if (copy != m_el) {
                            (copy->as<U>().*m_signature)().innerRemove(id);
                        }
                    }
                    m_el->removeReference(id);
                }
            };
            inline SetNode* lookForNodeInParents(ID id) {
                for (auto& subsetOf : m_superSets) {
                    SetNode* temp = 0;
                    if (subsetOf->m_root && (temp = subsetOf->search(id, subsetOf->m_root)) != 0) {
                        temp->m_guard = m_guard;
                        if (subsetOf->m_root->m_id == temp->m_id) {
                            if (temp->m_left) {
                                subsetOf->m_root = temp->m_left;
                            } else {
                                subsetOf->m_root = 0;
                            }
                            for (auto& superSet : m_superSets) {
                                if (superSet->m_root == temp) {
                                    superSet->m_root = subsetOf->m_root;
                                }
                            }
                        }
                        if (temp->m_left) {
                            if (temp->m_right) {
                                temp->m_right->m_parent = 0;
                                subsetOf->place(temp->m_right, temp->m_left);
                            }
                            temp->m_left->m_parent = 0;
                        }
                        if (temp->m_parent) {
                            if (temp->m_parent->m_left->m_id == temp->m_id) {
                                temp->m_parent->m_left = temp->m_parent->m_right;
                            } 
                            temp->m_parent->m_right = 0;
                            if (temp->m_left) {
                                subsetOf->place(temp->m_left, temp->m_parent);
                            }
                        }
                        temp->m_parent = 0;
                        temp->m_left = 0;
                        temp->m_right = 0;
                        for (auto& superSet : m_superSets) {
                            superSet->m_size--;
                        }
                        return temp;
                    }
                }
                return 0;
            };
            inline virtual SetNode* createNode(T& el) {
                SetNode* temp = lookForNodeInParents(el.getID());
                if (temp) {
                    return temp;
                }
                return new SetNode(static_cast<Element*>(&el));
            };
            virtual SetNode* createNode(ID id) {
                SetNode* ret = new SetNode();
                ret->m_id = id;
                return ret;
            };
            void release(ID id) {
                SetNode* searchResult = search(id, m_root);
                if (!searchResult) {
                    throw ID_doesNotExistException2(id);
                }
                if (searchResult->m_el) {
                    searchResult->m_el = 0;
                }
            };
            void innerReindex(ID oldID, ID newID) {
                if (m_root) {
                    SetNode* node = search(oldID, m_root);
                    if (node) {
                        if (node->m_parent) {
                            if (node->m_parent->m_left->m_id == oldID) {
                                if (node->m_parent->m_right->m_id > newID) {
                                    node->m_parent->m_left = node->m_parent->m_right;
                                    node->m_parent->m_right = node;
                                }
                            } else {
                                if (newID > node->m_parent->m_left->m_id) {
                                    node->m_parent->m_right = node->m_parent->m_left;
                                    node->m_parent->m_left = node;
                                }
                            }
                        }
                        for (auto& set : m_subSets) {
                            for (auto& superSet : static_cast<Set*>(set)->m_superSets) {
                                SetNode* node2 = superSet->m_root;
                                while (node2) {
                                    if (node2->m_left) {
                                        if (node2->m_left->m_id == oldID) {
                                            if (node2->m_right && node2->m_right->m_id > newID) {
                                                // switch
                                                SetNode* temp = node2->m_left;
                                                node2->m_left = node2->m_right;
                                                node2->m_right = temp;
                                            }
                                            node2 = 0;
                                        } else if (node2->m_right && node2->m_right->m_id == oldID) {
                                            if (newID > node2->m_left->m_id) {
                                                SetNode* temp = node2->m_left;
                                                node2->m_left = node2->m_right;
                                                node2->m_right = temp;
                                            }
                                            node2 = 0;
                                        } else {
                                            node2 = node2->m_left;
                                        }
                                    } else {
                                        if (node2->m_parent && node2->m_parent->m_right == node2) {
                                            break;
                                        }
                                        do {
                                            node2 = node2->m_parent;
                                        } while (node2 && !node2->m_right);
                                        if (node2) {
                                            node2 = node2->m_right;
                                        }
                                    }
                                }
                            }
                        }
                        node->m_id = newID;
                    }
                }
            };
            void reindex(ID oldID, ID newID) {
                if (m_root) {
                    SetNode* node = search(oldID, m_root);
                    if (node) {
                        if (node->m_parent) {
                            if (node->m_parent->m_left->m_id == oldID) {
                                if (node->m_parent->m_right->m_id > newID) {
                                    node->m_parent->m_left = node->m_parent->m_right;
                                    node->m_parent->m_right = node;
                                }
                            } else {
                                if (newID > node->m_parent->m_left->m_id) {
                                    node->m_parent->m_right = node->m_parent->m_left;
                                    node->m_parent->m_left = node;
                                }
                            }
                            for (auto& set : m_subSets) {
                                for (auto& superSet : static_cast<Set*>(set)->m_superSets) {
                                    SetNode* node2 = superSet->m_root;
                                    while (node2) {
                                        if (node2->m_left) {
                                            if (node2->m_left->m_id == oldID) {
                                                if (node2->m_right && node2->m_right->m_id > newID) {
                                                    // switch
                                                    SetNode* temp = node2->m_left;
                                                    node2->m_left = node2->m_right;
                                                    node2->m_right = temp;
                                                }
                                                node2 = 0;
                                            } else if (node2->m_right && node2->m_right->m_id == oldID) {
                                                if (newID > node2->m_left->m_id) {
                                                    SetNode* temp = node2->m_left;
                                                    node2->m_left = node2->m_right;
                                                    node2->m_right = temp;
                                                }
                                                node2 = 0;
                                            } else {
                                                node2 = node2->m_left;
                                            }
                                        } else {
                                            if (node2->m_parent && node2->m_parent->m_right == node2) {
                                                break;
                                            }
                                            do {
                                                node2 = node2->m_parent;
                                            } while (node2 && !node2->m_right);
                                            if (node2) {
                                                node2 = node2->m_right;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        node->m_id = newID;
                        if (m_el) {
                            if (m_el->m_node->m_managerElementMemory != m_el) {
                                (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().innerReindex(oldID, newID);
                            }
                            for (auto& copy : m_el->m_node->m_copies) {
                                if (copy != m_el) {
                                    (copy->as<U>().*m_signature)().innerReindex(oldID, newID);
                                }
                            }
                        }
                    }
                }
            };
            void eraseElement(ID id) {
                // this will always need to search tree (don't know any quicker way)
                if (contains(id)) {
                    innerRemove(id);
                }
            };
            void removeReadOnly(ID id) {
                if (m_root) {
                    innerRemove(id);
                    if (m_oppositeFunctor) {
                        T& el = m_el->m_manager->get<T>(m_el, id)->template as<T>();
                        (*m_oppositeFunctor)(el, 1);
                    }
                    if (m_el) {
                        if (m_el->m_node->m_managerElementMemory != m_el) {
                            (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().innerRemove(id);
                        }
                        for (auto& copy : m_el->m_node->m_copies) {
                            if (copy != m_el) {
                                (copy->as<U>().*m_signature)().innerRemove(id);
                            }
                        }
                        m_el->removeReference(id);
                    }
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
        public:
            inline Set(Element* el) : m_el(el) {};
            inline Set() {};
            void operator=(const Set &rhs) {
                m_size = rhs.m_size;
                if (rhs.m_root) {
                    for (auto& set : m_superSets) {
                        // find if root has already been copied
                        if (set->m_root) {
                            if ((m_root = static_cast<Set*>(set)->search(rhs.m_root->m_id, set->m_root)) != 0) {
                                break;
                            }
                        }
                    }
                    if (!m_root && m_guard != 0) {
                        // if haven't found it in supersets, look in subsets supersets
                        for (auto& set : m_subSets) {
                            for (auto& disjointSet : static_cast<Set*>(set)->m_superSets) {
                                if (disjointSet->m_root) {
                                    if ((m_root = static_cast<Set*>(disjointSet)->search(rhs.m_root->m_id, disjointSet->m_root)) != 0) {
                                        break;
                                    }
                                }
                            }
                            if (m_root) {
                                break;
                            }
                        }
                    }
                    if (!m_root) {
                        // if root hasn't been copied over, copy everything from rhs
                        SetNode* curr = rhs.m_root;
                        SetNode* copy = 0;
                        while(curr) {
                            SetNode* newNode = new SetNode();
                            newNode->m_id = curr->m_id;
                            newNode->m_el = curr->m_el;
                            newNode->m_guard = curr->m_guard;
                            newNode->m_name = curr->m_name;
                            if (curr->m_id == rhs.m_root->m_id) {
                                m_root = newNode;
                            }
                            if (curr->m_parent) {
                                if (!copy) {
                                    // we need to find parent from tree
                                    for (auto& set : m_superSets) {
                                        // find if parent has already been copied
                                        if ((copy = static_cast<Set*>(set)->search(curr->m_id, set->m_root)) != 0) {
                                            break;
                                        }
                                    }
                                }
                                if (copy) {
                                    newNode->m_parent = copy;
                                    if (copy->m_left) {
                                        copy->m_right = newNode;
                                    } else {
                                        copy->m_left = newNode;
                                    }
                                } else {
                                    throw ManagerStateException("copy set!");
                                }
                            }
                            if (curr->m_left) {
                                // if left go left for dfs
                                curr = curr->m_left;
                                copy = newNode;
                            } else {
                                // if there is nothing find first right we skipped
                                SetNode* og = newNode;
                                do {
                                    // find parent of first right
                                    curr = curr->m_parent;
                                    newNode = newNode->m_parent;
                                } while (curr && !curr->m_right);
                                if (curr) {
                                    if (curr->m_right->m_id == og->m_id) {
                                        // without this line it will just keep copying bottom right
                                        break;
                                    }
                                    curr = curr->m_right;
                                    copy = newNode;
                                }
                            }
                        }
                    }
                }
            };
            Set(const Set<T,U>& rhs) {
                *this = rhs;
                // maybe copy over some of the other aspects here?
            };
            virtual ~Set() { 
                if (m_rootRedefinedSet) {
                    SetNode* curr = m_root;
                    // start destroying from bottom left
                    while (curr && curr->m_left) {
                        curr = curr->m_left;
                    }
                    while (curr) {
                        if (curr->m_right) {
                            curr = curr->m_right;
                        } else if (curr->m_left) {
                            curr = curr->m_left;
                        } else {
                            // delete this node conditionally
                            if (curr->m_guard == m_guard) {
                                // we are going to delete it
                                SetNode* currParent = curr->m_parent;
                                for (auto& set : m_superSets) {
                                    // set supersets root to 0 if it is the node
                                    if (set->m_root->m_id == curr->m_id && set->m_root == curr) {
                                        set->m_root = 0;
                                    } else if (currParent && set->m_root && currParent != set->search(currParent->m_id, set->m_root)) {
                                        // this set owns this element through a different parent
                                        // we must find it and set the pointer to curr to 0
                                        SetNode* temp = set->m_root;
                                        while (temp->m_left != curr && temp->m_right != curr) {
                                            if (temp->m_right) {
                                                if (curr->m_id > temp->m_id) {
                                                    temp = temp->m_left;
                                                } else {
                                                    temp = temp->m_right;
                                                }
                                            } else if (temp->m_left) {
                                                temp = temp->m_left;
                                            } else {
                                                std::cerr << "could not find second parent for setNode with id: " << curr->m_id.string() << std::endl;
                                                temp = 0;
                                                break;
                                            }
                                        }
                                        if (temp) {
                                            if (temp->m_right == curr) {
                                                temp->m_right = 0;
                                            } else {
                                                temp->m_left = 0;
                                            }
                                        }
                                    }
                                }
                                if (currParent) {
                                    if (currParent->m_left == curr) {
                                        currParent->m_left = 0;
                                    } else {
                                        currParent->m_right = 0;
                                    }
                                }
                                delete curr;
                                curr =  currParent;
                                if (curr && curr->m_id == placeholderID) {
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                    }
                }
                if (m_ownsOppositeFunctor) {
                    delete m_oppositeFunctor;
                }
                for (auto& func : m_addFunctors) {
                    bool deleteFunc = true;
                    for (auto& set : m_superSets) {
                        if (static_cast<Set*>(set)->m_addFunctors.count(func)) {
                            deleteFunc = false;
                            break;
                        }
                    }
                    if (deleteFunc) {
                        for (auto& set : m_redefines) {
                            if (static_cast<Set*>(set)->m_addFunctors.count(func)) {
                                deleteFunc = false;
                                break;
                            }
                        }
                    }
                    if (deleteFunc) {
                        delete func;
                    }
                }
                for (auto& func : m_removeFunctors) {
                    bool deleteFunc = true;
                    for (auto& set : m_superSets) {
                        if (static_cast<Set*>(set)->m_removeFunctors.count(func)) {
                            deleteFunc = false;
                            break;
                        }
                    }
                    if (deleteFunc) {
                        delete func;
                    }
                }
                for (auto& set : m_redefines) {
                    Set* rSet = static_cast<Set*>(set);
                    std::vector<AbstractSet*>::iterator it;
                    if ((it = std::find(rSet->m_redefines.begin(), rSet->m_redefines.end(), this)) != rSet->m_redefines.end()) {
                        rSet->m_redefines.erase(it);
                    }
                }
            };
            /**
             * WARN: so for now ORDER MATTERS when subsetting a sequence
             * the first element subsetted should be the first element
             * instantiated, (e.g.):
             *  Set<> a;
             *  Set<> b;
             *  Set<> c;
             *  a.subsets(c); // BAD
             *  c.subsets(b); // BAD if subsetting a after
             *  c.subsets(a); // BAD
             *  c.subsets(a); // GOOD
             *  c.subsets(b); // GOOD because b instantiated after a
             **/
            template <class V = Element, class W = Element> void subsets(Set<V, W>& subsetOf) {
                if (std::find(m_superSets.begin(), m_superSets.end(), &subsetOf) == m_superSets.end()) {
                    m_superSets.push_back(&subsetOf);
                    subsetOf.m_subSets.push_back(this);
                    for (auto& set : m_superSets) {
                        // compare and update guard of superset to previous supersets
                        if (set != &subsetOf && set->m_guard <= subsetOf.m_guard) {
                            subsetOf.m_guard = set->m_guard + 1;
                        }
                    }
                    if (!m_oppositeFunctor && subsetOf.m_oppositeFunctor && m_el) {
                        m_oppositeFunctor = subsetOf.m_oppositeFunctor;
                    }
                    for (auto& set : subsetOf.m_superSets) {
                        this->subsets(*static_cast<Set*>(set));
                    }
                    if (m_guard <= subsetOf.m_guard) {
                        m_guard = subsetOf.m_guard + 1;
                    }
                    for (const auto& set : subsetOf.m_addFunctors) {
                        if (!m_addFunctors.count(set)) {
                            m_addFunctors.insert(set);
                        }
                    }
                    for (auto& set : subsetOf.m_removeFunctors) {
                        if (!m_removeFunctors.count(set)) {
                            m_removeFunctors.insert(set);
                        }
                    }
                    if (!m_root) {
                        m_root = subsetOf.m_root;
                    } else if (!subsetOf.m_root) {
                        subsetOf.m_root = m_root;
                    }
                }
            };
            void opposite(Set<U, T>& (T::*op)()) {
                /** TODO: static_assert that we have m_el for this instance **/
                if (m_el) {
                    OppositeFunctor<T,U>* opFunc = new OppositeFunctor<T,U>(dynamic_cast<U*>(m_el));
                    opFunc->m_signature = op;
                    m_oppositeFunctor = opFunc;
                    m_ownsOppositeFunctor = true;
                } else {
                    std::cerr << "WARN: opposite called when there is no element owning the set, make sure to use proper constructor!" << std::endl;
                }
            };
            template <class V = Element, class W = Element> void redefines(Set<V, W>& redefined) {
                if (m_root) {
                    std::cerr << "WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!" << std::endl;
                    return;
                }
                m_redefines.push_back(&redefined);
                redefined.m_redefines.push_back(this);
                redefined.m_rootRedefinedSet = false;
                for (auto& set : redefined.m_superSets) {
                    subsets(*static_cast<Set*>(set));
                }
                for (auto& func :redefined.m_addFunctors) {
                    if (!m_addFunctors.count(func)) {
                        m_addFunctors.insert(func);
                    }
                }
                m_guard = redefined.m_guard;
            };
            void addFunctor(SetFunctor* func) {
                m_addFunctors.insert(func);
            };
            void removeFunctor(SetFunctor* func) {
                m_removeFunctors.insert(func);
            };
            void removeFromJustThisSet(ID id) {
                if (m_root->m_id == id) {
                    m_root->m_guard--;
                    m_root = 0;
                } else {
                    SetNode* temp = search(id, m_root);
                    if (temp->m_left) {
                        if (temp->m_right) {
                            temp->m_right->m_parent = 0;
                            place(temp->m_right, temp->m_left);
                        }
                        temp->m_left->m_parent = 0;
                    }
                    if (temp->m_parent) {
                        if (temp->m_parent->m_left->m_id == temp->m_id) {
                            temp->m_parent->m_left = 0;
                        } else {
                            temp->m_parent->m_right = 0;
                        }
                        if (temp->m_left) {
                            place(temp->m_left, temp->m_parent);
                        }
                    }
                    temp->m_parent = 0;
                    temp->m_left = 0;
                    temp->m_right = 0;
                    temp->m_guard--;
                    m_superSets.front()->place(temp, m_superSets.front()->m_root);
                }
                m_size--;
            }
            void add(T& el) {
                if (m_readOnly) {
                    throw ReadOnlySetException(el.getID().string());
                }
                nonOppositeAdd(el);
                if (m_oppositeFunctor) {
                    (*m_oppositeFunctor)(el);
                }
            };
            template <class ... Ts> void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            };
            void add(ID id) {
                if (m_readOnly) {
                    throw ReadOnlySetException(id.string());
                }
                SetNode* node = createNode(id);
                add(node);
                if (m_el) {
                    if (m_el->m_manager) {
                        m_el->setReference(id);
                    }
                }
                if (m_el) {
                    if (m_el->m_node->m_managerElementMemory != m_el) {
                        (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().add(id);
                    }
                    for (auto& copy : m_el->m_node->m_copies) {
                        if (copy != m_el) {
                            (copy->as<U>().*m_signature)().add(id);
                        }
                    }
                }
            };
            void remove(T& el) {
                remove(el.getID());
            };
            void remove(ID id) {
                if (m_readOnly) {
                    throw ReadOnlySetException(id.string());
                }
                if (m_root) {
                    innerRemove(id);
                    if (m_oppositeFunctor) {
                        T& el = m_el->m_manager->get<T>(m_el, id)->template as<T>();
                        (*m_oppositeFunctor)(el, 1);
                    }
                    if (m_el) {
                        if (m_el->m_node->m_managerElementMemory != m_el) {
                            (m_el->m_node->m_managerElementMemory->as<U>().*m_signature)().innerRemove(id);
                        }
                        for (auto& copy : m_el->m_node->m_copies) {
                            if (copy != m_el) {
                                (copy->as<U>().*m_signature)().innerRemove(id);
                            }
                        }
                        m_el->removeReference(id);
                    }
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
            T& get(ID id) {
                if (m_root) {
                    SetNode* searchResult = search(id, m_root);
                    if (!searchResult) {
                        throw ID_doesNotExistException2(id);
                    }
                    if (!searchResult->m_el) {
                        searchResult->m_el = m_el->m_manager->get(m_el, id);
                    }
                    T* ret = dynamic_cast<T*>(searchResult->m_el);
                    if (ret) {
                        return *ret;
                    }
                }
                throw ID_doesNotExistException2(id);
            };
            T& get(std::string name) {
                if (m_root) {
                    SetNode* searchResult = search(name, m_root);
                    if (!searchResult) {
                        // TODO throw proper error
                        throw ManagerStateException("Improper name used in set!"); // TODO change
                    }
                    if (!searchResult->m_el) {
                        searchResult->m_el = m_el->m_manager->get(m_el, searchResult->m_id);
                    }
                    T* ret = dynamic_cast<T*>(searchResult->m_el);
                    if (ret) {
                        return *ret;
                    }
                } 
                throw ManagerStateException("Improper name used in set!"); // TODO change
            };
            T& get(int i) {
                int size = m_size;
                SetNode* node = m_root;  //this wont work
                while (i < size) {
                    // check size and determine which side to go down
                    size--;
                    size = size / 2;
                    if (i > size) {
                        node = node->m_left;
                    } else {
                        if (node->m_right) {
                            node = node->m_right;
                        }
                    }
                }
                if (!node->m_el) {
                    node->m_el = m_el->m_manager->get(m_el, node->m_id);
                }
                return *dynamic_cast<T*>(node->m_el);
            };
            T& front() {
                SetNode* node;
                if (m_root) {
                    node = m_root;
                    // select first non placeholder node
                    while (node->m_id == placeholderID) {
                        node = node->m_left;
                    }
                    if (!node->m_el) {
                        node->m_el = m_el->m_manager->get(m_el, node->m_id);
                    }
                    return *dynamic_cast<T*>(node->m_el);
                }
                throw ManagerStateException("TODO front empty");
            };
            T& back() {
                if (m_root) {
                    SetNode* temp = m_root;
                    while (temp->m_right) {
                        temp = temp->m_right;
                    }
                    if (temp->m_left) {
                        temp = temp->m_left;
                    }
                    if (!temp->m_el) {
                        temp->m_el = m_el->m_manager->get(m_el, temp->m_id);
                    }
                    return *dynamic_cast<T*>(temp->m_el);
                }
                throw ManagerStateException("TODO back empty");
            };
            bool contains(ID id) {
                bool ret = false;
                if (m_root) {
                    SetNode* t = search(id, m_root);
                    ret = t > 0;
                } 
                return ret;
            };
            int count(ID id) {
                if (contains(id)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            bool contains(std::string name) {
                bool ret = false;
                if (m_root) {
                    SetNode* t = search(name, m_root);
                    ret = t > 0;
                }
                return ret;
            };
            int count(std::string name) {
                if (contains(name)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            bool empty() const {
                return m_root == 0;
            };
            size_t size() const { return m_size; };
            SetIterator<T> begin() {
                SetIterator<T> it;
                if (m_root) {
                    it.m_node = m_root;
                } else {
                    it.m_node = &it.m_endNode;
                }
                it.m_el = m_el;
                if (it.m_node->m_id == placeholderID) {
                    it++;
                }
                return it;
            };
            SetIterator<T> end() {
                SetIterator<T> it;
                it.m_node = &it.m_endNode;
                it.m_el = m_el;
                return it;
            };
            ID_Set<T> ids() {
                ID_Set<T> set;
                set.m_root = m_root;
                return set;
            };
    };

    template <class T = Element> struct SetIterator {

        template <class V, class W> friend class Set;
        friend class AbstractSet;
        template < class V> friend class ID_Set;

        protected:
            Element* m_el;
            AbstractSet::SetNode* m_node;
            AbstractSet::SetNode m_endNode;
        public:
            T& operator*() {
                if (!m_node->m_el) {
                    m_node->m_el = m_el->m_manager->get(m_el, m_node->m_id);
                }
                return dynamic_cast<T&>(*m_node->m_el);
            };
            T* operator->() {
                if (!m_node->m_el) {
                    m_node->m_el = m_el->m_manager->get(m_el, m_node->m_id);
                }
                return dynamic_cast<T*>(m_node->m_el);
            };

            // This is dfs
            SetIterator operator++() {
                do {
                    if (m_node->m_left) {
                        // always go left
                        m_node = m_node->m_left;
                    } else {
                        if (!m_node->m_parent) {
                            // if there is no parent to go to we must end
                            m_node = &m_endNode;
                            break;
                        }
                        // we hit bottom, choose next right
                        AbstractSet::SetNode* temp;
                        AbstractSet::SetNode* last = dynamic_cast<AbstractSet::SetNode*>(m_node);
                        bool found = false;
                        do {
                            temp = dynamic_cast<AbstractSet::SetNode*>(last->m_parent);
                            if (temp->m_right) {
                                if (temp->m_right->m_id != last->m_id) {
                                    found = true;
                                    break;
                                }
                            }
                            last = temp;
                        } while (temp->m_parent);
                        if (!found) {
                            m_node = &m_endNode;
                        } else {
                            m_node = temp->m_right;
                        }
                    }
                } while (m_node->m_id == placeholderID);
                return *this;
            };
            SetIterator operator++(int) {
                do {
                    if (m_node->m_left) {
                        // always go left
                        m_node = dynamic_cast<AbstractSet::SetNode*>(m_node->m_left);
                    } else {
                        if (!m_node->m_parent) {
                            // if there is no parent to go to we must end
                            m_node = &m_endNode;
                            break;
                        }
                        // we hit bottom, choose next right
                        AbstractSet::SetNode* temp;
                        AbstractSet::SetNode* last = dynamic_cast<AbstractSet::SetNode*>(m_node);
                        bool found = false;
                        do {
                            temp = dynamic_cast<AbstractSet::SetNode*>(last->m_parent);
                            if (temp->m_right) {
                                if (temp->m_right->m_id != last->m_id) {
                                    found = true;
                                    break;
                                }
                            }
                            last = temp;
                        } while (temp->m_parent);
                        if (!found) {
                            m_node = &m_endNode;
                        } else {
                            m_node = dynamic_cast<AbstractSet::SetNode*>(temp->m_right);
                        }
                    }
                } while (m_node->m_id == placeholderID);
                SetIterator ret = *this;
                return ret;
            };
            friend bool operator== (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id == b.m_node->m_id; };
            friend bool operator!= (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id != b.m_node->m_id; };
    };

    template <class T = Element> struct SetID_Iterator : public SetIterator<T> {
        ID operator*() {
            return this->m_node->m_id;
        };
    };

    template <class T = Element> struct ID_Set {

        template <class U, class V> friend class Set;

        private:
            AbstractSet::SetNode* m_root = 0;
        public:
            SetID_Iterator<T> begin() {
                SetID_Iterator<T> it;
                it.m_node = m_root;
                return it;
            };
            SetID_Iterator<T> end() {
                SetID_Iterator<T> it;
                it.m_node = &it.m_endNode;
                return it;
            };
    };
}

#endif
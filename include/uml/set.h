#ifndef _UML_SET_H_
#define _UML_SET_H_

#include "element.h"
#include "umlManager.h"

/**
 * A quick note: The Set class is complicated and took a lot of time for me to figure out.
 * There are probably some oversights on my part, and some optimizations that could be implementd.
 **/

namespace UML {

    namespace {
        const ID placeholderID = ID::nullID();
    }

    template <class T, class U> class Set;
    template <class T, class U> class OrderedSet;
    template <class T> struct SetIterator;
    template <class T> struct ID_Set;
    // class AbstractSet;
    // void recursiveSetContains(ID id, AbstractSet* set);

    class ID_doesNotExistException : public std::exception {
        private:
            std::string m_msg;
        public:
            ID_doesNotExistException(ID id) : m_msg(id.string() + " was not found within the sequence!"){};
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

    class FixedSetException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "Tried to configure fixed set, this set Is fixed by the API!";
            }
    };

    class AbstractOppositeFunctor {
        public:
            virtual void operator()(Element& el) const = 0;
            virtual void operator()(Element& el, bool remove) const = 0;
            virtual ~AbstractOppositeFunctor() {};
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

    class SetFunctor {
        protected:
            Element& m_el;
        public:
            SetFunctor(Element* el) : m_el(*el) {};
            virtual void operator()(Element& el) const = 0;
            virtual ~SetFunctor() {};
    };

    class AbstractSet {
        template <class T, class U> friend class Set;
        template <class T, class U> friend class OrderedSet;
        template <class T> friend struct SetIterator;
        template <class T> friend struct ID_Set;
        friend void recursiveSetContains(ID id, AbstractSet* set);
        friend class Element;
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
            /**
             * The guard is the sets way of figuring out quick whether a node should be in it or not
             * When a node is created or added to a new set it is assigned a guard to manage this
             * the rules that determine a guard are as follows
             *  1) if the guard is 0 it is a root set (no super sets)
             *  2) all subsets of a set shall have a guard less than the sets guard
             *  3) all subsets of a set shall have different guards
             **/
            size_t m_guard = 0;
            size_t m_nextSubsetGuard = 1;
            std::vector<AbstractSet*> m_superSets;
            std::vector<AbstractSet*> m_subSets;
            std::vector<AbstractSet*> m_redefines;
            std::unordered_set<SetFunctor*> m_addFunctors;
            std::unordered_set<SetFunctor*> m_removeFunctors;
            /**
             * Places the node within the tree taking in account the id and the guard of the node
             * @param node the new node being placed within the tree
             * @param parent the node that you want to place the new node below
             **/
            void place(SetNode* node, SetNode* parent) {
                if (node->m_id != placeholderID && node->m_id == parent->m_id) {
                    delete node;
                    throw DuplicateElementInSetException();
                }
                if (parent->m_left) {
                    // prefer placement to left
                    if (parent->m_right) {
                        // both children are populated, determine where to placenewID
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
                            } else {
                                // TODO double check logic
                                place(node, parent->m_left);
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
            /**
             * Searches the tree for the node with given id from the node supplied
             * @param id the ID of the node you are looking for
             * @param node the node you are basing your search off of
             * @return pointer to the node that matched the ID, or 0 if the node was not found
             **/
            SetNode* search(ID id, SetNode* node) {
                if (node->m_id == id) {
                    // found match
                    return node;
                } else if (node->m_id != placeholderID) { // != is faster than == for id
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
                } else {
                    SetNode* ret = 0;
                    if (node->m_right) {
                        ret = search(id, node->m_right);
                        if (ret) {
                            return ret;
                        }
                    }
                    if (node->m_left) {
                        ret = search(id, node->m_left);
                    }
                    return ret;
                }
            };
            /**
             * Searches the tree for the node with the name supplied
             * @param name string of the name of the node being searched for
             * @param node the node that you are basing your search off of
             * @return the node that was found that matched the name supplied or 0 if no match was found
             **/
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
            /**
             * Gets all sets that are a superset of this one in dfs order and stored in a pointer to a vector, 
             * responsibility of vector destruction is required
             * TODO: fix subsets method to make sure it will be 'proper' bfs order for algorithms
             * @return the pointer to a vector of all supersets of this set
             **/
            std::vector<AbstractSet*>* getAllSuperSets() {
                std::list<AbstractSet*> stack;
                std::vector<AbstractSet*>* allSuperSets = new std::vector<AbstractSet*>;
                for (auto& subsetOf : m_superSets) {
                    stack.push_back(subsetOf);
                }
                while (!stack.empty()) {
                    AbstractSet* front = stack.front();
                    stack.pop_front();
                    if (std::find(allSuperSets->begin(), allSuperSets->end(), front) == allSuperSets->end()) {
                        allSuperSets->push_back(front);
                        for (auto& subsetOf : front->m_superSets) {
                            stack.push_front(subsetOf);
                        }
                    }
                }
                return allSuperSets;
            };
            void setName(SetNode* node);
            void instantiateSetNode(SetNode* node);
        public:
            /**
             * returns a bool on whether the id supplied is in the set
             * @param id the id of the element to be determined is in the set
             * @return true means the id supplied is in the set, false means the id supplied is not in the set
             **/
            bool contains(ID id) {
                bool ret = false;
                if (m_root) {
                    SetNode* t = search(id, m_root);
                    ret = t != 0;
                } 
                return ret;
            };
    };

    template <class T> struct SetIterator;
    template <class T> struct ID_Set;

    namespace Parsers {
        void setNamespace(NamedElement& el, ID id);
        void setOwner(Element& el, ID id);
    }

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
        friend class ActivityEdge;
        friend class ActivityGroup;
        friend class ActivityNode;
        friend class ActivityParameterNode;
        friend class ActivityPartition;
        friend class Association;
        friend class Behavior;
        friend class BehavioredClassifier;
        friend class BehavioralFeature;
        friend class CallBehaviorAction;
        friend class ClassifierTemplateParameter;
        friend class Classifier;
        friend class Comment;
        friend class ConnectableElement;
        friend class Connector;
        friend class ConnectorEnd;
        friend class Constraint;
        friend class DecisionNode;
        friend class Dependency;
        friend class DirectedRelationship;
        friend class ElementImport;
        friend class EncapsulatedClassifier;
        friend class ExceptionHandler;
        friend class Feature;
        friend class Generalization;
        friend class GeneralizationSet;
        friend class InstanceSpecification;
        friend class InstanceValue;
        friend class InterfaceRealization;
        friend class InterruptibleActivityRegion;
        friend class InvocationAction;
        friend class MultiplicityElement;
        friend class NamedElement;
        friend class Namespace;
        friend class ObjectFlow;
        friend class ObjectNode;
        friend class PackageImport;
        friend class PackageMerge;
        friend class ParameterableElement;
        friend class Parameter;
        friend class ParameterSet;
        friend class Port;
        friend class ProfileApplication;
        friend class Property;
        friend class Reception;
        friend class RedefinableElement;
        friend class RedefinableTemplateSignature;
        friend class Relationship;
        friend class Slot;
        friend class StructuredClassifier;
        friend class TypedElement;
        friend class TemplateBinding;
        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        friend class TemplateSignature;
        friend class Usage;

        friend void Parsers::setNamespace(NamedElement& el, ID id);
        friend void Parsers::setOwner(Element& el, ID id);

        protected:
            bool m_rootRedefinedSet = true;
            AbstractOppositeFunctor* m_oppositeFunctor = 0;
            bool m_ownsOppositeFunctor = false;
            std::vector<AbstractOppositeFunctor*> m_otherOpposites;
            Element* m_el = 0;
            bool m_readOnly = false;
            AbstractSet* m_setToInstantiate = 0;

            /**
             * increase the size of all supersets of this set, this does a graph bfs search for all nodes and increases their size
             **/
            void increaseSuperSetSize() {
                //graph bfs adjust all supersets above this size
                std::list<AbstractSet*> queue;
                std::vector<AbstractSet*> allSuperSets;
                for (auto& subsetOf : m_superSets) {
                    queue.push_back(subsetOf);
                }
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    if (std::find(allSuperSets.begin(), allSuperSets.end(), front) == allSuperSets.end()) {
                        allSuperSets.push_back(front);
                        for (auto& subsetOf : front->m_superSets) {
                            queue.push_back(subsetOf);
                        }
                    }
                }
                for (auto& subsetOf : allSuperSets) {
                    subsetOf->m_size++;
                }
            };
            /**
             * Adds a node into the tree for this set
             * @param node the new node being added to the tree
             **/
            void add(SetNode* node) {
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
                    // handle supersets
                    // graph dfs search
                    std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                    SetNode* lastNode = 0;
                    SetNode* lastPlaceholder = 0;
                    for (std::vector<AbstractSet*>::iterator it = allSuperSets->begin(); it != allSuperSets->end(); it++) {
                        if (!(*it)->m_root) {
                            (*it)->m_root = node;
                            (*it)->m_size++;
                        } else if ((*it)->m_root == lastNode) {
                            (*it)->m_root = lastPlaceholder;
                            (*it)->m_size++;
                        } else {
                            // create placeholder and set superset roots as well as skip over them in list
                            SetNode* temp = (*it)->m_root;
                            lastNode = temp;
                            while (temp->m_id == placeholderID && temp->m_right != node) {
                                temp = temp->m_left;
                            }
                            if (temp->m_guard != node->m_guard && node->m_guard == m_guard && temp->m_right != node) {
                                // create placeholder node
                                SetNode* placeholderNode = new SetNode();
                                placeholderNode->m_id = placeholderID;
                                placeholderNode->m_guard = (*it)->m_guard;
                                lastPlaceholder = placeholderNode;
                                SetNode* oldRoot = (*it)->m_root;
                                if (temp == (*it)->m_root) {
                                    (*it)->m_root = placeholderNode;
                                }
                                // find place to put placeholder
                                SetNode* tempParent = temp->m_parent;
                                while (tempParent && tempParent->m_guard > placeholderNode->m_guard) {
                                    tempParent = tempParent->m_parent;
                                }
                                bool swapRoot = false;
                                bool placeNode = true;
                                if (tempParent) {
                                    placeholderNode->m_parent = tempParent;
                                    if (tempParent->m_left == temp) {
                                        tempParent->m_left = placeholderNode;
                                    } else if (tempParent->m_right == temp) {
                                        tempParent->m_right = tempParent->m_left;
                                        tempParent->m_left = placeholderNode;
                                    } else {
                                        SetNode* leftTemp = tempParent->m_left;
                                        tempParent->m_left = placeholderNode;
                                        placeholderNode->m_left = leftTemp;
                                        placeholderNode->m_right = node;
                                        node->m_parent = placeholderNode;
                                        leftTemp->m_parent = placeholderNode;
                                        placeNode = false;
                                    }
                                } else if (temp->m_parent) {
                                    // edge case where we want to place placeholder if intermediate set
                                    temp = temp->m_parent;
                                    swapRoot = true;
                                }
                                if (swapRoot) {
                                    placeholderNode->m_left = temp;
                                    placeholderNode->m_right = node;
                                    temp->m_parent = placeholderNode;
                                    node->m_parent = placeholderNode;
                                    if (temp == (*it)->m_root) {
                                        (*it)->m_root = placeholderNode;
                                    }
                                } else if (placeNode) {
                                    place(temp, placeholderNode);
                                    place(node, placeholderNode);
                                }
                                (*it)->m_size++;
                                // set next sets roots to this placeholder until the root is different from temp
                                if (temp != oldRoot) {
                                    temp = oldRoot;
                                }
                                std::vector<AbstractSet*>::iterator oIt = it + 1;
                                while (oIt != allSuperSets->end() && (*oIt)->m_root == temp) {
                                    if ((*oIt)->m_root != (*it)->m_root) {
                                        (*oIt)->m_root = placeholderNode;
                                    }
                                    (*oIt)->m_size++;
                                    oIt++;
                                }
                                it = oIt - 1;
                            } else if (node->m_guard == (*it)->m_guard) {
                                // took node away in lookForNodeInParents
                                if ((*it)->m_root) {
                                    (*it)->place(node, (*it)->m_root);
                                    (*it)->m_size++;
                                    std::vector<AbstractSet*>* subsetOfSuperSets = (*it)->getAllSuperSets();
                                    for (auto& superSet : *subsetOfSuperSets) {
                                        superSet->m_size++;
                                    }
                                    delete subsetOfSuperSets;
                                } else {
                                    throw ManagerStateException("TODO, lookForNodeInParents edge case!");
                                }
                                break;
                            }
                        }
                    }
                    delete allSuperSets;
                } else {
                    if (m_root->m_guard > m_guard) {
                        // if the root is a subsetted sequence push it under this one
                        SetNode* temp = m_root;
                        m_root = node;
                        if (temp->m_parent) {
                            node->m_parent = temp->m_parent;
                            if (temp->m_parent->m_left->m_id == temp->m_id) {
                                if (temp->m_parent->m_right) {
                                    if (node->m_id > temp->m_parent->m_right->m_id) {
                                        temp->m_parent->m_left = node;
                                    } else {
                                        temp->m_parent->m_left = temp->m_parent->m_right;
                                        temp->m_parent->m_right = node;
                                    }
                                } else {
                                    temp->m_parent->m_left = node;
                                }
                            } else {
                                if (node->m_id > temp->m_parent->m_left->m_id) {
                                    temp->m_parent->m_right = temp->m_parent->m_left;
                                    temp->m_parent->m_left = node;
                                } else {
                                    temp->m_parent->m_right = node;
                                }
                            }
                        }
                        place(temp, node);
                        //graph bfs adjust all supersets above this size
                        std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                        for (auto& subsetOf : *allSuperSets) {
                            subsetOf->m_size++;
                            if (subsetOf->m_root == temp) {
                                subsetOf->m_root = node;
                            }
                        }
                        delete allSuperSets;
                    } else {
                        if (node->m_guard == m_guard) {
                            place(node, m_root);
                            increaseSuperSetSize();
                        } else {
                            throw ManagerStateException("TODO, diamond set special case");
                        }
                    }
                }
                node->m_guard = m_guard;
                m_size++;
                for (auto& redefined : m_redefines) {
                    redefined->m_size++;
                }
            };
            /**
             * adds an element to the set without invoking it's opposite, functors or singleton behavior
             * @param el the element being added to the set
             **/
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
            /**
             * Adds an element to the set without invoking its opposite functor
             * @param el the element being added to the set
             **/
            void nonOppositeAdd(T& el) {
                if (m_upper) {
                    // this is a workaround to a polymorphic add, look at size to determine if singleton or not
                    if (m_upper == 1) { // enforce singleton behavior
                        if (this->m_root) {
                            if (this->m_root->m_id == el.getID()) {
                                // edge case where adding from already added superset without invoking opposite
                                return;
                            }
                            this->removeReadOnly(this->m_root->m_id);
                        }
                    } else {
                        if (m_size >= m_upper) {
                            throw ManagerStateException("TODO: size > upper");
                        }
                    }
                }
                innerAdd(el);
                for (auto& func : m_addFunctors) {
                    (*func)(el);
                }
            };
            /**
             * polymorphic method to delete a node on removal or destruction
             * @param node the node being deleted from memory
             **/
            virtual void deleteNode(SetNode* node) {
                delete node;
            };
            /**
             * checks if the parent of the node being removed is a placeholder and if it needs to be deleted
             * @param node the node being removed to check for placeholders
             **/
            void removePlaceholder(SetNode* node) {
                if (node->m_parent->m_id == placeholderID) {
                    // bfs fix get rid of placeholder
                    std::list<AbstractSet*> queue;
                    for (auto& subsetOf : m_superSets) {
                        queue.push_back(subsetOf);
                    }
                    while (!queue.empty()) {
                        AbstractSet* front = queue.front();
                        queue.pop_front();
                        if (front->m_root == node->m_parent) {
                            front->m_root = node->m_parent->m_left;
                            if (front->m_root) {
                                front->m_root->m_parent = 0;
                            }
                            for (auto& subset : front->m_subSets) {
                                if (subset != this && subset->m_root && subset->m_root == node->m_parent) {
                                    // set proper root to other subset if removed
                                    SetNode* temp =  node->m_parent->m_left;
                                    if (temp->m_parent) {
                                        temp->m_parent = 0;
                                    }
                                    while (temp && temp->m_guard < subset->m_guard) {
                                        temp = node->m_left;
                                    }
                                    subset->m_root = temp;
                                }
                            }
                        }
                        for (auto& subsetOf : front->m_superSets) {
                            queue.push_back(subsetOf);
                        }
                    }
                    node->m_parent->m_left->m_parent = node->m_parent->m_parent;
                    if (node->m_parent->m_parent) {
                        node->m_parent->m_parent->m_left = node->m_parent->m_left;
                    }
                    // no polymorphic delete
                    delete node->m_parent;
                    node->m_parent = 0;
                }
            };
            /**
             * removes a node from the tree with the supplied id without invoking the opposite, copies, or functors
             * @param id the id of the node being removed
             **/
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
                                removePlaceholder(temp);
                            }
                        }
                    } else {
                        if (!temp->m_left) {
                            // removed node has no children
                            temp->m_parent->m_right = 0;
                            removePlaceholder(temp);
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
                    if (temp->m_parent && temp->m_parent->m_id == placeholderID) {
                        if (temp->m_parent->m_parent) {
                            temp->m_parent->m_parent->m_left = temp->m_parent->m_parent->m_right;
                        }
                        //bfs set superSet roots to 0 if applicable
                        std::list<AbstractSet*> queue;
                        for (auto& subsetOf : m_superSets) {
                            queue.push_back(subsetOf);
                        }
                        while (!queue.empty()) {
                            AbstractSet* front = queue.front();
                            queue.pop_front();
                            if (front->m_root == temp->m_parent) {
                                front->m_root = 0;
                                for (auto& subsetOf : front->m_superSets) {
                                    queue.push_back(subsetOf);
                                }
                            }
                        }
                        delete temp->m_parent;
                    }
                    for (auto& redefined : m_redefines) {
                        redefined->m_root = m_root;
                    }
                }
                std::list<AbstractSet*> subSetQueue;
                for (auto& subset: m_subSets) {
                    subSetQueue.push_back(subset);
                }
                std::vector<AbstractSet*> allSubSets;
                while (!subSetQueue.empty()) {
                    AbstractSet* front = subSetQueue.front();
                    subSetQueue.pop_front();
                    if (temp->m_guard >= front->m_guard && std::find(allSubSets.begin(), allSubSets.end(), front) == allSubSets.end()) {
                        allSubSets.push_back(front);
                        for (auto& subset : front->m_subSets) {
                            if (subset != this && std::find(allSubSets.begin(), allSubSets.end(), subset) == allSubSets.end()) {
                                subSetQueue.push_back(subset);
                            }
                        }
                        for (auto& superSet : front->m_superSets) {
                            if (superSet != this && 
                                std::find(allSubSets.begin(), allSubSets.end(), superSet) == allSubSets.end() &&
                                std::find(m_superSets.begin(), m_superSets.end(), front) == m_superSets.end()) {
                                subSetQueue.push_back(superSet);
                            }
                        }
                    }
                }
                for (auto& subset : allSubSets) {
                    if (temp->m_guard >= subset->m_guard) {
                        subset->m_size--;
                        if (subset->m_root && subset->m_root->m_id == id) {
                            if (temp->m_parent && temp->m_parent->m_guard >= subset->m_guard) {
                                subset->m_root = temp->m_parent;
                            } else if (temp->m_left && temp->m_left->m_guard >= subset->m_guard) {
                                subset->m_root = temp->m_left;
                            } else {
                                subset->m_root = 0;
                            }
                        }
                    }
                }
                // graph bfs adjust supersets
                std::list<AbstractSet*> queue;
                for (auto& subsetOf : m_superSets) {
                    queue.push_back(subsetOf);
                }
                std::vector<AbstractSet*> allSuperSets;
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    if (std::find(allSuperSets.begin(), allSuperSets.end(), front) == allSuperSets.end()) {
                        allSuperSets.push_back(front);
                    }
                    queue.pop_front();
                    for (auto& subsetOf : front->m_superSets) {
                        queue.push_back(subsetOf);
                    }
                }
                for (auto& subsetOf : allSuperSets) {
                    if (subsetOf->m_root && subsetOf->m_root->m_id == id) {
                        subsetOf->m_root = subsetOf->m_root->m_left;
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
            /**
             * removes the node with the supplied id without invoking the opposite functors
             * @param id the id of the node being removed
             **/
            void nonOppositeRemove(ID id) {
                innerRemove(id);
                if (m_el) {
                    m_el->removeReference(id);
                }
            };
            /**
             * looks for a node with the supplied id in its parent sets to see if a new node has to be created
             * @param id the id of the node we are looking for in parent sets
             * @return the node if found, or 0 if it was not found
             **/
            inline SetNode* lookForNodeInParents(ID id) {
                for (auto& subsetOf : m_superSets) {
                    SetNode* temp = 0;
                    if (subsetOf->m_root && (temp = subsetOf->search(id, subsetOf->m_root)) != 0) {
                        if (subsetOf->m_root->m_id == temp->m_id) {
                            if (temp->m_left) {
                                subsetOf->m_root = temp->m_left;
                            } else {
                                subsetOf->m_root = 0;
                            }
                            std::vector<AbstractSet*>* superSetAllSuperSets = subsetOf->getAllSuperSets();
                            for (auto& superSet : *superSetAllSuperSets) {
                                if (superSet->m_root == temp) {
                                    superSet->m_root = subsetOf->m_root;
                                }
                            }
                            delete superSetAllSuperSets;
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
                        std::vector<AbstractSet*>* superSetAllSuperSets = subsetOf->getAllSuperSets();
                        for (auto& superSet : *superSetAllSuperSets) {
                            superSet->m_size--;
                        }
                        subsetOf->m_size--;
                        delete superSetAllSuperSets;
                        return temp;
                    }
                }
                return 0;
            };
            /**
             * polymorphic method to create a new node for placement in the set, or find in parents set
             * @param el the Element we are creating a node for
             * @return the node that was created or found
             **/
            inline virtual SetNode* createNode(T& el) {
                if (m_setToInstantiate) {
                    SetNode* ret = static_cast<Set*>(m_setToInstantiate)->createNode(el);
                    ret->m_guard = m_guard;
                    return ret;
                }
                SetNode* temp = lookForNodeInParents(el.getID());
                if (temp) {
                    return temp;
                }
                SetNode* ret = new SetNode(static_cast<Element*>(&el));
                ret->m_guard = m_guard;
                return ret;
            };
            /**
             * polymorphic method to create a new node for placement in the set, or find in parents set
             * @param id the id of the Element we are creating a node for
             * @return the node that was created or found
             **/
            virtual SetNode* createNode(ID id) {
                if (m_setToInstantiate) {
                    SetNode* ret = static_cast<Set*>(m_setToInstantiate)->createNode(id);
                    ret->m_guard = m_guard;
                    return ret;
                }
                SetNode* temp = lookForNodeInParents(id);
                if (temp) {
                    return temp;
                }
                SetNode* ret = new SetNode();
                ret->m_id = id;
                ret->m_guard = m_guard;
                return ret;
            };
            /**
             * releases the pointer to the element that is being deleted from memory
             * @param id the id of the Element we are releasing
             **/
            void release(ID id) {
                if (m_root) {
                    SetNode* searchResult = search(id, m_root);
                    if (searchResult && searchResult->m_el) {
                        searchResult->m_el = 0;
                    }
                }
            };
            void restore(Element* el) {
                if (m_root) {
                    SetNode* res = search(el->getID(), m_root);
                    if (res) {
                        res->m_el = el;
                        setName(res);
                    }
                }
            };
            /**
             * reindex a node from one id to another without invoking copies or other referenced sets
             * @param oldID the old id of the node
             * @param newID the new id of the node
             **/
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
                            for (auto& superSet : set->m_superSets) {
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
            /**
             * reindex a node from one id to another
             * @param oldID the old id of the node
             * @param newID the new id of the node
             **/
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
                                for (auto& superSet : set->m_superSets) {
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
                    }
                }
            };
            bool reindexName(ID id, std::string newName) {
                if (m_root) {
                    SetNode* match = search(id, m_root);
                    if (match) {
                        match->m_name = newName;
                        return true;
                    } else {
                        return false;
                    }
                }
                return false;
            };
            /**
             * the element is being erased from the model, we must remove it
             * TODO
             **/
            void eraseElement(ID id) {
                // this will always need to search tree (don't know any quicker way)
                if (AbstractSet::contains(id)) {
                    innerRemove(id);
                }
            };
            /**
             * protected method to allow friends to add to a readonly set
             * @param id the id of the element we are adding
             **/
            void addReadOnly(ID id) {
                SetNode* node = createNode(id);
                add(node);
                if (m_el && m_el->m_manager) {
                    m_el->setReference(id);
                }
            };
             /**
             * protected method to allow friends to add to a readonly set
             * @param el the element we are adding
             **/
            void addReadOnly(T& el) {
                SetNode* node = createNode(el);
                add(node);
                if (m_el && m_el->m_manager) {
                    m_el->setReference(&el);
                }
            };
            /**
             * protected method to allow friends to remove from a readonly set
             * @param id the id of the node we are removing
             **/
            void removeReadOnly(ID id) {
                if (m_root) {
                    innerRemove(id);
                    if (m_oppositeFunctor) {
                        T& el = m_el->m_manager->get(m_el, id)->template as<T>();
                        (*m_oppositeFunctor)(el, 1);
                    }
                    if (m_el) {
                        m_el->removeReference(id);
                    }
                } else {
                    throw ID_doesNotExistException(id);
                }
            };
            bool loaded(ID id) {
                SetNode* res = search(id, m_root);
                if (!res) {
                    return false;
                }
                return res->m_el != 0;
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
                            if ((m_root = set->search(rhs.m_root->m_id, set->m_root)) != 0) {
                                break;
                            }
                        }
                    }
                    if (!m_root) {
                        // if haven't found it in supersets, look in subsets supersets
                        for (auto& set : m_subSets) {
                            for (auto& disjointSet : set->m_superSets) {
                                if (disjointSet->m_root) {
                                    if ((m_root = disjointSet->search(rhs.m_root->m_id, disjointSet->m_root)) != 0) {
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
                            if (!m_root && curr->m_id == rhs.m_root->m_id) {
                                m_root = newNode;
                            }
                            if (curr->m_parent) {
                                if (!copy) {
                                    // we need to find parent from tree
                                    for (auto& set : m_superSets) {
                                        // find if parent has already been copied
                                        if ((copy = set->search(curr->m_id, set->m_root)) != 0) {
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
                                    bool rootRight = false;
                                    if (curr->m_right->m_id == og->m_id) {
                                        // without this line it will just keep copying bottom right
                                        if (rhs.m_root->m_right && !m_root->m_right) {
                                            rootRight = true;
                                        } else {
                                            break;
                                        }
                                    }
                                    if (rootRight) {
                                        curr = rhs.m_root->m_right;
                                        copy = m_root;
                                    } else {
                                        curr = curr->m_right;
                                        copy = newNode;
                                    }
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
                            bool subsetNode = false;
                            for (auto& subSet : m_subSets) {
                                if (subSet->m_guard == curr->m_guard) {
                                    subsetNode = true;
                                    break;
                                }
                            }
                            if (curr->m_guard == m_guard || subsetNode) {
                                // we are going to delete it
                                SetNode* currParent = curr->m_parent;
                                // check for superset roots and secondary parents
                                std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                                for (auto& subsetOf : *allSuperSets) {
                                    if (subsetOf->m_root == curr) {
                                        subsetOf->m_root = subsetOf->m_root->m_left;
                                    } else if (currParent && subsetOf->m_root) {
                                        // confirm that it is not a parent of the placeholder
                                        SetNode* tempParent = currParent;
                                        while (tempParent && tempParent != subsetOf->m_root) {
                                            tempParent = tempParent->m_parent;
                                        }
                                        if (!tempParent) {
                                            // this set owns this element through a different parent
                                            // we must find it and set the pointer to curr to 0
                                            SetNode* temp = subsetOf->m_root;
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
                                                    // this is probably a diamond subset, we wont find it
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
                                }
                                delete allSuperSets;
                                if (curr == m_root) {
                                    for (auto& redefinedSet : m_redefines) {
                                        redefinedSet-> m_root = 0;
                                    }
                                }
                                if (currParent) {
                                    if (currParent->m_left == curr) {
                                        currParent->m_left = 0;
                                    } else {
                                        currParent->m_right = 0;
                                    }
                                    if (currParent->m_id == placeholderID) {
                                        if (currParent->m_left) {
                                            if (currParent->m_parent) {
                                                // intermediate placeholder edge case
                                                if (!curr->m_parent->m_right || currParent->m_parent->m_right->m_guard <= m_guard) {
                                                    currParent->m_parent->m_left = currParent->m_left;
                                                    currParent->m_left->m_parent = currParent->m_parent;
                                                    std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                                                    for (auto& subsetOf : *allSuperSets) {
                                                        if (subsetOf->m_root == currParent) {
                                                            subsetOf->m_root = currParent->m_left;
                                                        }
                                                    }
                                                    delete allSuperSets;
                                                    deleteNode(curr);
                                                    delete currParent;
                                                    break;
                                                }
                                            } else {
                                                std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                                                for (auto& subsetOf : *allSuperSets) {
                                                    if (subsetOf->m_root == currParent) {
                                                        subsetOf->m_root = currParent->m_left;
                                                    }
                                                }
                                                if (currParent->m_left->m_parent == currParent) {
                                                    currParent->m_left->m_parent = 0;
                                                }
                                                delete allSuperSets;
                                                deleteNode(curr);
                                                delete currParent;
                                                break;
                                            }
                                        } else if (currParent->m_right) {
                                            if (!currParent->m_parent) {
                                                std::vector<AbstractSet*>* allSuperSetsTemp = getAllSuperSets();
                                                std::vector<AbstractSet*> allSuperSets;
                                                /**
                                                 * TODO: implement this in other conditions for other edge case configurations
                                                 **/
                                                for (auto& subsetOf : *allSuperSetsTemp) {
                                                    allSuperSets.push_back(subsetOf);
                                                }
                                                for (auto& subsetOf : *allSuperSetsTemp) {
                                                    for (auto& subset : subsetOf->m_subSets) {
                                                        if (std::find(allSuperSets.begin(), allSuperSets.end(), subset) == allSuperSets.end()) {
                                                            allSuperSets.push_back(subset);
                                                            std::vector<AbstractSet*>* subsetAllSuperSets = subset->getAllSuperSets();
                                                            for (auto& subsetSuperSet : *subsetAllSuperSets) {
                                                                if (std::find(allSuperSets.begin(), allSuperSets.end(), subsetSuperSet) == allSuperSets.end()) {
                                                                    allSuperSets.push_back(subsetSuperSet);
                                                                }
                                                            }
                                                            delete subsetAllSuperSets;
                                                        }
                                                    }
                                                }
                                                for (auto& subsetOf : allSuperSets) {
                                                    if (subsetOf->m_root == currParent) {
                                                        subsetOf->m_root = currParent->m_right;
                                                    }
                                                }
                                                if (currParent->m_right->m_parent == currParent) {
                                                    currParent->m_right->m_parent = 0;
                                                }
                                                delete allSuperSetsTemp;
                                                deleteNode(curr);
                                                delete currParent;
                                                break;
                                            }
                                        }
                                    }
                                }
                                deleteNode(curr);
                                curr = currParent;
                            } else {
                                if (curr->m_id == placeholderID && !curr->m_left && !curr->m_right) {
                                    // we've created an empty placeholder node, lets get rid of it for now
                                    std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                                    for (auto& subsetOf : *allSuperSets) {
                                        if (subsetOf->m_root == curr) {
                                            subsetOf->m_root = 0;
                                        }
                                        for (auto& subset : subsetOf->m_subSets) {
                                            if (subset != this && 
                                                std::find(m_redefines.begin(), m_redefines.end(), subset) == m_redefines.end() && 
                                                subset->m_root && 
                                                subset->m_root->m_parent == curr) {
                                                subset->m_root->m_parent = curr->m_parent;
                                                if (curr->m_parent) {
                                                    curr->m_parent->m_left = subset->m_root;
                                                }
                                            }
                                            std::vector<AbstractSet*>* subsetAllSupersets = subset->getAllSuperSets();
                                            for (auto& subsetSuperSet : *subsetAllSupersets) { // triple for loop seems slow :/ but is based on complexity not size
                                                if (subsetSuperSet != subsetOf && subsetSuperSet->m_root && subsetSuperSet->m_root->m_parent == curr) {
                                                    subsetSuperSet->m_root->m_parent = 0;
                                                }
                                            }
                                            delete subsetAllSupersets;
                                        }
                                    }
                                    delete allSuperSets;
                                    if (curr->m_parent && curr->m_parent->m_left == curr) {
                                        curr->m_parent->m_left = 0;
                                    }
                                    delete curr;
                                }
                                break;
                            }
                        }
                    }
                }
                for (auto& subSet : m_subSets) {
                    subSet->m_root = 0; // edge case for m_parts
                }
                if (m_ownsOppositeFunctor) {
                    delete m_oppositeFunctor;
                }
                for (auto& func : m_addFunctors) {
                    bool deleteFunc = true;
                    for (auto& set : m_superSets) {
                        if (set->m_addFunctors.count(func)) {
                            deleteFunc = false;
                            break;
                        }
                    }
                    if (deleteFunc) {
                        for (auto& set : m_redefines) {
                            if (set->m_addFunctors.count(func)) {
                                deleteFunc = false;
                                break;
                            }
                        }
                    }
                    if (deleteFunc) {
                        delete func;
                    } else {
                        for (auto& subset : m_subSets) {
                            subset->m_addFunctors.erase(func);
                        }
                    }
                }
                for (auto& func : m_removeFunctors) {
                    bool deleteFunc = true;
                    for (auto& set : m_superSets) {
                        if (set->m_removeFunctors.count(func)) {
                            deleteFunc = false;
                            break;
                        }
                    }
                    if (deleteFunc) {
                        for (auto& set : m_redefines) {
                            if (set->m_removeFunctors.count(func)) {
                                deleteFunc = false;
                                break;
                            }
                        }
                        if (deleteFunc) {
                            delete func;
                        }
                    } else {
                        for (auto& subset : m_subSets) {
                            subset->m_removeFunctors.erase(func);
                        }
                    }
                }
                for (auto& set : m_redefines) {
                    std::vector<AbstractSet*>::iterator it;
                    if ((it = std::find(set->m_redefines.begin(), set->m_redefines.end(), this)) != set->m_redefines.end()) {
                        set->m_redefines.erase(it);
                    }
                }
                for (auto& subsetOf : m_superSets) {
                    subsetOf->m_subSets.erase(std::find(subsetOf->m_subSets.begin(), subsetOf->m_subSets.end(), this));
                }
                for (auto& subset : m_subSets) {
                    if (std::find(subset->m_superSets.begin(), subset->m_superSets.end(), this) != subset->m_superSets.end()) {
                        subset->m_superSets.erase(std::find(subset->m_superSets.begin(), subset->m_superSets.end(), this));
                    }
                }
            };
            /**
             * this set subsets the set supplied, meaning all elements within this set will be contained within the set supplied
             * but this set will not necessarily have all of the elements within the set supplied
             * @param subsetOf the set that we are a subset of
             **/
            template <class V = Element, class W = Element> void subsets(Set<V, W>& subsetOf) {
                if (std::find(m_superSets.begin(), m_superSets.end(), &subsetOf) == m_superSets.end()) {
                    m_superSets.push_back(&subsetOf);
                    std::vector<AbstractSet*>* allSuperSets = getAllSuperSets();
                    for (auto& subsetOfSubsetOf : *allSuperSets) {
                        if (m_guard < subsetOfSubsetOf->m_nextSubsetGuard) {
                            m_guard = subsetOfSubsetOf->m_nextSubsetGuard;
                        }
                    }
                    size_t max_guard = m_guard;
                    for (auto& subset : m_subSets) {
                        if (subset->m_guard <= max_guard) {
                            subset->m_guard = max_guard + 1;
                        }
                        max_guard++;
                    }
                    for (auto& subsetOfSubsetOf : *allSuperSets) {
                        if (max_guard >= subsetOfSubsetOf->m_nextSubsetGuard) {
                            subsetOfSubsetOf->m_nextSubsetGuard = max_guard + 1;
                        }
                    }
                    delete allSuperSets;
                    subsetOf.m_subSets.push_back(this);
                    for (auto& redefinedSet : subsetOf.m_redefines) {
                        redefinedSet->m_guard = subsetOf.m_guard;
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
                    if (subsetOf.m_oppositeFunctor && 
                        std::find(m_otherOpposites.begin(), m_otherOpposites.end(), subsetOf.m_oppositeFunctor) == m_otherOpposites.end()) {
                        m_otherOpposites.push_back(subsetOf.m_oppositeFunctor);
                    }
                    for (auto& op : subsetOf.m_otherOpposites) {
                        if (std::find(m_otherOpposites.begin(), m_otherOpposites.end(), op) == m_otherOpposites.end()) {
                            m_otherOpposites.push_back(op);
                        }
                    }
                    if (!m_root) {
                        m_root = subsetOf.m_root;
                    } else if (!subsetOf.m_root) {
                        subsetOf.m_root = m_root;
                    }
                    if (subsetOf.m_upper < 0 && subsetOf.m_upper != m_upper) {
                        m_setToInstantiate = &subsetOf;
                    }
                }
            };
            /**
             * creates a relationship between two sets by signature that if an element is added the set
             * the other set referenced by the signature supplied will have this sets owning element within it
             * @param op the signature for the set that this set is opposite of
             **/
            void opposite(Set<U, T>& (T::*op)()) {
                /** TODO: static_assert that we have m_el for this instance **/
                if (m_el) {
                    OppositeFunctor<T,U>* opFunc = new OppositeFunctor<T,U>(dynamic_cast<U*>(m_el));
                    opFunc->m_signature = op;
                    m_oppositeFunctor = opFunc;
                    m_ownsOppositeFunctor = true;
                    m_otherOpposites.clear();
                    for (auto& redefined : m_redefines) {
                        Set* redefinedSet = static_cast<Set*>(redefined);
                        if (redefinedSet->m_ownsOppositeFunctor) {
                            delete redefinedSet->m_oppositeFunctor;
                        }
                        redefinedSet->m_oppositeFunctor = opFunc;
                        redefinedSet->m_ownsOppositeFunctor = false;
                        redefinedSet->m_otherOpposites.clear();
                    }
                } else {
                    throw ManagerStateException("WARN: opposite called when there is no element owning the set, make sure to use proper constructor!");
                }
            };
            /**
             * makes sure that the set we are redefining is the same tree as ours and vice versa
             * @param redefined, the set that this set is redefining
             **/
            template <class V = Element, class W = Element> void redefines(Set<V, W>& redefined) {
                if (m_root) {
                    throw ManagerStateException("WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!");
                    return;
                }
                m_redefines.push_back(&redefined);
                redefined.m_redefines.push_back(this);
                redefined.m_rootRedefinedSet = false;
                for (auto& set : redefined.m_superSets) {
                    subsets(*static_cast<Set*>(set));
                }
                for (auto& subSet : redefined.m_subSets) {
                    m_subSets.push_back(subSet);
                    // subSet->m_superSets.push_back(this);
                }
                for (auto& func : redefined.m_addFunctors) {
                    if (!m_addFunctors.count(func)) {
                        m_addFunctors.insert(func);
                    }
                }
                for (auto& func : redefined.m_removeFunctors) {
                    if (!m_removeFunctors.count(func)) {
                        m_removeFunctors.insert(func);
                    }
                }
                if (redefined.m_ownsOppositeFunctor && !m_ownsOppositeFunctor) {
                    m_oppositeFunctor = redefined.m_oppositeFunctor;
                    m_otherOpposites.clear();
                } else {
                    for (auto& opp : redefined.m_otherOpposites) {
                        m_otherOpposites.push_back(opp);
                    }
                }
                if (redefined.m_upper < 0 && redefined.m_upper != m_upper) {
                    m_setToInstantiate = &redefined;
                } else if (m_upper < 0 && redefined.m_upper >= 0) {
                    redefined.m_setToInstantiate = this;
                }
                m_guard = redefined.m_guard;
            };

            void addFunctor(SetFunctor* func) {
                m_addFunctors.insert(func);
            };

            void removeFunctor(SetFunctor* func) {
                m_removeFunctors.insert(func);
            };

            void setReadOnly(bool readOnly) {
                m_readOnly = readOnly;
            }

            // TODO expand
            typedef Set<U,T>& (T::*oppositeSignature)();
            oppositeSignature getOpposite() const {
                if (m_oppositeFunctor) {
                    return dynamic_cast<OppositeFunctor<T,U>*>(m_oppositeFunctor)->m_signature;
                }
                return 0;
            };
            /**
             * removes an element with the supplied id from this set but keeps it within its supersets, not its subsets
             * @param id the id of the node we want to remove from just this set
             **/
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
            /**
             * Add an element to this set and all of its supersets
             * @param el the element being added to the set
             **/
            void add(T& el) {
                if (m_readOnly) {
                    throw ReadOnlySetException(el.getID().string());
                }
                nonOppositeAdd(el);
                if (m_oppositeFunctor) {
                    (*m_oppositeFunctor)(el);
                }
                for (auto& op : m_otherOpposites) {
                    (*op)(el);
                }
            };
             /**
             * Add an element to this set and all of its supersets
             * @param el the element being added to the set
             * @param els variardic variable to add more than one element in method call
             **/
            template <class ... Ts> void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            };
            /**
             * Adds an element to the set just using its id
             * @param id the id of the element we want to add to the set
             **/
            void add(ID id) {
                if (m_readOnly) {
                    throw ReadOnlySetException(id.string());
                }
                SetNode* node = createNode(id);
                add(node);
                if (m_el && m_el->m_manager) {
                    m_el->setReference(id);
                }
            };
            /**
             * removes the supplied element from this set
             * @param el the element being removed from the set
             **/
            void remove(T& el) {
                remove(el.getID());
            };
            /**
             * removes the element with the given id from the set
             * @param id the id of the element being removed from the set
             **/
            void remove(ID id) {
                if (m_readOnly) {
                    throw ReadOnlySetException(id.string());
                }
                if (m_root) {
                    innerRemove(id);
                    if (m_oppositeFunctor) {
                        T& el = m_el->m_manager->get(m_el, id)->template as<T>();
                        (*m_oppositeFunctor)(el, 1);
                    }
                    for (auto& op : m_otherOpposites) {
                        T& el = m_el->m_manager->get(m_el, id)->template as<T>();
                        (*op)(el, 1);
                    }
                    if (m_el) {
                        m_el->removeReference(id);
                    }
                } else {
                    throw ID_doesNotExistException(id);
                }
            };
            /**
             * gets the element with the given id from the set
             * @param id the id of the element we want to get
             * @return the element with the supplied id
             **/
            T& get(ID id) {
                if (m_root) {
                    SetNode* searchResult = search(id, m_root);
                    if (!searchResult) {
                        throw ID_doesNotExistException(id);
                    }
                    if (!searchResult->m_el) {
                        searchResult->m_el = m_el->m_manager->get(m_el, id);
                    }
                    T* ret = dynamic_cast<T*>(searchResult->m_el);
                    if (ret) {
                        return *ret;
                    }
                }
                throw ID_doesNotExistException(id);
            };
            /**
             * gets the element with the given name from the set
             * @param name the name of the element to retreive from the set
             * @return the first element with name supplied
             **/
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
            /**
             * Gets the i'th element in the set, no guarantee to be the same order after mutation
             * @param i the index of the element in the set
             * @return the element at the index supplied
             **/
            T& get(int i) {
                size_t size = m_size;
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
                    if (node->m_id == placeholderID) {
                        size = size * 2;
                        size ++;
                    }
                }
                if (!node->m_el) {
                    node->m_el = m_el->m_manager->get(m_el, node->m_id);
                }
                return *dynamic_cast<T*>(node->m_el);
            };
            /**
             * gets the first element in the set, no guarantee to be the same after mutation
             * @return the first element in the set
             **/
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
            /** 
             * gets the last element in the set, no guarantee to be the same after mutation
             * @return the last element in the set
             **/
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
                return AbstractSet::contains(id);
            };
            /**
             * return a bool on whether the element supplied is in the set
             * @param el the element to be determined is in the set
             * @return true means the element supplied is in the set, false means the element supplied is not in the set
             **/
            bool contains(T& el) {
                return AbstractSet::contains(el.getID());
            };
            /**
             * returns the number of times the id supplied is in the set
             * @param id the id of the element to be determined is in the set
             * @return 1 means the id supplied is in the set, 0 means the id supplied is not in the set
             **/
            int count(ID id) {
                if (contains(id)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            /**
             * returns the number of times the id supplied is in the set
             * @param el the element to be determined is in the set
             * @return 1 means the id supplied is in the set, 0 means the id supplied is not in the set
             **/
            int count(T& el) {
                return count(el.getID());
            };
            /**
             * return a bool on whether the element supplied is in the set
             * @param name the name of the element to be determined is in the set
             * @return true means the element supplied is in the set, false means the element supplied is not in the set
             **/
            bool contains(std::string name) {
                bool ret = false;
                if (m_root) {
                    SetNode* t = search(name, m_root);
                    ret = t != 0;
                }
                return ret;
            };
            /**
             * return an int on whether the element supplied is in the set
             * @param name the name of the element to be determined is in the set
             * @return 1 means the element supplied is in the set, 0 means the element supplied is not in the set
             **/
            int count(std::string name) {
                if (contains(name)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            /** 
             * method to determine if any elements are contained in the set
             * @return true means there is atleast one element in the set, false means there are no elements in the set
             **/
            bool empty() const {
                return m_root == 0;
            };
            /**
             * return the number of elements in the set
             * @return the number of elements in the set
             **/
            size_t size() const { 
                return m_size; 
            };
            /**
             * returns an iterator to the beginning of the set
             * @return the iterator
             **/
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
                it.m_root = m_root;
                it.m_guard = m_guard;
                return it;
            };
            /** 
             * returns an iterator to the end of the set
             * @return the iterator
             **/
            SetIterator<T> end() {
                SetIterator<T> it;
                it.m_node = &it.m_endNode;
                it.m_el = m_el;
                it.m_root = m_root;
                it.m_guard = m_guard;
                return it;
            };
            /**
             * returns an object that lets you iterate through just the ids of the set without accessing the elements
             * @return the ID_Set object that has basic begin and end methods to id iterators
             **/
            ID_Set<T> ids() {
                ID_Set<T> set;
                set.m_root = m_root;
                set.m_guard = m_guard;
                set.set = this;
                return set;
            };
    };

    template <class T = Element> struct SetIterator {

        template <class V, class W> friend class Set;
        friend class AbstractSet;
        template <class V> friend struct ID_Set;

        protected:
            Element* m_el = 0;
            AbstractSet::SetNode* m_node = 0;
            AbstractSet::SetNode* m_root = 0;
            AbstractSet::SetNode m_endNode;
            size_t m_guard = 0;
        public:

            SetIterator(const SetIterator& rhs) {
                m_el = rhs.m_el;
                if (rhs.m_node != &rhs.m_endNode) {
                    m_node = rhs.m_node;
                } else {
                    m_node = &m_endNode;
                }
                m_root = rhs.m_root;
                m_guard = rhs.m_guard;
            };

            SetIterator() {};

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
                        if (!m_node->m_parent/** || m_node->m_parent->m_guard < m_guard**/) {
                            // if there is no parent to go to we must end
                            m_node = &m_endNode;
                            break;
                        }
                        // we hit bottom, choose next right
                        AbstractSet::SetNode* temp;
                        AbstractSet::SetNode* last = dynamic_cast<AbstractSet::SetNode*>(m_node);
                        if (last == m_root) {
                            m_node = &m_endNode;
                            break;
                        }
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
                        } while (temp->m_parent && temp->m_parent->m_guard >= m_guard);
                        if (!found) {
                            m_node = &m_endNode;
                            break;
                        } else {
                            if (temp->m_right == m_node) {
                                m_node = &m_endNode;
                                break;
                            }
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
                        if (!m_node->m_parent /** || m_node->m_parent->m_guard < m_guard**/) {
                            // if there is no parent to go to we must end
                            m_node = &m_endNode;
                            break;
                        }
                        // we hit bottom, choose next right
                        AbstractSet::SetNode* temp;
                        AbstractSet::SetNode* last = dynamic_cast<AbstractSet::SetNode*>(m_node);
                        if (last == m_root) {
                            m_node = &m_endNode;
                            break;
                        }
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
                        } while (temp->m_parent && temp->m_parent->m_guard >= m_guard);
                        if (!found) {
                            m_node = &m_endNode;
                            break;
                        } else {
                            if (temp->m_right == m_node) {
                                m_node = &m_endNode;
                                break;
                            }
                            m_node = dynamic_cast<AbstractSet::SetNode*>(temp->m_right);
                        }
                    }
                } while (m_node->m_id == placeholderID);
                SetIterator ret = *this;
                return ret;
            };
            inline friend bool operator== (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id == b.m_node->m_id; };
            inline friend bool operator!= (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id != b.m_node->m_id; };
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
            size_t m_guard = 0;
            AbstractSet* set  = 0;
        public:
            SetID_Iterator<T> begin() {
                SetID_Iterator<T> it;
                it.m_node = m_root;
                it.m_root = m_root;
                it.m_guard = m_guard;
                if (it.m_node->m_id == placeholderID) {
                    it++;
                }
                return it;
            };
            SetID_Iterator<T> end() {
                SetID_Iterator<T> it;
                it.m_node = &it.m_endNode;
                it.m_root = m_root;
                it.m_guard = m_guard;
                return it;
            };
            ID front() {
                SetID_Iterator<T> it;
                it.m_node = m_root;
                it.m_root = m_root;
                if (it.m_node->m_id == placeholderID) {
                    it++;
                }
                return *it;
            };
            ID get(std::string name) {
                AbstractSet::SetNode* res = set->search(name, m_root);
                if (res) {
                    return res->m_id;
                }
                return ID::nullID();
            };
    };
}

#endif
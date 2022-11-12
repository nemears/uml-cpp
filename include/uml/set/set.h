#ifndef _UML_SET_SET_H_
#define _UML_SET_SET_H_

#include "uml/element.h"
#include "setLock.h"

namespace UML {

    struct SetNode {
        ElementPtr m_ptr;
        SetNode* m_parent = 0;
        SetNode* m_left = 0;
        SetNode* m_right = 0;
        unsigned int m_guard = 0;
    };

    template <class T, class U> class TypedSet;

    class AbstractSet2 {

        template <class V, class W, class OtherCreationPolicy> friend class Set2;

        protected:
            SetNode* m_root = 0;

            unsigned int m_guard = 0;
            std::vector<AbstractSet2*> m_superSets;
            std::vector<AbstractSet2*> m_subSets;
            std::vector<AbstractSet2*> m_redefines;

            size_t m_size = 0;

            /**
             * Gets all sets that are a superset of this one in dfs order and stored in a pointer to a vector, 
             * responsibility of vector destruction is required
             * TODO: fix subsets method to make sure it will be 'proper' bfs order for algorithms
             * @return the pointer to a vector of all supersets of this set
             **/
            std::vector<AbstractSet2*>* getAllSuperSets() {
                std::list<AbstractSet2*> stack;
                std::vector<AbstractSet2*>* allSuperSets = new std::vector<AbstractSet2*>;
                for (auto& subsetOf : this->m_superSets) {
                    stack.push_back(subsetOf);
                }
                while (!stack.empty()) {
                    AbstractSet2* front = stack.front();
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
        public:
            /**
             * this set subsets the set supplied, meaning all elements within this set will be contained within the set supplied
             * but this set will not necessarily have all of the elements within the set supplied
             * @param subsetOf the set that we are a subset of
             **/
            template <class V = Element, class W = Element>
            void subsets(TypedSet<V, W>& subsetOf) {
                if (std::find(m_superSets.begin(), m_superSets.end(), &subsetOf) == m_superSets.end()) {
                    m_superSets.push_back(&subsetOf);
                    std::vector<AbstractSet2*>* allSuperSets = getAllSuperSets();
                    for (auto& subsetOfSubsetOf : *allSuperSets) {
                        if (m_guard < subsetOfSubsetOf->m_guard + 1) {
                            m_guard = subsetOfSubsetOf->m_guard + 1;
                        }
                    }
                    size_t max_guard = m_guard;
                    for (auto& subset : m_subSets) {
                        if (subset->m_guard <= max_guard) {
                            subset->m_guard = max_guard + 1;
                        }
                        max_guard++;
                    }
                    // for (auto& subsetOfSubsetOf : *allSuperSets) {
                    //     if (max_guard >= subsetOfSubsetOf->m_nextSubsetGuard) {
                    //         subsetOfSubsetOf->m_nextSubsetGuard = max_guard + 1;
                    //     }
                    // }
                    delete allSuperSets;
                    subsetOf.m_subSets.push_back(this);
                    for (auto& redefinedSet : subsetOf.m_redefines) {
                        redefinedSet->m_guard = subsetOf.m_guard;
                    }
                    // for (const auto& set : subsetOf.m_addFunctors) {
                    //     if (!m_addFunctors.count(set)) {
                    //         m_addFunctors.insert(set);
                    //     }
                    // }
                    // for (auto& set : subsetOf.m_removeFunctors) {
                    //     if (!m_removeFunctors.count(set)) {
                    //         m_removeFunctors.insert(set);
                    //     }
                    // }
                    // if (subsetOf.m_oppositeFunctor && 
                    //     std::find(m_otherOpposites.begin(), m_otherOpposites.end(), subsetOf.m_oppositeFunctor) == m_otherOpposites.end()) {
                    //     m_otherOpposites.push_back(subsetOf.m_oppositeFunctor);
                    // }
                    // for (auto& op : subsetOf.m_otherOpposites) {
                    //     if (std::find(m_otherOpposites.begin(), m_otherOpposites.end(), op) == m_otherOpposites.end()) {
                    //         m_otherOpposites.push_back(op);
                    //     }
                    // }
                    // if (subsetOf.m_upper < 0 && subsetOf.m_upper != m_upper) {
                    //     m_setToInstantiate = &subsetOf;
                    // }
                }
            }
    };

    template <class T, class U>
    class TypedSet : public AbstractSet2 {

        template <class V, class W, class OtherCreationPolicy> friend class Set2;

        protected:
            virtual void innerAdd(T& el) = 0;
    };

    template <class T>
    class SetCreationPolicy {
        protected:
            SetNode* create(T& el) {
                SetNode* ret = new SetNode();
                ret->m_ptr = &el;
                return ret;
            }
    };

    template <
                class T, 
                class U, 
                class CreationPolicy = SetCreationPolicy<T>
            >
    class Set2 : public TypedSet<T, U> , public CreationPolicy {

        protected:
            U& m_el;
            bool m_readOnly = false;
            TypedSet<U,T>& (T::*m_oppositeSignature)() = 0;

            /**
             * Places the node within the tree taking in account the id and the guard of the node
             * @param node the new node being placed within the tree
             * @param parent the node that you want to place the new node below
             **/
            void place(SetNode* node, SetNode* parent) {
                if (node->m_ptr && node->m_ptr.id() == parent->m_ptr.id()) {
                    delete node;
                    throw DuplicateElementInSetException();
                }
                if (parent->m_left) {
                    // prefer placement to left
                    if (parent->m_right) {
                        // both children are populated, determine where to placenewID
                        if (node->m_ptr.id() > parent->m_right->m_ptr.id() && parent->m_left->m_guard <= this->m_guard) {
                            // place to left if greater than right
                            place(node, parent->m_left);
                        } else if (parent->m_right->m_guard <= this->m_guard) {
                            // place to right if less than right
                            place(node, parent->m_right);
                        } else {
                            if (node->m_ptr.id() > parent->m_right->m_ptr.id()) {
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
                        if (node->m_ptr.id() > parent->m_left->m_ptr.id()) {
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
            SetNode* search(ID id, SetNode* node) const {
                if (node->m_ptr.id() == id) {
                    // found match
                    return node;
                } else if (node->m_ptr) { // check for placeholder
                    if (node->m_right) {
                        // if there is a right there is both children filled out
                        if (id > node->m_right->m_ptr.id()) {
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

            void innerAdd(T& el) override {
                // add
                SetNode* node = CreationPolicy::create(el);
                innerAddDFS(node, this);
            }

            /**
             * searches superSet structure by finding the roots first and checking if it was placed
             * @param node: the node being placed
             * @param set: the current set we are checking
            */
            void innerAddDFS(SetNode* node, AbstractSet2* set) {
                if (!set->m_root) {
                    set->m_root = node;
                    set->m_size++;
                    for (AbstractSet2* superSet : set->m_superSets) {
                        innerAddDFS(node, superSet);
                    }
                    return;
                }
                
                if (set->m_superSets.size() == 0) {
                    // root set, place the node in the tree
                    
                }

                // place in supersets
                for (AbstractSet2* superSet : set->m_superSets) {
                    innerAddDFS(node, superSet);
                }
            }
        public:
            Set2(U& el) : m_el(el) {
                
            }
            Set2(U* el) : m_el(*el) {

            }
            virtual ~Set2() {
                // TODO
            }
            void opposite(TypedSet<U,T>& (T::*oppositeSignature)()) {
                m_oppositeSignature = oppositeSignature;
            }
            void add(UmlPtr<T> el) {
                add(*el);
            }
            void add(T& el) {
                // "lock" elements we are editing
                SetLock elLock = m_el.m_manager->lockEl(el);
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                if (m_readOnly) {
                    // TODO throw error
                }
                // add
                innerAdd(el);
                // handle opposites
                if (m_oppositeSignature) {
                    (el.*m_oppositeSignature)().innerAdd(m_el);   
                }
            }
            UmlPtr<T> get(ID id) const {
                // "lock" sets owner while we search
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                
                // get
                if (!m_root) {
                    // TODO throw error
                }
                SetNode* result = search(id, m_root);

                if (result) {
                    return result->m_ptr;
                }
                // TODO throw error
            }

            bool contains(T& el) const {
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                
                // get
                if (!m_root) {
                    return false;
                }
                SetNode* result = search(el.getID(), m_root);

                return result;
            }

            bool contains(UmlPtr<T> el) const {
                return contains(*el);
            }

            size_t size() const {
                return this->m_size;
            }
    };
}

#endif
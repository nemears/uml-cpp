#ifndef _UML_SET_SET_H_
#define _UML_SET_SET_H_

#include "uml/element.h"
#include "setLock.h"

namespace UML {

    class SetStateException : public std::exception {
        std::string m_msg;
        public:
            SetStateException(){};
            SetStateException(std::string msg) : m_msg("Error in Uml Set: " + msg) {};
            const char* what() const throw() override {
                return m_msg.c_str();
            };
    };

    class AbstractSet2;

    struct SetNode {
        ElementPtr m_ptr;
        SetNode* m_parent = 0;
        SetNode* m_left = 0;
        SetNode* m_right = 0;
        AbstractSet2* set = 0;
    };

    template <class T, class U> class TypedSet;

    class AbstractSet2 {

        template <class V, class W, class OtherCreationPolicy> friend class Set2;

        protected:
            SetNode* m_root = 0;

            std::vector<AbstractSet2*> m_superSets;
            std::vector<AbstractSet2*> m_subSets;
            std::vector<AbstractSet2*> m_redefines;

            size_t m_size = 0;

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
                    subsetOf.m_subSets.push_back(this);
                    // for (auto& redefinedSet : subsetOf.m_redefines) {
                    //     redefinedSet->m_guard = subsetOf.m_guard;
                    // }
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
                node->set = this;
                innerAddDFS(node, this);
            }

            /**
             * searches superSet structure by finding the roots first and checking if it was placed
             * @param node: the node being placed
             * @param set: the current set we are checking
            */
            void innerAddDFS(SetNode* node, AbstractSet2* set) {
                // place in supersets
                for (AbstractSet2* superSet : set->m_superSets) {
                    innerAddDFS(node, superSet);
                }
                
                if (set->m_superSets.size() == 0) {
                    // root set, place the node in the tree
                    // gather our sets supersets to place the node correctly
                    std::unordered_set<AbstractSet2*> allSuperSets;
                    {
                        allSuperSets.insert(this);
                        std::list<AbstractSet2*> queue;
                        for (auto superSet : this->m_superSets) {
                            queue.push_back(superSet);
                        }
                        while (!queue.empty()) {
                            AbstractSet2* front = queue.front();
                            queue.pop_front();
                            allSuperSets.insert(front);
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                    std::unordered_set<AbstractSet2*> allSubSets;
                    {
                        // allSubSets.insert(this);
                        std::list<AbstractSet2*> queue;
                        for (auto subSet : this->m_subSets) {
                            queue.push_back(subSet);
                        }
                        while (!queue.empty()) {
                            AbstractSet2* front = queue.front();
                            queue.pop_front();
                            allSubSets.insert(front);
                            for (auto subSet : front->m_subSets) {
                                queue.push_back(subSet);
                            }
                        }
                    }
                    SetNode* currNode = set->m_root;
                    // handle divider nodes
                    while (currNode && ((!allSuperSets.count(currNode->set) && !allSubSets.count(currNode->set)) || (!currNode->m_ptr && currNode->m_right))) {
                        bool createDividerNode = true;
                        if (!currNode->m_ptr) {
                            /**
                             * This is a divider node : m_ptr is null
                             * 
                             * Divider nodes represent a way to keep all subsets of
                             * a superset as part of the superset yet also keeping the
                             * necessary subsets seperate
                            */

                            createDividerNode = !allSuperSets.count(currNode->set);

                            if (!createDividerNode) {
                                if (allSubSets.count(currNode->m_left->set) || currNode->m_left->set == this) {
                                    currNode = currNode->m_left;
                                } else if (allSubSets.count(currNode->m_right->set) || currNode->m_right->set == this) {
                                    currNode = currNode->m_right;
                                } else {
                                    throw SetStateException("INTERNAL ERROR please report! Could not navigate through divider node");
                                }
                            }
                        }
                        if (createDividerNode) {
                            SetNode* dividerNode = new SetNode();
                            // find most similar set between node and currNode
                            AbstractSet2* dividerNodeScope = 0;
                            {
                                std::list<AbstractSet2*> queue;
                                for (auto superSet: currNode->set->m_superSets) {
                                    queue.push_back(superSet);
                                }
                                while (!queue.empty()) {
                                    AbstractSet2* superSet = queue.front();
                                    queue.pop_front();
                                    if (allSuperSets.count(superSet)) {
                                        dividerNodeScope = superSet;
                                    }
                                    for (auto superSuperSet : superSet->m_superSets) {
                                        queue.push_back(superSuperSet);
                                    }
                                }
                                if (!dividerNodeScope) {
                                    throw SetStateException("INTERNAL ERROR please report! Could not find shared set for divider node");
                                }
                            }
                            dividerNode->set = dividerNodeScope;
                            currNode->m_parent = dividerNode;
                            dividerNode->m_left = currNode;

                            // readjust roots if needed
                            {
                                std::list<AbstractSet2*> queue;
                                queue.push_back(set);
                                while (!queue.empty()) {
                                    AbstractSet2* front = queue.front();
                                    queue.pop_front();
                                    std::list<AbstractSet2*> frontQueue;
                                    if (front->m_root == currNode && allSuperSets.count(front)) {
                                        front->m_root = dividerNode;
                                    }
                                    for (auto subSet : front->m_subSets) {
                                        queue.push_back(subSet);
                                    }
                                }
                            }

                            currNode = dividerNode; 
                        }
                    }
                    while (currNode) {

                        if (currNode->m_ptr.id() == node->m_ptr.id() && currNode->set != this) {
                            // node is allready in supersets, adjust node to be in lower sets
                            // TODO
                        }

                        if (allSubSets.count(currNode->set)) {
                            // this is a subsets node, we must place it below ours
                            node->m_parent = currNode->m_parent;
                            node->m_left = currNode;
                            currNode->m_parent = node;
                            if (node->m_parent->m_left == currNode) {
                                node->m_parent->m_left = node;
                            } else if (node->m_parent->m_right == currNode) {
                                node->m_parent->m_right = node;
                            }

                            // adjust roots
                            for (auto superSet : allSuperSets) {
                                if (superSet->m_root == currNode) {
                                    superSet->m_root = node;
                                }
                            }
                            break;
                        }
                        
                        // no children, place to left
                        if (!currNode->m_left) {
                            currNode->m_left = node;
                            node->m_parent = currNode;
                            break;
                        }

                        // one child determine where to place
                        if (!currNode->m_right) {
                            if (node->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                                currNode->m_right = currNode->m_left;
                                currNode->m_left = node;
                            } else {
                                currNode->m_right = node;
                            }
                            node->m_parent = currNode;
                            break;
                        }

                        // two children, pick side to recurse
                        if (node->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                            currNode = currNode->m_left;
                        } else {
                            currNode = currNode->m_right;
                        }
                    }
                    for (auto superSet : allSuperSets) {
                        superSet->m_size++;
                    }
                }

                if (!set->m_root) {
                    set->m_root = node;
                }
            }
        public:
            Set2(U& el) : m_el(el) {
                
            }
            Set2(U* el) : m_el(*el) {

            }
            virtual ~Set2() {
                SetLock myLock = m_el.m_manager->lockEl(m_el); 

                // TODO lock all of the elements in the set lock?

                std::unordered_set<AbstractSet2*> allSuperSets;
                {
                    std::list<AbstractSet2*> queue;
                    for (auto superSet : this->m_superSets) {
                        queue.push_back(superSet);
                    }
                    while (!queue.empty()) {
                        AbstractSet2* front = queue.front();
                        queue.pop_front();
                        allSuperSets.insert(front);
                        for (auto superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }

                // start from bottom left
                SetNode* currNode = this->m_root;
                while (currNode && currNode->m_left) {
                    currNode = currNode->m_left;
                }

                while (currNode) {
                    if (currNode->m_right) {
                        currNode = currNode->m_right;
                    } else if (currNode->m_left) {
                        currNode = currNode->m_left;
                    } else {
                        // we are probably going to delete this node
                        SetNode* nodeToDelete = currNode;
                        if (allSuperSets.count(nodeToDelete->set)) {
                            // this is owned by a superset which will be deleted after us (probably, not necessarily guaranteed)
                            break;
                        }
                        if (currNode->m_parent) {
                            if (currNode->m_parent->m_left == currNode) {
                                currNode->m_parent->m_left = 0;
                            } else if (currNode->m_parent->m_right == currNode) {
                                currNode->m_parent->m_right = 0;
                            } else {
                                // throw SetStateException("Bad state could not find node in right or left of parent when deleting!");
                            }
                        }
                        
                        if (this->m_root == currNode) {
                            for (auto superSet : allSuperSets) {
                                if (superSet->m_root == currNode) {
                                    superSet->m_root = 0;
                                }
                            }
                        }

                        currNode = currNode->m_parent;
                        delete nodeToDelete;
                        if (currNode && allSuperSets.count(currNode->set)) {
                            break;
                        }
                    }
                }
                // for (auto superSet : this->m_superSets) {
                //     superSet->m_subSets.erase(std::find(superSet->m_subSets.begin(), superSet->m_subSets.end(), this));
                // }
                // for (auto subSet : this->m_subSets) {
                //     subSet->m_superSets.erase(std::find(subSet->m_superSets.begin(), subSet->m_superSets.end(), this));
                // }
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
                    throw SetStateException("Cannot add to read only set!");
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
                if (!this->m_root) {
                    throw SetStateException("Could not find element " + id.string() + " in set!");
                }
                SetNode* result = search(id, this->m_root);

                if (result) {
                    return result->m_ptr;
                }
                
                throw SetStateException("Could not find element " + id.string() + " in set!");
            }

            bool contains(T& el) const {
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                
                // get
                if (!this->m_root) {
                    return false;
                }
                SetNode* result = search(el.getID(), this->m_root);

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
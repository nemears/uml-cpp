#pragma once

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

        template <class V, class W, class OtherCreationPolicy> friend class PrivateSet;

        protected:
            SetNode* m_root = 0;

            std::vector<AbstractSet2*> m_superSets;
            std::vector<AbstractSet2*> m_subSets;
            std::vector<AbstractSet2*> m_redefines;

            bool m_rootRedefinedSet = true;

            size_t m_size = 0;

            std::unordered_set<AbstractSet2*> getAllSuperSets() {
                std::unordered_set<AbstractSet2*> allSuperSets;
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
                return allSuperSets;
            }

            std::unordered_set<AbstractSet2*> getAllSubSets() {
                std::unordered_set<AbstractSet2*> allSubSets;
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
                return allSubSets;
            }

        public:
            /**
             * this set subsets the set supplied, meaning all elements within this set will be contained within the set supplied
             * but this set will not necessarily have all of the elements within the set supplied
             * @param subsetOf the set that we are a subset of
             **/
            void subsets(AbstractSet2& subsetOf) {
                if (std::find(this->m_superSets.begin(), this->m_superSets.end(), &subsetOf) == this->m_superSets.end()) {
                    this->m_superSets.push_back(&subsetOf);
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
    class Singleton2;

    template <class T, class U>
    class TypedSet : public AbstractSet2 {

        template <class V, class W, class OtherCreationPolicy> friend class PrivateSet;
        template <class V, class W> friend class Singleton2;

        protected:
            virtual void innerAdd(T& el) = 0;
            virtual void innerRemove(ID id) = 0;
    };

    template <class T>
    class SetAllocationPolicy {
        protected:
            SetNode* create(T& el) {
                SetNode* ret = new SetNode();
                ret->m_ptr = &el;
                return ret;
            }

            void deleteNode(SetNode* node) {
                delete node;
            }
    };

    template <
                class T, 
                class U, 
                class AllocationPolicy = SetAllocationPolicy<T>
            >
    class PrivateSet : public TypedSet<T, U> , public AllocationPolicy {

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
                SetNode* node = AllocationPolicy::create(el);
                node->set = this;
                if (!this->m_rootRedefinedSet) {
                    for (auto redefinedSet : this->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            node->set = redefinedSet;
                            break;
                        }
                    }
                }
                innerAddDFS(node, node->set);
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

                // adjust redefines
                for (AbstractSet2* redefinedSet : set->m_redefines) {
                    if (!redefinedSet->m_root) {
                        redefinedSet->m_root = node;
                    }
                    redefinedSet->m_size++;
                }
                
                if (set->m_superSets.size() == 0 && set->m_rootRedefinedSet) {
                    // root set, place the node in the tree
                    // gather our set's supersets and subsets
                    std::unordered_set<AbstractSet2*> allSuperSetsAndMe = this->getAllSuperSets();
                    allSuperSetsAndMe.insert(node->set);
                    std::unordered_set<AbstractSet2*> allSubSets = this->getAllSubSets();
                    SetNode* currNode = set->m_root;
                    // handle divider nodes
                    while (currNode && ((!allSuperSetsAndMe.count(currNode->set) && !allSubSets.count(currNode->set)) || (!currNode->m_ptr && currNode->m_right))) {
                        bool createDividerNode = true;
                        if (!currNode->m_ptr) {
                            /**
                             * This is a divider node : m_ptr is null
                             * 
                             * Divider nodes represent a way to keep all subsets of
                             * a superset as part of the superset yet also keeping the
                             * necessary subsets seperate
                            */

                            createDividerNode = !allSuperSetsAndMe.count(currNode->set);

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
                                    if (allSuperSetsAndMe.count(superSet)) {
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
                                    if (front->m_root == currNode && allSuperSetsAndMe.count(front)) {
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
                            for (auto superSet : allSuperSetsAndMe) {
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
                    for (auto superSet : allSuperSetsAndMe) {
                        superSet->m_size++;
                    }
                }

                if (!set->m_root) {
                    set->m_root = node;
                }
            }

            SetNode* getParent(SetNode* match, SetNode* search) {
                if (search->m_ptr) {
                    if (search->m_right) {
                        if (match->m_ptr.id() > search->m_right->m_ptr.id()) {
                            if (search->m_left == match) {
                                return search;
                            }
                            return getParent(match, search->m_left);
                        } else {
                            if (search->m_right == match) {
                                return search;
                            }
                            return getParent(match, search->m_right);
                        }
                    } else if (search->m_left) {
                        if (search->m_left == match) {
                            return search;
                        } else {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                } else {
                    if (search->m_right) {
                        if (search->m_right == match) {
                            return search;
                        }
                        SetNode* ret = getParent(match, search->m_right);
                        if (ret) {
                            return ret;
                        }
                    }
                    if (search->m_left) {
                        if (search->m_left == match) {
                            return search;
                        }
                        SetNode* ret = getParent(match, search->m_left);
                        if (ret) {
                            return ret;
                        }
                    }
                    return 0;
                }
            }

            void innerRemove(ID id) override {
                if (!this->m_root) {
                    throw SetStateException("Could not remove element of id " + id.string() + " from set because it is not in the set");
                }
                SetNode* node = search(id, this->m_root);
                if (!node) {
                    throw SetStateException("Could not remove element of id " + id.string() + " from set because it is not in the set");
                }

                std::unordered_set<AbstractSet2*> allSuperSets = this->getAllSuperSets();
                allSuperSets.insert(this);
                for (auto redefinedSet : this->m_redefines) {
                    allSuperSets.insert(redefinedSet);
                }
                for (auto superSet : allSuperSets) {
                    if (superSet->m_superSets.size() == 0 && superSet->m_rootRedefinedSet) {
                        // root set
                        if (node->m_parent) {
                            SetNode* parent = getParent(node, superSet->m_root);
                            if (!parent) {
                                throw SetStateException("Could not remove element of id " + id.string() + " from set because we could not identify it's parent");
                            }
                            if (!parent->m_ptr) {
                                // check if we need to get rid of the divider node
                                if (!node->m_left) {
                                    // nothing to place in divider node so we don't need the divider node anymore
                                    SetNode* newParent = parent->m_left == node ? parent->m_right : parent->m_left;
                                    for (auto subSet : superSet->getAllSubSets()) {
                                        if (subSet->m_root == parent) {
                                            subSet->m_root = newParent;
                                        }
                                    }
                                    if (superSet->m_root == parent) {
                                        superSet->m_root = newParent;
                                    }
                                    newParent->m_parent = parent->m_parent;
                                    if (newParent->m_parent) {
                                        if (newParent->m_parent->m_left == parent) {
                                            newParent->m_parent->m_left = newParent;
                                        } else {
                                            newParent->m_parent->m_right = newParent;
                                        }
                                    }
                                    delete parent;
                                    superSet->m_size--;
                                    continue;
                                }
                            }
                            if (parent->m_left == node) {
                                parent->m_left = node->m_left;
                                if (parent->m_left) {
                                    parent->m_left->m_parent = parent;
                                } else {
                                    // left is empty, so we need to put right in left
                                    parent->m_left = parent->m_right;
                                    parent->m_right = 0;
                                }
                            } else if (parent->m_right == node) {
                                parent->m_right = node->m_left;
                                if (parent->m_right) {
                                    parent->m_right->m_parent = parent;
                                }
                            } else {
                                throw SetStateException("Could not remove element of id " + id.string() + " from set because of an internal error");
                            }
                            // place right
                            SetNode* currNode = 0;
                            if (node->m_right) {
                                currNode = node->m_left;
                            }
                            while (currNode) {
                                if (!currNode->m_left) {
                                    currNode->m_left = node->m_right;
                                    node->m_right->m_parent = currNode;
                                } else if (!currNode->m_right) {
                                    currNode->m_right = node->m_right;
                                    node->m_right->m_parent = currNode;
                                } else if (node->m_right->m_ptr.id() > currNode->m_right->m_ptr.id()) {
                                    currNode = currNode->m_left;
                                } else {
                                    currNode = currNode->m_right;
                                }
                            }
                        } else {
                            if (node->m_left) {
                                node->m_left->m_parent = 0;
                                if (node->m_right) {
                                    // place right
                                    node->m_right->m_parent = 0;
                                    SetNode* currNode = node->m_left;
                                    while (currNode) {
                                        if (!currNode->m_left) {
                                            currNode->m_left = node->m_right;
                                            node->m_right->m_parent = currNode;
                                        } else if (!currNode->m_right) {
                                            currNode->m_right = node->m_right;
                                            node->m_right->m_parent = currNode;
                                        } else if (node->m_right->m_ptr.id() > currNode->m_right->m_ptr.id()) {
                                            currNode = currNode->m_left;
                                        } else {
                                            currNode = currNode->m_right;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (superSet->m_root == node) {
                        superSet->m_root = node->m_left;
                    }
                    superSet->m_size--;
                }
                for (auto subSet : this->m_subSets) {
                    if (subSet->m_root == node) {
                        subSet->m_root = node->m_left;
                    }
                }
                AllocationPolicy::deleteNode(node);
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
                el.m_node->setReference(m_el);
                m_el.m_node->setReference(el);
                // handle opposites
                if (m_oppositeSignature) {
                    (el.*m_oppositeSignature)().innerAdd(m_el);   
                }
            }
            void remove(ID id) {
                // "lock" elements we are editing
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                T* el = 0;
                try {
                    el = &m_el.m_node->m_references.at(id).node->m_managerElementMemory->template as<T>(); // should be safe because we have a ptr
                } catch (std::exception e) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                SetLock elLock = m_el.m_manager->lockEl(*el);
                if (m_readOnly) {
                    throw SetStateException("Cannot remove from read only set!");
                }
                // remove
                innerRemove(id);
                el->m_node->removeReference(m_el);
                m_el.m_node->removeReference(*el);
                // handle opposites
                if (m_oppositeSignature) {
                    (el->*m_oppositeSignature)().innerRemove(m_el.getID());
                }
            }
        public:
            PrivateSet(U& el) : m_el(el) {
                
            }
            PrivateSet(U* el) : m_el(*el) {

            }
            virtual ~PrivateSet() {
                if (!this->m_rootRedefinedSet) {
                    return;
                }
                SetLock myLock = m_el.m_manager->lockEl(m_el); 

                // TODO lock all of the elements in the set lock?

                std::unordered_set<AbstractSet2*> allSuperSets = this->getAllSuperSets();

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
                for (auto superSet : this->m_superSets) {
                    superSet->m_subSets.erase(std::find(superSet->m_subSets.begin(), superSet->m_subSets.end(), this));
                }
                for (auto subSet : this->m_subSets) {
                    subSet->m_superSets.erase(std::find(subSet->m_superSets.begin(), subSet->m_superSets.end(), this));
                }
            }
            void opposite(TypedSet<U,T>& (T::*oppositeSignature)()) {
                m_oppositeSignature = oppositeSignature;
            }
            /**
             * makes sure that the set we are redefining is the same tree as ours and vice versa
             * @param redefined, the set that this set is redefining
             **/
            template <class V = Element, class W = Element> 
            void redefines(TypedSet<V, W>& redefined) {
                if (this->m_root) {
                    throw SetStateException("WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!");
                }
                this->m_redefines.push_back(&redefined);
                redefined.m_redefines.push_back(this);
                redefined.m_rootRedefinedSet = false;
                for (auto set : redefined.m_superSets) {
                    this->subsets(*set);
                }
                for (auto subSet : redefined.m_subSets) {
                    subSet->subsets(*this);
                }
                // for (auto& func : redefined.m_addFunctors) {
                //     if (!m_addFunctors.count(func)) {
                //         m_addFunctors.insert(func);
                //     }
                // }
                // for (auto& func : redefined.m_removeFunctors) {
                //     if (!m_removeFunctors.count(func)) {
                //         m_removeFunctors.insert(func);
                //     }
                // }
                // if (redefined.m_ownsOppositeFunctor && !m_ownsOppositeFunctor) {
                //     m_oppositeFunctor = redefined.m_oppositeFunctor;
                //     m_otherOpposites.clear();
                // } else {
                //     for (auto& opp : redefined.m_otherOpposites) {
                //         m_otherOpposites.push_back(opp);
                //     }
                // }
                // if (redefined.m_upper < 0 && redefined.m_upper != m_upper) {
                //     m_setToInstantiate = &redefined;
                // } else if (m_upper < 0 && redefined.m_upper >= 0) {
                //     redefined.m_setToInstantiate = this;
                // }
            }
            
            // Shared Accessors, all of these can be used by subclasses
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

    template <class T, class U>
    class Set2 : public PrivateSet<T,U> {
        public:
            Set2(U* el) : PrivateSet<T,U>(el) {}
            Set2(U& el) : PrivateSet<T,U>(el) {};
            void add(UmlPtr<T> el) {
                add(*el);
            }
            void add(T& el) {
                PrivateSet<T,U>::add(el);
            }
            void remove(ID id) {
                PrivateSet<T,U>::remove(id);
            }
            void remove(T& el) {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) {
                remove(el.id());
            }
    };
}
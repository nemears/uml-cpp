#pragma once

#include "uml/namedElement.h"
#include "uml/umlPtr.h"
#include "setLock.h"
#include "doNothingPolicy.h"
#include "uml/macros.h"
#include "uml/forwardDeclarations.h"

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

    class AbstractSet;

    struct SetNode {
        ElementPtr m_ptr;
        SetNode* m_parent = 0;
        SetNode* m_left = 0;
        SetNode* m_right = 0;
        AbstractSet* set = 0;
    };

    template <class T, class U> class TypedSet;
    void recursiveSetContains(ID id, AbstractSet* set);

    class AbstractSet {

        template <class T, class U, class AdditionPolicy, class RemovalPolicy, class AllocationPolicy> friend class PrivateSet;
        friend void recursiveSetContains(ID id, AbstractSet* set);

        protected:
            SetNode* m_root = 0;

            std::vector<AbstractSet*> m_superSets;
            std::vector<AbstractSet*> m_subSets;
            std::vector<AbstractSet*> m_redefines;

            bool m_rootRedefinedSet = true;

            size_t m_size = 0;

            virtual void runAddPolicy(Element& el) = 0;
            virtual void runRemovePolicy(Element& el) = 0;

            std::unordered_set<AbstractSet*> getAllSuperSets() const {
                std::unordered_set<AbstractSet*> allSuperSets;
                std::list<AbstractSet*> queue;
                for (auto superSet : this->m_superSets) {
                    queue.push_back(superSet);
                }
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    allSuperSets.insert(front);
                    for (auto superSet : front->m_superSets) {
                        queue.push_back(superSet);
                    }
                }
                return allSuperSets;
            }

            std::unordered_set<AbstractSet*> getAllSubSets() const {
                std::unordered_set<AbstractSet*> allSubSets;
                std::list<AbstractSet*> queue;
                for (auto subSet : this->m_subSets) {
                    queue.push_back(subSet);
                }
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
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
            void subsets(AbstractSet& subsetOf) {
                if (std::find(this->m_superSets.begin(), this->m_superSets.end(), &subsetOf) == this->m_superSets.end()) {
                    this->m_superSets.push_back(&subsetOf);
                    subsetOf.m_subSets.push_back(this);
                }
            }

            virtual bool contains(ID id) const = 0;
    };

    template <class T, class U, class AdditionPolicy, class RemovalPolicy>
    class CustomSingleton;

    template <class T, class U>
    class TypedSet : public AbstractSet {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy, class AllocationPolicy> friend class PrivateSet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomSingleton;

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

            SetNode* create(UmlPtr<T> el) {
                SetNode* ret = new SetNode();
                ret->m_ptr = el;
                return ret;
            }

            void deleteNode(SetNode* node) {
                delete node;
            }
    };

    template <class T>
    class OppositeInterface {
        public:
            virtual bool enabled() {
                return false;
            }
            virtual void addOpposite(T& el) {}
            virtual void removeOpposite(T& el) {}
    };

    namespace Parsers {
        void setNamespace(NamedElement& el, ID id);
        void setOwner(Element& el, ID id);
    }

    template <
                class T, 
                class U, 
                class AdditionPolicy = DoNothing<T,U>,
                class RemovalPolicy = DoNothing<T,U>,
                class AllocationPolicy = SetAllocationPolicy<T>
            >
    class PrivateSet : virtual public TypedSet<T, U> , public AllocationPolicy {

        FRIEND_ALL_UML()

        friend AdditionPolicy;
        friend RemovalPolicy;

        friend void Parsers::setNamespace(NamedElement& el, ID id);
        friend void Parsers::setOwner(Element& el, ID id);

        protected:
            U& m_el;
            bool m_readOnly = false;
            OppositeInterface<T> m_opposite = OppositeInterface<T>();

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

            SetNode* search(std::string name, SetNode* node) const {
                if (node->m_ptr->isSubClassOf(ElementType::NAMED_ELEMENT)) {
                    if (node->m_ptr->as<NamedElement>().getName() == name) {
                        return node;
                    }
                }

                if (node->m_left) {
                    SetNode* ret = search(name, node->m_left);
                    if (ret) {
                        return ret;
                    }
                }

                if (node->m_right) {
                    SetNode* ret = search(name, node->m_right);
                    if (ret) {
                        return ret;
                    }
                }
                
                return 0;
            }

            void runAddPolicy(Element& el) override {
                AdditionPolicy::apply(el.as<T>(), m_el);
            }
            void runRemovePolicy(Element& el) override {
                RemovalPolicy::apply(el.as<T>(), m_el);
            }

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

                AdditionPolicy::apply(el, m_el);
                for (auto superSet : this->getAllSuperSets()) { // TODO calling getSuperSets twice, maybe call it once and pass it down into innerAddDfs
                    superSet->runAddPolicy(el);
                }
            }

            void innerAdd(ID id) {
                SetNode* node = AllocationPolicy::create(m_el.m_manager->createPtr(id));
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

                // if (m_addFunctor) {
                //     (*m_addFunctor)(el);
                // }
                // for (auto superSet : this->getAllSuperSets()) { // TODO calling getSuperSets twice, maybe call it once and pass it down into innerAddDfs
                //     superSet->runAddFunctor(el);
                // }
            }

            template<class V, class W>
            void innerAddToOtherSet(TypedSet<V,W>& set, V& el) {
                set.innerAdd(el);
            }

            /**
             * searches superSet structure by finding the roots first and checking if it was placed
             * @param node: the node being placed
             * @param set: the current set we are checking
            */
            void innerAddDFS(SetNode* node, AbstractSet* set) {
                // place in supersets
                for (AbstractSet* superSet : set->m_superSets) {
                    innerAddDFS(node, superSet);
                }

                // adjust redefines
                for (AbstractSet* redefinedSet : set->m_redefines) {
                    if (!redefinedSet->m_root) {
                        redefinedSet->m_root = node;
                    }
                    redefinedSet->m_size++;
                }
                
                if (set->m_superSets.size() == 0 && set->m_rootRedefinedSet) {
                    // root set, place the node in the tree
                    // gather our set's supersets and subsets
                    std::unordered_set<AbstractSet*> allSuperSetsAndMe = this->getAllSuperSets();
                    allSuperSetsAndMe.insert(node->set);
                    std::unordered_set<AbstractSet*> allSubSets = this->getAllSubSets();
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
                            AbstractSet* dividerNodeScope = 0;
                            {
                                std::list<AbstractSet*> queue;
                                for (auto superSet: currNode->set->m_superSets) {
                                    queue.push_back(superSet);
                                }
                                while (!queue.empty()) {
                                    AbstractSet* superSet = queue.front();
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
                                std::list<AbstractSet*> queue;
                                queue.push_back(set);
                                while (!queue.empty()) {
                                    AbstractSet* front = queue.front();
                                    queue.pop_front();
                                    std::list<AbstractSet*> frontQueue;
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

                std::unordered_set<AbstractSet*> allSuperSets = this->getAllSuperSets();
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
                for (auto superSet : allSuperSets) {
                    superSet->runRemovePolicy(*node->m_ptr);
                }
                AllocationPolicy::deleteNode(node);
            }
            
            template <class V, class W>
            void innerRemoveFromOtherSet(TypedSet<V,W>& set, ID id) {
                set.innerRemove(id);
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
                if (m_opposite.enabled()) {
                    m_opposite.addOpposite(el);
                }
            }
            void addReadOnly(T& el) {
                // "lock" elements we are editing
                SetLock elLock = m_el.m_manager->lockEl(el);
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                // add
                innerAdd(el);
                el.m_node->setReference(m_el);
                m_el.m_node->setReference(el);
                // handle opposites
                if (m_opposite.enabled()) {
                    m_opposite.addOpposite(el);
                }
            }
            void add(ID id) {
                // "lock" elements we are editing
                // SetLock elLock = m_el.m_manager->lockEl(el);
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                if (m_readOnly) {
                    throw SetStateException("Cannot add to read only set!");
                }
                // add
                innerAdd(id);
                // el.m_node->setReference(m_el);
                m_el.m_node->setReference(id);
                // handle opposites
                // if (m_oppositeSignature) {
                //     (el.*m_oppositeSignature)().innerAdd(m_el);   
                // }
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
                if (m_opposite.enabled()) {
                    m_opposite.removeOpposite(*el);
                }
            }
            void removeReadOnly(ID id) {
                // "lock" elements we are editing
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                T* el = 0;
                try {
                    el = &m_el.m_node->m_references.at(id).node->m_managerElementMemory->template as<T>(); // should be safe because we have a ptr
                } catch (std::exception e) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                SetLock elLock = m_el.m_manager->lockEl(*el);
                // remove
                innerRemove(id);
                el->m_node->removeReference(m_el);
                m_el.m_node->removeReference(*el);
                // handle opposites
                if (m_opposite.enabled()) {
                    m_opposite.removeOpposite(*el);
                }
            }
            /**
             * Removes all elements from set
             **/
            void clear() {
                if (m_readOnly) {
                    throw SetStateException("Cannot clear read only set");
                }
                while (this->m_root) {
                    SetNode* nodeToRemove = this->m_root;
                    while (!nodeToRemove->m_ptr) {
                        nodeToRemove = nodeToRemove->m_left;
                    }
                    remove(nodeToRemove->m_ptr.id());
                }
            }
            void removeFromJustThisSet(ID id) {
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
                SetNode* node = search(id, this->m_root);
                if (!node) {
                    throw SetStateException("Could not find el with id of " + id.string() + "in set");
                }
                if (this->m_root == node) {
                    this->m_root = 0;
                }
                if (this->m_superSets.size() == 0) {
                    return;
                }
                if (this->m_superSets.size() == 1) {
                    node->set = this->m_superSets[0];
                    return;
                }
                for (auto superSet : this->m_superSets) {
                    // create a copy
                    // SetNode* copy = AllocationPolicy::createNode(node->m_ptr);
                    // TODO replace;
                }
            }
            void reindexDFS(SetNode* node, AbstractSet* set) {
                for (auto superSet : set->m_superSets) {
                    reindexDFS(node, superSet);
                }
                if (set->m_superSets.size() == 0) {
                    SetNode* parent = getParent(node, this->m_root);
                    if (parent->m_left == node) {
                        if (parent->m_right && parent->m_right->m_ptr.id() > node->m_ptr.id()) {
                            parent->m_left = parent->m_right;
                            parent->m_right = node;
                        }
                    } else if (parent->m_right == node) {
                        if (node->m_ptr.id() > parent->m_left->m_ptr.id()) {
                            parent->m_right = parent->m_left;
                            parent->m_left = node;
                        }
                    } else {
                        throw SetStateException("bad state reindexing, bad parent found! contact developer");
                    }
                }
            }
            void reindex(ID newID) {
                if (!this->m_root) {
                    return;
                }
                SetNode* node = search(newID, this->m_root);
                if (!node->m_parent) {
                    return;
                } 
                reindexDFS(node, this);
            }

            void eraseElement(ID id) {
                remove(id);
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

                std::unordered_set<AbstractSet*> allSuperSets = this->getAllSuperSets();

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

            template <class S>
            void opposite(S& (T::*oppositeSignature)()) {

                class OppositeInterfaceAdapter : public OppositeInterface<T> {
                    public:
                        S& (T::*signature)() = 0;
                        U& me;
                        OppositeInterfaceAdapter(U& u, S& (T::*sig)()) : me(u) {
                            signature = sig;
                        }
                        bool enabled() override {
                            return true;
                        }
                        void addOpposite(T& el) override {
                            (el.*signature)().innerAdd(me);
                        }
                        void removeOpposite(T& el) override {
                            (el.*signature)().innerRemove(me.getID());
                        }
                };

                m_opposite = OppositeInterfaceAdapter(m_el, oppositeSignature);
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
            }

            // Shared Accessors, all of these can be used by subclasses
            T& get(ID id) const {
                // "lock" sets owner while we search
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                
                // get
                if (!this->m_root) {
                    throw SetStateException("Could not find element " + id.string() + " in set!");
                }
                SetNode* result = search(id, this->m_root);

                if (result) {
                    return result->m_ptr->as<T>();
                }
                
                throw SetStateException("Could not find element " + id.string() + " in set!");
            }

            T& get(std::string name) const {
                SetLock myLck = m_el.lockEl(m_el);

                if (!this->m_root) {
                    throw SetStateException("Could not find named element of name " + name + " in set!");
                }

                SetNode* result = search(name, this->m_root);

                if (result) {
                    return result->m_ptr->as<T>();
                }
                
                throw SetStateException("Could not find named element of name " + name + " in set!");
            }

            bool contains(ID id) const override {
                SetLock myLock = m_el.m_manager->lockEl(m_el);
                
                // get
                if (!this->m_root) {
                    return false;
                }
                SetNode* result = search(id, this->m_root);

                return result;
            }
            
            bool contains(T& el) const {
                return contains(el.getID());
            }

            bool contains(UmlPtr<T> el) const {
                return contains(el.id());
            }

            bool contains(std::string name) const {
                SetLock myLck = m_el.lockEl(m_el);

                if (!this->m_root) {
                    return false;
                }

                SetNode* result = search(name, this->m_root);

                return result;
            }

            size_t size() const {
                return this->m_size;
            }

            bool empty() const {
                return this->m_size == 0;
            }
    };

    template <class T, class U, class AdditionPolicy, class RemovalPolicy> class CustomSet;

    template <class T>
    class SetIterator {

        template <class V> friend class ID_Set;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomSet;

        protected:
            SetNode* root = 0;
            SetNode* curr = 0;
            std::unordered_set<AbstractSet*> validSets;
        public:
            SetIterator() {};
            SetIterator(SetIterator& rhs) {
                curr = rhs.curr;
                validSets = rhs.validSets;
            }
            T& operator*() {
                return curr->m_ptr->as<T>();
            }
            UmlPtr<T> operator->() {
                return curr->m_ptr;
            }
            SetIterator<T> operator++() {
                do {
                    if (curr->m_left) {
                        // always go left
                        curr = curr->m_left;
                    } else {
                        if (!curr->m_parent/** || m_node->m_parent->m_guard < m_guard**/) {
                            // if there is no parent to go to we must end
                            curr = 0;
                            break;
                        }
                        // we hit bottom, choose next right
                        SetNode* temp;
                        SetNode* last = curr;
                        if (last == root) {
                            curr = 0;
                            break;
                        }
                        bool found = false;
                        do {
                            temp = last->m_parent;
                            if (temp->m_right) {
                                if (temp->m_right->m_ptr.id() != last->m_ptr.id()) {
                                    found = true;
                                    break;
                                }
                            }
                            last = temp;
                        } while (temp->m_parent && validSets.count(temp->m_parent->set));
                        if (!found) {
                            curr = 0;
                            break;
                        } else {
                            if (temp->m_right == curr) {
                                curr = 0;
                                break;
                            }
                            curr = temp->m_right;
                        }
                    }
                } while (!curr->m_ptr);
                return *this;
            }
            SetIterator operator++(int) {
                return ++(*this);
            }
            friend bool operator== (const SetIterator& lhs, const SetIterator& rhs) {
                if (!lhs.curr && !lhs.curr) {
                    return true;
                }
                if (!lhs.curr || !lhs.curr) {
                    return false;
                }
                return lhs.curr == rhs.curr;
            }
            friend bool operator!= (const SetIterator& lhs, const SetIterator& rhs) {
                if (!lhs.curr && !lhs.curr) {
                    return false;
                }
                if (!lhs.curr || !lhs.curr) {
                    return true;
                }
                return lhs.curr != rhs.curr;
            }
    };

    template <class T>
    class SetID_Iterator : public SetIterator<T> {
        public:
            ID operator*() {
                return this->curr->m_ptr.id();
            }
            UmlPtr<T> operator->() = delete;
    };

    template <class T>
    class ID_Set {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomSet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            SetNode* root = 0;
            std::unordered_set<AbstractSet*> validSets;
        public:
            SetID_Iterator<T> begin() {
                SetID_Iterator<T> ret;
                ret.curr = root;
                ret.root = root;
                ret.validSets = validSets;
                return ret;
            };
            SetID_Iterator<T> end() {
                return SetID_Iterator<T>();
            };
            ID front() const {
                SetNode* curr = this->root;
                if (!curr) {
                    throw SetStateException("Cannot get front element because set is empty!");
                }

                while (!curr->m_ptr) {
                    if (!curr->m_left) {
                        throw SetStateException("Internal error while getting front element, contact developer!");
                    }
                    curr = curr->m_left;
                }

                return curr->m_ptr.id();
            }
            ID back() const {
                SetNode* curr = this->root;
                if (!curr) {
                    throw SetStateException("Cannot get back element because set is empty!");
                }

                // go all the way right
                while (curr->m_right) {
                    curr = curr->m_right;
                }

                // now go all the way left
                while (curr->m_left) {
                    curr = curr->m_left;
                }

                return curr->m_ptr.id();
            }
    };

    template <class T, class U>
    class Set : virtual public TypedSet<T,U> {
        public:
            virtual bool contains(ID id) const = 0;
            virtual bool contains(T& el) const = 0;
            virtual bool contains(UmlPtr<T> el) const = 0;
            virtual bool contains(std::string name) const = 0;
            virtual bool empty() const = 0;
            virtual size_t size() const = 0;
            virtual T& get(ID id) const = 0;
            virtual T& get(std::string name) const = 0;
            virtual T& front() const = 0;
            virtual T& back() const = 0;
            virtual void add(ID id) = 0;
            virtual void add(UmlPtr<T> el) = 0;
            virtual void add(T& el) = 0;
            template <class ... Ts>
            void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            }
            virtual void remove(ID id) = 0;
            virtual void remove(T& el) = 0;
            virtual void remove(UmlPtr<T> el) = 0;
            virtual void removeFromJustThisSet(ID id) = 0;
            virtual void clear() = 0;
            virtual SetIterator<T> begin() = 0;
            virtual SetIterator<T> end() = 0;
            virtual ID_Set<T> ids() = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothing<T, U>,
                class RemovalPolicy = DoNothing<T, U>
            >
    class CustomSet : public PrivateSet<T,U, AdditionPolicy, RemovalPolicy> , public Set<T,U> {
        public:
            CustomSet(U* el) : PrivateSet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            CustomSet(U& el) : PrivateSet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            bool contains(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(id);
            }
            bool contains(T& el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(el);
            }
            bool contains(UmlPtr<T> el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(el);
            }
            bool contains(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(name);
            }
            T& get(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::get(id);
            }
            T& get(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::get(name);
            }
            T& front() const override {
                SetNode* curr = this->m_root;
                if (!curr) {
                    throw SetStateException("Cannot get front element because set is empty!");
                }

                while (!curr->m_ptr) {
                    if (!curr->m_left) {
                        throw SetStateException("Internal error while getting front element, contact developer!");
                    }
                    curr = curr->m_left;
                }

                return curr->m_ptr->as<T>();
            }
            T& back() const override {
                SetNode* curr = this->m_root;
                if (!curr) {
                    throw SetStateException("Cannot get back element because set is empty!");
                }

                // go all the way right
                while (curr->m_right) {
                    curr = curr->m_right;
                }

                // now go all the way left
                while (curr->m_left) {
                    curr = curr->m_left;
                }

                return curr->m_ptr->as<T>();
            }
            bool empty() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::empty();
            }
            size_t size() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::size();
            }
            void add(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(id);
            }
            void add(UmlPtr<T> el) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(el.id());
            }
            void add(T& el) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(el);
            }
            void remove(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::remove(id);
            }
            void remove(T& el) override {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) override {
                remove(el.id());
            }
            void removeFromJustThisSet(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::removeFromJustThisSet(id);
            }
            void clear() override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::clear();
            }
            SetIterator<T> begin() override {
                SetIterator<T> ret;
                ret.curr = this->m_root;
                ret.root = this->m_root;
                ret.validSets = this->getAllSuperSets();
                ret.validSets.insert(this);
                return ret;
            }
            SetIterator<T> end() override {
                return SetIterator<T>();
            }

            ID_Set<T> ids() override {
                ID_Set<T> ret;
                ret.root = this->m_root;
                ret.validSets = this->getAllSuperSets();
                ret.validSets.insert(this);
                return ret;
            };
    };
}
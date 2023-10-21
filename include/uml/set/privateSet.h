#pragma once

#include "abstractSet.h"

namespace UML {
    template <class T, class U, class AdditionPolicy, class RemovalPolicy>
    class CustomSingleton;

    template <class T, class U>
    class TypedSet : virtual public AbstractSet {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy, class AllocationPolicy> friend class PrivateSet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomSingleton;
        friend class Usage; // ??

        protected:
            virtual void innerAdd(T& el) = 0;
            virtual void innerRemove(ID id) = 0;
    };

    class SetAllocationPolicy {
        protected:
            SetNode* create(Element& el) {
                SetNode* ret = new SetNode();
                ret->m_ptr = &el;
                return ret;
            }

            SetType setType() const {
                return SetType::SET;
            }
            template <class T>
            SetNode* create(UmlPtr<T> el) {
                SetNode* ret = new SetNode();
                ret->m_ptr = el;
                return ret;
            }

            void adjustSuperSets(__attribute__((unused)) SetNode* node, __attribute__((unused)) std::unordered_set<AbstractSet*>& allSuperSetsAndMe) {}
    };

    template <class T>
    class OppositeInterface {
        public:
            virtual ~OppositeInterface() = default;
            virtual bool enabled() = 0;
            virtual void addOpposite(T& el) = 0;
            virtual void removeOpposite(T& el) = 0;
            bool skip = false;
    };

    template <class T>
    class NoOpposite : public OppositeInterface<T> {
        public:
            bool enabled() override {
                return false;
            }
            void addOpposite(__attribute__((unused)) T& el) override {}
            void removeOpposite(__attribute__((unused)) T& el) override {}
    };

    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

    template <
                class T, 
                class U, 
                class AdditionPolicy = DoNothingAdd<T,U>,
                class RemovalPolicy = DoNothingRemove<T,U>,
                class AllocationPolicy = SetAllocationPolicy
            >
    class PrivateSet :  virtual public TypedSet<T, U>, 
                        virtual protected AllocationPolicy, 
                        protected AdditionPolicy, 
                        protected RemovalPolicy {

        FRIEND_ALL_UML()

        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

        protected:
            U& m_el;
            OppositeInterface<T>* m_opposite = new NoOpposite<T>();
            
            SetNode* search(ID id, SetNode* node) const {
                if (!node) {
                    return 0;
                }

                if (id == node->m_ptr.id()) {
                    return node;
                } else if (!node->m_ptr) {
                    SetNode* ret = search(id, node->m_left);
                    if (ret) {
                        return ret;
                    }
                    return search(id, node->m_right);
                } else if (id < node->m_ptr.id()) {
                    return search(id, node->m_left);
                } else {
                    return search(id, node->m_right);
                }
            }

            SetNode* search(std::string name, SetNode* node) const {
                if (node->m_ptr && node->m_ptr->isSubClassOf(ElementType::NAMED_ELEMENT)) {
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
            bool oppositeEnabled() override {
                return m_opposite->enabled();
            }
            void oppositeAdd(Element& el) override {
                m_opposite->addOpposite(el.as<T>());
            }
            void oppositeRemove(Element& el) override {
                m_opposite->removeOpposite(el.as<T>());
            }

            void createDividerNode(SetNode* node, SetNode* existingRoot, AbstractSet* rootSet) {
                SetNode* dividerNode = new SetNode;

                // TODO find minimum set with both oldRoot's set and new node's set
                // travel bfs down the tree from the root set provided
                std::list<AbstractSet*> queue;
                queue.push_back(rootSet);
                AbstractSet* candidate = 0;
                do {
                    AbstractSet* currSet = queue.front();
                    queue.pop_front();

                    // compare set to the node's set's supersets
                    std::list<AbstractSet*> nodeStack;
                    nodeStack.push_front(node->set);
                    bool superSetOfNode = false;
                    do {
                        AbstractSet* currNodeSet = nodeStack.front();
                        nodeStack.pop_front();
                        if (currNodeSet == currSet) {
                            superSetOfNode = true;
                            break;
                        }
                        for (auto superset : currNodeSet->m_superSets) {
                            nodeStack.push_front(superset);
                        }
                    } while (!nodeStack.empty());

                    // compare set to rootNode's set's supersets
                    std::list<AbstractSet*> rootStack;
                    rootStack.push_front(existingRoot->set);
                    bool superSetOfRoot = false;
                    do {
                        AbstractSet* currRootSet = rootStack.front();
                        rootStack.pop_front();

                        if (currRootSet == currSet) {
                            superSetOfRoot = true;
                            break;
                        }
                        
                        for (auto superset : currRootSet->m_superSets) {
                            rootStack.push_front(superset);
                        }
                    } while (!rootStack.empty());

                    if (superSetOfNode && superSetOfRoot) {
                        // if both are good, we have a candidate
                        candidate = currSet;

                        // add children of candidates, only they can be candidates further down the tree
                        for (auto subset : currSet->m_subSets) {
                            queue.push_back(subset);
                        }
                    }
                } while (!queue.empty());

                if (!candidate) {
                    throw SetStateException("could not find common set for divider node in set, please contact developer!");
                }

                dividerNode->set = candidate;

                SetNode* currNode = rootSet->m_root;

                // adjust roots of super sets
                queue.push_back(candidate);
                do {
                    AbstractSet* currSet = queue.front();
                    queue.pop_front();
                    if (currSet->m_root == existingRoot) {
                        currSet->m_root = dividerNode;
                        for (auto superset : currSet->m_superSets) {
                            queue.push_back(superset);
                        }
                    }
                } while (!queue.empty());

                // rootNode has already been placed, adjust parents
                if (currNode == existingRoot) {
                    // no parent in this tree
                    if (node->m_ptr.id() < existingRoot->m_ptr.id()) {
                        dividerNode->m_left = node;
                        dividerNode->m_right = existingRoot;
                    } else {
                        dividerNode->m_right = node;
                        dividerNode->m_left = existingRoot;
                    }
                    node->m_parent = dividerNode;
                    existingRoot->m_parent = dividerNode;
                    return;
                }
                while (currNode) {

                    if (currNode->m_left == existingRoot) {
                        // currNode is parent of existing root in this tree,
                        // replace m_left with dividerNode
                        currNode->m_left = dividerNode;
                        dividerNode->m_parent = currNode;
                        dividerNode->m_left = existingRoot;
                        dividerNode->m_right = node; 
                        existingRoot->m_parent = dividerNode;
                        node->m_parent = dividerNode;
                        break;
                    } else if (currNode->m_right == existingRoot) {
                       // currNode is parent of existing root in this tree,
                       // replace m_right with dividerNode 
                       currNode->m_right = dividerNode;
                       dividerNode->m_parent = currNode;
                       dividerNode->m_right = existingRoot;
                       dividerNode->m_left = node;
                       existingRoot->m_parent = dividerNode;
                       node->m_parent = dividerNode;
                       break;
                    }

                    // check which side is a viable candidate
                    std::list<AbstractSet*> stack;
                    stack.push_front(existingRoot->set);
                    do {
                        AbstractSet* currSet = stack.front();
                        stack.pop_front();
                        if (currSet == currNode->m_left->set) {
                            currNode = currNode->m_left;
                            break;
                        } else if (currSet == currNode->m_right->set) {
                           currNode = currNode->m_right; 
                           break;
                        }
                        for (auto superset : currSet->m_superSets) {
                            stack.push_front(superset);
                        }
                    } while (!stack.empty()); 
                }
                
            }

            void addNodeToSet(SetNode* node) {
                // we want to place this in all of our root super sets
                // climb up graph dfs to find root super sets
                std::list<AbstractSet*> stack;
                stack.push_front(this);
                std::unordered_set<AbstractSet*> visited;
                bool treeAlreadyCreated = false;
                do {
                    // take set from top of stack
                    AbstractSet* set = stack.front();
                    stack.pop_front();

                    // skip sets allready visited
                    if (visited.count(set)) {
                        // TODO check that we need this, place a breakpoint here and run all tests
                        continue;
                    } else {
                        visited.insert(set);
                    }

                    if (!set->m_superSets.empty()) {
                        // add supersets to stack
                        for (auto superSet : set->m_superSets) {
                            stack.push_front(superSet);
                        }
                    } else if (!treeAlreadyCreated) {
                        // add node to set from top
                        for (auto redefinedSet : set->m_redefines) {
                            if (redefinedSet->m_rootRedefinedSet) {
                                set = redefinedSet;
                            }
                        }
                        if (set->m_root) {
                            SetNode* currNode = set->m_root;
                            while (currNode) {
                                if (!currNode->m_ptr) {
                                    // this is a divider node, see which side our node belongs
                                    std::list<AbstractSet*> dividerStack;
                                    dividerStack.push_front(this);
                                    bool createDividerNodeFlag = false;
                                    do {
                                        AbstractSet* currSet = dividerStack.front();
                                        dividerStack.pop_front();
                                        if (currSet == currNode->m_left->set) {
                                            currNode = currNode->m_left;
                                            break;
                                        } else if (currSet == currNode->m_right->set) {
                                            currNode = currNode->m_right;
                                            break;
                                        }
                                        for (auto superSet : currSet->m_superSets) {
                                            dividerStack.push_front(superSet);
                                        }
                                        if (dividerStack.empty()) {
                                            // neither side is appropriate, we need a new divider node
                                            createDividerNodeFlag = true;
                                            createDividerNode(node, currNode, set);
                                        }
                                    } while (!dividerStack.empty());
                                    if (createDividerNodeFlag) {
                                        break;
                                    }
                                } else {
                                    // not a divider node
                                    if (currNode->set == node->set) {
                                        currNode->insert(node);
                                        // because there is already a node of this set, we can skip adding to other trees because this set's root has already been established
                                        treeAlreadyCreated = true;
                                        break;
                                    } else {
                                        // there is data that belongs in a superset to this set but it is not divided
                                        createDividerNode(node, currNode, set);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    // handle redefines
                    for (auto redefinedSet : set->m_redefines) {
                        // set root
                        if (!redefinedSet->m_root) {
                            redefinedSet->m_root = node;
                            if (set->m_root) {
                                // remove this after we are sure it works lol
                                throw SetStateException("bad state for set with redefines, our set has root already but redefined does not. Please contact the developer!");
                            }
                        }

                        // increase size
                        redefinedSet->m_size++;
                    }

                    if (!set->m_root) {
                        // set is empty, set root
                        set->m_root = node;
                    }

                    // increase size
                    set->m_size++;
                } while (!stack.empty());
            }

            void innerAdd(T& el) override {
                SetNode* node = this->createNode(el);
                addNodeToSet(node);

                // run add policies bfs with queue
                std::list<AbstractSet*> queue;
                queue.push_back(this);
                do {
                    AbstractSet* set = queue.front();
                    queue.pop_front();
                    for (auto superSet : set->m_superSets) {
                        queue.push_back(superSet);
                    }
                    set->runAddPolicy(el);
                } while (!queue.empty());
            }

            void innerAdd(ID id) {
                SetNode* node = createNode(id);
                addNodeToSet(node);
                // we are done, we can't run policies because we don't have both elements
            }

            SetNode* getParent(SetNode* match, SetNode* search) {
                if (search->m_ptr) {
                    if (search->m_right) {
                        if (match->m_ptr.id() > search->m_right->m_ptr.id()) {
                            if (search->m_left == match) {
                                return search;
                            }
                            if (search->m_left) {
                                return getParent(match, search->m_left);
                            }
                            return 0;
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
                            if (search->m_left) {
                                return getParent(match, search->m_left);
                            }
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

                std::unordered_set<AbstractSet*> allSuperSets = node->set->getAllSuperSets();
                allSuperSets.insert(node->set);
                for (auto redefinedSet : node->set->m_redefines) {
                    allSuperSets.insert(redefinedSet);
                }
                for (auto superSet : allSuperSets) {
                    if (superSet->m_superSets.size() == 0 && superSet->m_rootRedefinedSet) {
                        // root set
                        if (node->m_parent) {
                            SetNode* parent = getParent(node, superSet->m_root);
                            if (!parent) {
                                continue;
                                // throw SetStateException("Could not remove element of id " + id.string() + " from set because we could not identify it's parent");
                            }
                            if (!parent->m_ptr) {
                                // check if we need to get rid of the divider node
                                if (!node->m_left) {
                                    // nothing to place in divider node so we don't need the divider node anymore
                                    SetNode* newParent = parent->m_left == node ? parent->m_right : parent->m_left;
                                    for (auto otherSuperSet : allSuperSets) {
                                        if (otherSuperSet->m_root == parent) {
                                            otherSuperSet->m_root = newParent;
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

                                    // balance right
                                    SetNode* currNode = parent->m_right;
                                    while (currNode) {
                                        if (currNode->m_right) {
                                            if (currNode->m_right->m_ptr.id() > parent->m_left->m_ptr.id()) {
                                                parent->m_right = parent->m_left;
                                                parent->m_left = currNode->m_right;
                                                parent->m_left->m_parent = parent;
                                            } else {
                                                parent->m_right = currNode->m_right;
                                                parent->m_right->m_parent = parent;
                                            }
                                            currNode->m_right = 0;
                                            break;
                                        } else if (currNode->m_left) {
                                            if (currNode->m_left->m_ptr.id() > parent->m_left->m_ptr.id()) {
                                                parent->m_right = parent->m_left;
                                                parent->m_left = currNode->m_left;
                                                parent->m_left->m_parent = parent;
                                            } else {
                                                parent->m_right = currNode->m_left;
                                                parent->m_right->m_parent = parent;
                                            }
                                            currNode->m_left = 0;
                                            parent = currNode;
                                            currNode = currNode->m_left;
                                        } else {
                                            parent->m_right = 0;
                                            break;
                                        }
                                    }
                                }
                            } else if (parent->m_right == node) {
                                parent->m_right = node->m_left;
                                if (parent->m_right) {
                                    if (parent->m_right->m_ptr.id() > parent->m_left->m_ptr.id()) {
                                        parent->m_right = parent->m_left;
                                        parent->m_left = node->m_left;
                                    } else {
                                        parent->m_right->m_parent = parent;
                                    }
                                    node->m_left->m_parent = parent;
                                } else {
                                    // balance tree
                                    SetNode* currNode = parent->m_left;
                                    while (currNode) {
                                        if (currNode->m_right) {
                                            if (currNode->m_right->m_ptr.id() > parent->m_left->m_ptr.id()) {
                                                parent->m_right = parent->m_left;
                                                parent->m_left = currNode->m_right;
                                            } else {
                                                parent->m_right = currNode->m_right;
                                            }
                                            currNode->m_right->m_parent = parent;
                                            currNode->m_right = 0;
                                            parent = currNode;
                                            currNode = currNode->m_left;
                                        } else if (currNode->m_left) {
                                            if (currNode->m_left->m_ptr.id() > parent->m_left->m_ptr.id()) {
                                                parent->m_right = parent->m_left;
                                                parent->m_left = currNode->m_left;
                                            } else {
                                                parent->m_right = currNode->m_left;
                                            }
                                            currNode->m_left->m_parent = parent;
                                            currNode->m_left = 0;
                                            parent = currNode;
                                            currNode = currNode->m_right;
                                        } else {
                                            break;
                                        }
                                    }
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
                                    break;
                                } else if (!currNode->m_right) {
                                    if (node->m_right->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                                        currNode->m_right = currNode->m_left;
                                        currNode->m_left = node->m_right;
                                    } else {
                                        currNode->m_right = node->m_right;
                                    }
                                    node->m_right->m_parent = currNode;
                                    break;
                                } else if (node->m_right->m_ptr.id() > currNode->m_right->m_ptr.id()) {
                                    currNode = currNode->m_left;
                                } else {
                                    currNode = currNode->m_right;
                                }
                            }
                        } else {
                            if (node->m_left) {
                                node->m_left->m_parent = 0;

                                // place right in left
                                SetNode* currNode = node->m_left;
                                SetNode* nodeToMove = node->m_right;

                                while (currNode && nodeToMove) {
                                    if (!currNode->m_left) {
                                        currNode->m_left = nodeToMove;
                                        nodeToMove->m_parent = currNode;
                                        // determine how to handle nodeToMove's children
                                        if (nodeToMove->m_right) {
                                            if (nodeToMove->m_right->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                                                currNode->m_right = currNode->m_left;
                                                currNode->m_left = nodeToMove->m_right;
                                            } else {
                                                currNode->m_right = nodeToMove->m_right;
                                            }
                                            nodeToMove->m_right->m_parent = currNode;
                                            nodeToMove->m_right = 0;

                                            SetNode* temp = nodeToMove;
                                            nodeToMove = nodeToMove->m_left;
                                            temp->m_left = 0;
                                        } else if (nodeToMove->m_left) {
                                            if (nodeToMove->m_left->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                                                currNode->m_right = currNode->m_left;
                                                currNode->m_left = nodeToMove->m_left;
                                            } else {
                                                currNode->m_right = nodeToMove->m_left;
                                            }
                                            nodeToMove->m_left->m_parent = currNode;
                                            nodeToMove->m_left = 0;
                                            break;
                                        } else {
                                            break;
                                        }
                                    } else if (!currNode->m_right) {
                                        if (nodeToMove->m_ptr.id() > currNode->m_left->m_ptr.id()) {
                                            currNode->m_right = currNode->m_left;
                                            currNode->m_left = nodeToMove;
                                        } else {
                                            currNode->m_right = nodeToMove;
                                        }
                                        nodeToMove->m_parent = currNode;
                                        break;
                                    } else {
                                        if (nodeToMove->m_ptr.id() > currNode->m_right->m_ptr.id()) {
                                            currNode = currNode->m_left;
                                        } else {
                                            currNode = currNode->m_right;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                for (auto superSet : allSuperSets) {
                    if (superSet->m_root == node) {
                        superSet->m_root = node->m_left;
                    }
                    superSet->m_size--;
                    superSet->runRemovePolicy(*node->m_ptr);
                }
                delete node;
            }

            void handleOppositeAdd(T& el) {
                if (m_opposite->skip) {
                    m_opposite->skip = false;
                    return;
                }
                std::list<AbstractSet*> queue;
                queue.push_back(this);
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    bool lookUpTree = true;
                    if (front->oppositeEnabled() && front->m_rootRedefinedSet) {
                        front->oppositeAdd(el);
                        lookUpTree = false;
                    } else {
                        for (auto redefinedSet : front->m_redefines) {
                            if (redefinedSet->oppositeEnabled() && redefinedSet->m_rootRedefinedSet) {
                                redefinedSet->oppositeAdd(el);
                                lookUpTree = false;
                                break;
                            }
                        }
                    }
                    if (lookUpTree) {
                        if (front->oppositeEnabled()) {
                            front->oppositeAdd(el);
                            lookUpTree = false;
                        } else {
                            for (auto redefinedSet : front->m_redefines) {
                                if (redefinedSet->oppositeEnabled() && lookUpTree) {
                                    redefinedSet->oppositeAdd(el);
                                    lookUpTree = false;
                                    break;
                                }
                            }
                        }
                    }
                    if (!lookUpTree) {
                        continue;
                    }
                    for (auto superSet : front->m_superSets) {
                        queue.push_back(superSet);
                    }
                }
            }

            void add(T& el) {
                // "lock" elements we are editing
                [[maybe_unused]] SetLock elLock = m_el.m_manager->lockEl(el);
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                innerAdd(el);
                el.m_node->setReference(m_el);
                m_el.m_node->setReference(el);
                handleOppositeAdd(el);
            }
            void add(ID id) {
                // "lock" elements we are editing
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                innerAdd(id);
                m_el.m_node->setReference(id);
            }
            void handleOppositeRemove(Element& el) override {
                std::list<AbstractSet*> queue;
                queue.push_back(this);
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    bool lookUpTree = true;
                    if (front->oppositeEnabled() && front->m_rootRedefinedSet) {
                        front->oppositeRemove(el);
                        lookUpTree = false;
                    } else {
                        for (auto redefinedSet : front->m_redefines) {
                            if (redefinedSet->oppositeEnabled() && redefinedSet->m_rootRedefinedSet) {
                                redefinedSet->oppositeRemove(el);
                                lookUpTree = false;
                                break;
                            }
                        }
                    }
                    if (lookUpTree) {
                        if (front->oppositeEnabled()) {
                            front->oppositeRemove(el);
                            lookUpTree = false;
                        } else {
                            for (auto redefinedSet : front->m_redefines) {
                                if (redefinedSet->oppositeEnabled() && lookUpTree) {
                                    redefinedSet->oppositeRemove(el);
                                    lookUpTree = false;
                                    break;
                                }
                            }
                        }
                    }
                    if (!lookUpTree) {
                        continue;
                    }
                    for (auto superSet : front->m_superSets) {
                        queue.push_back(superSet);
                    }
                }
            }
            void remove(ID id) override {
                // "lock" elements we are editing
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                T* el = 0;
                try {
                    el = &m_el.m_node->m_references.at(id).node->m_managerElementMemory->template as<T>(); // should be safe because we have a ptr
                } catch (std::exception& e) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                [[maybe_unused]] SetLock elLock = m_el.m_manager->lockEl(*el);
                if (!this->m_root) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                SetNode* result = search(id, this->m_root);
                if (!result) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                AbstractSet* setThatNodeWasAdded = result->set;
                innerRemove(id);
                el->m_node->removeReference(m_el);
                m_el.m_node->removeReference(*el);
                setThatNodeWasAdded->handleOppositeRemove(*el);
            }
            /**
             * Removes all elements from set
             **/
            void clear() {
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
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                T* el = 0;
                try {
                    el = &m_el.m_node->m_references.at(id).node->m_managerElementMemory->template as<T>(); // should be safe because we have a ptr
                } catch (std::exception& e) {
                    throw SetStateException("Could not find el with id of " + id.string() + " in set");
                }
                [[maybe_unused]] SetLock elLock = m_el.m_manager->lockEl(*el);
                SetNode* node = search(id, this->m_root);
                if (!node) {
                    throw SetStateException("Could not find el with id of " + id.string() + "in set");
                }
                this->m_size--;
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
                // for (auto superSet : this->m_superSets) {
                    // create a copy
                    // SetNode* copy = AllocationPolicy::createNode(node->m_ptr);
                    // TODO replace;
                // }
            }
            void reindexDFS(SetNode* node, AbstractSet* set) {
                for (auto superSet : set->m_superSets) {
                    reindexDFS(node, superSet);
                }
                if (set->m_superSets.size() == 0) {
                    SetNode* parent = getParent(node, this->m_root);
                    if (!parent) {
                        return;
                    }
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
                if (!node || !node->m_parent) {
                    return;
                } 
                reindexDFS(node, this);
            }

            void eraseElement(ID id) {
                try {
                    remove(id);
                } catch (SetStateException& exception) {

                }
            }

            SetType setType() const override {
                return AllocationPolicy::setType();
            }

            void adjustSuperSets(SetNode* node, std::unordered_set<AbstractSet*>& allSuperSetsAndMe) override {
                this->m_setToInstantiate ? this->m_setToInstantiate->adjustSuperSets(node, allSuperSetsAndMe) : AllocationPolicy::adjustSuperSets(node, allSuperSetsAndMe);
            }
            SetNode* createNode (Element& el) override {
                SetNode* ret = 0;
                if (this->m_setToInstantiate) {
                    ret = this->m_setToInstantiate->createNode(el);
                } else {
                    ret = AllocationPolicy::create(el);
                }
                ret->set = this;
                return ret;
            }
            SetNode* createNode (ID id) override {
                SetNode* ret = 0;
                if (this->m_setToInstantiate) {
                    ret = this->m_setToInstantiate->createNode(id);
                } else {
                    ret = AllocationPolicy::create(m_el.m_manager->createPtr(id));
                }
                ret->set = this;
                return ret;
            }
        public:
            PrivateSet(U& el) : m_el(el) {
                
            }
            PrivateSet(U* el) : m_el(*el) {

            }
            virtual ~PrivateSet() {
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el); 
                delete m_opposite;
                if (this->m_root) {
                    if (!this->m_root->m_ptr) {
                        // divider node
                        if (this->m_root->m_left && this->m_root->m_right) {
                            // bad
                            throw SetStateException("I know im not supposed to throw an error but this state is bad");
                        }
                        SetNode* newRoot = 0;
                        if (this->m_root->m_left) {
                            newRoot = this->m_root->m_left;
                            this->m_root->m_left = 0;
                        } else {
                            newRoot = this->m_root->m_right;
                            this->m_root->m_right = 0;
                        }
                        newRoot->m_parent = 0;
                        delete this->m_root;
                        this->m_root = newRoot;
                    }
                   
                    // adjust roots and parents from base of trees
                    std::list<AbstractSet*> queue;
                    for (auto superset : this->m_superSets) {
                        queue.push_back(superset);
                    }
                    while (!queue.empty()) {
                        AbstractSet* currSet = queue.front();
                        queue.pop_front();
                        if (currSet->m_root == this->m_root) {
                            currSet->m_root = 0;
                        } else if (currSet->m_root->m_left == this->m_root) {
                            currSet->m_root->m_left = 0;
                        } else if (currSet->m_root->m_right == this->m_root) {
                            currSet->m_root->m_right = 0;
                        }
                        for (auto superset : currSet->m_superSets) {
                            queue.push_back(superset);
                        }
                    }
                    this->m_root->m_parent = 0;

                    // delete everything
                    while (this->m_root) {
                        delete this->m_root;
                    }
                }

                for (auto superSet : this->m_superSets) {
                    superSet->m_subSets.erase(std::find(superSet->m_subSets.begin(), superSet->m_subSets.end(), this));
                }
                for (auto subSet : this->m_subSets) {
                    subSet->m_superSets.erase(std::find(subSet->m_superSets.begin(), subSet->m_superSets.end(), this));
                }
            }

            // root
            void setRoot(SetNode* node) override {
                this->m_root = node;
            }
            
            SetNode* getRoot() const override {
                return this->m_root;
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
                delete m_opposite;
                m_opposite = new OppositeInterfaceAdapter(m_el, oppositeSignature);
            }
            /**
             * makes sure that the set we are redefining is the same tree as ours and vice versa
             * @param redefined, the set that this set is redefining
             **/
            template <class V, class W> 
            void redefines(TypedSet<V, W>& redefined) {
                if (this->m_root) {
                    throw SetStateException("WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!");
                }

                // add to redefined set's redefines redefines
                for (AbstractSet* redefinedSet : redefined.m_redefines) {
                    redefinedSet->m_redefines.insert(this);
                    this->m_redefines.insert(redefinedSet);
                }

                // make sure our subsets and supersets are the same
                for (auto set : redefined.m_superSets) {
                    this->subsets(*set);
                }
                for (auto subSet : redefined.m_subSets) {
                    subSet->subsets(*this);
                }
                for (auto set : this->m_superSets) {
                    redefined.subsets(*set);
                }
                for (auto subSet : this->m_subSets) {
                    subSet->subsets(redefined);
                }

                // add this set's redefines to redefined set
                for (AbstractSet* redefinedSet : this->m_redefines) {
                    redefinedSet->m_redefines.insert(&redefined);
                    redefined.m_redefines.insert(redefinedSet);
                }

                // add to our redefines
                this->m_redefines.insert(&redefined);

                // add to redefined set's redefines
                redefined.m_redefines.insert(this);

                for (AbstractSet* redefinedSet : this->m_redefines) {
                    redefinedSet->m_rootRedefinedSet = false;
                    if (redefinedSet->setType() == SetType::ORDERED_SET && this->setType() != SetType::ORDERED_SET) {
                        this->m_setToInstantiate = redefinedSet;
                    } else if (this->setType() == SetType::ORDERED_SET && redefinedSet->setType() != SetType::ORDERED_SET && !redefinedSet->m_setToInstantiate) {
                        redefinedSet->m_setToInstantiate = this;
                    }
                }
            }

            // Shared Accessors, all of these can be used by subclasses
            T& get(ID id) const {
                // "lock" sets owner while we search
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                
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
                [[maybe_unused]] SetLock myLck = m_el.lockEl(m_el);

                if (!this->m_root) {
                    throw SetStateException("Could not find named element of name " + name + " in set!");
                }

                SetNode* result = search(name, this->m_root);

                if (result) {
                    return result->m_ptr->as<T>();
                }
                
                throw SetStateException("Could not find named element of name " + name + " in set!");
            }

            bool contains(ID id) const {
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el);
                
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
                [[maybe_unused]] SetLock myLck = m_el.lockEl(m_el);

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
    template <class T> class PtrSet;

    template <class T>
    class SetIterator {

        template <class V> friend class ID_Set;
        template <class V> friend class PtrSet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomReadOnlySet;

        protected:
            SetNode* root = 0;
            SetNode* curr = 0;
        public:
            SetIterator() {};
            SetIterator(SetIterator& rhs) {
                root = rhs.root;
                curr = rhs.curr;
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
                            if (temp->m_right && temp->m_left != root) {
                                if (temp->m_right->m_ptr.id() != last->m_ptr.id()) {
                                    found = true;
                                    break;
                                } else if (temp == root) {
                                    curr = 0;
                                    break;
                                }
                            }
                            last = temp;
                        } while (temp->m_parent);
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
                if (!lhs.curr && !rhs.curr) {
                    return true;
                }
                if (!lhs.curr || !rhs.curr) {
                    return false;
                }
                return lhs.curr == rhs.curr;
            }
            friend bool operator!= (const SetIterator& lhs, const SetIterator& rhs) {
                if (!lhs.curr && !rhs.curr) {
                    return false;
                }
                if (!lhs.curr || !rhs.curr) {
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

        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomReadOnlySet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            SetNode* root = 0;
        public:
            SetID_Iterator<T> begin() {
                SetID_Iterator<T> ret;
                ret.curr = root;
                ret.root = root;
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

    template <class T>
    class SetPtrIterator : public SetIterator<T> {
        public:
            UmlPtr<T> operator*() {
                return this->curr->m_ptr;
            }
    };

    template <class T>
    class PtrSet {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomReadOnlySet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            SetNode* root = 0;
        public:
            SetPtrIterator<T> begin() {
                SetPtrIterator<T> ret;
                ret.curr = root;
                ret.root = root;
                return ret;
            };
            SetPtrIterator<T> end() {
                return SetPtrIterator<T>();
            };
            UmlPtr<T> front() const {
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

                return curr->m_ptr;
            }
            UmlPtr<T> back() const {
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

                return curr->m_ptr;
            }
    };
}

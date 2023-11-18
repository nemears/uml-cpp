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

            void adjustSuperSets(__attribute__((unused)) SetNode* node) {}
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

            // gets closest relative between setA and setB via the tree of rootSet
            AbstractSet* getClosestSuperset(AbstractSet* setA, AbstractSet* setB, AbstractSet* rootSet) {
                std::list<AbstractSet*> queue;
                queue.push_back(rootSet);
                AbstractSet* relative = 0;
                do {
                    AbstractSet* currSet = queue.front();
                    queue.pop_front();

                    // compare set to the node's set's supersets
                    std::list<AbstractSet*> stackA;
                    stackA.push_front(setA);
                    bool superSetOfA = false;
                    do {
                        AbstractSet* currSetA = stackA.front();
                        stackA.pop_front();
                        if (currSetA == currSet) {
                            superSetOfA = true;
                            break;
                        }
                        for (auto superset : currSetA->m_superSets) {
                            stackA.push_front(superset);
                        }
                    } while (!stackA.empty());

                    // compare set to rootNode's set's supersets
                    std::list<AbstractSet*> stackB;
                    stackB.push_front(setB);
                    bool superSetOfB = false;
                    do {
                        AbstractSet* currRootSet = stackB.front();
                        stackB.pop_front();

                        if (currRootSet == currSet) {
                            superSetOfB = true;
                            break;
                        }
                        
                        for (auto superset : currRootSet->m_superSets) {
                            stackB.push_front(superset);
                        }
                    } while (!stackB.empty());

                    if (superSetOfA && superSetOfB) {
                        // if both are good, we have a candidate
                        relative = currSet;

                        // add children of candidates, only they can be candidates further down the tree
                        for (auto subset : currSet->m_subSets) {
                            queue.push_back(subset);
                        }
                    }
                } while (!queue.empty());

                return relative;
            }

            SetNode* createDividerNode(SetNode* node, SetNode* existingRoot, AbstractSet* rootSet) {
                SetNode* dividerNode = new SetNode;
                AbstractSet* candidate = getClosestSuperset(existingRoot->set, node->set, rootSet);

                if (!candidate) {
                    throw SetStateException("could not find common set for divider node in set, please contact developer!");
                }

                dividerNode->set = candidate;
                if (node->m_ptr.id() < existingRoot->m_ptr.id()) {
                    dividerNode->m_left = node;
                    dividerNode->m_right = existingRoot;
                } else {
                    dividerNode->m_right = node;
                    dividerNode->m_left = existingRoot;
                }
                node->m_parent = dividerNode;
                existingRoot->m_parent = dividerNode;
                return dividerNode;
            }

            void insertDividerIntoTree(SetNode* node, SetNode* existingRoot, SetNode* dividerNode, AbstractSet* rootSet, std::unordered_set<AbstractSet*>& visited) {
                SetNode* currNode = rootSet->m_root;

                // adjust roots of super sets
                std::list<AbstractSet*> queue;
                queue.push_back(node->set);
                do {
                    AbstractSet* currSet = queue.front();
                    queue.pop_front();
                    if (currSet->m_root == existingRoot) {
                        currSet->m_root = dividerNode;
                        for (auto redefinedSet : currSet->m_redefines) {
                            redefinedSet->m_root = dividerNode;
                        }
                        visited.insert(currSet);
                    }
                    for (auto superset : currSet->m_superSets) {
                        queue.push_back(superset);
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
                        break;
                    } else if (currNode->m_right == existingRoot) {
                       // currNode is parent of existing root in this tree,
                       // replace m_right with dividerNode 
                       currNode->m_right = dividerNode;
                       dividerNode->m_parent = currNode;
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

            void adjustTreeRoots(SetNode* dividerNode, AbstractSet* set) {
                std::list<AbstractSet*> stack;
                stack.push_front(set);
                do {
                    AbstractSet* superset = stack.front();
                    stack.pop_front();
                    if (superset->m_root != dividerNode) {
                        superset->m_root = dividerNode;
                        for (auto redefinedSet : superset->m_redefines) {
                            redefinedSet->m_root = dividerNode;
                        }
                        for (auto superSuperSet : superset->m_superSets) {
                            stack.push_front(superSuperSet);
                        }
                    }
                } while (!stack.empty());
            }

            bool isParent(AbstractSet* set, AbstractSet* parentCandidate) {
                std::list<AbstractSet*> stack;
                stack.push_front(set);
                do {
                    AbstractSet* currParentSet = stack.front();
                    stack.pop_front();
                    if (currParentSet == parentCandidate) {
                        return true;
                    }
                    for (auto superset : currParentSet->m_superSets) {
                        stack.push_front(superset);
                    }
                } while (!stack.empty());
                return false;
            }

            void addNodeToSet(SetNode* node) {
                // sort through redefines to find main set
                for (auto redefinedSet : node->set->m_redefines) {
                    if (redefinedSet->m_rootRedefinedSet) {
                        node->set = redefinedSet;
                    }
                }

                // adjust supersets
                node->set->adjustSuperSets(node);

                std::list<AbstractSet*> stack;
                std::unordered_set<AbstractSet*> visited;

                if (this->m_root && this->m_root->m_ptr && this->m_root->set == node->set) {
                    // this set has already been subsetted in active tree
                    // just insert into the tree skip complexity check
                    try {
                        this->m_root->insert(node);
                    } catch (SetStateException& exception) {
                        // delete node and propogate exception
                        delete node;
                        throw exception;
                    }
                } else {
                    // we want to place this in all of our root super sets
                    // climb up graph dfs to find root super sets
                    stack.push_front(node->set);
                    bool treeAlreadyCreated = false;
                    do {
                        // take set from top of stack
                        AbstractSet* set = stack.front();
                        stack.pop_front();

                        // skip sets allready visited
                        if (visited.count(set)) {
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
                                SetNode* existingNode;
                                if ((existingNode = search(node->m_ptr.id(), currNode))) { // slow but only way i think
                                    // node is in superset
                                    if (existingNode->set == node->set) {
                                        // TODO do we have to do some cleanup here?
                                        delete node;
                                        throw SetStateException("Duplicate element added to set!");
                                    } else {
                                        // I don't know if this works with multiple tree roots with unrelated sets 
                                        // TODO it doesn't deploymentTest.mountAndEdit
                                        // TODO test that!

                                        node->copyData(existingNode);

                                        existingNode->set->innerRemove(node->m_ptr.id());

                                        // I kind of get why we have to do this but I forget specifics
                                        if (node->m_ptr.loaded()) {
                                            m_opposite->skip = true;
                                        }

                                        // addNodeToSet(node);
                                        // TODO keep orderedSet stuff? ClassTest.addCompositePropertyTest
                                        continue;
                                    }
                                }
                                while (currNode) {
                                    if (!currNode->m_ptr) {
                                        // this is a divider node, see which side our node belongs
                                        std::list<AbstractSet*> dividerStack;
                                        dividerStack.push_front(node->set);
                                        bool createDividerNodeFlag = false;
                                        bool isLeft = false;
                                        bool isRight = false;
                                        do {
                                            AbstractSet* currSet = dividerStack.front();
                                            dividerStack.pop_front();
                                            if (!isLeft) {
                                                isLeft = currSet == currNode->m_left->set;
                                            }

                                            if (!isRight) {
                                                isRight = currSet == currNode->m_right->set;
                                            }

                                            for (auto superSet : currSet->m_superSets) {
                                                dividerStack.push_front(superSet);
                                            }
                                            if (dividerStack.empty()) {
                                                if (isLeft && isRight) {
                                                    // diamond subset handle
                                                    SetNode* leftDividerNode = createDividerNode(node, currNode->m_left, set);
                                                    SetNode* rightDividerNode = createDividerNode(node, currNode->m_right, set);
                                                    insertDividerIntoTree(node, currNode->m_left, leftDividerNode, set, visited);
                                                    insertDividerIntoTree(node, currNode->m_right, rightDividerNode, set, visited);

                                                    createDividerNodeFlag = true;
                                                } else if (isLeft) {
                                                    currNode = currNode->m_left;
                                                } else if (isRight) {
                                                    currNode = currNode->m_right;
                                                } else {
                                                    // neither side is appropriate, we need a new divider node
                                                    // find most appropriate side to base root off of
                                                    // basically see if either side is a subset of ours vs not
                                                    // left
                                                    bool leftIsSubset = false;
                                                    std::list<AbstractSet*> relativesQueue;
                                                    relativesQueue.push_back(currNode->m_left->set);
                                                    do {
                                                        AbstractSet* leftSuperSet = relativesQueue.front();
                                                        relativesQueue.pop_front();
                                                        if (leftSuperSet == node->set) {
                                                            leftIsSubset = true;
                                                            break;
                                                        }
                                                        for (auto superset : leftSuperSet->m_superSets) {
                                                            relativesQueue.push_back(superset);
                                                        }
                                                    } while (!relativesQueue.empty());

                                                    // right
                                                    bool rightIsSubset = false;
                                                    relativesQueue.push_back(currNode->m_right->set);
                                                    do {
                                                        AbstractSet* rightSuperSet = relativesQueue.front();
                                                        relativesQueue.pop_front();
                                                        if (rightSuperSet == node->set) {
                                                            rightIsSubset = true;
                                                            break;
                                                        }
                                                        for (auto superset : rightSuperSet->m_superSets) {
                                                            relativesQueue.push_back(superset);
                                                        }
                                                    } while (!relativesQueue.empty());

                                                    if (leftIsSubset && rightIsSubset) {
                                                        // if both are a subset, create a dividerNode with currNode and node placing our set at the 
                                                        // "top" of the tree to maintain subsetting
                                                        SetNode* dividerNode = createDividerNode(node, currNode, set);
                                                        insertDividerIntoTree(node, currNode, dividerNode, set, visited);
                                                        // make sure our set's new root is the divider node, as well as parents until we have corrected
                                                        // and hit where divider node was created
                                                        adjustTreeRoots(dividerNode, node->set);
                                                        createDividerNodeFlag = true;
                                                    } else if (leftIsSubset) {
                                                        // if only left is a subset, create a dividerNode with currNode->m_left placing currNode->m_right
                                                        // on top of this set, but keep currNode->m_left below our node
                                                        SetNode* dividerNode = createDividerNode(node, currNode->m_left, set);

                                                        // check for if currNode->set is a superset
                                                        if (isParent(node->set, currNode->set)) {
                                                            // we must also make sure our set's new root is the divider node to include currNode->m->left and node
                                                            insertDividerIntoTree(node, currNode->m_left, dividerNode, set, visited);
                                                            adjustTreeRoots(dividerNode, node->set);
                                                        } else {
                                                            // create divider node where node and currNode->m_left are children, 
                                                            // and then create divider node where dividerNode and currNode are children
                                                            SetNode* newRoot = createDividerNode(dividerNode, currNode, set);
                                                            insertDividerIntoTree(dividerNode, currNode, newRoot, set, visited);
                                                            adjustTreeRoots(dividerNode, node->set);
                                                            AbstractSet* closestSuperset = getClosestSuperset(node->set, currNode->set, currSet);
                                                            adjustTreeRoots(newRoot, closestSuperset);
                                                        }
                                                        createDividerNodeFlag = true;
                                                    } else if (rightIsSubset) {
                                                        // if only right is a subset create a dividerNode with currNode->m_right placing currNode->m_left
                                                        // on top of this set, but keep currNode->m_right below our node
                                                        SetNode* dividerNode = createDividerNode(node, currNode->m_right, set);
                                                        // we must also make sure our set's new root is the divider node to include currNode->m->m_right and node
                                                        
                                                        // check for if currNode->set is a superset
                                                        if (isParent(node->set, currNode->set)) {
                                                            insertDividerIntoTree(node, currNode->m_right, dividerNode, set, visited);
                                                            adjustTreeRoots(dividerNode, node->set);
                                                        } else {
                                                            // create divider node where node and currNode->m_left are children, 
                                                            // and then create divider node where dividerNode and currNode are children
                                                            SetNode* newRoot = createDividerNode(dividerNode, currNode, set);
                                                            insertDividerIntoTree(dividerNode, currNode, newRoot, set, visited);
                                                            adjustTreeRoots(dividerNode, node->set);
                                                            AbstractSet* closestSuperset = getClosestSuperset(node->set, currNode->set, currSet);
                                                            adjustTreeRoots(newRoot, closestSuperset);
                                                        }
                                                        createDividerNodeFlag = true;
                                                    } else {
                                                        // neither side is a subset, make a dividerNode and make sure our root stays as node, not the divider node
                                                        // choose most appropriate side, left or right and recurse, or if 
                                                        // neither side is more appropriate than currNode, make dividerNode here
                                                        AbstractSet* rightCandidate = getClosestSuperset(node->set, currNode->m_right->set, set);
                                                        AbstractSet* leftCandidate = getClosestSuperset(node->set, currNode->m_left->set, set);
                                                        // find out which one is closer,
                                                        std::list<AbstractSet*> distanceStack;
                                                        distanceStack.push_front(node->set);
                                                        size_t distance = 0;
                                                        size_t rightDistance = 0;
                                                        size_t leftDistance = 0;
                                                        size_t currNodeDistance = 0;
                                                        do {
                                                            AbstractSet* distanceSet = distanceStack.front();
                                                            distanceStack.pop_front();
                                                            if (distanceSet == rightCandidate) {
                                                                rightDistance = distance;
                                                            } 
                                                            if (distanceSet == leftCandidate) {
                                                                leftDistance = distance;
                                                            }
                                                            if (distanceSet == currNode->set) {
                                                                currNodeDistance = distance;
                                                            }

                                                            if (distanceSet->m_superSets.empty()) {
                                                                distance = 0;
                                                            } else {
                                                                distance++;
                                                                for (auto superset : distanceSet->m_superSets) {
                                                                    distanceStack.push_front(superset);
                                                                }
                                                            }
                                                        } while (!distanceStack.empty());

                                                        if (rightDistance < leftDistance && rightDistance < currNodeDistance) {
                                                            // recurse right
                                                            currNode = currNode->m_right;
                                                        } else if (leftDistance < rightDistance && leftDistance < currNodeDistance) {
                                                            // recurse left
                                                            currNode = currNode->m_left;
                                                        } else {
                                                            // create divider node and put us on top off currNode
                                                            SetNode* dividerNode = createDividerNode(node, currNode, set);
                                                            insertDividerIntoTree(node, currNode, dividerNode, set, visited);
                                                            createDividerNodeFlag = true;
                                                            // don't adjust roots
                                                        }
                                                    }
                                                }
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
                                            // make sure our root stays as node
                                            SetNode* dividerNode = createDividerNode(node, currNode, set);
                                            insertDividerIntoTree(node, currNode, dividerNode, set, visited);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    } while (!stack.empty());
                }

                // adjust roots and size
                visited.clear();
                stack.push_front(node->set);
                do {
                    AbstractSet* set = stack.front();
                    stack.pop_front();

                    if (visited.count(set)) {
                        continue;
                    } else {
                        visited.insert(set);
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

                    for (auto superset : set->m_superSets) {
                        stack.push_front(superset);
                    }
                } while (!stack.empty());
            }

            void innerAdd(T& el) override {
                SetNode* node = this->createNode(el);
                addNodeToSet(node);

                // run add policies bfs with queue
                std::list<AbstractSet*> queue;
                queue.push_back(node->set);
                do {
                    AbstractSet* set = queue.front();
                    queue.pop_front();
                    for (auto superSet : set->m_superSets) {
                        queue.push_back(superSet);
                    }
                    set->runAddPolicy(el);
                    for (auto redefinedSet : set->m_redefines) {
                        redefinedSet->runAddPolicy(el);
                    }
                } while (!queue.empty());
            }

            void innerAdd(ID id) {
                SetNode* node = createNode(id);
                addNodeToSet(node);
                // we are done, we can't run policies because we don't have both elements
            }

            bool isValidSuccesor(AbstractSet* succesorCandidateSet, AbstractSet* setToCheck) {
                std::list<AbstractSet*> stack;
                stack.push_front(setToCheck);
                do {
                    AbstractSet* set = stack.front();
                    stack.pop_front();

                    if (set == succesorCandidateSet) {
                        // it is subset we are good
                        return true;
                    }

                    for (auto superset : set->m_superSets) {
                        stack.push_front(superset);
                    }
                } while (!stack.empty());

                return false;
            }

            void removeDividerNode(SetNode* dividerNodeToDelete, SetNode* remainingChild) {
                SetNode* successor = dividerNodeToDelete->m_parent;
                remainingChild->m_parent = successor;
                if (successor) {
                    if (successor->m_left == dividerNodeToDelete) {
                        successor->m_left = remainingChild;
                        if (!isValidSuccesor(dividerNodeToDelete->set, successor->m_right->set)) {
                            successor = remainingChild;
                        }
                    } else {
                        successor->m_right = remainingChild;
                        if (!isValidSuccesor(dividerNodeToDelete->set, successor->m_left->set)) {
                            successor = remainingChild;
                        }
                    }
                }

                if (!successor) {
                    successor = remainingChild;
                }

                // adjust intermediate roots
                AbstractSet* rootRedefinedSet = this;
                for (auto redefinedSet : this->m_redefines) {
                    if (redefinedSet->m_rootRedefinedSet) {
                        rootRedefinedSet = redefinedSet;
                    }
                }

                std::list<AbstractSet*> stack;
                stack.push_front(rootRedefinedSet);
                do {
                    AbstractSet* currSet = stack.front();
                    stack.pop_front();
                    if (currSet->m_root == dividerNodeToDelete) {
                        SetNode* newRoot = successor;
                        if (!this->isValidSuccesor(successor->set, currSet)) {
                            newRoot = remainingChild;
                        }
                        currSet->m_root = newRoot;
                        for (auto redefinedSet : currSet->m_redefines) {
                            redefinedSet->m_root = newRoot;
                        }
                        for (auto superset : currSet->m_superSets) {
                            if (superset->m_root == dividerNodeToDelete) {
                                stack.push_front(superset);
                            }
                        }
                    }
                    if (currSet->m_root != successor) {
                        for (auto superset : currSet->m_superSets) {
                            stack.push_front(superset);
                        }
                    }
                } while (!stack.empty());
            }

            void innerRemove(ID id) override {
                if (!this->m_root) {
                    throw SetStateException("could not rempve element of id " + id.string() + " in set, set is empty!");
                }
                SetNode* node = search(id, this->m_root);
                if (!node) {
                    throw SetStateException("could not remove element of id " + id.string() + " in set, element with that id could not be located within the set!");
                }
                std::list<AbstractSet*> stack;
                stack.push_front(this);
                if (node->m_parent && !node->m_parent->m_ptr && !node->m_left && !node->m_right) {
                    // parent is a divider node that needs to be removed
                    do {
                        AbstractSet* currSet = stack.front();
                        stack.pop_front();

                        if (currSet->m_root == node) {
                            currSet->m_root = 0;
                            for (auto superset : currSet->m_superSets) {
                                stack.push_front(superset);
                            }
                        } else {
                            SetNode* currNode = currSet->m_root;
                            while (currNode) {
                                if (currNode->m_left == node) {
                                    SetNode* dividerNodeToDelete = currNode;
                                    dividerNodeToDelete->m_left = 0;
                                    removeDividerNode(dividerNodeToDelete, dividerNodeToDelete->m_right);
                                    dividerNodeToDelete->m_right = 0;
                                    dividerNodeToDelete->m_parent = 0;
                                    delete dividerNodeToDelete;
                                    break;
                                } else if (currNode->m_right == node) {
                                    SetNode* dividerNodeToDelete = currNode;
                                    currNode->m_right = 0;
                                    removeDividerNode(dividerNodeToDelete, dividerNodeToDelete->m_left);
                                    dividerNodeToDelete->m_left = 0;
                                    dividerNodeToDelete->m_parent = 0;
                                    delete dividerNodeToDelete;
                                    break;
                                } else {
                                    // find the node
                                    if (search(id, currNode->m_left)) {
                                        currNode = currNode->m_left;
                                    } else if (search(id, currNode->m_right)) {
                                        currNode = currNode->m_right;
                                    } else {
                                        throw SetStateException("Bad state for removing node from tree, cannot find node in either subtree, contact dev!");
                                    }
                                }
                            }
                        }
                    } while (!stack.empty());
                    node->m_parent = 0;
                } else {
                    // just adjust roots
                    do {
                        AbstractSet* currSet = stack.front();
                        stack.pop_front();

                        if (currSet->m_root == node && !node->m_left && !node->m_right) {
                            currSet->m_root = 0;
                        }

                        for (auto superset : currSet->m_superSets) {
                            stack.push_front(superset);
                        }
                    } while (!stack.empty());
                }

                // adjust all sets this node affects
                stack.push_front(node->set);
                std::unordered_set<AbstractSet*> visited;
                do {
                    AbstractSet* currSet = stack.front();
                    stack.pop_front();
                    if (visited.count(currSet)) {
                        continue;
                    } else {
                        visited.insert(currSet);
                    }
                    if (currSet->m_root == node && !node->m_left && !node->m_right) {
                        currSet->m_root = 0;
                    }

                    // handle redefines
                    for (auto redefinedSet : currSet->m_redefines) {
                        // set root
                        if (redefinedSet->m_root == node && !node->m_left && !node->m_right) {
                            redefinedSet->m_root = 0;
                        }

                        // increase size
                        redefinedSet->m_size--;
                    }

                    currSet->m_size--;

                    for (auto superset : currSet->m_superSets) {
                        stack.push_front(superset);
                    }
                } while (!stack.empty());

                // run removal policies
                std::list<AbstractSet*> queue;
                queue.push_back(node->set);
                do {
                    AbstractSet* set = queue.front();
                    queue.pop_front();
                    for (auto superSet : set->m_superSets) {
                        queue.push_back(superSet);
                    }
                    set->runRemovePolicy(*node->m_ptr);
                    for (auto redefinedSet : set->m_redefines) {
                        redefinedSet->runRemovePolicy(*node->m_ptr);
                    }
                } while (!queue.empty());

                delete node;
            }

            // bugged out with redefines!
            void handleOppositeAdd(T& el) {
                if (m_opposite->skip) {
                    m_opposite->skip = false;
                    return;
                }
                std::list<AbstractSet*> queue;
                if (!this->m_rootRedefinedSet) {
                    for (auto redefinedSet : this->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            queue.push_back(redefinedSet);
                        }
                    }
                    if (queue.empty()) {
                        throw SetStateException("Bad state with redefines, called in PrivateSet::handleOppositeAdd, contact developer!");
                    }
                } else {
                    queue.push_back(this);
                }
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    if (front->oppositeEnabled()) {
                        // opposite handled
                        front->oppositeAdd(el);
                    } else {
                        // just leave it up to the redefines, they might override it
                        // TODO this is based on id hash, not necessarily a good order to go by
                        // TODO redo redefines with a list probably to make the redefine order matter based on
                        // TODO order overwritten
                        bool oppositeFound = false;
                        for (auto redefinedSet : front->m_redefines) {
                            if (redefinedSet->oppositeEnabled()) {
                                redefinedSet->oppositeAdd(el);
                                oppositeFound = true;
                                break;
                            }
                        }
                        if (!oppositeFound) {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
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
                if (m_opposite->skip) {
                    m_opposite->skip = false;
                    return;
                }
                std::list<AbstractSet*> queue;
                if (!this->m_rootRedefinedSet) {
                    for (auto redefinedSet : this->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            queue.push_back(redefinedSet);
                        }
                    }
                    if (queue.empty()) {
                        throw SetStateException("Bad state with redefines, called in PrivateSet::handleOppositeAdd, contact developer!");
                    }
                } else {
                    queue.push_back(this);
                }
                while (!queue.empty()) {
                    AbstractSet* front = queue.front();
                    queue.pop_front();
                    if (front->oppositeEnabled()) {
                        front->oppositeRemove(el);
                    } else {
                        // just leave it up to the redefines, they might override it
                        // TODO this is based on id hash, not necessarily a good order to go by
                        // TODO redo redefines with a list probably to make the redefine order matter based on
                        // TODO order overwritten
                        bool oppositeFound = false;
                        for (auto redefinedSet : front->m_redefines) {
                            if (redefinedSet->oppositeEnabled()) {
                                redefinedSet->oppositeRemove(el);
                                oppositeFound = true;
                                break;
                            }
                        }
                        if (!oppositeFound) {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
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

            // TODO remove or reimplement
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
                    node->set = *this->m_superSets.begin();
                    return;
                }
                // for (auto superSet : this->m_superSets) {
                    // create a copy
                    // SetNode* copy = AllocationPolicy::createNode(node->m_ptr);
                    // TODO replace;
                // }
            }

            // TODO reimplement
            void reindex(ID newID) {
                if (!this->m_root) {
                    return;
                }
                SetNode* node = search(newID, this->m_root);
                if (!node || !node->m_parent) {
                    return;
                } 
                // reindexDFS(node, this);
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

            void adjustSuperSets(SetNode* node) override {
                this->m_setToInstantiate ? this->m_setToInstantiate->adjustSuperSets(node) : AllocationPolicy::adjustSuperSets(node);
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
            PrivateSet(U& el) : m_el(el) {}
            PrivateSet(U* el) : m_el(*el) {}
            virtual ~PrivateSet() {
                [[maybe_unused]] SetLock myLock = m_el.m_manager->lockEl(m_el); 
                delete m_opposite;
                if (this->m_root && this->m_rootRedefinedSet && this->m_root->set == this) {
                    if (this->m_root->m_ptr) {
                        // not a divider node

                        // adjust roots and parents from base of trees
                        std::list<AbstractSet*> queue;
                        for (auto superset : this->m_superSets) {
                            queue.push_back(superset);
                        }
                        while (!queue.empty()) {
                            AbstractSet* currSet = queue.front();
                            queue.pop_front();
                            if (currSet->m_root) {
                                if (currSet->m_root == this->m_root) {
                                    // it is the root
                                    currSet->m_root = 0;
                                } else {
                                    // it is a divider node, make sure we are destroying properly by checking all divider nodes
                                    std::list<SetNode*> dividerStack;
                                    dividerStack.push_front(currSet->m_root);
                                    do {
                                        SetNode* currNode = dividerStack.front();
                                        dividerStack.pop_front();
                                        if (currNode->m_left == this->m_root) {
                                            currNode->m_left = 0;
                                            if (currNode->m_right) {
                                                removeDividerNode(currNode, currNode->m_right);
                                                currNode->m_parent = 0;
                                                currNode->m_right = 0;
                                                delete currNode;
                                            }
                                            break;
                                        } else if (currNode->m_right == this->m_root) {
                                            currNode->m_right = 0;
                                            if (currNode->m_left) {
                                                removeDividerNode(currNode, currNode->m_left);
                                                currNode->m_parent = 0;
                                                currNode->m_left = 0;
                                                delete currNode;
                                            }
                                            break;
                                        }
                                        // recurse left
                                        if (currNode->m_left && !currNode->m_left->m_ptr) {
                                            dividerStack.push_front(currNode->m_left);
                                        }
                                        // recurse right
                                        if (currNode->m_right && !currNode->m_right->m_ptr) {
                                            dividerStack.push_front(currNode->m_right);
                                        }
                                    } while (!dividerStack.empty());
                                }
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
                    } else {
                        // It is a divider node, try to adjust to redefine
                        if (this->m_redefines.empty()) {
                            throw SetStateException("Report to Developer! \nI know don't throw errors in destructor :( \nTODO remove this when confident in destructor and ready to release");
                        }
                        this->m_root->set = *this->m_redefines.begin();
                    }
                }

                // adjust redefines, subsets, supersets
                for (auto redefinedSet : this->m_redefines) {
                    if (this->m_rootRedefinedSet) {
                        // switch with first
                        redefinedSet->m_rootRedefinedSet = true;
                        this->m_rootRedefinedSet = false;
                        for (auto superSet : this->m_superSets) {
                            redefinedSet->m_superSets.insert(superSet);
                            superSet->m_subSets.insert(redefinedSet);
                        }
                        for (auto subSet : this->m_subSets) {
                            redefinedSet->m_subSets.insert(subSet);
                            subSet->m_superSets.insert(redefinedSet);
                        }
                    }
                    redefinedSet->m_redefines.remove(this);
                }
                for (auto superSet : this->m_superSets) {
                    superSet->m_subSets.erase(this);
                    
                    // add remaining subsets to supersets to keep track of still
                    for (auto subSet : this->m_subSets) {
                        superSet->m_subSets.insert(subSet);
                        subSet->m_superSets.insert(superSet);
                    }
                }
                for (auto subSet : this->m_subSets) {
                    subSet->m_superSets.erase(this);
                }
            }

            // root
            void setRoot(SetNode* node) override {
                AbstractSet* currSet = this;
                for (auto redefinedSet : this->m_redefines) {
                    if (redefinedSet->m_rootRedefinedSet) {
                        currSet = redefinedSet;
                    }
                }

                std::list<AbstractSet*> stack;
                stack.push_front(currSet);

                // keep track of original root
                SetNode* oldRoot = currSet->m_root;

                // replace all roots that shared with this set
                do {
                    currSet = stack.front();
                    stack.pop_front();
                    if (currSet->m_root == oldRoot) {
                        currSet->m_root = node;
                        for (auto redefinedSet : currSet->m_redefines) {
                            redefinedSet->m_root = node;
                        }
                        for (auto superset : currSet->m_superSets) {
                            stack.push_front(superset);
                        }
                    }
                } while (!stack.empty());
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
             * makes sure that only one set of the redefines has subsets and supersets
             * @param redefined, the set that this set is redefining
             **/
            template <class V, class W> 
            void redefines(TypedSet<V, W>& redefined) {
                if (this->m_root) {
                    throw SetStateException("WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!");
                }

                AbstractSet* oldRootRedefined = &redefined;

                if (!oldRootRedefined->m_rootRedefinedSet) {
                    for (AbstractSet* redefinedSet : redefined.m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            oldRootRedefined = redefinedSet;
                            break;
                        }
                    }
                }

                // add to redefined set's redefines redefines
                for (AbstractSet* redefinedSet : oldRootRedefined->m_redefines) {
                    if (std::find(redefinedSet->m_redefines.begin(), redefinedSet->m_redefines.end(), this) == redefinedSet->m_redefines.end()) {
                        redefinedSet->m_redefines.push_front(this);
                    }
                    if (std::find(this->m_redefines.begin(), this->m_redefines.end(), redefinedSet) == this->m_redefines.end()) {
                        this->m_redefines.push_front(redefinedSet);
                    }                    
                }

                // add this set's redefines to redefined set
                for (AbstractSet* redefinedSet : this->m_redefines) {
                    if (std::find(redefinedSet->m_redefines.begin(), redefinedSet->m_redefines.end(), oldRootRedefined) == redefinedSet->m_redefines.end()) {
                        redefinedSet->m_redefines.push_front(oldRootRedefined);
                    }
                    if (std::find(oldRootRedefined->m_redefines.begin(), oldRootRedefined->m_redefines.end(), redefinedSet) == oldRootRedefined->m_redefines.end()) {
                        oldRootRedefined->m_redefines.push_front(redefinedSet);
                    }
                    for (AbstractSet* otherSetsRedefinedSet : oldRootRedefined->m_redefines) {
                        if (redefinedSet != otherSetsRedefinedSet) {
                            // dont add itself
                            if (std::find(redefinedSet->m_redefines.begin(), redefinedSet->m_redefines.end(), otherSetsRedefinedSet) == redefinedSet->m_redefines.end()) {
                                redefinedSet->m_redefines.push_front(otherSetsRedefinedSet);
                            }
                            if (std::find(otherSetsRedefinedSet->m_redefines.begin(), otherSetsRedefinedSet->m_redefines.end(), redefinedSet) == otherSetsRedefinedSet->m_redefines.end()) {
                                otherSetsRedefinedSet->m_redefines.push_front(redefinedSet);
                            }
                        }
                    }
                }

                if (std::find(this->m_redefines.begin(), this->m_redefines.end(), oldRootRedefined) == this->m_redefines.end()) {
                    this->m_redefines.push_front(oldRootRedefined);
                }
                
                if (std::find(oldRootRedefined->m_redefines.begin(), oldRootRedefined->m_redefines.end(), this) == oldRootRedefined->m_redefines.end()) {
                    oldRootRedefined->m_redefines.push_front(this);
                }

                // move subsets and supersets from theirs to ours
                for (auto superset : oldRootRedefined->m_superSets) {
                    this->m_superSets.insert(superset);
                    superset->m_subSets.erase(oldRootRedefined);
                    superset->m_subSets.insert(this);
                }
                oldRootRedefined->m_superSets.clear();

                for (auto subset : oldRootRedefined->m_subSets) {
                    this->m_subSets.insert(subset);
                    subset->m_superSets.erase(oldRootRedefined);
                    subset->m_superSets.insert(this);
                }
                oldRootRedefined->m_subSets.clear();


                for (AbstractSet* redefinedSet : this->m_redefines) {
                    redefinedSet->m_rootRedefinedSet = false;
                    if (redefinedSet->setType() == SetType::ORDERED_SET && this->setType() != SetType::ORDERED_SET) {
                        this->m_setToInstantiate = redefinedSet;
                    } else if (this->setType() == SetType::ORDERED_SET && redefinedSet->setType() != SetType::ORDERED_SET && !redefinedSet->m_setToInstantiate) {
                        redefinedSet->m_setToInstantiate = this;
                    }
                }

                std::list<AbstractSet*> queue;
                queue.push_back(this);
                do {
                    AbstractSet* currSet = queue.front();
                    queue.pop_front();

                    if (currSet->setType() != SetType::ORDERED_SET || currSet == this) {
                        if (currSet != this) {
                            currSet->m_setToInstantiate = this;
                        }
                        for (auto subset : currSet->m_subSets) {
                            queue.push_back(subset);
                        }
                    }
                } while (!queue.empty());
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
            std::list<SetNode*> m_stack;

            void loadleft(SetNode* node) {
                SetNode* curr = node;
                while (curr) {
                    m_stack.push_front(curr);
                    curr = curr->m_left;
                }
            }

        public:
            SetIterator() {};
            SetIterator(SetIterator& rhs) {
                m_stack = rhs.m_stack;
            }
            T& operator*() {
                return m_stack.front()->m_ptr->as<T>();
            }
            UmlPtr<T> operator->() {
                return m_stack.front()->m_ptr;
            }
            SetIterator<T> operator++() {
                do {
                    SetNode* front = m_stack.front();
                    m_stack.pop_front();
                    if (front->m_right) {
                        loadleft(front->m_right);
                    }
                } while (!m_stack.empty() && !m_stack.front()->m_ptr);

                return *this;
            }
            SetIterator operator++(int) {
                return ++(*this);
            }
            friend bool operator== (const SetIterator& lhs, const SetIterator& rhs) {
                if (lhs.m_stack.size() != rhs.m_stack.size()) {
                    return false;
                }

                if (lhs.m_stack.empty() && rhs.m_stack.empty()) {
                    return true;
                }

                return lhs.m_stack.front() == rhs.m_stack.front();
            }
            friend bool operator!= (const SetIterator& lhs, const SetIterator& rhs) {
                if (lhs.m_stack.size() != rhs.m_stack.size()) {
                    return true;
                }

                if (lhs.m_stack.empty() && rhs.m_stack.empty()) {
                    return false;
                }

                return lhs.m_stack.front() != rhs.m_stack.front();
            }
    };

    template <class T>
    class SetID_Iterator : public SetIterator<T> {
        public:
            ID operator*() {
                return this->m_stack.front()->m_ptr.id();
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
                if (!root) {
                    return ret;
                }
                ret.loadleft(root);
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
                return this->m_stack.front()->m_ptr;
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
                if (!root) {
                    return ret;
                }

                ret.loadleft(root);

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

#pragma once

#include "uml/types/namedElement.h"
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
        virtual void deleteNode() {
            delete this;
        }
    };

    template <class T, class U> class TypedSet;

    enum class SetType {
        SET,
        ORDERED_SET,
        BAG,
        LIST
    };

    class AbstractSet {

        template <class T, class U, class AdditionPolicy, class RemovalPolicy, class AllocationPolicy> friend class PrivateSet;
        friend class OrderedSetNodeAllocationPolicy;
        friend class OrderedSetNode;

        protected:
            SetNode* m_root = 0;

            std::vector<AbstractSet*> m_superSets;
            std::vector<AbstractSet*> m_subSets;
            std::vector<AbstractSet*> m_redefines;

            AbstractSet* m_setToInstantiate = 0;

            bool m_rootRedefinedSet = true;

            // m_guard lets us know the set's scope quickly, a subset must have a higher guard than its superset. root sets always have guards of 0
            size_t m_guard = 0;

            size_t m_size = 0;

            virtual void runAddPolicy(Element& el) = 0;
            virtual void runRemovePolicy(Element& el) = 0;
            virtual bool oppositeEnabled() = 0;
            virtual void oppositeAdd(Element& el) = 0;
            virtual void oppositeRemove(Element& el) = 0;
            virtual void handleOppositeRemove(Element& el) = 0;
            virtual SetType setType() const = 0;
            virtual void adjustSuperSets(SetNode* node, std::unordered_set<AbstractSet*>& allSuperSetsAndMe) = 0;
            virtual SetNode* createNode(Element& el) = 0;
            virtual SetNode* createNode(ID id) = 0;
            virtual void innerRemove(ID id) = 0;
            virtual void remove(ID id) = 0;


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
                    for (auto redefinedSet : front->m_redefines) {
                        allSuperSets.insert(redefinedSet);
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
                    for (auto redefinedSet : front->m_redefines) {
                        allSubSets.insert(redefinedSet);
                    }
                }
                return allSubSets;
            }

        public:

            AbstractSet& operator=(AbstractSet&&) = delete;

            /**
             * this set subsets the set supplied, meaning all elements within this set will be contained within the set supplied
             * but this set will not necessarily have all of the elements within the set supplied
             * @param subsetOf the set that we are a subset of
             **/
            void subsets(AbstractSet& subsetOf) {
                if (std::find(this->m_superSets.begin(), this->m_superSets.end(), &subsetOf) == this->m_superSets.end()) {
                    this->m_superSets.push_back(&subsetOf);
                    subsetOf.m_subSets.push_back(this);
                    if (subsetOf.m_guard >= m_guard) {
                        m_guard = subsetOf.m_guard + 1;
                    }
                    // Handle ordered set, TODO other set types
                    if (!m_setToInstantiate && subsetOf.setType() == SetType::ORDERED_SET /**&& setType() != SetType::ORDERED_SET**/) {
                        if (subsetOf.m_setToInstantiate) {
                            // we want highest up the tree orderedSet
                            m_setToInstantiate = subsetOf.m_setToInstantiate;
                        } else {
                            m_setToInstantiate = &subsetOf;
                        }
                    } else if (subsetOf.m_setToInstantiate) {
                        m_setToInstantiate = subsetOf.m_setToInstantiate;
                    }
                }
            }

            // virtual bool contains(ID id) const = 0;
    };

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

            void deleteNode(SetNode* node) {
                delete node;
            }
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

            void innerAdd(T& el) override {
                // add
                AbstractSet* instantiationgSet = this;
                if (!this->m_rootRedefinedSet) {
                    for (auto redefinedSet : this->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            instantiationgSet = redefinedSet;
                            break;
                        }
                    }
                }
                SetNode* node = instantiationgSet->createNode(el);
                
                std::unordered_set<AbstractSet*> allSuperSetsAndMe = this->getAllSuperSets();
                allSuperSetsAndMe.insert(node->set);
                for (auto redefinedSet : instantiationgSet->m_redefines) {
                    allSuperSetsAndMe.insert(redefinedSet);
                }
                std::unordered_set<AbstractSet*> allSubSets = instantiationgSet->getAllSubSets();

                node->set->adjustSuperSets(node, allSuperSetsAndMe);

                std::unordered_map<AbstractSet*, bool> visited;
                for (auto set : allSuperSetsAndMe) {
                    visited[set] = false;
                }

                innerAddDFS(node, node->set, allSuperSetsAndMe, allSubSets, visited);

                for (auto superSet : allSuperSetsAndMe) {
                    superSet->m_size++;
                }

                for (auto superSet : allSuperSetsAndMe) {
                    superSet->runAddPolicy(el);
                }
            }

            void innerAdd(ID id) {
                // add
                AbstractSet* instantiationgSet = this;
                if (!this->m_rootRedefinedSet) {
                    for (auto redefinedSet : this->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            instantiationgSet = redefinedSet;
                            this->m_size++;
                            break;
                        }
                    }
                }
                SetNode* node = instantiationgSet->createNode(id);
                
                std::unordered_set<AbstractSet*> allSuperSetsAndMe = this->getAllSuperSets();
                allSuperSetsAndMe.insert(node->set);
                for (auto redefinedSet : this->m_redefines) {
                    allSuperSetsAndMe.insert(redefinedSet);
                }
                std::unordered_set<AbstractSet*> allSubSets = this->getAllSubSets();

                node->set->adjustSuperSets(node, allSuperSetsAndMe);

                std::unordered_map<AbstractSet*, bool> visited;
                for (auto set : allSuperSetsAndMe) {
                    visited[set] = false;
                }

                innerAddDFS(node, node->set, allSuperSetsAndMe, allSubSets, visited);

                for (auto superSet : allSuperSetsAndMe) {
                    superSet->m_size++;
                }
            }

            /**
             * searches superSet structure by finding the roots first and checking if it was placed
             * @param node: the node being placed
             * @param set: the current set we are checking
            */
            void innerAddDFS(   
                                SetNode* node, 
                                AbstractSet* set, 
                                std::unordered_set<AbstractSet*>& allSuperSetsAndMe, 
                                std::unordered_set<AbstractSet*>& allSubSets,
                                std::unordered_map<AbstractSet*, bool>& visited
                            ) {

                if (visited[set]) {
                    return;
                }

                visited[set] = true;

                // place in supersets
                for (AbstractSet* superSet : set->m_superSets) {
                    innerAddDFS(node, superSet, allSuperSetsAndMe, allSubSets, visited);
                }

                // adjust redefines
                for (AbstractSet* redefinedSet : set->m_redefines) {
                    if (!redefinedSet->m_root && !redefinedSet->m_rootRedefinedSet) {
                        redefinedSet->m_root = node;
                    }
                }
                
                if (set->m_superSets.size() == 0 && set->m_rootRedefinedSet) {
                    // root set, place the node in the tree
                    // gather our set's supersets and subsets
                    SetNode* currNode = set->m_root;

                    // handle divider nodes
                    while (currNode) {
                        if (!currNode->m_ptr) {
                            if (allSuperSetsAndMe.count(currNode->set)) {
                                if (allSuperSetsAndMe.count(currNode->m_left->set) || allSubSets.count(currNode->m_left->set)) {
                                    currNode = currNode->m_left;
                                    continue;
                                } else if (allSuperSetsAndMe.count(currNode->m_right->set) || allSubSets.count(currNode->m_right->set)) {
                                    currNode = currNode->m_right;
                                    continue;
                                } else if (currNode->m_left->m_ptr && !currNode->m_right->m_ptr) {
                                    currNode = currNode->m_left;
                                } else if (currNode->m_right->m_ptr && !currNode->m_left->m_ptr) {
                                    currNode = currNode->m_right;
                                } else {
                                    // special case, we are going to have to change currNode's set variable because 
                                    // it is no longer valid as the scope for the divider node
                                    
                                    // find most similar set
                                    AbstractSet* dividerNodeSet = 0;
                                    std::unordered_set setsSubSets = set->getAllSubSets();
                                    setsSubSets.insert(set);
                                    std::list<AbstractSet*> queue;
                                    std::unordered_set leftsSuperSets = currNode->m_left->set->getAllSuperSets();
                                    queue.push_back(currNode->m_right->set);
                                    while (!queue.empty()) {
                                        AbstractSet* front = queue.front();
                                        queue.pop_front();
                                        if (leftsSuperSets.count(front) && front != currNode->set) {
                                            dividerNodeSet = front;
                                            break;
                                        }
                                        for (auto superSet : front->m_superSets) {
                                            if (setsSubSets.count(superSet)) {
                                                queue.push_back(superSet);
                                            }
                                        }
                                    }

                                    if (dividerNodeSet) {
                                        currNode->set = dividerNodeSet;
                                    }
                                }
                            }
                        } else if (allSuperSetsAndMe.count(currNode->set) || allSubSets.count(currNode->set)) {
                            // regular node, check if it is in related sets, else continue with loop and create divider node
                            break;
                        }

                        // create divider node
                        SetNode* dividerNode = new SetNode();

                        // handle parent from this tree
                        SetNode* parent = getParent(currNode, set->m_root);
                        if (parent) {
                            if (parent->m_left == currNode) {
                                parent->m_left = dividerNode;
                            } else {
                                parent->m_right = parent->m_left;
                                parent->m_left = dividerNode;
                            }
                            dividerNode->m_parent = parent;
                        }

                        // find most similar set
                        AbstractSet* dividerNodeSet = 0;
                        std::unordered_set setsSubSets = set->getAllSubSets();
                        setsSubSets.insert(set);
                        std::list<AbstractSet*> queue;
                        queue.push_back(currNode->set);
                        while (!queue.empty()) {
                            AbstractSet* front = queue.front();
                            queue.pop_front();
                            if (allSuperSetsAndMe.count(front)) {
                                dividerNodeSet = front;
                                break;
                            }
                            for (auto superSet : front->m_superSets) {
                                if (setsSubSets.count(superSet)) {
                                    queue.push_back(superSet);
                                }
                            }
                        }

                        if (dividerNodeSet == 0) {
                            throw SetStateException("Bad state could not find similar node between");
                        }

                        dividerNode->set = dividerNodeSet;

                        // add currNode and node below this divider node
                        currNode->m_parent = dividerNode;
                        node->m_parent = dividerNode;
                        if (currNode->m_ptr.id() > node->m_ptr.id()) {
                            dividerNode->m_left = currNode;
                            dividerNode->m_right = node;
                        } else {
                            dividerNode->m_right = currNode;
                            dividerNode->m_left = node;
                        }

                        // readjust roots if needed
                        queue.clear();
                        if (currNode->m_ptr) {
                            queue.push_back(currNode->set);
                        } else {
                            // divider node, we need to check its children's sets
                            std::list<SetNode*> queuingQueue;
                            queuingQueue.push_back(currNode);
                            while (!queuingQueue.empty()) {
                                SetNode* front = queuingQueue.front();
                                queuingQueue.pop_front();
                                if (!front->m_ptr) {
                                    queuingQueue.push_back(front->m_left);
                                    queuingQueue.push_back(front->m_right);
                                } else {
                                    queue.push_back(front->set);
                                }
                            }
                        }
                        
                        while (!queue.empty()) {
                            AbstractSet* front = queue.front();
                            queue.pop_front();
                            if (front->m_root == currNode && allSuperSetsAndMe.count(front) && setsSubSets.count(front)) {
                                front->m_root = dividerNode;
                            }
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }

                        return;
                    }
                    while (currNode) {
                        
                        if (
                                currNode->m_ptr.id() == node->m_ptr.id() || 
                                (currNode->m_left && currNode->m_left->m_ptr.id() == node->m_ptr.id())
                            ) 
                        {
                            currNode = currNode->m_ptr.id() == node->m_ptr.id() ? currNode : currNode->m_left;
                            if (std::find(this->m_redefines.begin(), this->m_redefines.end(), currNode->set) != this->m_redefines.end()) {
                                break;
                            } else if (allSuperSetsAndMe.count(currNode->set)) {
                                // node already exists in superset
                                // TODO move to allocation policy
                                if (currNode->set == this) {
                                    // TODO different behavior with Bag and List
                                    delete node;
                                    throw SetStateException("Node already in set TODO bags and list");
                                }

                                // remove it
                                currNode->set->innerRemove(currNode->m_ptr.id());
                                
                                // add again
                                visited[set] = false;
                                if (currNode->m_ptr.loaded()) {
                                    // TODO dont do opposite
                                    m_opposite->skip = true;
                                }
                                innerAddDFS(node, set, allSuperSetsAndMe, allSubSets, visited);
                                return;
                            } else {
                                // TODO may be valid in some edge cases
                                throw SetStateException("Node already in this set!");
                            }
                        }

                        if (allSubSets.count(currNode->set)) {
                            // this is a subsets node, we must place it below ours
                            // adjust roots
                            for (auto subSet : set->getAllSuperSets()) {
                                if (!allSuperSetsAndMe.count(subSet)) {
                                    continue;
                                }
                                if (subSet->m_root == currNode) {
                                    subSet->m_root = node;
                                }
                            }
                            if (set->m_root == currNode) {
                                set->m_root = node;
                            }

                            // mark all parents as being visited allready and check their roots
                            for (auto superSet : allSuperSetsAndMe) {
                                visited[superSet] = true;
                                if (superSet->m_root == currNode) {
                                    superSet->m_root = node;
                                }
                            }

                            if (currNode->m_parent == node) {
                                break;
                            }
                            node->m_parent = currNode->m_parent;
                            node->m_left = currNode;
                            currNode->m_parent = node;
                            if (node->m_parent) {
                                if (node->m_parent->m_left == currNode) {
                                    node->m_parent->m_left = node;
                                } else if (node->m_parent->m_right == currNode) {
                                    node->m_parent->m_right = node;
                                }
                            }
                            break;
                        }
                        
                        // no children, place to left
                        if (!currNode->m_left) {
                            currNode->m_left = node;
                            node->m_parent = currNode;
                            
                            // mark all parents as being visited allready
                            for (auto superSet : currNode->set->getAllSuperSets()) {
                                visited[superSet] = true;
                            }
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

                            // mark all parents as being visited allready
                            if (currNode->m_ptr) {
                                for (auto superSet : currNode->set->getAllSuperSets()) {
                                    visited[superSet] = true;
                                }
                            } else {
                                // handle divider node superset search
                                std::list<AbstractSet*> queue;
                                std::list<SetNode*> queueingQueue;
                                queueingQueue.push_back(currNode);
                                while (!queueingQueue.empty()) {
                                    SetNode* front = queueingQueue.front();
                                    queueingQueue.pop_front();
                                    if (!front->m_ptr) {
                                        if (front->m_left != node) {
                                            queueingQueue.push_back(front->m_left);
                                        }
                                        if (front->m_right != node) {
                                            queueingQueue.push_back(front->m_right);
                                        }
                                    } else {
                                        queue.push_back(front->set);
                                    }
                                }
                                while (!queue.empty()) {
                                    AbstractSet* front = queue.front();
                                    queue.pop_front();
                                    visited[front] = true;
                                    for (auto superSet : front->m_superSets) {
                                        queue.push_back(superSet);
                                    }
                                }
                            }
                            break;
                        }

                        // two children, pick side to recurse
                        if (node->m_ptr.id() > currNode->m_right->m_ptr.id()) {
                            currNode = currNode->m_left;
                        } else {
                            currNode = currNode->m_right;
                        }
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
                node->deleteNode();
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
                if (!this->m_rootRedefinedSet) {
                    for (auto superSet : this->m_superSets) {
                        superSet->m_subSets.erase(std::find(superSet->m_subSets.begin(), superSet->m_subSets.end(), this));
                    }
                    for (auto subSet : this->m_subSets) {
                        subSet->m_superSets.erase(std::find(subSet->m_superSets.begin(), subSet->m_superSets.end(), this));
                    }
                    return;
                }

                std::unordered_set<AbstractSet*> allSuperSets = this->getAllSuperSets();
                if (allSuperSets.empty()) {
                    allSuperSets.insert(this); // edge case, need someone to find parent from
                }

                std::unordered_set<AbstractSet*> allSubSets = this->getAllSubSets(); // should be empty or small

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

                        if (allSuperSets.count(nodeToDelete->set) && 
                            nodeToDelete->set != (this->m_setToInstantiate ? this->m_setToInstantiate : this)) {
                            // this is owned by a superset which will be deleted after us (probably, not necessarily guaranteed)
                            break;
                        }

                        // fix all parents
                        for (auto superSet : allSuperSets) {
                            if (superSet->m_superSets.size() != 0 || !superSet->m_rootRedefinedSet || !superSet->m_root) {
                                continue;
                            }
                            SetNode* parent = getParent(nodeToDelete, superSet->m_root);
                            if (parent) {
                                if (parent->m_left == currNode) {
                                    parent->m_left = 0;
                                } else if (parent->m_right == currNode) {
                                    parent->m_right = 0;
                                }
                            }   
                        }
                        
                        // reset relevant roots
                        if (currNode == this->m_root) {
                            this->m_root = 0;
                            for (auto superSet : allSuperSets) {
                                if (superSet->m_root == currNode) {
                                    superSet->m_root = 0;
                                }
                            }
                        }
                        // subset can always have root be any node in this set
                        for (auto subSet : allSubSets) {
                            if (subSet->m_root == currNode) {
                                subSet->m_root = 0;
                            }
                        }

                        currNode = currNode->m_parent;
                        delete nodeToDelete;

                        while (currNode && !currNode->m_ptr && !currNode->m_left && !currNode->m_right) {
                            // parent is a divider node with no children, lets get rid of it
                            nodeToDelete = currNode;

                            //adjust roots
                            for (auto superSet : allSuperSets) {
                                if (superSet->m_root == currNode) {
                                    superSet->m_root = 0;
                                }
                            }

                            // fix all parents
                            for (auto superSet : allSuperSets) {
                                if (superSet->m_superSets.size() != 0 || !superSet->m_rootRedefinedSet || !superSet->m_root) {
                                    continue;
                                }
                                SetNode* parent = getParent(nodeToDelete, superSet->m_root);
                                if (parent) {
                                    if (parent->m_left == currNode) {
                                        parent->m_left = 0;
                                    } else if (parent->m_right == currNode) {
                                        parent->m_right = 0;
                                    }
                                }   
                            }

                            currNode = currNode->m_parent;
                            delete nodeToDelete;
                        }

                        if (currNode && !currNode->m_ptr) {
                            // parent is a divider node, we are done
                            break;
                        }

                        if (currNode && allSuperSets.count(currNode->set) && this->m_superSets.size() != 0) {
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
                this->m_redefines.push_back(&redefined);
                for (auto redefinedSet : redefined.m_redefines) {
                    this->m_redefines.push_back(redefinedSet);
                    redefinedSet->m_rootRedefinedSet = false;
                }
                redefined.m_redefines.push_back(this);
                redefined.m_rootRedefinedSet = false;
                if (!this->m_setToInstantiate && redefined.setType() == SetType::ORDERED_SET && this->setType() != SetType::ORDERED_SET) {
                        this->m_setToInstantiate = &redefined;
                    }
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
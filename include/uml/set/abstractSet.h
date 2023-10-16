#pragma once

#include "uml/types/namedElement.h"
#include "setNode.h"
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
        friend struct OrderedSetNode;
        friend class SetNode;

        protected:
            SetNode* m_root = 0;

            std::vector<AbstractSet*> m_superSets;
            std::vector<AbstractSet*> m_subSets;
            std::unordered_set<AbstractSet*> m_redefines;

            AbstractSet* m_setToInstantiate = 0;

            bool m_rootRedefinedSet = true;

            // m_guard lets us know the set's scope quickly, a subset must have a higher guard than its superset. root sets always have guards of 0
            size_t m_guard = 0;

            size_t m_size = 0;

            // root
            virtual void setRoot(SetNode* node) = 0;
            virtual SetNode* getRoot() const = 0;

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
}

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
    template <class T>
    class OrderedID_Set;
    template <class T>
    class OrderedPtrSet;
    template <
                class T, 
                class U,
                class AdditionPolicy,
                class RemovalPolicy
            >
    class CustomOrderedSet;

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
        template <class T>
        friend class OrderedID_Set;
        template <class T>
        friend class OrderedPtrSet;
        template <
                class T, 
                class U,
                class AdditionPolicy,
                class RemovalPolicy
            >
        friend class CustomOrderedSet;

        protected:
            SetNode* m_root = 0;

            std::unordered_set<AbstractSet*> m_superSets;
            std::unordered_set<AbstractSet*> m_subSets;
            std::unordered_set<AbstractSet*> m_redefines;

            AbstractSet* m_setToInstantiate = 0;

            bool m_rootRedefinedSet = true;

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
            virtual void adjustSuperSets(SetNode* node) = 0;
            virtual SetNode* createNode(Element& el) = 0;
            virtual SetNode* createNode(ID id) = 0;
            virtual void innerRemove(ID id) = 0;
            virtual void remove(ID id) = 0;

        public:
            AbstractSet& operator=(AbstractSet&&) = delete;

            /**
             * this set subsets the set supplied, meaning all elements within this set will be contained within the set supplied
             * but this set will not necessarily have all of the elements within the set supplied
             * @param subsetOf the set that we are a subset of
             **/
            void subsets(AbstractSet* subsetOf) {
                if (!subsetOf->m_rootRedefinedSet) {
                    for (auto redefinedSet : subsetOf->m_redefines) {
                        if (redefinedSet->m_rootRedefinedSet) {
                            subsetOf = redefinedSet;
                            break;
                        }
                    }
                }
                if (!this->m_superSets.count(subsetOf)) {
                    this->m_superSets.insert(subsetOf);
                    subsetOf->m_subSets.insert(this);
                    // Handle ordered set, TODO other set types
                    if (!m_setToInstantiate && subsetOf->setType() == SetType::ORDERED_SET) {
                        if (subsetOf->m_setToInstantiate) {
                            // we want highest up the tree orderedSet
                            m_setToInstantiate = subsetOf->m_setToInstantiate;
                        } else {
                            m_setToInstantiate = subsetOf;
                        }
                    } else if (subsetOf->m_setToInstantiate) {
                        m_setToInstantiate = subsetOf->m_setToInstantiate;
                    }
                }
            }

            void subsets(AbstractSet& subsetOf) {
                subsets(&subsetOf);
            }
    };
}

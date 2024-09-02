#pragma once

#include "uml/managers/baseElement.h"
#include <memory>
#include <unordered_set>
#include <uml/umlPtr.h>

namespace UML {
    enum class SetType {
        SET,
        SINGLETON,
        ORDERED_SET,
        LIST,
        BAG
    };
    std::ostream& operator<<(std::ostream& stream, const SetType& setType);

    class SetStructure;
    template <class S, class WrapperPolicy>
    class WrapperSet;

    class AbstractSet {

        template <class Tlist, class P1, class P2>
        friend class Manager;

        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;
        template <class T>
        friend class SetDataPolicy;
        template <class T>
        friend class SingletonDataPolicy;
        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        template <class T, class U, class ApiPolicy>
        friend class OrderedSet;
        friend class Element;
        template <class S, class WrapperPolicy>
        friend class WrapperSet;
        // TODO remove or find better way than a lot of friends
        friend class Connector;
//        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);
        protected:
            std::shared_ptr<SetStructure> m_structure;
            virtual bool hasData() const = 0;
            virtual bool containsData(AbstractElementPtr ptr) const = 0;
            virtual bool removeData(AbstractElementPtr ptr) = 0;
            virtual void runAddPolicy(AbstractElement& el) = 0;
            virtual void runRemovePolicy(AbstractElement& el) = 0;
            virtual bool oppositeEnabled() const = 0;
            virtual void oppositeAdd(AbstractElement& el) = 0;
            virtual void oppositeRemove(AbstractElement& el) = 0;
            virtual void nonOppositeAdd(AbstractElementPtr ptr) = 0;
            virtual void innerAdd(AbstractElementPtr ptr) = 0;
            virtual void nonOppositeRemove(AbstractElementPtr ptr) = 0;
            virtual void innerRemove(AbstractElementPtr ptr) = 0;
            virtual void weakRemove(AbstractElementPtr ptr) = 0;
            virtual void allocatePtr(__attribute__((unused)) AbstractElementPtr ptr, __attribute__((unused)) SetStructure& set) {}
            virtual void deAllocatePtr(__attribute__((unused)) AbstractElementPtr ptr) {}

            class iterator {
                template <class Tlist, class P1, class P2>
                friend class Manager;
                template <class T>
                friend class SetDataPolicy;
                template <class T>
                friend class SingletonDataPolicy;
                template <class S, class WrapperPolicy>
                friend class WrapperSet;
                friend class IndexablePolicy;

                protected:
                    virtual AbstractElementPtr getCurr() const = 0;
                    virtual void next() = 0;
                    virtual std::unique_ptr<iterator> clone() const = 0;
                public:
                    size_t m_hash = 0;
                    iterator() {}
                    iterator(const iterator& rhs) {
                        m_hash = rhs.m_hash;
                    }
                    virtual ~iterator() {}
                    bool operator==(const iterator& rhs) const {
                        return rhs.m_hash == m_hash;
                    }
                    bool operator!=(const iterator& rhs) const {
                        return rhs.m_hash != m_hash;
                    }
            };
            virtual std::unique_ptr<iterator> beginPtr() const = 0;
            virtual std::unique_ptr<iterator> endPtr() const = 0;
        public:
            AbstractSet();
            virtual ~AbstractSet();
            virtual void subsets(AbstractSet& superSet);
            virtual void redefines(AbstractSet& redefinedSet);
            virtual bool contains(AbstractElementPtr ptr) const = 0;
            size_t size() const;
            bool empty() const;
            virtual SetType setType() const = 0;

    };

    class SetStructure {
        public:
            AbstractSet& m_set;
            std::unordered_set<std::shared_ptr<SetStructure>> m_subSetsWithData;
            std::unordered_set<std::shared_ptr<SetStructure>> m_superSets;
            std::unordered_set<std::shared_ptr<SetStructure>> m_subSets;
            std::unordered_set<std::shared_ptr<SetStructure>> m_redefinedSets;
            std::shared_ptr<SetStructure> m_rootRedefinedSet;
            size_t m_size = 0;
            SetStructure(AbstractSet& set) : m_set(set) {}
    };
    
}

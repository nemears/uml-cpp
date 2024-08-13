#pragma once

#include <exception>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../umlPtr.h"
#include "privateSet.h"
#include "uml/set/abstractSet.h"

namespace UML {

    template <class T>
    class SetDataPolicy : virtual public AbstractSet {
        protected:        
            std::unordered_map<ID, UmlPtr<T>> m_data;
            class iterator : public AbstractSet::iterator {
                friend class SetDataPolicy;
                private:
                    std::weak_ptr<SetStructure> m_me;
                    typename std::unordered_map<ID, UmlPtr<T>>::const_iterator m_dataIt;
                    bool m_iterateSubSets = false;
                    std::unordered_set<std::shared_ptr<SetStructure>>::const_iterator m_subSetsWithDataIt;
                    std::unique_ptr<AbstractSet::iterator> m_currSetIt;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return std::make_unique<iterator>(*this);
                    }
                    ElementPtr getCurr() const override {
                        if (m_iterateSubSets) {
                            return m_currSetIt->getCurr();
                        }
                        auto me = m_me.lock();
                        if (m_dataIt != dynamic_cast<SetDataPolicy&>(me->m_set).m_data.end()) {
                            return m_dataIt->second;
                        }
                        return UmlPtr<T>();
                    }
                    void next() override {
                        auto me = m_me.lock();
                        if (!m_iterateSubSets) {
                            m_dataIt++;
                            auto dataEnd = dynamic_cast<SetDataPolicy&>(me->m_set).m_data.end();
                            if (m_dataIt == dataEnd) {
                                if (m_subSetsWithDataIt != me->m_subSetsWithData.end()) {
                                    m_currSetIt = (*m_subSetsWithDataIt)->m_set.beginPtr();
                                    m_iterateSubSets = true;
                                }
                            }
                            
                        } else if (m_iterateSubSets && m_subSetsWithDataIt != me->m_subSetsWithData.end()) {
                            std::shared_ptr<SetStructure> currentSet = *m_subSetsWithDataIt;
                            m_currSetIt->next();
                            if (*m_currSetIt == *currentSet->m_set.endPtr()) {
                                m_subSetsWithDataIt++;
                                if (m_subSetsWithDataIt != me->m_subSetsWithData.end()) {
                                    m_currSetIt = (*m_subSetsWithDataIt)->m_set.beginPtr();
                                }
                            }
                        } else {
                            throw SetStateException("bad state in iterator");
                        }
                        auto curr = getCurr();
                        if (curr) {
                            std::hash<ID> hasher;
                            m_hash = hasher(curr.id());
                        } else {
                            m_hash = 0;
                        }
                    }
                public:
                    iterator() {};
                    iterator(const iterator& rhs) : AbstractSet::iterator(rhs) {
                        m_me = rhs.m_me.lock();
                        m_dataIt = rhs.m_dataIt;
                        m_subSetsWithDataIt = rhs.m_subSetsWithDataIt;
                        m_iterateSubSets = rhs.m_iterateSubSets;
                        if (m_iterateSubSets) {
                            m_currSetIt = rhs.m_currSetIt->clone();
                        }
                        m_hash = rhs.m_hash;
                    }
                    T& operator*() {
                        return getCurr()->template as<T>();
                    }
                    UmlPtr<T> operator->() {
                        return getCurr(); 
                    }
                    iterator operator++() {
                        next();
                        return *this;                        
                    }
                    iterator operator++(int) {
                        return ++(*this);
                    }
            };
     protected:
            void allocatePtr(ElementPtr ptr, SetStructure& set) override {
                if (m_data.count(ptr.id())) {
                    if (m_structure.get() == &set) {
                        throw SetStateException("duplicate element added to set!");
                    }
                    nonOppositeRemove(ptr);
                } 
            }
            bool hasData() const {
                return !m_data.empty();
            }
            bool containsData(UmlPtr<T> ptr) const {
                return m_data.count(ptr.id()) > 0;
            }
            void addData(UmlPtr<T> ptr) {
                if (!ptr) {
                    throw SetStateException("cannot add null ptr to set!");
                }
                if (m_data.count(ptr.id())) {
                    throw SetStateException("duplicate element added to set!");
                }
                for (auto subSetWithData : m_structure->m_subSetsWithData) {
                    if (subSetWithData->m_set.contains(ptr)) {
                        throw SetStateException("duplicate element added to set!");
                    }
                }
                m_data[ptr.id()] = ptr;
            }
            bool removeData(UmlPtr<T> ptr) {
                return m_data.erase(ptr.id()) == 1;
            }
            std::unique_ptr<AbstractSet::iterator> beginPtr() const override {
                return std::unique_ptr<iterator>(new iterator(begin()));
            }
            std::unique_ptr<AbstractSet::iterator> endPtr() const override {
                return std::make_unique<iterator>();
            }
        public:
            virtual ~SetDataPolicy() {}
            SetType setType() const override {
                return SetType::SET;
            }
            UmlPtr<T> front() const {
                if (m_data.size() > 0) {
                    return m_data.begin()->second;
                }
                if (!m_structure->m_subSetsWithData.empty()) {
                    return UmlPtr<T>((*m_structure->m_subSetsWithData.begin())->m_set.beginPtr()->getCurr());
                }
                return UmlPtr<T>();
            }
            UmlPtr<T> get(ID id) const {
                try {
                    return m_data.at(id);
                } catch(std::exception& exception) {
                    for (auto subSetWithData : m_structure->m_subSetsWithData) {
                        auto subSetDataIt = subSetWithData->m_set.beginPtr();
                        while (
                                *subSetDataIt != *subSetWithData->m_set.endPtr() &&
                                subSetDataIt->getCurr().id() != id
                        ) {
                            subSetDataIt->next();
                        }
                        if (*subSetDataIt != *subSetWithData->m_set.endPtr()) {
                            return UmlPtr<T>(subSetDataIt->getCurr());
                        }
                    }
                }
                return UmlPtr<T>();
            }
            iterator begin() const {
                iterator ret;
                ret.m_me = m_structure;
                ret.m_dataIt = m_data.begin();
                std::hash<ID> hasher;
                ret.m_subSetsWithDataIt = m_structure->m_subSetsWithData.begin();
                if (ret.m_dataIt != m_data.end()) {
                    ret.m_hash = hasher(ret.m_dataIt->second.id());
                } else if (ret.m_subSetsWithDataIt != m_structure->m_subSetsWithData.end()) {
                    ret.m_currSetIt = (*ret.m_subSetsWithDataIt)->m_set.beginPtr(); 
                    ret.m_hash = hasher(ret.m_currSetIt->getCurr().id());
                    ret.m_iterateSubSets = true;
                }
                return ret;
            }
            iterator end() const {
                return iterator();
            }
    };

//     declaration at uml/types/element.h
//     template <class T, class U, class ApiPolicy>
//     using ReadOnlySet = PrivateSet<T, U, SetDataPolicy<T>, ApiPolicy>;

    template <class T, class U, class ApiPolicy>
    class Set : public ReadOnlySet<T, U, ApiPolicy> {
        public:
            Set (U* me) : ReadOnlySet<T, U, ApiPolicy>(me) {}
            void add(UmlPtr<T> ptr) {
                this->innerAdd(ptr);
            }
            void add(ID& id) {
                this->nonOppositeAdd(this->m_el.m_manager->createPtr(id));
            }
            void add(T& el) {
                this->innerAdd(UmlPtr<T>(&el));
            }
            template <class ... Ts>
            void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            }
            void remove(UmlPtr<T> ptr) {
                this->innerRemove(ptr);
            }
            void remove(ID& id) {
                this->innerRemove(UmlPtr<T>(id));
            }
            void remove(T& el) {
                this->innerRemove(UmlPtr<T>(&el));
            }
            void clear() {
                while (this->front()) {
                    remove(this->front());
                }
            }
    };
}

#pragma once

#include <exception>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../umlPtr.h"
#include "privateSet.h"
#include "uml/set/abstractSet.h"
#include "uml/set/doNothingPolicy.h"

namespace UML {

    template <class T>
    class SetDataPolicy : virtual public AbstractSet {
        private:        
            std::unordered_map<ID, UmlPtr<T>> m_data;
            class iterator : public AbstractSet::iterator {
                friend class SetDataPolicy;
                private:
                    std::weak_ptr<SetDataPolicy<T>> m_me;
                    typename std::unordered_map<ID, UmlPtr<T>>::const_iterator m_dataIt;
                    std::unordered_set<std::shared_ptr<AbstractSet>>::const_iterator m_subSetsWithDataIt;
                    std::unique_ptr<AbstractSet::iterator> m_currSetIt;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return std::make_unique<iterator>(*this);
                    }
                    ElementPtr getCurr() const override {
                        auto me = m_me.lock();
                        if (m_dataIt != me->m_data.end()) {
                            return m_dataIt->second;
                        } else {
                            return m_currSetIt->getCurr();
                        }
                    }
                    void next() override {
                        auto me = m_me.lock();
                        if (m_dataIt != me->m_data.end()) {
                            m_dataIt++;
                        } else {
                            std::shared_ptr<AbstractSet> currentSet = *m_subSetsWithDataIt;
                            if (!m_currSetIt) {
                                m_currSetIt = currentSet->beginPtr()->clone();
                            } else if (*m_currSetIt != *currentSet->endPtr()->clone()) {
                                m_currSetIt->next();
                            } else {
                                throw SetStateException("iterating past end!");
                            }
                        }
                    }
                public:
                    iterator() {};
                    iterator(const iterator& rhs) : AbstractSet::iterator(rhs) {
                        m_me = rhs.m_me.lock();
                        m_dataIt = rhs.m_dataIt;
                        m_subSetsWithDataIt = rhs.m_subSetsWithDataIt;
                        m_currSetIt = rhs.m_currSetIt->clone();
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
                    bool operator==(const AbstractSet::iterator& right) const {
                        return &right == this;
                    }
                    bool operator!=(const AbstractSet::iterator& right) const {
                        return &right != this;
                    }
            };
     protected:
            bool hasData() const {
                return !m_data.empty();
            }
            bool containsData(UmlPtr<T> ptr) const {
                return m_data.count(ptr.id()) > 0;
            }
            void addData(UmlPtr<T> ptr) {
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
                return UmlPtr<T>();
            }
            UmlPtr<T> get(ID id) {
                try {
                    return m_data.at(id);
                } catch(std::exception& exception) {
                    for (auto subSetWithData : m_subSetsWithData) {
                        if (subSetWithData->setType() == SetType::SET) {
                            try {
                                return std::dynamic_pointer_cast<SetDataPolicy>(subSetWithData)->m_data.at(id);
                            } catch (std::exception&) {}
                        }
                    }
                }
                return UmlPtr<T>();
            }
            iterator begin() const {
                iterator ret;
                ret.m_me = std::dynamic_pointer_cast<SetDataPolicy>(m_rootRedefinedSet);
                ret.m_dataIt = m_data.begin();
                ret.m_subSetsWithDataIt = m_subSetsWithData.begin();
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
                this->innerAdd(this->m_el.m_manager->createPtr(id));
            }
            void add(T& el) {
                this->innerAdd(UmlPtr<T>(&el));
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

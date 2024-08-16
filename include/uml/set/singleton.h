#pragma once

#include "uml/set/abstractSet.h"
#include "uml/set/privateSet.h"
#include "uml/umlPtr.h"
#include <memory>

namespace UML {
    template <class T>
    class SingletonDataPolicy : virtual public AbstractSet {
        protected:
            UmlPtr<T> m_data;
            class iterator : public AbstractSet::iterator {
                friend class SingletonDataPolicy;
                private:
                    std::weak_ptr<SetStructure> m_me;
                    bool m_done = true;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return std::make_unique<iterator>(*this);
                    }
                    ElementPtr getCurr() const override {
                        if (m_done) {
                            return UmlPtr<T>();
                        }
                        return dynamic_cast<SingletonDataPolicy&>(m_me.lock()->m_set).m_data;
                    }
                    void next() override {
                        m_done = true;
                        m_hash = 0;
                    }
                public:
                    iterator() {};
                    iterator(const iterator& rhs) : AbstractSet::iterator(rhs) {
                        m_me = rhs.m_me.lock();
                        m_done = rhs.m_done;
                    }
                    T& operator*() {
                        return *getCurr();
                    }
                    UmlPtr<T> operator->() {
                        return getCurr();
                    }
                    iterator operator++() {
                        next();
                        return *this;
                    }
                    iterator operator++(int) {
                        return (*this)++;
                    }
            };
        protected:
            void allocatePtr(ElementPtr ptr, __attribute__((unused)) SetStructure& set) override {
                if (m_data && m_data.id() != ptr.id()) {
                    innerRemove(m_data);
                }
            }
            bool hasData() const {
                return m_data.has();
            }
            bool containsData(UmlPtr<T> ptr) const {
                return ptr.id() == m_data.id(); // TODO rn we are allowing null
            }
            void addData(UmlPtr<T> ptr) {
                
                m_data = ptr;
            }
            bool removeData(UmlPtr<T> ptr) {
                if (m_data.id() != ptr.id()) {
                    throw SetStateException("Tried to remove element that was not in singleton");
                }
                m_data = UmlPtr<T>();
                return true;
            }
            std::unique_ptr<AbstractSet::iterator> beginPtr() const override {
                return std::unique_ptr<AbstractSet::iterator>(new iterator(begin()));
            }
            std::unique_ptr<AbstractSet::iterator> endPtr() const override {
                return std::unique_ptr<AbstractSet::iterator>(new iterator(end()));
            }
        public:
            UmlPtr<T> get() const {
                if (m_structure->m_rootRedefinedSet.get() != m_structure.get()) {
                    return m_structure->m_rootRedefinedSet->m_set.beginPtr()->getCurr();
                }
                if (m_data) {
                    return m_data;
                }
                for (auto subSetWithData : m_structure->m_subSetsWithData) {
                    auto end = subSetWithData->m_set.endPtr();
                    auto begin = subSetWithData->m_set.beginPtr();
                    if (*begin != *end) {
                        return UmlPtr<T>(subSetWithData->m_set.beginPtr()->getCurr());
                    }
                }
                return UmlPtr<T>();
            }
            iterator begin() const {
                auto val = get();
                if (val) {
                    iterator ret;
                    std::hash<ID> hasher;
                    ret.m_hash = hasher(val.id());
                    ret.m_me = m_structure;
                    ret.m_done = false;
                    return ret;
                }
                return end();
            }
            iterator end() const {
                iterator ret;
                ret.m_me = m_structure;
                return iterator();
            }
            SetType setType() const override {
                return SetType::SINGLETON;
            }
    };
//     declaration in uml/types/element.h    
//     template <class T, class U, class ApiPolicy>
//     using ReadOnlySingleton = PrivateSet<T, U, SingletonDataPolicy<T>, ApiPolicy>;

    template <class T, class U, class ApiPolicy>
    class Singleton : public ReadOnlySingleton<T,U,ApiPolicy> {
        private:
            void checkCurrentValueHelper() {
                AbstractSet& redefinedSet = this->m_structure->m_rootRedefinedSet->m_set;
                ElementPtr currVal = this->get();
                if (currVal) {
                    redefinedSet.innerRemove(currVal);
                }                
            }
            void setHelper(UmlPtr<T> ptr) {
                checkCurrentValueHelper();
                if (ptr) {
                    this->innerAdd(ptr);
                }
            }
        public:
            Singleton(U* me) : ReadOnlySingleton<T, U, ApiPolicy>(me) {}
            void set(UmlPtr<T> ptr) {
                this->setHelper(ptr);
            }
            void set(T& ref) {
                this->setHelper(UmlPtr<T>(&ref));
            }
            void set(ID id) {
                if (id == ID::nullID()) {
                    set(0);
                    return;
                }
                UmlPtr<T> ptr = this->m_el.m_manager->createPtr(id); 
                if (ptr) {
                    this->m_structure->m_rootRedefinedSet->m_set.nonOppositeAdd(ptr);
                }
            }
    };
}

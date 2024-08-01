#pragma once

#include "uml/set/doNothingPolicy.h"
#include "uml/set/privateSet.h"
#include "uml/umlPtr.h"
#include <memory>

namespace UML {
    template <class T>
    class SingletonDataPolicy : virtual public AbstractSet {
        private:
            UmlPtr<T> m_data;
            class iterator : public AbstractSet::iterator {
                friend class SingletonDataPolicy;
                private:
                    std::weak_ptr<SingletonDataPolicy<T>> m_me;
                    bool m_done = true;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return std::make_unique<iterator>(*this);
                    }
                    ElementPtr getCurr() const override {
                        if (m_done) {
                            return UmlPtr<T>();
                        }
                        return m_me.lock()->m_data;
                    }
                    void next() override {
                        m_done = true;
                        m_hash = 0;
                    }
                public:
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
            bool hasData() const {
                return m_data.has();
            }
            bool containsData(UmlPtr<T> ptr) const {
                return ptr.id() == m_data.id(); // TODO rn we are allowing null
            }
            void addData(UmlPtr<T> ptr) {
                if (m_data && m_data.id() != ptr.id()) {
                    // we need to remove the data before adding it
                    innerRemove(m_data);
                }
                m_data = ptr;
            }
            bool removeData(UmlPtr<T> ptr) {
                if (m_data.id() != ptr.id()) {
                    throw SetStateException("Tried to remove element that was not in singleton");
                }
                m_data = UmlPtr<T>();
                return true;
            }
        public:
            std::unique_ptr<AbstractSet::iterator> beginPtr() const override {
                auto val = get();
                if (val) {
                    std::unique_ptr<iterator> ret = std::unique_ptr<iterator>();
                    std::hash<ID> hasher;
                    ret->m_hash = hasher(val.id());
                    ret->m_me = std::dynamic_pointer_cast<SingletonDataPolicy>(m_rootRedefinedSet);
                    ret->m_done = false;
                    return ret;
                }
                return endPtr();
            }
            std::unique_ptr<AbstractSet::iterator> endPtr() const override {
                return std::unique_ptr<iterator>();
            }
            UmlPtr<T> get() const {
                if (m_data) {
                    return m_data;
                }
                for (auto subSetWithData : m_subSetsWithData) {
                    if (*subSetWithData->beginPtr() != *subSetWithData->endPtr()) {
                        return UmlPtr<T>(subSetWithData->beginPtr()->getCurr());
                    }
                }
                return UmlPtr<T>();
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
        public:
            Singleton(U* me) : ReadOnlySingleton<T, U, ApiPolicy>(me) {}
            void set(UmlPtr<T> ptr) {
                this->addData(ptr);
            }
            void set(T& ref) {
                this->addData(UmlPtr<T>(&ref));
            }
            void set(ID id) {
                this->addData(this->m_el.m_manager->createPtr(id));
            }
    };
}

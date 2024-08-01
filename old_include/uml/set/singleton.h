#pragma once

#include "uml/set/abstractSet.h"
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
                private:
                    std::weak_ptr<SingletonDataPolicy<T>> m_me;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return new iterator(*this);
                    }
                    UmlPtr<T> getCurr() const override {
                        if (!m_me) {
                            return UmlPtr<T>();
                        }
                        return m_me->m_data;
                    }
                    void next() override {
                        m_me = 0;
                    }
                public:
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
            bool hasData() {
                return m_data.has();
            }
            bool containsData(UmlPtr<T> ptr) {
                return ptr.id() == m_data.id(); // TODO rn we are allowing null
            }
            void addData(UmlPtr<T> ptr) {
                if (m_data && m_data.id() != ptr.id()) {
                    // we need to remove the data before adding it
                    innerRemove(m_data);
                }
                m_data = ptr;
            }
            void removeData(UmlPtr<T> ptr) {
                if (m_data.id() != ptr.id()) {
                    throw SetStateException("Tried to remove element that was not in singleton");
                }
                m_data = UmlPtr<T>();
            }
            UmlPtr<T> get() {
                return m_data();
            }
        public:
            std::unique_ptr<AbstractSet::iterator> begin() const override {
                std::unique_ptr<iterator> ret = std::unique_ptr<iterator>(new iterator);
                ret->m_me = m_rootRedefinedSet;
                return ret;
            }
            iterator end() const override {
                return std::unique_ptr<iterator>(new iterator);
            }
            SetType setType() const override {
                return SetType::SINGLETON;
            }
    };
    
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySingleton = PrivateSet<T, U, SingletonDataPolicy<T>, ApiPolicy>;

    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class Singleton : public ReadOnlySingleton<T,U,ApiPolicy> {
        public:
            void set(UmlPtr<T> ptr) {
                addPtr(ptr);
            }
            UmlPtr<T> get() {
                return ReadOnlySingleton<T,U,ApiPolicy>::get();
            }
    };
}

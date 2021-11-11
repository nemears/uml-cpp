#ifndef UML_SINGLETON_2
#define UML_SINGLETON_2

#include "set.h"

namespace UML {
    template <class T = Element, class U = Element> class Singleton2 : public Set<T, U> {
        public:
            Singleton2(U* el) : Set<T, U>(el) {};
            Singleton2() {};
            Singleton2<T,U>(const Singleton2<T,U>& rhs) : Set<T, U>(rhs) {};
            T* get() {
                if (this->m_root) {
                    return dynamic_cast<T*>(this->m_root->m_el);
                } else {
                    return 0;
                }
            };
            T& getRef() {
                if (this->m_root) {
                    return *dynamic_cast<T*>(this->m_root->m_el);
                } else {
                    // TODO throw exception
                    throw ManagerStateException();
                }
            };
            ID id() const {
                if (this->m_root) {
                    return this->m_root->m_id;
                } else {
                    // TODO throw proper exception
                    throw ManagerStateException("id()");
                }
            };
            bool has() const {
                return this->m_root != 0;
            };
            void set(T* el) {
                if (this->m_root) {
                    this->remove(this->m_root->m_id);
                }
                if (el) {
                    this->add(*el);
                }
            };
            void set(T& el) {
                set(&el);
            };
            void set(ID id) {
                if (this->m_root) {
                    this->remove(this->m_root->m_id);
                }
                this->add(id);
            };
    };
}

#endif
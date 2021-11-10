#ifndef UML_SINGLETON_2
#define UML_SINGLETON_2

#include "set.h"

namespace UML {
    template <class T = Element, class U = Element> class Singleton2 : public Set<T, U> {
        public:
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
    };
}

#endif
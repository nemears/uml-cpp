#ifndef _UML_SINGLETON_H_
#define _UML_SINGLETON_H_

#include "set.h"
#include "managers/managerNode.h"

namespace UML {

    class NullReferenceException : public std::exception {
        const char* what() const throw() override {
            return "Tried to get a reference of a null Singleton! You can check if the singleton is set with the has\"Signature\"() method.\n";
        };
    };

    template <class T = Element, class U = Element> class Singleton : public Set<T, U> {
        
        friend NamedElement;

        public:
            Singleton(U* el) : Set<T, U>(el) {
                this->m_upper = 1;
            };
            Singleton() {
                this->m_upper = 1;
            };
            Singleton<T,U>(const Singleton<T,U>& rhs) : Set<T, U>(rhs) {
                this->m_upper = 1;
            };
            UmlPtr<T> get() const {
                UmlPtr<T> ret(0);
                if (this->m_root) {
                    ret.m_id = this->m_root->m_id;
                    ret.m_ptr = this->m_root ? dynamic_cast<T*>(this->m_root->m_el) : 0;
                    if (ret.m_ptr) {
                        ret.m_manager = ret.m_ptr->m_manager;
                        ret.m_node = ret.m_ptr->m_node;
                        ret.m_manager->assignPtr(ret);
                    }
                    if (this->m_el) {
                        ret.m_manager = this->m_el->m_manager;
                    }
                }
                return ret;
            };
            void set(T* el) {
                if (el) {
                    if (this->m_root && this->m_root->m_id != el->getID()) {
                        this->remove(this->m_root->m_id);
                    }
                } else {
                    if (this->m_root) {
                        this->remove(this->m_root->m_id);
                    }
                }
                if (el && !this->m_root) {
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
                this->Set<T,U>::add(id);
            };
            void set(UmlPtr<T> ptr) {
                if (ptr) {
                    if (ptr.loaded()) {
                        set(*ptr);
                    } else {
                        set(ptr.id());
                    }
                }
            };
    };
}

#endif
#ifndef UML_SINGLETON_2
#define UML_SINGLETON_2

#include "set.h"

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
            T* get() {
                if (this->m_root) {
                    if (this->m_root->m_el == 0) { // TODO make this faster
                        this->m_root->m_el = &this->m_el->m_manager->template get<T>(this->m_root->m_id);
                    }
                    return dynamic_cast<T*>(this->m_root->m_el);
                } else {
                    return 0;
                }
            };
            T& getRef() {
                if (this->m_root) {
                    if (this->m_root->m_el == 0) {
                        this->m_root->m_el = &this->m_el->m_manager->get(this->m_root->m_id);
                    }
                    return *dynamic_cast<T*>(this->m_root->m_el);
                } else {
                    // TODO throw exception
                    throw UML::NullReferenceException();
                }
            };
            ID id() const {
                if (this->m_root) {
                    return this->m_root->m_id;
                } else {
                    return ID::nullID();
                }
            };
            bool has() const {
                return this->m_root != 0;
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
    };
}

#endif
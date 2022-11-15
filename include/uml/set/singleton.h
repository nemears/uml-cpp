#pragma once

#include "set.h"

namespace UML {
    template <class T, class U>
    class Singleton2 : public Set2<T, U> {
        public:
            Singleton2(U& el) : Set2<T, U>(el) {}
            Singleton2(U* el) : Set2<T, U>(el) {}
            UmlPtr<T> get() {
                SetLock myLck = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root) {
                    return this->m_root->m_ptr;
                }
                return UmlPtr<T>();
            }
            void set(T* el) {
                SetLock myLck = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root && (!el || el->getID() != this->m_root->m_ptr.id())) {
                    SetLock elLock = this->m_el.m_manager->lockEl(*this->m_root->m_ptr);
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot remove from read only set!");
                    }

                    (*this->m_root->m_ptr).m_node->removeReference(this->m_el);
                    this->m_el.m_node->removeReference(*this->m_root->m_ptr);

                    // remove
                    this->innerRemove(this->m_root->m_ptr.id());
                    // handle opposites
                    if (this->m_oppositeSignature) {
                        (el->*this->m_oppositeSignature)().innerRemove(this->m_el.getID());
                    }
                }
                if (el) {
                    SetLock elLock = this->m_el.m_manager->lockEl(*el);
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot add to read only set!");
                    }
                    // add
                    Set2<T,U>::innerAdd(*el);
                    el->m_node->setReference(this->m_el);
                    this->m_el.m_node->setReference(*el);
                    // handle opposites
                    if (this->m_oppositeSignature) {
                        (el->*this->m_oppositeSignature)().innerAdd(this->m_el);   
                    }
                }
            }

            void set(T& el) {
                set(&el);
            }

            void set(UmlPtr<T> el) {
                if (el) {
                    set(*el);
                } else {
                    set(0);
                }
            }
    };
}
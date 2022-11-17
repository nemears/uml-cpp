#pragma once

#include "set.h"

namespace UML {

    template <class T, class U>
    class Singleton : virtual public TypedSet<T,U> {
        virtual UmlPtr<T> get() = 0;
        virtual void set(T* el) = 0;
        virtual void set(T& el) = 0;
        virtual void set(UmlPtr<T> el) = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothing<T, U>,
                class RemovalPolicy = DoNothing<T, U>
            >
    class CustomSingleton : public PrivateSet<T, U, AdditionPolicy, RemovalPolicy> , public Singleton<T,U> {
        public:
            CustomSingleton(U& el) : PrivateSet<T, U>(el) {}
            CustomSingleton(U* el) : PrivateSet<T, U>(el) {}
            UmlPtr<T> get() override {
                SetLock myLck = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root) {
                    return this->m_root->m_ptr;
                }
                return UmlPtr<T>();
            }
            void set(T* el) override {
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
                    PrivateSet<T,U>::innerAdd(*el);
                    el->m_node->setReference(this->m_el);
                    this->m_el.m_node->setReference(*el);
                    // handle opposites
                    if (this->m_oppositeSignature) {
                        (el->*this->m_oppositeSignature)().innerAdd(this->m_el);   
                    }
                }
            }

            void set(T& el) override {
                set(&el);
            }

            void set(UmlPtr<T> el) override {
                if (el) {
                    set(*el);
                } else {
                    set(0);
                }
            }
    };
}
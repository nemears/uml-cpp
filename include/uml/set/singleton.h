#pragma once

#include "set.h"

namespace UML {

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomSingleton : public PrivateSet<T, U, AdditionPolicy, RemovalPolicy> {

        FRIEND_ALL_UML()
        friend void Parsers::setNamespace(NamedElement& el, ID id);
        friend void Parsers::setOwner(Element& el, ID id);

        protected:
            void innerAdd(T& el) override {
                if (this->m_root && el.getID() != this->m_root->m_ptr.id()) {
                    [[maybe_unused]] SetLock elLock = this->m_el.m_manager->lockEl(*this->m_root->m_ptr);
                    (*this->m_root->m_ptr).m_node->removeReference(this->m_el);
                    this->m_el.m_node->removeReference(*this->m_root->m_ptr);
                    T& elToRemove = this->m_root->m_ptr->template as<T>();
                    // remove
                    this->innerRemove(this->m_root->m_ptr.id());
                    this->handleOppositeRemove(elToRemove);
                }
                // add
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::innerAdd(el);
                el.m_node->setReference(this->m_el);
                this->m_el.m_node->setReference(el);
            }
            void innerAdd(ID id) {
               if (this->m_root && (id == ID::nullID() || id != this->m_root->m_ptr.id())) {
                    [[maybe_unused]] SetLock elLock = this->m_el.m_manager->lockEl(*this->m_root->m_ptr);

                    (*this->m_root->m_ptr).m_node->removeReference(this->m_el);
                    this->m_el.m_node->removeReference(*this->m_root->m_ptr);

                    T& elToRemove = this->m_root->m_ptr->template as<T>();
                    // remove
                    this->innerRemove(this->m_root->m_ptr.id());
                    // handle opposites
                    this->handleOppositeRemove(elToRemove);
                }
                if (id != ID::nullID()) {
                    // add
                    PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::innerAdd(id);
                    this->m_el.m_node->setReference(id);
                }
            }
        
        public:
            CustomSingleton(U& el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy>(el) {}
            CustomSingleton(U* el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy>(el) {}
            UmlPtr<T> get() const {
                [[maybe_unused]] SetLock myLck = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root) {
                    return this->m_root->m_ptr;
                }
                return UmlPtr<T>();
            }
            void set(T* el) {
                [[maybe_unused]] SetLock myLck = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root && (!el || el->getID() != this->m_root->m_ptr.id())) {
                    [[maybe_unused]] SetLock elLock = this->m_el.m_manager->lockEl(*this->m_root->m_ptr);
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot remove from read only set!");
                    }

                    (*this->m_root->m_ptr).m_node->removeReference(this->m_el);
                    this->m_el.m_node->removeReference(*this->m_root->m_ptr);
                    T& elToRemove = this->m_root->m_ptr->template as<T>();
                    // remove
                    this->innerRemove(this->m_root->m_ptr.id());
                    this->handleOppositeRemove(elToRemove);
                }
                if (el) {
                    [[maybe_unused]] SetLock elLock = this->m_el.m_manager->lockEl(*el);
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot add to read only set!");
                    }
                    // add
                    PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::innerAdd(*el);
                    el->m_node->setReference(this->m_el);
                    this->m_el.m_node->setReference(*el);
                    // handle opposites
                    this->handleOppositeAdd(*el);
                }
            }
            void set(ID id) {
                // "lock" elements we are editing
                [[maybe_unused]] SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (this->m_root && (id == ID::nullID() || id != this->m_root->m_ptr.id())) {
                    [[maybe_unused]] SetLock elLock = this->m_el.m_manager->lockEl(*this->m_root->m_ptr);
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot remove from read only set!");
                    }

                    (*this->m_root->m_ptr).m_node->removeReference(this->m_el);
                    this->m_el.m_node->removeReference(*this->m_root->m_ptr);

                    T& elToRemove = this->m_root->m_ptr->template as<T>();
                    // remove
                    this->innerRemove(this->m_root->m_ptr.id());
                    // handle opposites
                    this->handleOppositeRemove(elToRemove);
                }
                if (id != ID::nullID()) {
                    if (this->m_readOnly) {
                        throw SetStateException("Cannot add to read only set!");
                    }
                    // add
                    PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::innerAdd(id);
                    this->m_el.m_node->setReference(id);
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
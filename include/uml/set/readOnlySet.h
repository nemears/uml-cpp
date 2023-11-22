#pragma once

#include "privateSet.h"

namespace UML {

    template <class T, class U>
    class ReadOnlySet : virtual public TypedSet<T,U> {
        public:
            virtual bool contains(ID id) const = 0;
            virtual bool contains(T& el) const = 0;
            virtual bool contains(UmlPtr<T> el) const = 0;
            virtual bool contains(std::string name) const = 0;
            virtual bool empty() const = 0;
            virtual size_t size() const = 0;
            virtual T& get(ID id) const = 0;
            virtual T& get(std::string name) const = 0;
            virtual T& front() const = 0;
            virtual T& back() const = 0;
            virtual SetIterator<T> begin() = 0;
            virtual SetIterator<T> end() = 0;
            virtual ID_Set<T> ids() = 0;
            virtual PtrSet<T> ptrs() = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomReadOnlySet : public PrivateSet<T,U, AdditionPolicy, RemovalPolicy> , virtual public ReadOnlySet<T,U> {
        public:
            CustomReadOnlySet(U* el) : PrivateSet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            CustomReadOnlySet(U& el) : PrivateSet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            virtual ~CustomReadOnlySet() = default;
            bool contains(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(id);
            }
            bool contains(T& el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(el);
            }
            bool contains(UmlPtr<T> el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(el);
            }
            bool contains(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::contains(name);
            }
            T& get(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::get(id);
            }
            T& get(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::get(name);
            }
            T& front() const override {
                SetNode* curr = this->m_root;
                if (!curr) {
                    throw SetStateException("Cannot get front element because set is empty!");
                }

                while (curr->m_left) {
                    curr = curr->m_left;
                }

                return curr->m_ptr->as<T>();
            }
            T& back() const override {
                SetNode* curr = this->m_root;
                if (!curr) {
                    throw SetStateException("Cannot get back element because set is empty!");
                }

                // go all the way right
                while (curr->m_right) {
                    curr = curr->m_right;
                }

                // edge case where root is back
                if (curr != this->m_root) {
                    // now go all the way left
                    while (curr->m_left) {
                        curr = curr->m_left;
                    }
                }

                return curr->m_ptr->as<T>();
            }
            bool empty() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::empty();
            }
            size_t size() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::size();
            }

            SetIterator<T> begin() override {
                SetIterator<T> ret;
                if (!this->m_root) {
                    return ret;
                }
                ret.loadleft(this->m_root);
                return ret;
            }
            SetIterator<T> end() override {
                return SetIterator<T>();
            }

            ID_Set<T> ids() override {
                ID_Set<T> ret;
                ret.root = this->m_root;
                return ret;
            }

            PtrSet<T> ptrs() override {
                PtrSet<T> ret;
                ret.root = this->m_root;
                return ret;
            }
    };

}
#pragma once

#include "readOnlySet.h"

namespace UML {
    template <class T, class U>
    class Set : virtual public ReadOnlySet<T,U> {
        public:
            virtual void add(ID id) = 0;
            virtual void add(UmlPtr<T> el) = 0;
            virtual void add(T& el) = 0;
            template <class ... Ts>
            void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            }
            virtual void remove(ID id) = 0;
            virtual void remove(T& el) = 0;
            virtual void remove(UmlPtr<T> el) = 0;
            virtual void removeFromJustThisSet(ID id) = 0;
            virtual void clear() = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomSet : public CustomReadOnlySet<T,U, AdditionPolicy, RemovalPolicy> , virtual public Set<T,U> {
        public:
            CustomSet(U* el) : CustomReadOnlySet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            CustomSet(U& el) : CustomReadOnlySet<T,U, AdditionPolicy, RemovalPolicy>(el) {}
            virtual ~CustomSet() = default;
            void add(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(id);
            }
            void add(UmlPtr<T> el) override {
                if (el.loaded()) {
                    PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(*el);
                } else {
                    PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(el.id());
                }
            }
            void add(T& el) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::add(el);
            }
            void remove(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::remove(id);
            }
            void remove(T& el) override {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) override {
                remove(el.id());
            }
            void removeFromJustThisSet(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::removeFromJustThisSet(id);
            }
            void clear() override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy>::clear();
            }
    };
}
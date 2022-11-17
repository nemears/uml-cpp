#pragma once

namespace UML {
    template <class T, class U>
    class DoNothing {
        void apply(T& el, U& me) {

        }
    };
}
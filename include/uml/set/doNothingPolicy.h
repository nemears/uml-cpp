#pragma once

namespace UML {
    template <class T, class U>
    class DoNothing {
        public:
            static void apply(T& el, U& me) {

            }
    };
}
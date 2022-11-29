#pragma once

namespace UML {
    template <class T, class U>
    class DoNothingAdd {
        public:
            static void apply(T& el, U& me) {
                (void) el;
                (void) me;
            }
    };
    template <class T, class U>
    class DoNothingRemove {
        public:
            static void apply(T& el, U& me) {
                (void) el;
                (void) me;
            }
    };
}
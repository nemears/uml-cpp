#pragma once

namespace UML {

    class Element;

    class DoNothingPolicy {
        public:
            void elementAdded(__attribute__((unused)) Element& el, __attribute__((unused)) Element& me) {}
            void elementRemoved(__attribute__((unused)) Element& el, __attribute__((unused)) Element& me) {}
    };
}

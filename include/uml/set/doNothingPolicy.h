#pragma once

namespace UML {

    class AbstractElement;

    class DoNothingPolicy {
        public:
            void elementAdded(__attribute__((unused)) AbstractElement& el, __attribute__((unused)) AbstractElement& me) {}
            void elementRemoved(__attribute__((unused)) AbstractElement& el, __attribute__((unused)) AbstractElement& me) {}
    };
}

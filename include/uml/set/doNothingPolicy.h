#pragma once

namespace UML {

    class Element;

    class DoNothingPolicy {
        void addElement(__attribute__((unused)) Element& el, __attribute__((unused)) Element& me) {}
        void removeElement(__attribute__((unused)) Element& el, __attribute__((unused)) Element& me) {}
    };
}

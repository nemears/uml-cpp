#pragma once

#include "classifier.h"
#include "uml/managers/typeInfo.h"

namespace UML {
    class Signal : public Classifier {

        friend struct ElementInfo<Signal>;

        protected:
            IndexableOrderedSet<Property, Signal> m_ownedAttributes = IndexableOrderedSet<Property, Signal>(this);
        public:
            Signal(std::size_t elementType, AbstractManager& manager);
            IndexableOrderedSet<Property, Signal>& getOwnedAttributes();
            typedef TypeInfo<std::tuple<Classifier>, Signal> Info;
    };

    template <>
    struct ElementInfo<Signal> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Signal"; }
        static SetList sets(Signal& el) {
            return SetList {
                makeSetPair("ownedAttributes", el.m_ownedAttributes)
            };
        }
    };
}

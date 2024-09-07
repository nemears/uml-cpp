#pragma once

#include "classifier.h"
#include "uml/set/orderedSet.h"
#include "uml/types/element.h"

namespace UML {
    class Interface : public Classifier {

        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, Interface> m_ownedAttributes = OrderedSet<Property, Interface>(this);
            OrderedSet<Operation, Interface> m_ownedOperations = OrderedSet<Operation, Interface>(this);
            OrderedSet<Classifier, Interface> m_nestedClassifiers = OrderedSet<Classifier, Interface>(this);
            Interface(std::size_t elementType, AbstractManager& manager);
        public:
            OrderedSet<Property, Interface>& getOwnedAttributes();
            OrderedSet<Operation, Interface>& getOwnedOperations();
            OrderedSet<Classifier, Interface>& getNestedClassifiers();
            typedef TypeInfo<std::tuple<Classifier>, Interface> Info;
    };

    template <>
    struct ElementInfo<Interface> {
        static const bool abstract = false;
        inline static const std::string name {"Interface"};
        static SetList sets(Interface& el) {
            return SetList {
                makeSetPair("ownedAttributes", el.getOwnedAttributes()),
                makeSetPair("ownedOperations", el.getOwnedOperations()),
                makeSetPair("nestedClassifiers", el.getNestedClassifiers())
            };
        }
    };
}

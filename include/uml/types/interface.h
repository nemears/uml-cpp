#pragma once

#include "classifier.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Interface : public Classifier {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, Interface> m_ownedAttributes = OrderedSet<Property, Interface>(this);
            OrderedSet<Operation, Interface> m_ownedOperations = OrderedSet<Operation, Interface>(this);
            OrderedSet<Classifier, Interface> m_nestedClassifiers = OrderedSet<Classifier, Interface>(this);
            Interface();
        public:
            virtual ~Interface();
            OrderedSet<Property, Interface>& getOwnedAttributes();
            OrderedSet<Operation, Interface>& getOwnedOperations();
            OrderedSet<Classifier, Interface>& getNestedClassifiers();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INTERFACE_UML;
            }
    };
}

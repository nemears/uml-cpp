#pragma once

#include "classifier.h"
#include "uml/set/orderedSet.h"

namespace UML {

    class Operation;

    class DataType : public Classifier {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, DataType> m_ownedAttributes = OrderedSet<Property, DataType>(this);
            OrderedSet<Operation, DataType> m_ownedOperations = OrderedSet<Operation, DataType>(this);
            DataType();
        public:
            ~DataType();
            OrderedSet<Property, DataType>& getOwnedAttributes();
            OrderedSet<Operation, DataType>& getOwnedOperations();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

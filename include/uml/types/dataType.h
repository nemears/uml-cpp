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
            IndexableOrderedSet<Property, DataType> m_ownedAttributes = IndexableOrderedSet<Property, DataType>(this);
            IndexableOrderedSet<Operation, DataType> m_ownedOperations = IndexableOrderedSet<Operation, DataType>(this);
            DataType();
        public:
            ~DataType();
            IndexableOrderedSet<Property, DataType>& getOwnedAttributes();
            IndexableOrderedSet<Operation, DataType>& getOwnedOperations();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

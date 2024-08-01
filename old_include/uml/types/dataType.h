#ifndef _UML_DATA_TYPE_H_
#define _UML_DATA_TYPE_H_

#include "classifier.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class DataType : public Classifier {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            CustomOrderedSet<Property, DataType> m_ownedAttributes = CustomOrderedSet<Property, DataType>(this);
            CustomOrderedSet<Operation, DataType> m_ownedOperations = CustomOrderedSet<Operation, DataType>(this);
            TypedSet<Property, DataType>& getOwnedAttributesSet();
            TypedSet<Operation, DataType>& getOwnedOperationsSet();
            DataType();
        public:
            ~DataType();
            OrderedSet<Property, DataType>& getOwnedAttributes();
            OrderedSet<Operation, DataType>& getOwnedOperations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

#endif
#ifndef DATATYPE_H
#define DATATYPE_H

#include "classifier.h"
#include "orderedSet.h"

namespace UML {
    class DataType : public Classifier {

        friend class UmlManager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, DataType> m_ownedAttributes = OrderedSet<Property, DataType>(this);
            OrderedSet<Operation, DataType> m_ownedOperations = OrderedSet<Operation, DataType>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<Property, DataType>& getOwnedAttributesSet();
            Set<Operation, DataType>& getOwnedOperationsSet();
            void init();
            void copy(const DataType& rhs);
            DataType();
        public:
            ~DataType();
            DataType(const DataType& rhs);
            OrderedSet<Property, DataType>& getOwnedAttributes();
            OrderedSet<Operation, DataType>& getOwnedOperations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

#endif
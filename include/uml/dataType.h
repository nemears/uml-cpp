#ifndef DATATYPE_H
#define DATATYPE_H

#include "classifier.h"

namespace UML {
    class DataType : public Classifier {
        friend class UmlManager;
        protected:
            Set<Property, DataType> m_ownedAttributes = Set<Property, DataType>(this);
            // Sequence<Operation> m_ownedOperation = Sequence<Operation>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const DataType& rhs);
            DataType();
        public:
            ~DataType();
            DataType(const DataType& rhs);
            Set<Property, DataType>& getOwnedAttributes();
            // Sequence<Operation>& getOwnedOperation();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

#endif
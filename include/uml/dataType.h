#ifndef _UML_DATA_TYPE_H_
#define _UML_DATA_TYPE_H_

#include "classifier.h"
#include "orderedSet.h"

namespace UML {
    class DataType : public Classifier {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, DataType> m_ownedAttributes = OrderedSet<Property, DataType>(this);
            OrderedSet<Operation, DataType> m_ownedOperations = OrderedSet<Operation, DataType>(this);
            Set<Property, DataType>& getOwnedAttributesSet();
            Set<Operation, DataType>& getOwnedOperationsSet();
            void init();
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
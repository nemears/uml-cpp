#pragma once

#include "classifier.h"

namespace UML {

    class Operation;

    class DataType : public Classifier {

        friend class Property;
        friend class Operation;

        protected:
            IndexableOrderedSet<Property, DataType> m_ownedAttributes = IndexableOrderedSet<Property, DataType>(this);
            IndexableOrderedSet<Operation, DataType> m_ownedOperations = IndexableOrderedSet<Operation, DataType>(this);
        public:
            DataType(std::size_t elementType, AbstractManager& manager);
            IndexableOrderedSet<Property, DataType>& getOwnedAttributes();
            IndexableOrderedSet<Operation, DataType>& getOwnedOperations();
            typedef TypeInfo<std::tuple<Classifier>, DataType> Info;
    };

    template <>
    struct ElementInfo<DataType> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"DataType"};
        static SetList sets(DataType& el) {
            return SetList {
                makeSetPair("ownedAttributes", el.getOwnedAttributes()),
                makeSetPair("ownedOperations", el.getOwnedOperations()),
            };
        }
    };
}

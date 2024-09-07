#pragma once

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {
        protected:
            PrimitiveType(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<DataType>, PrimitiveType> Info;
    };

    template <>
    struct ElementInfo<PrimitiveType> {
        static const bool abstract = false;
        inline static const std::string name {"PrimitiveType"};
        static SetList sets(__attribute__((unused)) PrimitiveType& el) {
            return SetList{};
        }
    };
}

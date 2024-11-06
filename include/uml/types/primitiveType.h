#pragma once

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {
        public:
            PrimitiveType(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<DataType>, PrimitiveType> Info;
    };

    template <>
    struct ElementInfo<PrimitiveType> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "PrimitiveType"; }
        static SetList sets(__attribute__((unused)) PrimitiveType& el) {
            return SetList{};
        }
    };
}

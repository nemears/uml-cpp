#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"
#include "valueSpecification.h"

namespace UML {
    class LiteralSpecification : public ValueSpecification {
        protected:
            LiteralSpecification(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<ValueSpecification>, LiteralSpecification> Info;
    };

    template <>
    struct ElementInfo<LiteralSpecification> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "LiteralSpecification"; }
    };
}

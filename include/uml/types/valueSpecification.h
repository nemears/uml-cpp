#pragma once

#include "typedElement.h"
#include "packageableElement.h"
#include "uml/managers/typeInfo.h"

namespace UML {

    class ValueSpecification : public TypedElement , public PackageableElement {

        protected:
            ValueSpecification(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<TypedElement, PackageableElement>, ValueSpecification> Info;
    };

    template <>
    struct ElementInfo<ValueSpecification> : public DefaultInfo {
        static const bool abstract = true;
        static constexpr std::string_view name {"ValueSpecification"};
    };
}

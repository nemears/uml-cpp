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
    struct ElementInfo<ValueSpecification> {
        static const bool abstract = true;
        inline static std::string name {"ValueSpecification"};
        static SetList sets(__attribute__((unused)) ValueSpecification& el) {
            return SetList{};
        }
    };
}

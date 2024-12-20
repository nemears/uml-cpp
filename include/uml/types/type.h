#pragma once

#include "packageableElement.h"

namespace UML{

    class TypedElement;
    class Association;
    class Operation;

    class Type : virtual public PackageableElement {

        friend class TypedElement;
        friend class Association;
        friend class Operation;
        
        protected:
            Type(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<PackageableElement>, Type> Info;
    };

    template <>
    struct ElementInfo<Type> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Type"; }
        static SetList sets(__attribute__((unused)) Type& el) {
            return SetList {};
        }
    };
}

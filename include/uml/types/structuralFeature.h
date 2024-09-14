#pragma once

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : virtual public TypedElement, public MultiplicityElement, public Feature {
        protected:
            bool m_readOnly = false;
            StructuralFeature(std::size_t elementType, AbstractManager& manager);
        public:
            class InvalidValueException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
            bool isReadOnly() const;
            void setReadOnly(bool readOnly);
            typedef TypeInfo<std::tuple<TypedElement, MultiplicityElement, Feature>, StructuralFeature> Info;
    };

    template <>
    struct ElementInfo<StructuralFeature> : public DefaultInfo {
        static const bool abstract = true;
        inline static std::string name{"StructuralFeature"};
        static SetList sets(__attribute__((unused)) StructuralFeature& el) {
            return SetList{};
        }
    };
}

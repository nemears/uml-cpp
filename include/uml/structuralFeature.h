#ifndef STRUCTURALFEATUREH
#define STRUCTURALFEATUREH

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : virtual public TypedElement, public MultiplicityElement, public Feature {
        public:
            StructuralFeature(){};
            StructuralFeature(const StructuralFeature& feature) : TypedElement(feature), MultiplicityElement(feature), Feature(feature) {};
            ElementType getElementType() const override;
            class InvalidValueException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
            bool isSubClassOf(ElementType eType) override;
            StructuralFeature& operator=(StructuralFeature&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::STRUCTURAL_FEATURE;
            };
    };
}

#endif
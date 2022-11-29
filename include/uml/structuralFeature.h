#ifndef _UML_STRUCTURAL_FEATURE_H_
#define _UML_STRUCTURAL_FEATURE_H_

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : virtual public TypedElement, public MultiplicityElement, public Feature {
        protected:
            bool m_readOnly = false;
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            StructuralFeature();
        public:
            class InvalidValueException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
            bool isReadOnly() const;
            void setReadOnly(bool readOnly);
            bool isSubClassOf(ElementType eType) const override;
            StructuralFeature& operator=(StructuralFeature&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::STRUCTURAL_FEATURE;
            };
    };
}

#endif
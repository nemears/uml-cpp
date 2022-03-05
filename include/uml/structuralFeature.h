#ifndef _UML_STRUCTURAL_FEATURE_H_
#define _UML_STRUCTURAL_FEATURE_H_

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : virtual public TypedElement, public MultiplicityElement, public Feature {
        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            StructuralFeature();
        public:
            class InvalidValueException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
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
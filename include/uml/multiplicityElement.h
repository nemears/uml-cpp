#ifndef MULTIPLICITYELEMENT_H
#define MULTIPLICITYELEMENT_H

#include "element.h"

namespace UML {

    class ValueSpecification;

    class MultiplicityElement : virtual public Element {
        protected:
            int lower;
            int upper;
            ID m_lowValID;
            ValueSpecification* m_lowValPtr;
            ID m_upValID;
            ValueSpecification* m_upValPtr;
        private:
            bool multiplicityIsSpecified = false;
            bool lowSpecified = false;
            bool upSpecified = false;
        public:
            MultiplicityElement();
            int getLower();
            void setLower(const int low);
            ValueSpecification* getLowerValue();
            void setLowerValue(ValueSpecification* val);
            int getUpper();
            void setUpper(const int up);
            ValueSpecification* getUpperValue();
            void setUpperValue(ValueSpecification* val);
            class MultiplicityNotSpecifiedException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to access multiplicity but it was not specified yet";
                    }
            } multiplicityNotSpecifiedException;
            bool multiplicitySpecified();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MULTIPLICITY_ELEMENT;
            };
    };
}

#endif
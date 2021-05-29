#ifndef MULTIPLICITYELEMENT_H
#define MULTIPLICITYELEMENT_H

#include "element.h"
#include "valueSpecification.h"

namespace UML {
    class MultiplicityElement : virtual public Element {
        protected:
            int lower;
            int upper;
            ValueSpecification* m_lowVal;
            ValueSpecification* m_upVal;
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
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::MULTIPLICITY_ELEMENT;
            };
    };
}

#endif
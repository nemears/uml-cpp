#ifndef MULTIPLICITYELEMENT_H
#define MULTIPLICITYELEMENT_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class ValueSpecification;

    class MultiplicityElement : virtual public Element {
        protected:
            int m_lower = -1;
            int m_upper = -1;
            Singleton<ValueSpecification, MultiplicityElement> m_lowVal = Singleton<ValueSpecification, MultiplicityElement>(this);
            class RemoveLowerValueProcedures : public AbstractSingletonProcedure<ValueSpecification, MultiplicityElement> {
                public:
                    RemoveLowerValueProcedures(MultiplicityElement* me) : AbstractSingletonProcedure<ValueSpecification, MultiplicityElement>(me) {};
                    void operator()(ValueSpecification* el) const override;
            };
            class AddLowerValueProcedures : public AbstractSingletonProcedure<ValueSpecification, MultiplicityElement> {
                public:
                    AddLowerValueProcedures(MultiplicityElement* me) : AbstractSingletonProcedure<ValueSpecification, MultiplicityElement>(me) {};
                    void operator()(ValueSpecification* el) const override;
            };
            Singleton<ValueSpecification, MultiplicityElement> m_upVal = Singleton<ValueSpecification, MultiplicityElement>(this);
            class RemoveUpperValueProcedures : public AbstractSingletonProcedure<ValueSpecification, MultiplicityElement> {
                public:
                    RemoveUpperValueProcedures(MultiplicityElement* me) : AbstractSingletonProcedure<ValueSpecification, MultiplicityElement>(me) {};
                    void operator()(ValueSpecification* el) const override;
            };
            class AddUpperValueProcedures : public AbstractSingletonProcedure<ValueSpecification, MultiplicityElement> {
                public:
                    AddUpperValueProcedures(MultiplicityElement* me) : AbstractSingletonProcedure<ValueSpecification, MultiplicityElement>(me) {};
                    void operator()(ValueSpecification* el) const override;
            };
            void referencingReleased(ID id) override;
        private:
            bool m_multiplicityIsSpecified = false;
            bool m_lowSpecified = false;
            bool m_upSpecified = false;
        public:
            MultiplicityElement();
            MultiplicityElement(const MultiplicityElement& rhs);
            int getLower();
            void setLower(const int low);
            ValueSpecification* getLowerValue();
            ValueSpecification& getLowerValueRef();
            bool hasLowerValue() const;
            void setLowerValue(ValueSpecification* val);
            void setLowerValue(ValueSpecification& val);
            int getUpper();
            void setUpper(const int up);
            ValueSpecification* getUpperValue();
            ValueSpecification& getUpperValueRef();
            bool hasUpperValue() const;
            void setUpperValue(ValueSpecification* val);
            void setUpperValue(ValueSpecification& val);
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
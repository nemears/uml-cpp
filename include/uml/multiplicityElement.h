#ifndef _UML_MULTIPLICITY_ELEMENT_H_
#define _UML_MULTIPLICITY_ELEMENT_H_

#include "element.h"
#include "set/singleton.h"
#include "valueSpecification.h"

namespace UML {

    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class MultiplicityElement : virtual public Element {
        protected:
            bool m_isOrdered = false;
            bool m_isUnique = true;
            int m_lower = -1;
            int m_upper = -1;
            class AddLowerPolicy {
                public:
                    void apply(ValueSpecification& el, MultiplicityElement& me);
            };
            class RemoveLowerPolicy {
                public:
                    void apply(ValueSpecification& el, MultiplicityElement& me);
            };
            class AddUpperPolicy {
                public:
                    void apply(ValueSpecification& el, MultiplicityElement& me);
            };
            class RemoveUpperPolicy {
                public:
                    void apply(ValueSpecification& el, MultiplicityElement& me);
            };
            CustomSingleton<ValueSpecification, MultiplicityElement, AddLowerPolicy, RemoveLowerPolicy> m_lowVal = CustomSingleton<ValueSpecification, MultiplicityElement, AddLowerPolicy, RemoveLowerPolicy>(this);
            CustomSingleton<ValueSpecification, MultiplicityElement, AddUpperPolicy, RemoveUpperPolicy> m_upVal = CustomSingleton<ValueSpecification, MultiplicityElement, AddUpperPolicy, RemoveUpperPolicy>(this);
            TypedSet<ValueSpecification, MultiplicityElement>& getLowerValueSingleton();
            TypedSet<ValueSpecification, MultiplicityElement>& getUpperValueSingleton();
            void init();
            MultiplicityElement();
        private:
            bool m_multiplicityIsSpecified = false;
            bool m_lowSpecified = false;
            bool m_upSpecified = false;
        public:
            virtual ~MultiplicityElement();
            int getLower();
            void setLower(const int low);
            ValueSpecificationPtr getLowerValue() const;
            void setLowerValue(ValueSpecification* val);
            void setLowerValue(ValueSpecification& val);
            void setLowerValue(ValueSpecificationPtr val);
            void setLowerValue(ID id);
            int getUpper();
            void setUpper(const int up);
            ValueSpecificationPtr getUpperValue() const;
            void setUpperValue(ValueSpecification* val);
            void setUpperValue(ValueSpecification& val);
            void setUpperValue(ValueSpecificationPtr val);
            void setUpperValue(ID id);
            class MultiplicityNotSpecifiedException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to access multiplicity but it was not specified yet";
                    }
            } multiplicityNotSpecifiedException;
            bool multiplicitySpecified();
            bool isOrdered() const;
            void setIsOrdered(bool isOrdered);
            bool isUnique() const;
            void setIsUnique(bool isUnique);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MULTIPLICITY_ELEMENT;
            };
    };
}

#endif
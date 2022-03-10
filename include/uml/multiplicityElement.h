#ifndef _UML_MULTIPLICITY_ELEMENT_H_
#define _UML_MULTIPLICITY_ELEMENT_H_

#include "element.h"
#include "singleton.h"
#include "valueSpecification.h"

namespace UML {

    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class MultiplicityElement : virtual public Element {
        protected:
            int m_lower = -1;
            int m_upper = -1;
            Singleton<ValueSpecification, MultiplicityElement> m_lowVal = Singleton<ValueSpecification, MultiplicityElement>(this);
            Singleton<ValueSpecification, MultiplicityElement> m_upVal = Singleton<ValueSpecification, MultiplicityElement>(this);
            class AddLowerFunctor : public SetFunctor {
                public:
                    AddLowerFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            class RemoveLowerFunctor : public SetFunctor {
                public:
                    RemoveLowerFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            class AddUpperFunctor : public SetFunctor {
                public:
                    AddUpperFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            class RemoveUpperFunctor : public SetFunctor {
                public:
                    RemoveUpperFunctor(Element* them) : SetFunctor(them) {};
                    void operator()(Element& el) const override;
            };
            Set<ValueSpecification, MultiplicityElement>& getLowerValueSingleton();
            Set<ValueSpecification, MultiplicityElement>& getUpperValueSingleton();
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
            int getUpper();
            void setUpper(const int up);
            ValueSpecificationPtr getUpperValue() const;
            void setUpperValue(ValueSpecification* val);
            void setUpperValue(ValueSpecification& val);
            class MultiplicityNotSpecifiedException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to access multiplicity but it was not specified yet";
                    }
            } multiplicityNotSpecifiedException;
            bool multiplicitySpecified();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MULTIPLICITY_ELEMENT;
            };
    };
}

#endif
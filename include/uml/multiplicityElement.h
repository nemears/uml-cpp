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
            void copy(const MultiplicityElement& rhs);
            MultiplicityElement();
        private:
            bool m_multiplicityIsSpecified = false;
            bool m_lowSpecified = false;
            bool m_upSpecified = false;
        public:
            MultiplicityElement(const MultiplicityElement& rhs);
            virtual ~MultiplicityElement();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MULTIPLICITY_ELEMENT;
            };
    };
}

#endif
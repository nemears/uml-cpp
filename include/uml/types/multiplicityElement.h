#pragma once

#include "element.h"
#include "uml/set/singleton.h"

namespace UML {

    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class MultiplicityElement : virtual public Element {

        friend struct ElementInfo<MultiplicityElement>;

        protected:
            bool m_isOrdered = false;
            bool m_isUnique = true;
            int m_lower = -1;
            int m_upper = -1;
            struct LowerPolicy {
                void elementAdded(ValueSpecification& el, MultiplicityElement& me);
                void elementRemoved(ValueSpecification& el, MultiplicityElement& me);
            };
            struct UpperPolicy {
                void elementAdded(ValueSpecification& el, MultiplicityElement& me);
                void elementRemoved(ValueSpecification& el, MultiplicityElement& me);
            };
            Singleton<ValueSpecification, MultiplicityElement, LowerPolicy> m_lowVal = Singleton<ValueSpecification, MultiplicityElement, LowerPolicy>(this);
            Singleton<ValueSpecification, MultiplicityElement, UpperPolicy> m_upVal = Singleton<ValueSpecification, MultiplicityElement, UpperPolicy>(this);
            Singleton<ValueSpecification, MultiplicityElement, LowerPolicy>& getLowerValueSingleton();
            Singleton<ValueSpecification, MultiplicityElement, UpperPolicy>& getUpperValueSingleton();
            MultiplicityElement(std::size_t elementType, AbstractManager& manager);
        private:
            bool m_multiplicityIsSpecified = false;
            bool m_lowSpecified = false;
            bool m_upSpecified = false;
        public:
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
            bool lowSpecified() const;
            bool upSpecified() const;
            bool isOrdered() const;
            void setIsOrdered(bool isOrdered);
            bool isUnique() const;
            void setIsUnique(bool isUnique);
            typedef TypeInfo<std::tuple<Element>, MultiplicityElement> Info;
    };

    template <>
    struct ElementInfo<MultiplicityElement> {
        static const bool abstract = true;
        inline static const std::string name{"MultiplicityElement"};
        static SetList sets(MultiplicityElement& el) {
            return SetList {
                makeSetPair("lowerValue", el.m_lowVal),
                makeSetPair("upperValue", el.m_upVal)
            };
        }
    };
}

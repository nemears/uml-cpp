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
    struct ElementInfo<MultiplicityElement> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "MultiplicityElement"; }
        static SetList sets(MultiplicityElement& el) {
            return SetList {
                makeSetPair("lowerValue", el.m_lowVal),
                makeSetPair("upperValue", el.m_upVal)
            };
        }
        static const bool extraData = true;
        struct MultiplicityElementUpperPolicy : public AbstractDataPolicy {
            MultiplicityElement& el;
            MultiplicityElementUpperPolicy(MultiplicityElement& el) : el(el) {}
            std::string getData() override {
                if (el.getUpper() >= 0 && !el.getUpperValue()) {
                    return std::to_string(el.getUpper());
                }
                return "";
            }
            void setData(std::string data) override {
                el.setUpper(std::stoi(data));
            }
        };
        struct MultiplicityElementLowerPolicy : public AbstractDataPolicy {
            MultiplicityElement& el;
            MultiplicityElementLowerPolicy(MultiplicityElement& el) : el(el) {}
            std::string getData() override {
                if (el.getLower() >= 0 && !el.getLowerValue()) {
                    return std::to_string(el.getLower());
                }
                return "";
            }
            void setData(std::string data) override {
                el.setLower(std::stoi(data));
            }
        };
        struct MultiplicityElementIsOrderedPolicy : public AbstractDataPolicy {
            MultiplicityElement& el;
            MultiplicityElementIsOrderedPolicy(MultiplicityElement& el) : el(el) {}
            std::string getData() override {
                if (el.isOrdered()) {
                    return "true";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "true") {
                    el.setIsOrdered(true);
                } else if (data == "false") {
                    el.setIsOrdered(false);
                }
            }
        };
        struct MultiplicityElementIsUniquePolicy : public AbstractDataPolicy {
            MultiplicityElement& el;
            MultiplicityElementIsUniquePolicy(MultiplicityElement& el) : el(el) {}
            std::string getData() override {
                if (!el.isUnique()) {
                    return "false";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "true") {
                    el.setIsUnique(true);
                } else if (data == "false") {
                    el.setIsUnique(false);
                }
            }
        };
        static DataList data(MultiplicityElement& el) {
            return DataList {
                createDataPair("upper", new MultiplicityElementUpperPolicy(el)),
                createDataPair("lower", new MultiplicityElementLowerPolicy(el)),
                createDataPair("isOrdered", new MultiplicityElementIsOrderedPolicy(el)),
                createDataPair("isUnique", new MultiplicityElementIsUniquePolicy(el))
            };
        }
    };
}
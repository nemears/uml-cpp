#pragma once

#include "literalSpecification.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        protected:
            double m_value = 0;
        public:
            LiteralReal(std::size_t elementType, AbstractManager& manager);
            double getValue();
            void setValue(double val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralReal> Info;
    };

    template <>
    struct ElementInfo<LiteralReal> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "LiteralReal"; }
        static const bool extraData = true;
        struct LiteralRealValuePolicy : public AbstractDataPolicy {
            LiteralReal& el;
            LiteralRealValuePolicy(LiteralReal& el) : el(el) {}
            std::string getData() override {
                return std::to_string(el.getValue());
            }
            void setData(std::string data) override {
                el.setValue(std::stod(data));
            }
        };
        static DataList data(LiteralReal& el) {
            return DataList {
                createDataPair("value", new LiteralRealValuePolicy(el))
            };
        }
    };
}

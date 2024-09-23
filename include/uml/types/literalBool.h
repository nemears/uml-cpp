#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {

        protected:
            bool m_value = false;
        public:
            LiteralBool(std::size_t elementType, AbstractManager& manager);
            bool getValue();
            void setValue(bool val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralBool> Info;
    };

    template <>
    struct ElementInfo<LiteralBool> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"LiteralBool"};
        static const bool extraData = true;
        struct LiteralBoolValuePolicy : public AbstractDataPolicy {
            LiteralBool& el;
            LiteralBoolValuePolicy(LiteralBool& el) : el(el) {}
            std::string getData() override {
                if (el.getValue()) {
                    return "true";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "true") {
                    el.setValue(true);
                }
            }
        };
        static DataList data(LiteralBool& el) {
            return DataList {
                createDataPair("value", new LiteralBoolValuePolicy(el))
            };
        }
    };
}

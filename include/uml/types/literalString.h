#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralString : public LiteralSpecification {

        protected:
            std::string m_value;
        public:
            LiteralString(std::size_t elementType, AbstractManager& manager);
            std::string getValue();
            void setValue(const std::string& val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralString> Info;
    };

    template <>
    struct ElementInfo<LiteralString> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "LiteralString"; }
        static const bool extraData = true;
        struct LiteralStringValuePolicy : public AbstractDataPolicy {
            LiteralString& el;
            LiteralStringValuePolicy(LiteralString& el) : el(el) {}
            std::string getData() override {
                return el.getValue();
            }
            void setData(std::string data) override {
                el.setValue(data);
            }
        };
        static DataList data(LiteralString& el) {
            return DataList {
                createDataPair("value", new LiteralStringValuePolicy(el))
            };
        }
    };
}

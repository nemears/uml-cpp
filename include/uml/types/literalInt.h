#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {
        protected:
            int m_value = 0; // TODO change to long so we can hold any number?
        public:
            LiteralInt(std::size_t elementType, AbstractManager& manager);
            int getValue();
            void setValue(int val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralInt> Info;
    };

    template <>
    struct ElementInfo<LiteralInt> : public DefaultInfo {
        static const bool abstract = false;
        inline static std::string name {"LiteralInt"};
        static const bool extraData = true;
        struct LiteralIntValuePolicy : public AbstractDataPolicy {
            LiteralInt& el;
            LiteralIntValuePolicy(LiteralInt& el) : el(el) {}
            std::string getData() override {
                return std::to_string(el.getValue());
            }
            void setData(std::string data) override {
                el.setValue(std::stoi(data));
            }
        };
        static DataList data(LiteralInt& el) {
            return DataList {
                createDataPair("value", new LiteralIntValuePolicy(el))
            };
        }
    };
}

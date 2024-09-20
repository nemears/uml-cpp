#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {
        protected:
            unsigned long m_val;
            bool m_infinite;
        public:
            LiteralUnlimitedNatural(std::size_t elementType, AbstractManager& manager); 
            /** 
             *  NOTE: this will always be initialized to 0
             **/   
            bool isInfinite();
            unsigned long getNumberValue();
            void setNumberValue(unsigned long val);
            void setInfinite();
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralUnlimitedNatural> Info;
    };

    template <>
    struct ElementInfo<LiteralUnlimitedNatural> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"LiteralUnlimitedNatural"};
        static const bool extraData = true;
        struct LiteralUnlimitedNaturalValuePolicy : public AbstractDataPolicy {
            LiteralUnlimitedNatural& el;
            LiteralUnlimitedNaturalValuePolicy(LiteralUnlimitedNatural& el) : el(el) {}
            std::string getData() override {
                if (el.isInfinite()) {
                    return "*";
                }
                return std::to_string(el.getNumberValue());
            }
            void setData(std::string data) override {
                if (data == "*") {
                    el.setInfinite();
                } else {
                    el.setNumberValue(std::stol(data));
                }
            }
        };
        static DataList data(LiteralUnlimitedNatural& el) {
            return DataList {
                createDataPair("value", new LiteralUnlimitedNaturalValuePolicy(el))
            };
        }
    };
}
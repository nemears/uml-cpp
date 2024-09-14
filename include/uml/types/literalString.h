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
        inline static std::string name {"LiteralString"};
    };
}

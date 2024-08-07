#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralString : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            std::string m_value;
            LiteralString();
        public:
            std::string getValue();
            void setValue(const std::string& val);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_STRING;
            };
    };
}

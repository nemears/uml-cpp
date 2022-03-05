#ifndef _UML_LITERAL_STRING_H_
#define _UML_LITERAL_STRING_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralString : public LiteralSpecification {

        friend class UmlManager;

        protected:
            std::string m_value;
            LiteralString();
        public:
            std::string getValue();
            void setValue(std::string val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_STRING;
            };
    };
}

#endif
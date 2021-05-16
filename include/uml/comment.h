#ifndef COMMENT_H
#define COMMENT_H

#include "uml/element.h"

namespace UML {
    class Comment : public Element {
        private:
            std::string m_body;
        public:
            std::string getBody();
            void setBody(std::string body);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
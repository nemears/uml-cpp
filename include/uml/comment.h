#ifndef COMMENT_H
#define COMMENT_H

#include "uml/element.h"

namespace UML {
    class Comment : public Element {
        private:
            std::string m_body;
            Element* m_owningElement;
        public:
            Comment();
            std::string getBody();
            void setBody(std::string body);
            Element* getOwningElement();
            void setOwningElement(Element* el);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

#endif
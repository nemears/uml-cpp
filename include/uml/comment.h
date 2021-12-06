#ifndef COMMENT_H
#define COMMENT_H

#include "uml/element.h"
#include "singleton.h"

namespace UML {
    class Comment : public Element {

        friend class UmlManager;

        private:
            std::string m_body;
            Comment();
        public:
            Comment(const Comment& rhs);
            std::string getBody();
            void setBody(std::string body);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

#endif
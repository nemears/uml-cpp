#ifndef COMMENT_H
#define COMMENT_H

#include "uml/element.h"
#include "singleton.h"

namespace UML {

    class RemoveOwnedCommentFunctor;

    class Comment : public Element {
        friend class RemoveOwnedCommentFunctor;
        private:
            std::string m_body;
            ID m_owningElementID;
            Element* m_owningElementPtr;
        public:
            Comment();
            std::string getBody();
            void setBody(std::string body);
            Element* getOwningElement();
            void setOwningElement(Element* el);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

#endif
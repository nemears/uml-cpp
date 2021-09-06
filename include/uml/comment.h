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
            Singleton<Element, Comment> m_owningElement = Singleton<Element, Comment>(this);
            class RemoveOwningElementProcedure : public AbstractSingletonProcedure<Element, Comment> {
                public:
                    RemoveOwningElementProcedure(Comment* me) : AbstractSingletonProcedure<Element, Comment>(me) {};
                    void operator()(Element* el) const override;
            };
            class AddOwningElementProcedure : public AbstractSingletonProcedure<Element, Comment> {
            public:
                AddOwningElementProcedure(Comment* me) : AbstractSingletonProcedure<Element, Comment>(me) {};
                void operator()(Element* el) const override;
            };
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
#ifndef COMMENT_H
#define COMMENT_H

#include "uml/element.h"
#include "singleton.h"

namespace UML {

    class RemoveOwnedCommentFunctor;
    namespace Parsers {
        class SetOwningElement;
    }

    class Comment : public Element {

        friend class UmlManager;
        friend class RemoveOwnedCommentFunctor;
        friend class Parsers::SetOwningElement;

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
                void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Comment();
        public:
            Comment(const Comment& lhs);
            std::string getBody();
            void setBody(std::string body);
            Element* getOwningElement();
            Element& getOwningElementRef();
            ID getOwningElementID() const;
            bool hasOwningElement() const;
            void setOwningElement(Element* el);
            void setOwningElement(Element& el);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

#endif
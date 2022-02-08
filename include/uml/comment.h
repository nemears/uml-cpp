#ifndef COMMENT_H
#define COMMENT_H

#include "element.h"
#include "set.h"

namespace UML {
    class Comment : public Element {

        friend class UmlManager;

        protected:
            Set<Element, Comment> m_annotatedElements = Set<Element, Comment>(this);
            std::string m_body;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Comment& rhs);
            Comment();
        public:
            Comment(const Comment& rhs);
            std::string getBody();
            void setBody(std::string body);
            Set<Element, Comment>& getAnnotatedElements();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

#endif
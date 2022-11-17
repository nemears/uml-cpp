#ifndef _UML_COMMENT_H_
#define _UML_COMMENT_H_

#include "element.h"
#include "set/set.h"

namespace UML {
    class Comment : public Element {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Element, Comment> m_annotatedElements = Set<Element, Comment>(this);
            std::string m_body; // TODO move to literal string?
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceErased(ID id) override;
            void init();
            Comment();
        public:
            virtual ~Comment();
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
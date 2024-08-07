#pragma once

#include "element.h"
#include "uml/set/set.h"

namespace UML {
    class Comment : public Element {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Element, Comment> m_annotatedElements = Set<Element, Comment>(this);
            std::string m_body; // TODO move to literal string?
            void referenceErased(ID id) override;
            Comment();
        public:
            virtual ~Comment();
            std::string getBody() const;
            void setBody(const std::string& body);
            Set<Element, Comment>& getAnnotatedElements();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::COMMENT;
            };
    };
}

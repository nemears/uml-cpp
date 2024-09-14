#pragma once

#include "element.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"

namespace UML {
    class Comment : public Element {

        friend struct ElementInfo<Comment>;

        protected:
            Set<Element, Comment> m_annotatedElements = Set<Element, Comment>(this);
            std::string m_body; // TODO move to literal string?
        public:
            Comment(std::size_t elementType, AbstractManager& manager);
            std::string getBody() const;
            void setBody(const std::string& body);
            Set<Element, Comment>& getAnnotatedElements();
            typedef TypeInfo<std::tuple<Element>, Comment> Info;
    };

    template <>
    struct ElementInfo<Comment> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Comment"};
        static SetList sets(Comment& el) {
            return SetList {
                makeSetPair("annotatedElements", el.m_annotatedElements)
            };
        }
    };
}

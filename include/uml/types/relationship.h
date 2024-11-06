#pragma once

#include "uml/set/set.h"
#include "uml/types/element.h"

namespace UML {
    class Relationship : virtual public Element {
        protected:
            ReadOnlySet<Element, Relationship> m_relatedElements = ReadOnlySet<Element, Relationship>(this);
            Relationship(std::size_t elementType, AbstractManager& manager);
        public:
            ReadOnlySet<Element, Relationship>& getRelatedElements();
            typedef TypeInfo<std::tuple<Element>, Relationship> Info;
    };

    template <>
    struct ElementInfo<Relationship> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Relationship"; }
        static SetList sets(Relationship& el) {
            return SetList{
                makeSetPair("relatedElement", el.getRelatedElements())
            };
        }
    };
}

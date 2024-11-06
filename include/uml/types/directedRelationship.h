#pragma once

#include "relationship.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            ReadOnlySet<Element, DirectedRelationship> m_targets = ReadOnlySet<Element, DirectedRelationship>(this);
            ReadOnlySet<Element, DirectedRelationship> m_sources = ReadOnlySet<Element, DirectedRelationship>(this);
            DirectedRelationship(std::size_t elementType, AbstractManager& manager);
        public:
            ReadOnlySet<Element, DirectedRelationship>& getTargets();
            ReadOnlySet<Element, DirectedRelationship>& getSources();
            typedef TypeInfo<std::tuple<Relationship>, DirectedRelationship> Info;
    };

    template <>
    struct ElementInfo<DirectedRelationship> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "DirectedRelationship"; }
        static SetList sets(DirectedRelationship& el) {
            return SetList{
                makeSetPair("targets", el.getTargets()),
                makeSetPair("sources", el.getSources())
            };
        }
    };
}

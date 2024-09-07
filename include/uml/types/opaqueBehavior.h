#pragma once

#include "behavior.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        friend struct ElementInfo<OpaqueBehavior>;

        protected:
            OrderedSet<LiteralString, OpaqueBehavior> m_bodies = OrderedSet<LiteralString, OpaqueBehavior>(this);
            OpaqueBehavior(std::size_t elementType, AbstractManager& manager);
        public:
            OrderedSet<LiteralString, OpaqueBehavior>& getBodies();
            typedef TypeInfo<std::tuple<Behavior>, OpaqueBehavior> Info;
    };

    template <>
    struct ElementInfo<OpaqueBehavior> {
        static const bool abstract = false;
        inline static const std::string name {"OpaqueBehavior"};
        static SetList sets(OpaqueBehavior& el) {
            return SetList {
                makeSetPair("bodies", el.m_bodies)
            };
        }
    };
}

#ifndef JOINNODE_H
#define JOINNODE_H

#include "activityNode.h"

namespace UML {
    class JoinNode : public ActivityNode {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::JOIN_NODE;
            };
    };
}

#endif
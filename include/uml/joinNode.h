#ifndef JOINNODE_H
#define JOINNODE_H

#include "activityNode.h"

namespace UML {
    class JoinNode : public ActivityNode {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
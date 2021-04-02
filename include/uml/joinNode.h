#ifndef JOINNODE_H
#define JOINNODE_H

#include "activityNode.h"

namespace UML {
    class JoinNode : public ActivityNode {
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
#ifndef FORKNODE_H
#define FORKNODE_H

#include "activityNode.h"

namespace UML {
    class ForkNode : public ActivityNode {
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
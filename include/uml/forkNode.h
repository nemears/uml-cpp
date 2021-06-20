#ifndef FORKNODE_H
#define FORKNODE_H

#include "activityNode.h"

namespace UML {
    class ForkNode : public ActivityNode {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FORK_NODE;
            };
    };
}

#endif
#ifndef INITIALNODE_H
#define INITIALNODE_H

#include "activityNode.h"

namespace UML {
    class InitialNode : public ActivityNode {
        ElementType getElementType() override;
        bool isSubClassOf(ElementType eType) override;
    };
}

#endif
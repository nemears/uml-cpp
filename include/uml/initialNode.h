#ifndef INITIALNODE_H
#define INITIALNODE_H

#include "activityNode.h"

namespace UML {
    class InitialNode : public ActivityNode {
        ElementType getElementType() const override;
        bool isSubClassOf(ElementType eType) override;
    };
}

#endif
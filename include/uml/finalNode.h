#ifndef FINALNODE_H
#define FINALNODE_H

#include "activityNode.h"

namespace UML {
    class FinalNode : public ActivityNode {
        ElementType getElementType() const override;
        bool isSubClassOf(ElementType eType) override;
    };
}

#endif
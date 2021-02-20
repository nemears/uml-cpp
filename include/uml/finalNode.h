#ifndef FINALNODE_H
#define FINALNODE_H

#include "activityNode.h"

namespace UML {
    class FinalNode : public ActivityNode {
        ElementType getElementType() override;
    };
}

#endif
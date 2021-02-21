#ifndef DECISIONNODE_H
#define DECISIONNODE_H

#include "activityNode.h"

namespace UML {
    class DecisionNode : public ActivityNode {
        public:
            ElementType getElementType() override;
    };
}

#endif
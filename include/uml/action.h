#ifndef ACTION_H
#define ACTION_H

#include "activityNode.h"

namespace UML {
    class Action : public ActivityNode {
        public:
            ElementType getElementType() override;
    };
}

#endif
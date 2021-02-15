#ifndef OBJECTFLOW_H
#define OBJECTFLOW_H

#include "activityEdge.h"

namespace UML {
    class ObjectFlow : public ActivityEdge {
        public:
            ElementType getElementType() override;
    };
}

#endif
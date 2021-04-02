#ifndef CONTROLFLOW_H
#define CONTROLFLOW_H

#include "activityEdge.h"

namespace UML {
    class ControlFlow : public ActivityEdge {
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
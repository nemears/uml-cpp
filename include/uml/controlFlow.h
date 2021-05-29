#ifndef CONTROLFLOW_H
#define CONTROLFLOW_H

#include "activityEdge.h"

namespace UML {
    class ControlFlow : public ActivityEdge {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::CONTROL_FLOW;
            };
    };
}

#endif
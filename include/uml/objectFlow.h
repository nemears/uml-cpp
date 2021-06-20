#ifndef OBJECTFLOW_H
#define OBJECTFLOW_H

#include "activityEdge.h"

namespace UML {
    class ObjectFlow : public ActivityEdge {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_FLOW;
            };
    };
}

#endif
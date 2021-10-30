#ifndef OBJECTFLOW_H
#define OBJECTFLOW_H

#include "activityEdge.h"

namespace UML {
    class ObjectFlow : public ActivityEdge {

        friend class UmlManager;

        protected:
            ObjectFlow();
        public:
            ObjectFlow(const ObjectFlow& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_FLOW;
            };
    };
}

#endif
#ifndef CONTROLFLOW_H
#define CONTROLFLOW_H

#include "activityEdge.h"

namespace UML {
    class ControlFlow : public ActivityEdge {
        
        friend class UmlManager;

        protected:
            ControlFlow();
        public:
            ControlFlow(const ControlFlow& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONTROL_FLOW;
            };
    };
}

#endif
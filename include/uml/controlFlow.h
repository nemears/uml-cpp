#ifndef _UML_CONTROL_FLOW_H_
#define _UML_CONTROL_FLOW_H_

#include "activityEdge.h"

namespace UML {
    class ControlFlow : public ActivityEdge {
        
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            ControlFlow();
        public:
            virtual ~ControlFlow();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONTROL_FLOW;
            };
    };
}

#endif
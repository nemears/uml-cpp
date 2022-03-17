#ifndef _UML_CONTROL_NODE_H_
#define _UML_CONTROL_NODE_H_

#include "activityNode.h"

namespace UML {
    class ControlNode : public ActivityNode {
        protected:
            ControlNode();
        public:
            ~ControlNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONTROL_NODE;
            }
    };
}

#endif
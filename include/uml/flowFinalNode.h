#ifndef _UML_FLOW_FINAL_NODE_H_
#define _UML_FLOW_FINAL_NODE_H_

#include "finalNode.h"

namespace UML {
    class FlowFinalNode : public FinalNode {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

        protected:
            FlowFinalNode();
        public:
            virtual ~FlowFinalNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FLOW_FINAL_NODE;
            };
    };
}

#endif
#ifndef DECISIONNODE_H
#define DECISIONNODE_H

#include "activityNode.h"
#include "objectFlow.h"

namespace UML {
    class DecisionNode : public ActivityNode {
        protected:
            ObjectFlow* decisionInputFlow;
            DecisionNode();
        public:
            DecisionNode(const DecisionNode& rhs);
            ObjectFlow* getDecisionInputFlow();
            void setDecisionInputFlow(ObjectFlow* inputFlow);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DECISION_NODE;
            };
    };
}

#endif
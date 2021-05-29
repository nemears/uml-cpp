#ifndef DECISIONNODE_H
#define DECISIONNODE_H

#include "activityNode.h"
#include "objectFlow.h"

namespace UML {
    class DecisionNode : public ActivityNode {
        protected:
            ObjectFlow* decisionInputFlow;
        public:
            ObjectFlow* getDecisionInputFlow();
            void setDecisionInputFlow(ObjectFlow* inputFlow);
            ElementType getElementType() const override;
            DecisionNode() {
                decisionInputFlow = 0;
            };
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::DECISION_NODE;
            };
    };
}

#endif
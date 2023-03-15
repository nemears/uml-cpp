#ifndef _UML_DECISION_NODE_H_
#define _UML_DECISION_NODE_H_

#include "controlNode.h"
#include "objectFlow.h"
#include "behavior.h"

namespace UML {

    typedef UmlPtr<ObjectFlow> ObjectFlowPtr;
    typedef UmlPtr<Behavior> BehaviorPtr;

    class DecisionNode : public ControlNode {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

        protected:
            CustomSingleton<ObjectFlow, DecisionNode> m_decisionInputFlow = CustomSingleton<ObjectFlow, DecisionNode>(this);
            CustomSingleton<Behavior, DecisionNode> m_decisionInput = CustomSingleton<Behavior, DecisionNode>(this);
            TypedSet<ObjectFlow, DecisionNode>& getDecisionInputFlowSingleton();
            TypedSet<Behavior, DecisionNode>& getDecisionInputSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            DecisionNode();
        public:
            virtual ~DecisionNode();
            ObjectFlowPtr getDecisionInputFlow() const;
            void setDecisionInputFlow(ObjectFlow* inputFlow);
            void setDecisionInputFlow(ObjectFlow& inputFlow);
            void setDecisionInputFlow(ID id);
            BehaviorPtr getDecisionInput() const;
            void setDecisionInput(Behavior* input);
            void setDecisionInput(Behavior& input);
            void setDecisionInput(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DECISION_NODE;
            };
    };
}

#endif
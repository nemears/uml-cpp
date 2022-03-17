#ifndef _UML_DECISION_NODE_H_
#define _UML_DECISION_NODE_H_

#include "controlNode.h"
#include "objectFlow.h"
#include "behavior.h"

namespace UML {

    typedef UmlPtr<ObjectFlow> ObjectFlowPtr;
    typedef UmlPtr<Behavior> BehaviorPtr;

    class DecisionNode : public ControlNode {
        protected:
            Singleton<ObjectFlow, DecisionNode> m_decisionInputFlow = Singleton<ObjectFlow, DecisionNode>(this);
            Singleton<Behavior, DecisionNode> m_decisionInput = Singleton<Behavior, DecisionNode>(this);
            Set<ObjectFlow, DecisionNode>& getDecisionInputFlowSingleton();
            Set<Behavior, DecisionNode>& getDecisionInputSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
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
#ifndef _UML_JOIN_NODE_H_
#define _UML_JOIN_NODE_H_

#include "controlNode.h"

namespace UML {

    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class JoinNode : public ControlNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<ValueSpecification, JoinNode> m_joinSpec = CustomSingleton<ValueSpecification, JoinNode>(this);
            TypedSet<ValueSpecification, JoinNode>& getJoinSpecSingleton();
            void init();
            JoinNode();
        public:
            ~JoinNode();
            ValueSpecificationPtr getJoinSpec() const;
            void setJoinSpec(ValueSpecification* joinSpec);
            void setJoinSpec(ValueSpecification& joinSpec);
            void setJoinSpec(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::JOIN_NODE;
            };
    };
}

#endif
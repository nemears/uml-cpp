#ifndef JOINNODE_H
#define JOINNODE_H

#include "activityNode.h"

namespace UML {
    class JoinNode : public ActivityNode {

        friend class UmlManager;

        protected:
            JoinNode();
        public:
            JoinNode(const JoinNode& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::JOIN_NODE;
            };
    };
}

#endif
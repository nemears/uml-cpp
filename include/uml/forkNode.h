#ifndef FORKNODE_H
#define FORKNODE_H

#include "activityNode.h"

namespace UML {
    class ForkNode : public ActivityNode {

        friend class UmlManager;
        
        protected:
            ForkNode();
        public:
            ForkNode(const ForkNode& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FORK_NODE;
            };
    };
}

#endif
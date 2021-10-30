#ifndef INITIALNODE_H
#define INITIALNODE_H

#include "activityNode.h"

namespace UML {
    class InitialNode : public ActivityNode {

        friend class UmlManager;

        protected:
            InitialNode();
        public:
            InitialNode(const InitialNode& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INITIAL_NODE;
            };
    };
}

#endif
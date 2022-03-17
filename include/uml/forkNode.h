#ifndef _UML_FORK_NODE_H_
#define _UML_FORK_NODE_H_

#include "controlNode.h"

namespace UML {
    class ForkNode : public ControlNode {

        friend class UmlManager;
        
        protected:
            ForkNode();
        public:
            ~ForkNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FORK_NODE;
            };
    };
}

#endif
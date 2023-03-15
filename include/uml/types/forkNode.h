#ifndef _UML_FORK_NODE_H_
#define _UML_FORK_NODE_H_

#include "controlNode.h"

namespace UML {
    class ForkNode : public ControlNode {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        
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
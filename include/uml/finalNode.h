#ifndef _UML_FINAL_NODE_H_
#define _UML_FINAL_NODE_H_

#include "controlNode.h"

namespace UML {
    class FinalNode : public ControlNode {
        
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            FinalNode();
        public:
            ~FinalNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FINAL_NODE;
            };
    };
}

#endif
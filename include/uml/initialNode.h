#ifndef _UML_INITIAL_NODE_H_
#define _UML_INITIAL_NODE_H_

#include "controlNode.h"

namespace UML {
    class InitialNode : public ControlNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            InitialNode();
        public:
            ~InitialNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INITIAL_NODE;
            };
    };
}

#endif
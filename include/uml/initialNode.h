#ifndef _UML_INITIAL_NODE_H_
#define _UML_INITIAL_NODE_H_

#include "controlNode.h"

namespace UML {
    class InitialNode : public ControlNode {

        friend class UmlManager;

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
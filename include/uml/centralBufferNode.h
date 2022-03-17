#ifndef _UML_CENTRAL_BUFFER_NODE_H_
#define _UML_CENTRAL_BUFFER_NODE_H_

#include "objectNode.h"

namespace UML {
    class CentralBufferNode : public ObjectNode {

        friend class UmlManager;

        protected:
            CentralBufferNode();
        public:
            virtual ~CentralBufferNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CENTRAL_BUFFER_NODE;
            };
    };
}

#endif
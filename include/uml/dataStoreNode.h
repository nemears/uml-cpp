#ifndef _UML_DATA_STORE_NODE_H_
#define _UML_DATA_STORE_NODE_H_

#include "centralBufferNode.h"

namespace UML {
    class DataStoreNode : public CentralBufferNode {

        friend class UmlManager;

        protected:
            DataStoreNode();
        public:
            virtual ~DataStoreNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_STORE_NODE;
            };
    };
}

#endif
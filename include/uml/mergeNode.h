#ifndef _UML_MERGE_NODE_H_
#define _UML_MERGE_NODE_H_

#include "controlNode.h"

namespace UML {
    class MergeNode : public ControlNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            MergeNode();
        public:
            ~MergeNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MERGE_NODE;
            };
    };
}

#endif
#ifndef MERGENODE_H
#define MERGENODE_H

#include "activityNode.h"

namespace UML {
    class MergeNode : public ActivityNode {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::MERGE_NODE;
            };
    };
}

#endif
#ifndef MERGENODE_H
#define MERGENODE_H

#include "activityNode.h"

namespace UML {
    class MergeNode : public ActivityNode {
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
#ifndef MERGENODE_H
#define MERGENODE_H

#include "activityNode.h"

namespace UML {
    class MergeNode : public ActivityNode {

        friend class UmlManager;

        protected:
            MergeNode();
        public:
            MergeNode(const MergeNode& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MERGE_NODE;
            };
    };
}

#endif
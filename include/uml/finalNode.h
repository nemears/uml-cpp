#ifndef FINALNODE_H
#define FINALNODE_H

#include "activityNode.h"

namespace UML {
    class FinalNode : public ActivityNode {
        
        friend class UmlManager;

        protected:
            FinalNode();
        public:
            FinalNode(const FinalNode& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::FINAL_NODE;
            };
    };
}

#endif
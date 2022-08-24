#ifndef _UML_ACTIVITY_FINAL_NODE_H_
#define _UML_ACTIVITY_FINAL_NODE_H_

#include "finalNode.h"

namespace UML {
    class ActivityFinalNode : public FinalNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            ActivityFinalNode();
        public:
            virtual ~ActivityFinalNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_FINAL_NODE;
            };
    };
}

#endif
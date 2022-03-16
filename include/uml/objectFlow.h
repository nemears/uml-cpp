#ifndef _UML_OBJECT_FLOW_H_
#define _UML_OBJECT_FLOW_H_

#include "activityEdge.h"

namespace UML {
    class ObjectFlow : public ActivityEdge {

        friend class UmlManager;

        protected:
            ObjectFlow();
        public:
            virtual ~ObjectFlow();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_FLOW;
            };
    };
}

#endif
#ifndef _UML_EXECUTABLE_NODE_H_
#define _UML_EXECUTABLE_NODE_H_

#include "activityNode.h"

namespace UML {

    class ExceptionHandler;

    class ExecutableNode : public ActivityNode {
        protected:
            CustomSet<ExceptionHandler, ExecutableNode> m_handlers = CustomSet<ExceptionHandler, ExecutableNode>(this);
            ExecutableNode();
        public:
            virtual ~ExecutableNode();
            Set<ExceptionHandler, ExecutableNode>& getHandlers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXECUTABLE_NODE;
            };
    };
}

#endif
#ifndef _UML_EXTENSION_HANDLER_H_
#define _UML_EXTENSION_HANDLER_H_

#include "element.h"
#include "set/singleton.h"

namespace UML {

    class ExecutableNode;
    class ObjectNode;
    class Classifier;

    typedef UmlPtr<ExecutableNode> ExecutableNodePtr;
    typedef UmlPtr<ObjectNode> ObjectNodePtr;

    class ExceptionHandler : public Element {

        friend class ExecutableNode;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<ExecutableNode, ExceptionHandler> m_protectedNode = CustomSingleton<ExecutableNode, ExceptionHandler>(this);
            CustomSingleton<ExecutableNode, ExceptionHandler> m_handlerBody = CustomSingleton<ExecutableNode, ExceptionHandler>(this);
            CustomSingleton<ObjectNode, ExceptionHandler> m_exceptionInput = CustomSingleton<ObjectNode, ExceptionHandler>(this);
            CustomSet<Classifier, ExceptionHandler> m_exceptionTypes = CustomSet<Classifier, ExceptionHandler>(this);
            TypedSet<ExecutableNode, ExceptionHandler>& getProtectedNodeSingleton();
            TypedSet<ExecutableNode, ExceptionHandler>& getHandlerBodySingleton();
            TypedSet<ObjectNode, ExceptionHandler>& getExceptionInputSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            ExceptionHandler();
        public:
            virtual ~ExceptionHandler();
            ExecutableNodePtr getProtectedNode() const;
            void setProtectedNode(ExecutableNode* protectedNode);
            void setProtectedNode(ExecutableNode& protectedNode);
            void setProtectedNode(ExecutableNodePtr protectedNode);
            void setProtectedNode(ID id);
            ExecutableNodePtr getHandlerBody() const;
            void setHandlerBody(ExecutableNode* handlerBody);
            void setHandlerBody(ExecutableNode& handlerBody);
            void setHandlerBody(ExecutableNodePtr handlerBody);
            void setHandlerBody(ID id);
            ObjectNodePtr getExceptionInput() const;
            void setExceptionInput(ObjectNode* exceptionInput);
            void setExceptionInput(ObjectNode& exceptionInput);
            void setExceptionInput(ObjectNodePtr exceptionInput);
            void setExceptionInput(ID id);
            Set<Classifier, ExceptionHandler>& getExceptionTypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXCEPTION_HANDLER;
            }
    };
}

#endif
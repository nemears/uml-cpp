#ifndef _UML_EXTENSION_HANDLER_H_
#define _UML_EXTENSION_HANDLER_H_

#include "element.h"
#include "singleton.h"

namespace UML {

    class ExecutableNode;
    class ObjectNode;
    class Classifier;

    typedef UmlPtr<ExecutableNode> ExecutableNodePtr;
    typedef UmlPtr<ObjectNode> ObjectNodePtr;

    class ExceptionHandler : public Element {

        friend class ExecutableNode;
        friend class UmlManager;

        protected:
            Singleton<ExecutableNode, ExceptionHandler> m_protectedNode = Singleton<ExecutableNode, ExceptionHandler>(this);
            Singleton<ExecutableNode, ExceptionHandler> m_handlerBody = Singleton<ExecutableNode, ExceptionHandler>(this);
            Singleton<ObjectNode, ExceptionHandler> m_exceptionInput = Singleton<ObjectNode, ExceptionHandler>(this);
            Set<Classifier, ExceptionHandler> m_exceptionTypes = Set<Classifier, ExceptionHandler>(this);
            Set<ExecutableNode, ExceptionHandler>& getProtectedNodeSingleton();
            Set<ExecutableNode, ExceptionHandler>& getHandlerBodySingleton();
            Set<ObjectNode, ExceptionHandler>& getExceptionInputSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void reindexName(ID id, std::string newName) override;
            void init();
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
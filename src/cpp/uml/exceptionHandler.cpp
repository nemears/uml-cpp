#include "uml/exceptionHandler.h"
#include "uml/executableNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/objectNode.h"
#include "uml/umlPtr.h"
#include "uml/namespace.h"
#include "uml/dependency.h"
#include "uml/behavior.h"
#include "uml/generalization.h"
#include "uml/property.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/deployment.h"

using namespace UML;

TypedSet<ExecutableNode, ExceptionHandler>& ExceptionHandler::getProtectedNodeSingleton() {
    return m_protectedNode;
}

TypedSet<ExecutableNode, ExceptionHandler>& ExceptionHandler::getHandlerBodySingleton() {
    return m_handlerBody;
}

TypedSet<ObjectNode, ExceptionHandler>& ExceptionHandler::getExceptionInputSingleton() {
    return m_exceptionInput;
}

void ExceptionHandler::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_handlerBody.reindex(newID);
    m_exceptionInput.reindex(newID);
    m_exceptionTypes.reindex(newID);
}

void ExceptionHandler::referenceErased(ID id) {
    Element::referenceErased(id);
    m_handlerBody.eraseElement(id);
    m_exceptionInput.eraseElement(id);
    m_exceptionTypes.eraseElement(id);
}

ExceptionHandler::ExceptionHandler() : Element(ElementType::EXCEPTION_HANDLER) {
    m_protectedNode.subsets(*m_owner);
    m_protectedNode.opposite(&ExecutableNode::getHandlers);
}

ExceptionHandler::~ExceptionHandler() {
    
}

ExecutableNodePtr ExceptionHandler::getProtectedNode() const {
    return m_protectedNode.get();
}

void ExceptionHandler::setProtectedNode(ExecutableNode* protectedNode) {
    m_protectedNode.set(protectedNode);
}

void ExceptionHandler::setProtectedNode(ExecutableNode& protectedNode) {
    m_protectedNode.set(protectedNode);
}

void ExceptionHandler::setProtectedNode(ExecutableNodePtr protectedNode) {
    m_protectedNode.set(protectedNode);
}

void ExceptionHandler::setProtectedNode(ID id) {
    m_protectedNode.set(id);
}

ExecutableNodePtr ExceptionHandler::getHandlerBody() const {
    return m_handlerBody.get();
}

void ExceptionHandler::setHandlerBody(ExecutableNode* handlerBody) {
    m_handlerBody.set(handlerBody);
}

void ExceptionHandler::setHandlerBody(ExecutableNode& handlerBody) {
    m_handlerBody.set(handlerBody);
}

void ExceptionHandler::setHandlerBody(ExecutableNodePtr handlerBody) {
    m_handlerBody.set(handlerBody);
}

void ExceptionHandler::setHandlerBody(ID id) {
    m_handlerBody.set(id);
}

ObjectNodePtr ExceptionHandler::getExceptionInput() const {
    return m_exceptionInput.get();
}

void ExceptionHandler::setExceptionInput(ObjectNode* exceptionInput) {
    m_exceptionInput.set(exceptionInput);
}

void ExceptionHandler::setExceptionInput(ObjectNode& exceptionInput) {
    m_exceptionInput.set(exceptionInput);
}

void ExceptionHandler::setExceptionInput(ObjectNodePtr exceptionInput) {
    m_exceptionInput.set(exceptionInput);
}

void ExceptionHandler::setExceptionInput(ID id) {
    m_exceptionInput.set(id);
}

Set<Classifier, ExceptionHandler>& ExceptionHandler::getExceptionTypes() {
    return m_exceptionTypes;
}

bool ExceptionHandler::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXCEPTION_HANDLER;
    }

    return ret;
}
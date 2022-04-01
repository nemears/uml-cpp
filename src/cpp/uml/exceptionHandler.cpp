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
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<ExecutableNode, ExceptionHandler>& ExceptionHandler::getProtectedNodeSingleton() {
    return m_protectedNode;
}

Set<ExecutableNode, ExceptionHandler>& ExceptionHandler::getHandlerBodySingleton() {
    return m_handlerBody;
}

Set<ObjectNode, ExceptionHandler>& ExceptionHandler::getExceptionInputSingleton() {
    return m_exceptionInput;
}

void ExceptionHandler::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_handlerBody.release(id);
    m_exceptionInput.release(id);
    m_exceptionTypes.release(id);
}

void ExceptionHandler::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_handlerBody.reindex(oldID, newID);
    m_exceptionInput.reindex(oldID, newID);
    m_exceptionTypes.reindex(oldID, newID);
}

void ExceptionHandler::referenceErased(ID id) {
    Element::referenceErased(id);
    m_handlerBody.eraseElement(id);
    m_exceptionInput.eraseElement(id);
    m_exceptionTypes.eraseElement(id);
}

void  ExceptionHandler::reindexName(ID id, std::string newName) {
    Element::reindexName(id, newName);
    m_handlerBody.reindexName(id, newName);
    m_exceptionInput.reindexName(id, newName);
    m_exceptionTypes.reindexName(id, newName);
}

void ExceptionHandler::init() {
    m_protectedNode.subsets(*m_owner);
    m_protectedNode.opposite(&ExecutableNode::getHandlers);
    m_protectedNode.m_signature = &ExceptionHandler::getProtectedNodeSingleton;
    m_handlerBody.m_signature = &ExceptionHandler::getHandlerBodySingleton;
    m_handlerBody.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_handlerBody.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_exceptionInput.m_signature = &ExceptionHandler::getExceptionInputSingleton;
    m_exceptionInput.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_exceptionInput.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_exceptionTypes.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_exceptionTypes.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_exceptionTypes.m_signature = &ExceptionHandler::getExceptionTypes;
}

ExceptionHandler::ExceptionHandler() : Element(ElementType::EXCEPTION_HANDLER) {
    init();
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
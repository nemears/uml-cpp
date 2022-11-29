#include "uml/invocationAction.h"
#include "uml/exceptionHandler.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/objectNode.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Port, InvocationAction>& InvocationAction::getOnPortSingleton() {
    return m_onPort;
}

void InvocationAction::referenceReindexed(ID newID) {
    Action::referenceReindexed(newID);
    m_onPort.reindex(newID);
}

void InvocationAction::referenceErased(ID id) {
    Action::referenceErased(id);
    m_onPort.eraseElement(id);
}

void InvocationAction::init() {
    m_arguments.subsets(m_inputs);
}

InvocationAction::InvocationAction() : Element(ElementType::INVOCATION_ACTION) {
    init();
}

InvocationAction::~InvocationAction() {

}

OrderedSet<InputPin, InvocationAction>& InvocationAction::getArguments() {
    return m_arguments;
}

PortPtr InvocationAction::getOnPort() const {
    return m_onPort.get();
}

void InvocationAction::setOnPort(Port* port) {
    m_onPort.set(port);
}

void InvocationAction::setOnPort(Port& port) {
    m_onPort.set(port);
}
void InvocationAction::setOnPort(PortPtr port) {
    m_onPort.set(port);
}

void InvocationAction::setOnPort(ID id) {
    m_onPort.set(id);
}

bool InvocationAction::isSubClassOf(ElementType eType) const {
    bool ret = Action::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INVOCATION_ACTION;
    }

    return ret;
}
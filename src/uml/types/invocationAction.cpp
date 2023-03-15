#include "uml/types/invocationAction.h"
#include "uml/types/exceptionHandler.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/objectNode.h"
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

InvocationAction::InvocationAction() : Element(ElementType::INVOCATION_ACTION) {
    m_arguments.subsets(m_inputs);
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
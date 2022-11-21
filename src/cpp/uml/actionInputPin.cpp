#include "uml/actionInputPin.h"
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

TypedSet<Action, ActionInputPin>& ActionInputPin::getFromActionSingleton() {
    return m_fromAction;
}

void ActionInputPin::init() {
    m_fromAction.subsets(*m_ownedElements);
}

ActionInputPin::ActionInputPin() : Element(ElementType::ACTION_INPUT_PIN) {
    init();
}

ActionInputPin::~ActionInputPin() {

}

ActionPtr ActionInputPin::getFromAction() const {
    return m_fromAction.get();
}

void ActionInputPin::setFromAction(Action* action) {
    m_fromAction.set(action);
}

void ActionInputPin::setFromAction(Action& action) {
    m_fromAction.set(action);
}

void ActionInputPin::setFromAction(ActionPtr action) {
    m_fromAction.set(action);
}

void ActionInputPin::setFromAction(ID id) {
    m_fromAction.set(id);
}

bool ActionInputPin::isSubClassOf(ElementType eType) const {
    bool ret = InputPin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION_INPUT_PIN;
    }

    return ret;
}
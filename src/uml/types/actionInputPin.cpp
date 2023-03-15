#include "uml/types/actionInputPin.h"
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

TypedSet<Action, ActionInputPin>& ActionInputPin::getFromActionSingleton() {
    return m_fromAction;
}

ActionInputPin::ActionInputPin() : Element(ElementType::ACTION_INPUT_PIN) {
    m_fromAction.subsets(*m_ownedElements);
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
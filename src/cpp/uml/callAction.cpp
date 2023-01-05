#include "uml/callAction.h"
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

CallAction::CallAction() : Element(ElementType::CALL_ACTION) {
    m_results.subsets(m_outputs);
}

CallAction::~CallAction() {

}

OrderedSet<OutputPin, CallAction>& CallAction::getResults() {
    return m_results;
}

bool CallAction::isSynchronous() const {
    return m_isSynchronous;
}

void CallAction::setIsSynchronous(bool val) {
    m_isSynchronous = val;
}

bool CallAction::isSubClassOf(ElementType eType) const {
    bool ret = InvocationAction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CALL_ACTION;
    }

    return ret;
}
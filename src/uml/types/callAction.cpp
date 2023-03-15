#include "uml/types/callAction.h"
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
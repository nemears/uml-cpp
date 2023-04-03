#include "uml/types/action.h"
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

Action::Action() : Element(ElementType::ACTION) {
    m_inputs.subsets(*m_ownedElements);
    m_outputs.subsets(*m_ownedElements);
    m_localPreconditions.subsets(*m_ownedElements);
    m_localPostconditions.subsets(*m_ownedElements);
}

Action::~Action() {
    
}

bool Action::isLocallyReentrant() const {
    return m_isLocallyReentrant;
}

void Action::setIsLocallyReentrant(bool val) {
    m_isLocallyReentrant = val;
}

ReadOnlySet<InputPin, Action>& Action::getInputs() {
    return m_inputs;
}

ReadOnlySet<OutputPin, Action>& Action::getOutputs() {
    return m_outputs;
}

Set<Constraint, Action>& Action::getLocalPreconditions() {
    return m_localPreconditions;
}

Set<Constraint, Action>& Action::getLocalPostconditions() {
    return m_localPostconditions;
}

bool Action::isSubClassOf(ElementType eType) const {
    bool ret = ExecutableNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION;
    }

    return ret;
}
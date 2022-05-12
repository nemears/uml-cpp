#include "uml/action.h"
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

void Action::init() {
    m_inputs.subsets(*m_ownedElements);
    m_inputs.setReadOnly(true);
    m_outputs.subsets(*m_ownedElements);
    m_outputs.setReadOnly(true);
    m_localPreconditions.subsets(*m_ownedElements);
    m_localPostconditions.subsets(*m_ownedElements);
}

Action::Action() : Element(ElementType::ACTION) {
    init();
}

Action::~Action() {
    
}

bool Action::isLocallyReentrant() const {
    return m_isLocallyReentrant;
}

void Action::setIsLocallyReentrant(bool val) {
    m_isLocallyReentrant = val;
}

Set<InputPin, Action>& Action::getInputs() {
    return m_inputs;
}

Set<OutputPin, Action>& Action::getOutputs() {
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
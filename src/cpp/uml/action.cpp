#include "uml/action.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Action::init() {
    m_inputs.subsets(*m_ownedElements);
    m_inputs.m_signature = &Action::getInputs;
    m_outputs.subsets(*m_ownedElements);
    m_outputs.m_signature = &Action::getOutputs;
}

Action::Action() : Element(ElementType::ACTION) {
    init();
}

Action::~Action() {
    
}

Set<InputPin, Action>& Action::getInputs() {
    return m_inputs;
}

Set<OutputPin, Action>& Action::getOutputs() {
    return m_outputs;
}

bool Action::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION;
    }

    return ret;
}
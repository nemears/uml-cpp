#include "uml/action.h"

using namespace UML;

void Action::AddInputFunctor::operator()(InputPin& el) const {
    if (el.getOwner() != m_el) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Action::AddOutputFunctor::operator()(OutputPin& el) const {
    if (el.getOwner() != m_el) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

Action::Action() {
    m_inputs.addProcedures.push_back(new AddInputFunctor(this));
    m_outputs.addProcedures.push_back(new AddOutputFunctor(this));
}

Action::~Action() {
    
}

Sequence<InputPin>& Action::getInputs() {
    return m_inputs;
}

Sequence<OutputPin>& Action::getOutputs() {
    return m_outputs;
}

ElementType Action::getElementType() const {
    return ElementType::ACTION;
}

bool Action::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION;
    }

    return ret;
}
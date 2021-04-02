#include "uml/action.h"

using namespace UML;

void Action::AddPinFunctor::operator()(Element& el) const {
    if (el.getOwner() != m_el) {
        m_el->getOwnedElements().add(el);
    }
}

Action::Action() {
    m_inputs = new Sequence<InputPin>;
    m_inputs->addProcedures.push_back(new AddPinFunctor(this));
    m_outputs = new Sequence<OutputPin>;
    m_outputs->addProcedures.push_back(new AddPinFunctor(this));
}

Action::~Action() {
    delete m_inputs;
    delete m_outputs;
}

Sequence<InputPin>& Action::getInputs() {
    return * m_inputs;
}

Sequence<OutputPin>& Action::getOutputs() {
    return *m_outputs;
}

ElementType Action::getElementType() {
    return ElementType::ACTION;
}

bool Action::isSubClassOf(ElementType eType) {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION;
    }

    return ret;
}
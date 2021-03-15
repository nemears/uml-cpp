#include "uml/action.h"

using namespace UML;

Action::Action() {
    m_inputs = new Sequence<InputPin>;
    m_outputs = new Sequence<OutputPin>;
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
#include "uml/action.h"
#include "uml/inputPin.h"
#include "uml/outputPin.h"

using namespace UML;

void Action::AddInputFunctor::operator()(InputPin& el) const {
    if (el.getOwner() != m_el) {
        m_el->getOwnedElements().internalAdd(el);
    }
    updateCopiedSequenceAddedTo(el, &Action::getInputs);
}

void Action::RemoveInputFunctor::operator()(InputPin& el) const {
    if (el.getOwner() == m_el) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Action::getInputs);
}

void Action::AddOutputFunctor::operator()(OutputPin& el) const {
    if (el.getOwner() != m_el) {
        m_el->getOwnedElements().internalAdd(el);
    }
    updateCopiedSequenceAddedTo(el, &Action::getOutputs);
}

void Action::RemoveOutputFunctor::operator()(OutputPin& el) const {
    if (*el.getOwner() == *m_el) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Action::getOutputs);
}

void Action::referencingReleased(ID id) {
    ActivityNode::referencingReleased(id);
    m_inputs.elementReleased(id, &Action::getInputs);
    m_outputs.elementReleased(id, &Action::getOutputs);
}

void Action::referenceReindexed(ID oldID, ID newID) {
    ActivityNode::referenceReindexed(oldID, newID);
    if (m_inputs.count(oldID)) {
        m_inputs.reindex(oldID, newID, &Action::getInputs);
    }
    if (m_outputs.count(oldID)) {
        m_outputs.reindex(oldID, newID, &Action::getOutputs);
    }
}

Action::Action() : Element(ElementType::ACTION) {
    m_inputs.addProcedures.push_back(new AddInputFunctor(this));
    m_inputs.removeProcedures.push_back(new RemoveInputFunctor(this));
    m_outputs.addProcedures.push_back(new AddOutputFunctor(this));
    m_outputs.removeProcedures.push_back(new RemoveOutputFunctor(this));
}

Action::Action(const Action& rhs) : 
ActivityNode(rhs),
RedefinableElement(rhs),
NamedElement(rhs),
Element(rhs, ElementType::ACTION) {
    m_inputs = rhs.m_inputs;
    m_outputs = rhs.m_outputs;
    m_inputs.removeProcedures.clear();
    m_inputs.addProcedures.clear();
    m_outputs.addProcedures.clear();
    m_outputs.removeProcedures.clear();
    m_inputs.addProcedures.push_back(new AddInputFunctor(this));
    m_inputs.removeProcedures.push_back(new RemoveInputFunctor(this));
    m_outputs.addProcedures.push_back(new AddOutputFunctor(this));
    m_outputs.removeProcedures.push_back(new RemoveOutputFunctor(this));
}

Action::~Action() {
    
}

Sequence<InputPin>& Action::getInputs() {
    return m_inputs;
}

Sequence<OutputPin>& Action::getOutputs() {
    return m_outputs;
}

bool Action::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTION;
    }

    return ret;
}
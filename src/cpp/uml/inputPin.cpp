#include "uml/inputPin.h"
#include "uml/sequence.h"

using namespace UML;

void InputPin::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_owner) {
        if(m_owner->isSubClassOf(ElementType::ACTION)) {
            dynamic_cast<Action*>(m_owner)->getInputs().reindex(oldID, newID);
        }
    }

    ActivityNode::reindexID(oldID, newID);
}

void InputPin::reindexName(string oldName, string newName) {
    if (m_owner) {
        if (m_owner->isSubClassOf(ElementType::ACTION)) {
            dynamic_cast<Action*>(m_owner)->getInputs().reindex(m_id, oldName, newName);
        }
    }

    ActivityNode::reindexName(oldName, newName);
}

ElementType InputPin::getElementType() {
    return ElementType::INPUT_PIN;
}
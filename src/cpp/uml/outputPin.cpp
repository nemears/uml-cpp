#include "uml/outputPin.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

void OutputPin::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_owner) {
        if(m_owner->isSubClassOf(ElementType::ACTION)) {
            dynamic_cast<Action*>(m_owner)->getOutputs().reindex(oldID, newID);
        }
    }

    ActivityNode::reindexID(oldID, newID);
}

void OutputPin::reindexName(string oldName, string newName) {
    if (m_owner) {
        if (m_owner->isSubClassOf(ElementType::ACTION)) {
            dynamic_cast<Action*>(m_owner)->getOutputs().reindex(m_id, oldName, newName);
        }
    }

    ActivityNode::reindexName(oldName, newName);
}

ElementType OutputPin::getElementType() const {
    return ElementType::OUTPUT_PIN;
}

bool OutputPin::isSubClassOf(ElementType eType) {
    bool ret = Pin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OUTPUT_PIN;
    }

    return ret;
}
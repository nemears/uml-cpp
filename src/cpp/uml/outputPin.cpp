#include "uml/outputPin.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

void OutputPin::reindexID(ID oldID, ID newID) {
    if (!m_ownerID.isNull()) {
        if(getOwner()->isSubClassOf(ElementType::ACTION)) {
            dynamic_cast<Action*>(getOwner())->getOutputs().reindex(oldID, newID);
        }
    }

    ActivityNode::reindexID(oldID, newID);
}

// void OutputPin::reindexName(string oldName, string newName) {
//     if (!m_ownerID.isNull()) {
//         if (getOwner()->isSubClassOf(ElementType::ACTION)) {
//             dynamic_cast<Action*>(getOwner())->getOutputs().reindex(m_id, oldName, newName);
//         }
//     }

//     ActivityNode::reindexName(oldName, newName);
// }

ElementType OutputPin::getElementType() const {
    return ElementType::OUTPUT_PIN;
}

bool OutputPin::isSubClassOf(ElementType eType) const {
    bool ret = Pin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OUTPUT_PIN;
    }

    return ret;
}
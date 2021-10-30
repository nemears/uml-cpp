#include "uml/inputPin.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

// void InputPin::reindexName(string oldName, string newName) {
//     if (m_owner) {
//         if (m_owner->isSubClassOf(ElementType::ACTION)) {
//             dynamic_cast<Action*>(m_owner)->getInputs().reindex(m_id, oldName, newName);
//         }
//     }

//     ActivityNode::reindexName(oldName, newName);
// }

InputPin::InputPin() : Element(ElementType::INPUT_PIN) {}

InputPin::InputPin(const InputPin& rhs) : Element(rhs, ElementType::INPUT_PIN) {}

bool InputPin::isSubClassOf(ElementType eType) const {
    bool ret = Pin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INPUT_PIN;
    }

    return ret;
}
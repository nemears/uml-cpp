#include "uml/uml-stable.h"

using namespace UML;

// void ConnectableElement::restoreReferences() {
//     TypedElement::restoreReferences();
//     ParameterableElement::restoreReferences();
//     if (m_ends.contains(el->getID())) {
//         el->setReference(this);
//     }
//     if (el->isSubClassOf(ElementType::CONNECTOR_END)) {
//         ConnectorEnd& end = el->as<ConnectorEnd>();
//         if (end.getRole().id() == m_id && !m_ends.contains(end.getID())) {
//             m_ends.innerAdd(end);
//         }
//     }
// }

void ConnectableElement::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

ConnectableElement::ConnectableElement() : Element(ElementType::CONNECTABLE_ELEMENT) {
    // m_ends.m_readOnly = true;
    m_ends.opposite(&ConnectorEnd::getRoleSingleton);
}

ConnectableElement::~ConnectableElement() {
    
}

ReadOnlySet<ConnectorEnd, ConnectableElement>& ConnectableElement::getEnds() {
    return m_ends;
}

bool ConnectableElement::is(ElementType eType) const {
    bool ret = TypedElement::is(eType);

    if (!ret) {
        ret = ParameterableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CONNECTABLE_ELEMENT;
    }

    return ret;
}

#include "uml/namedElement.h"
#include "uml/sequence.h"

using namespace UML;

void NamedElement::setName(const string &name) {
    reindexName(m_name, name);
    m_name = name;
}

void NamedElement::reindexName(string oldName, string newName) {
    if (m_owner) {
        m_owner->getOwnedElements().reindex(m_id, oldName, newName);
    }
}

string NamedElement::getName() {
    return m_name;
}

ElementType NamedElement::getElementType() {
    return ElementType::NAMED_ELEMENT;
}
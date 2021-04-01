#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"

using namespace UML;

NamedElement::NamedElement() {
    m_namespace = 0;
}

void NamedElement::setName(const string &name) {
    reindexName(m_name, name);
    m_name = name;
}

void NamedElement::reindexName(string oldName, string newName) {
    if (m_owner) {
        m_owner->getOwnedElements().reindex(m_id, oldName, newName);
    }

    if (m_namespace) {
        m_namespace->getMembers().reindex(m_id, oldName, newName);
    }
}

void NamedElement::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_namespace) {
        m_namespace->getMembers().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

string NamedElement::getName() {
    return m_name;
}

Namespace* NamedElement::getNamespace() {
    return m_namespace;
}

void NamedElement::setNamespace(Namespace* nmspc) {

    // if in another namespace remove it
    if (m_namespace) {
        if (m_namespace != nmspc) {
            if (m_namespace->getMembers().count(m_id)) {
                m_namespace->getMembers().remove(*this);
            }
        }
    }

    // overwrite namespace
    m_namespace = nmspc;

    // add to members of namespace
    if (!m_namespace->getMembers().count(m_id)) {
        m_namespace->getMembers().add(*this);
    }
}

ElementType NamedElement::getElementType() {
    return ElementType::NAMED_ELEMENT;
}
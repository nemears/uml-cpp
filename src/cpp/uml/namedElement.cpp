#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/namedElementFunctors.h"

using namespace UML;
using namespace std;

void AddMemberNamespaceFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Namespace&>(el).getMembers().count(m_el->getID())) {
        dynamic_cast<Namespace&>(el).getMembers().add(*dynamic_cast<NamedElement*>(m_el));
    }
}

void RemoveMemberNamespaceFunctor::operator()(Element& el) const {
    if (dynamic_cast<Namespace&>(el).getMembers().count(m_el->getID())) {
        dynamic_cast<Namespace&>(el).getMembers().remove(*dynamic_cast<NamedElement*>(m_el));
    }
}

NamedElement::NamedElement() {
    m_namespace = 0;
    m_memberNamespace = new Sequence<Namespace>;
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
}

NamedElement::~NamedElement() {
    delete m_memberNamespace;
}

NamedElement::NamedElement(const NamedElement& el) : Element(el) {
    m_namespace = el.m_namespace;
    m_name = el.m_name;
    m_memberNamespace = new Sequence<Namespace>(*el.m_memberNamespace);
    m_memberNamespace->addProcedures.clear();
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.clear();
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
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
        m_namespace->getOwnedMembers().reindex(m_id, oldName, newName);
    }

    for (auto const& nmspc : *m_memberNamespace) {
        nmspc->getMembers().reindex(m_id, oldName, newName);
    }
}

void NamedElement::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_namespace) {
        m_namespace->getOwnedMembers().reindex(oldID, newID);
    }

    for (auto const& nmspc : *m_memberNamespace) {
        nmspc->getMembers().reindex(oldID, newID);
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
            if (m_namespace->getOwnedMembers().count(m_id)) {
                m_namespace->getOwnedMembers().remove(*this);
            }
        }
    }

    // overwrite namespace
    m_namespace = nmspc;

    // add to owned members of namespace
    if (m_namespace) {
        if (!m_namespace->getOwnedMembers().count(m_id)) {
            m_namespace->getOwnedMembers().add(*this);
        }
    }
}

Sequence<Namespace>& NamedElement::getMemberNamespace() {
    return *m_memberNamespace;
}

VisibilityKind NamedElement::getVisibility() {
    return m_visibility;
}

void NamedElement::setVisibility(VisibilityKind visibility) {
    m_visibility = visibility;
}

ElementType NamedElement::getElementType() const {
    return ElementType::NAMED_ELEMENT;
}

bool NamedElement::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}
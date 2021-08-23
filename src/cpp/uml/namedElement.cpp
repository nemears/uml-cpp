#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/namedElementFunctors.h"
#include "uml/classifier.h"
#include "uml/universalFunctions.h"

using namespace UML;
using namespace std;

void AddMemberNamespaceFunctor::operator()(Namespace& el) const {
    if (!el.getMembers().count(m_el->getID())) {
        el.getMembers().add(*m_el);
    }
}

void RemoveMemberNamespaceFunctor::operator()(Namespace& el) const {
    if (el.getMembers().count(m_el->getID())) {
        el.getMembers().remove(*m_el);
    }
}

NamedElement::NamedElement() {
    m_namespacePtr = 0;
    m_memberNamespace = new Sequence<Namespace>(this);
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
}

NamedElement::~NamedElement() {
    delete m_memberNamespace;
}

NamedElement::NamedElement(const NamedElement& el) : Element(el) {
    m_namespaceID = el.m_namespaceID;
    m_namespacePtr = el.m_namespacePtr;
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
    if (getOwner()) {
        getOwner()->getOwnedElements().reindex(m_id, oldName, newName);
    }

    if (getNamespace()) {
        getNamespace()->getOwnedMembers().reindex(m_id, oldName, newName);
    }

    for (auto & nmspc : *m_memberNamespace) {
        nmspc.getMembers().reindex(m_id, oldName, newName);
    }
}

void NamedElement::reindexID(ID oldID, ID newID) {
    if (!m_namespaceID.isNull()) {
        if (!m_namespacePtr) {
            m_namespacePtr = &m_manager->get<Namespace>(m_namespaceID);
        }
        m_namespacePtr->getOwnedMembers().reindex(oldID, newID);
    }

    for (auto& nmspc : *m_memberNamespace) {
        nmspc.getMembers().reindex(oldID, newID);
    }

    Element::reindexID(oldID, newID);
}

void NamedElement::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_memberNamespace->m_manager = manager;
}

string NamedElement::getName() {
    return m_name;
}

Namespace* NamedElement::getNamespace() {
    return universalGet<Namespace>(m_namespaceID, m_namespacePtr, m_manager);
}

void NamedElement::setNamespace(Namespace* nmspc) {
    if (!isSameOrNull(m_namespaceID, nmspc)) {
        if (m_manager) {
            m_manager->removeReference(m_id, m_namespaceID);
        }
        if (!m_namespacePtr) {
            m_namespacePtr = &m_manager->get<Namespace>(m_namespaceID);
        }
        if (m_namespacePtr->getMembers().count(m_id)) {
            m_namespacePtr->getMembers().internalRemove(*this);
        }
        m_namespacePtr = 0;
        m_namespaceID = ID::nullID();
    }

    if (nmspc) {
        m_namespaceID = nmspc->getID();
    } else {
        m_namespaceID = ID::nullID();
    }

    if (!m_manager) {
        m_namespacePtr = nmspc;
    }

    if (nmspc) {
        if (m_manager) {
            m_manager->setReference(m_id, m_namespaceID, this);
        }
        if (!nmspc->getOwnedMembers().count(m_id)) {
            nmspc->getOwnedMembers().add(*this);
        }
    }

    if (m_manager) {
        m_manager->updateCopiesSingleton<NamedElement>(this, m_namespaceID, &NamedElement::m_namespaceID, &NamedElement::m_namespacePtr);
    }
}

Sequence<Namespace>& NamedElement::getMemberNamespace() {
    return *m_memberNamespace;
}

VisibilityKind NamedElement::getVisibility() {
    return m_visibility;
}

void NamedElement::setVisibility(VisibilityKind visibility) {
    if (m_visibility != visibility) {
        if (visibility == VisibilityKind::PRIVATE) {
            vector<Classifier*> removeFromMe;
            for (auto& nmspc: getMemberNamespace()) {
                if (nmspc.isSubClassOf(ElementType::CLASSIFIER)) {
                    if (dynamic_cast<Classifier&>(nmspc).getInheritedMembers().count(m_id)) {
                        removeFromMe.push_back(dynamic_cast<Classifier*>(&nmspc));
                    }
                }
            }
            for (auto& clazz: removeFromMe) {
                clazz->getInheritedMembers().remove(*this);
            }
        }
    }
    m_visibility = visibility;
    if (m_visibility != visibility) {
        if (m_visibility != VisibilityKind::PRIVATE) {
            for (auto& nmspc: getMemberNamespace()) {
                if (nmspc.isSubClassOf(ElementType::CLASSIFIER)) {
                    if (!dynamic_cast<Classifier&>(nmspc).getInheritedMembers().count(m_id)) {
                        dynamic_cast<Classifier&>(nmspc).getInheritedMembers().add(*this);
                    }
                }
            }
        }
    }
}

ElementType NamedElement::getElementType() const {
    return ElementType::NAMED_ELEMENT;
}

bool NamedElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}

void NamedElement::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
    if (m_namespaceID == id) {
        if (!released->as<Namespace>().getOwnedMembers().count(m_id)) {
            released->as<Namespace>().getOwnedMembers().add(*this);
        }
    }
    if (m_memberNamespace->count(id)) {
        if (!released->as<Namespace>().getMembers().count(m_id)) {
            released->as<Namespace>().getMembers().add(*this);
            /** TODO: rethink implementation, this might aquire a released element, but there may be no way around that**/
            if (released->isSubClassOf(ElementType::CLASSIFIER) && m_visibility != VisibilityKind::PRIVATE) {
                for (auto& general : released->as<Classifier>().getGenerals()) {
                    if (general.getMembers().count(m_id)) {
                        released->as<Classifier>().getInheritedMembers().add(*this);
                    }
                }
            }
        }
    }
}

void NamedElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_memberNamespace->elementReleased(id);
    if (m_namespaceID == id) {
        m_namespacePtr = 0;
    }
}
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/namedElementFunctors.h"
#include "uml/classifier.h"

using namespace UML;
using namespace std;

void AddMemberNamespaceFunctor::operator()(Namespace& el) const {
    oppositeSequenceAdd(el, &Namespace::getMembers);
    updateCopiedSequenceAddedTo(el, &NamedElement::getMemberNamespace);
}

void RemoveMemberNamespaceFunctor::operator()(Namespace& el) const {
    oppositeSequenceRemove(el, &Namespace::getMembers);
    updateCopiedSequenceRemovedFrom(el, &NamedElement::getMemberNamespace);
}

void NamedElement::RemoveNamespaceProcedures::operator()(Namespace* el) const {
    if (el->getMembers().count(m_me->getID())) {
        el->getMembers().internalRemove(*m_me);
    }
}

void NamedElement::AddNamespaceProcedures::operator()(Namespace* el) const {
    if (!el->getOwnedMembers().count(m_me->getID())) {
        el->getOwnedMembers().add(*m_me);
    }
}

void NamedElement::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    if (m_namespace.id() == oldID) {
        m_namespace.reindex(oldID, newID);
    }
    if (m_memberNamespace->count(oldID)) {
        m_memberNamespace->reindex(oldID, newID, &NamedElement::getMemberNamespace);
    }
}

NamedElement::NamedElement() {
    m_memberNamespace = new Sequence<Namespace>(this);
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
    m_namespace.m_signature = &NamedElement::m_namespace;
    m_namespace.m_removeProcedures.push_back(new RemoveNamespaceProcedures(this));
    m_namespace.m_addProcedures.push_back(new AddNamespaceProcedures(this));
}

NamedElement::~NamedElement() {
    delete m_memberNamespace;
}

NamedElement::NamedElement(const NamedElement& el) : Element(el) {
    m_name = el.m_name;
    m_visibility = el.m_visibility;
    m_memberNamespace = new Sequence<Namespace>(*el.m_memberNamespace);
    m_memberNamespace->m_el = this;
    m_memberNamespace->addProcedures.clear();
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.clear();
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
    m_namespace = el.m_namespace;
    m_namespace.m_me = this;
    m_namespace.m_removeProcedures.clear();
    m_namespace.m_addProcedures.clear();
    m_namespace.m_removeProcedures.push_back(new RemoveNamespaceProcedures(this));
    m_namespace.m_addProcedures.push_back(new AddNamespaceProcedures(this));
}

void NamedElement::setName(const string &name) {
    reindexName(m_name, name);
    m_name = name;
    if (m_node->m_managerElementMemory != this) {
        m_node->m_managerElementMemory->as<NamedElement>().m_name = name;
    }
    for (auto& copy : m_node->m_copies) {
        if (copy != this) {
            copy->as<NamedElement>().m_name = name;
        }
    }
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

void NamedElement::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_memberNamespace->m_manager = manager;
}

string NamedElement::getName() {
    return m_name;
}

Namespace* NamedElement::getNamespace() {
    return m_namespace.get();
}

Namespace& NamedElement::getNamespaceRef() {
    return m_namespace.getRef();
}

bool NamedElement::hasNamespace() const {
    return m_namespace.has();
}

void NamedElement::setNamespace(Namespace* nmspc) {
    m_namespace.set(nmspc);
}

void NamedElement::setNamespace(Namespace& nmspc) {
    m_namespace.set(nmspc);
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

    if (m_node) {
        if (m_node->m_managerElementMemory != this) {
            m_node->m_managerElementMemory->as<NamedElement>().m_visibility = m_visibility;
        }
        for (auto& copy : m_node->m_copies) {
            if (copy != this) {
                copy->as<NamedElement>().m_visibility = m_visibility;
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
    if (m_namespace.id() == id) {
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
    m_memberNamespace->elementReleased(id, &NamedElement::getMemberNamespace);
    if (m_namespace.id() == id) {
        m_namespace.release();
    }
}
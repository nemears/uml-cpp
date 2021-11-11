#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/namedElementFunctors.h"
#include "uml/classifier.h"
#include "uml/dependency.h"

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
    if (m_me->getMemberNamespace().count(*el)) {
        m_me->getMemberNamespace().remove(*el);
    }
    if (m_me->getOwnerID() == el->getID()) {
        m_me->setOwner(0);
    }
    m_me->updateQualifiedName("");
}

void NamedElement::AddNamespaceProcedures::operator()(Namespace* el) const {
    if (!el->getOwnedMembers().count(m_me->getID())) {
        el->getOwnedMembers().add(*m_me);
    }
    if (!m_me->getMemberNamespace().count(*el)) {
        m_me->getMemberNamespace().add(*el);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
    m_me->updateQualifiedName(el->getQualifiedName());
}

void NamedElement::AddNamespaceProcedures::operator()(ID id) const {
    if (!m_me->getMemberNamespace().count(id)) {
        m_me->getMemberNamespace().addByID(id);
    }
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void AddClientDependencyFunctor::operator()(Dependency& el) const {
    if (!el.getClient().count(m_el->getID())) {
        el.getClient().add(*m_el);
    }
    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }
}

void AddClientDependencyFunctor::operator()(ID id) const {
    if (!m_el->getDirectedRelationships().count(id)) {
        m_el->getDirectedRelationships().addByID(id);
    }
}

void RemoveClientDependencyFunctor::operator()(Dependency& el) const {
    if (el.getClient().count(m_el->getID())) {
        el.getClient().remove(*m_el);
    }
    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(el);
    }
}

void AddSupplierDependencyFunctor::operator()(Dependency& el) const {
    if (!el.getSupplier().count(m_el->getID())) {
        el.getSupplier().add(*m_el);
    }
    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }
}

void AddSupplierDependencyFunctor::operator()(ID id) const {
    if (!m_el->getDirectedRelationships().count(id)) {
        m_el->getDirectedRelationships().addByID(id);
    }
}

void RemoveSupplierDependencyFunctor::operator()(Dependency& el) const {
    if (el.getSupplier().count(m_el->getID())) {
        el.getSupplier().remove(*m_el);
    }
}

void NamedElement::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_namespace.reindex(oldID, newID);
    m_memberNamespace->reindex(oldID, newID, &NamedElement::getMemberNamespace);
    m_clientDependencies->reindex(oldID, newID, &NamedElement::getClientDependencies);
    m_supplierDependencies->reindex(oldID, newID, &NamedElement::getSupplierDependencies);
}

void NamedElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_memberNamespace->elementReleased(id, &NamedElement::getMemberNamespace);
    m_namespace.release(id);
    m_clientDependencies->elementReleased(id, &NamedElement::getClientDependencies);
    m_supplierDependencies->elementReleased(id, &NamedElement::getSupplierDependencies);
}

void NamedElement::restoreReferences() {
    Element::restoreReferences();
    m_memberNamespace->restoreReferences();
    m_namespace.restoreReference();
    m_clientDependencies->restoreReferences();
    m_supplierDependencies->restoreReferences();
}

void NamedElement::referenceErased(ID id) {
    Element::referenceErased(id);
    m_memberNamespace->elementErased(id);
    m_namespace.elementErased(id);
    m_clientDependencies->elementErased(id);
    m_supplierDependencies->elementErased(id);
}

NamedElement::NamedElement() : Element(ElementType::NAMED_ELEMENT) {
    m_memberNamespace = new Sequence<Namespace>(this);
    m_memberNamespace->addProcedures.push_back(new AddMemberNamespaceFunctor(this));
    m_memberNamespace->removeProcedures.push_back(new RemoveMemberNamespaceFunctor(this));
    m_namespace.m_signature = &NamedElement::m_namespace;
    m_namespace.m_removeProcedures.push_back(new RemoveNamespaceProcedures(this));
    m_namespace.m_addProcedures.push_back(new AddNamespaceProcedures(this));
    m_clientDependencies = new Sequence<Dependency>(this);
    m_clientDependencies->addProcedures.push_back(new AddClientDependencyFunctor(this));
    m_clientDependencies->removeProcedures.push_back(new RemoveClientDependencyFunctor(this));
    m_supplierDependencies = new Sequence<Dependency>(this);
    m_supplierDependencies->addProcedures.push_back(new AddSupplierDependencyFunctor(this));
    m_supplierDependencies->removeProcedures.push_back(new RemoveSupplierDependencyFunctor(this));
}

NamedElement::~NamedElement() {
    delete m_memberNamespace;
    delete m_clientDependencies;
    delete m_supplierDependencies;
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
    m_clientDependencies = new Sequence<Dependency>(*el.m_clientDependencies);
    m_clientDependencies->m_el = this;
    m_clientDependencies->addProcedures.clear();
    m_clientDependencies->removeProcedures.clear();
    m_clientDependencies->addProcedures.push_back(new AddClientDependencyFunctor(this));
    m_clientDependencies->removeProcedures.push_back(new RemoveClientDependencyFunctor(this));
    m_supplierDependencies = new Sequence<Dependency>(*el.m_supplierDependencies);
    m_supplierDependencies->m_el = this;
    m_supplierDependencies->addProcedures.clear();
    m_supplierDependencies->removeProcedures.clear();
    m_supplierDependencies->addProcedures.push_back(new AddSupplierDependencyFunctor(this));
    m_supplierDependencies->removeProcedures.push_back(new RemoveSupplierDependencyFunctor(this));
}

void NamedElement::setName(const string &name) {
    reindexName(m_name, name);
    m_name = name;
    updateCopiesScalar(name, &NamedElement::m_name);
}

void NamedElement::reindexName(string oldName, string newName) {
    // if (getOwner()) {
    //     getOwner()->getOwnedElements().reindex(m_id, oldName, newName);
    // }

    if (getNamespace()) {
        getNamespace()->getOwnedMembers().reindex(m_id, oldName, newName);
    }

    for (auto & nmspc : *m_memberNamespace) {
        nmspc.getMembers().reindex(m_id, oldName, newName);
    }
}

string NamedElement::getName() {
    return m_name;
}

string NamedElement::getQualifiedName() {
    return (hasNamespace() ? m_absoluteNamespace + "::" : "") +  m_name;
}

void NamedElement::updateQualifiedName(string absoluteNamespace) {
    m_absoluteNamespace = absoluteNamespace;
    updateCopiesScalar(absoluteNamespace, &NamedElement::m_absoluteNamespace);
}

Namespace* NamedElement::getNamespace() {
    return m_namespace.get();
}

Namespace& NamedElement::getNamespaceRef() {
    return m_namespace.getRef();
}

ID NamedElement::getNamespaceID() const {
    return m_namespace.id();
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

Sequence<Dependency>& NamedElement::getClientDependencies() {
    return *m_clientDependencies;
}

Sequence<Dependency>& NamedElement::getSupplierDependencies() {
    return *m_supplierDependencies;
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

    updateCopiesScalar(visibility, &NamedElement::m_visibility);
}

bool NamedElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}
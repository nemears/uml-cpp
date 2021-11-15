#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/classifier.h"
#include "uml/dependency.h"

using namespace UML;
using namespace std;

void NamedElement::UpdateQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName(el.as<NamedElement>().getQualifiedName());
}

void NamedElement::RemoveQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName("");
}

void NamedElement::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_memberNamespace.reindex(oldID, newID);
    m_namespace.reindex(oldID, newID);
    m_clientDependencies.reindex(oldID, newID);
}

void NamedElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_memberNamespace.release(id);
    m_namespace.release(id);
    m_clientDependencies.release(id);
}

void NamedElement::restoreReferences() {
    Element::restoreReferences();
    // m_memberNamespace->restoreReferences();
    // m_namespace.restoreReference();
    // m_clientDependencies->restoreReferences();
    // m_supplierDependencies->restoreReferences();
}

void NamedElement::referenceErased(ID id) {
    Element::referenceErased(id);
    m_memberNamespace.eraseElement(id);
    m_clientDependencies.eraseElement(id);
}

Set<Namespace, NamedElement>& NamedElement::getNamespaceSingleton() {
    return m_namespace;
}

void NamedElement::init() {
    m_memberNamespace.opposite(&Namespace::getMembers);
    m_memberNamespace.m_signature = &NamedElement::getMemberNamespace;
    m_memberNamespace.m_readOnly = true;
    m_namespace.subsets(*m_owner);
    m_namespace.subsets(m_memberNamespace);
    m_namespace.opposite(&Namespace::getOwnedMembers);
    m_namespace.m_signature = &NamedElement::getNamespaceSingleton;
    m_namespace.m_readOnly = true;
    m_namespace.m_addFunctors.insert(new UpdateQualifiedNameFunctor(this));
    m_namespace.m_removeFunctors.insert(new RemoveQualifiedNameFunctor(this));
    m_clientDependencies.opposite(&Dependency::getClient);
    m_clientDependencies.m_signature = &NamedElement::getClientDependencies;
}

void NamedElement::copy(const NamedElement& rhs) {
    // TODO see if we can delete these in Set
    for (auto& func : m_namespace.m_addFunctors) {
        if (!m_owner->m_addFunctors.count(func)) {
            delete func;
        }
    }
    for (auto& func : m_namespace.m_removeFunctors) {
        if (!m_owner->m_removeFunctors.count(func)) {
            delete func;
        }
    }
    m_name = rhs.m_name;
    m_visibility = rhs.m_visibility;
    m_memberNamespace = Set<Namespace, NamedElement>(rhs.m_memberNamespace);
    m_memberNamespace.m_el = this;
    m_namespace = Singleton2<Namespace, NamedElement>(rhs.m_namespace);
    m_namespace.m_el = this;
    m_clientDependencies = Set<Dependency, NamedElement>(rhs.m_clientDependencies);
    m_clientDependencies.m_el = this;
}

NamedElement::NamedElement() : Element(ElementType::NAMED_ELEMENT) {
    init();
}

NamedElement::~NamedElement() {

}

NamedElement::NamedElement(const NamedElement& el) : Element(ElementType::NAMED_ELEMENT) {
    // abstract
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

    // if (getNamespace()) {
    //     getNamespace()->getOwnedMembers().reindex(m_id, oldName, newName);
    // }

    // for (auto & nmspc : *m_memberNamespace) {
    //     nmspc.getMembers().reindex(m_id, oldName, newName);
    // }
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

Set<Namespace, NamedElement>& NamedElement::getMemberNamespace() {
    return m_memberNamespace;
}

Set<Dependency, NamedElement>& NamedElement::getClientDependencies() {
    return m_clientDependencies;
}

VisibilityKind NamedElement::getVisibility() {
    return m_visibility;
}

void NamedElement::setVisibility(VisibilityKind visibility) {
    // if (m_visibility != visibility) {
    //     if (visibility == VisibilityKind::PRIVATE) {
    //         vector<Classifier*> removeFromMe;
    //         for (auto& nmspc: getMemberNamespace()) {
    //             if (nmspc.isSubClassOf(ElementType::CLASSIFIER)) {
    //                 if (dynamic_cast<Classifier&>(nmspc).getInheritedMembers().count(m_id)) {
    //                     removeFromMe.push_back(dynamic_cast<Classifier*>(&nmspc));
    //                 }
    //             }
    //         }
    //         for (auto& clazz: removeFromMe) {
    //             clazz->getInheritedMembers().remove(*this);
    //         }
    //     }
    // }
    m_visibility = visibility;
    // if (m_visibility != visibility) {
    //     if (m_visibility != VisibilityKind::PRIVATE) {
    //         for (auto& nmspc: getMemberNamespace()) {
    //             if (nmspc.isSubClassOf(ElementType::CLASSIFIER)) {
    //                 if (!dynamic_cast<Classifier&>(nmspc).getInheritedMembers().count(m_id)) {
    //                     dynamic_cast<Classifier&>(nmspc).getInheritedMembers().add(*this);
    //                 }
    //             }
    //         }
    //     }
    // }

    updateCopiesScalar(visibility, &NamedElement::m_visibility);
}

bool NamedElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}
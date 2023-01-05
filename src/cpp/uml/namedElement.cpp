#include "uml/namedElement.h"
#include "uml/umlPtr.h"
#include "uml/namespace.h"
#include "uml/dependency.h"
#include "uml/behavior.h"
#include "uml/generalization.h"
#include "uml/property.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void NamedElement::UpdateQualifiedNamePolicy::apply(Namespace& el, NamedElement& me) {
    me.updateQualifiedName(el.getQualifiedName());
}

void NamedElement::RemoveQualifiedNamePolicy::apply(Namespace& el, NamedElement& me) {
    (void*) &el;
    me.updateQualifiedName("");
}

void NamedElement::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_clientDependencies->reindex(newID);
}

void NamedElement::referenceErased(ID id) {
    Element::referenceErased(id);
    m_clientDependencies->eraseElement(id);
}

TypedSet<Namespace, NamedElement>& NamedElement::getNamespaceSingleton() {
    return *m_namespace;
}

NamedElement::NamedElement() : Element(ElementType::NAMED_ELEMENT) {
    m_namespace = new CustomSingleton<Namespace, NamedElement, UpdateQualifiedNamePolicy, RemoveQualifiedNamePolicy>(this);
    m_namespace->subsets(*m_owner);
    m_namespace->opposite(&Namespace::getOwnedMembers);
    m_namespace->m_readOnly = true;
    m_clientDependencies = new CustomSet<Dependency, NamedElement>(this);
    m_clientDependencies->opposite(&Dependency::getClients);
}

NamedElement::~NamedElement() {
    delete m_namespace;
    delete m_clientDependencies;
}

void NamedElement::setName(const std::string &name) {
    m_name = name;
}

std::string NamedElement::getName() {
    return m_name;
}

std::string NamedElement::getQualifiedName() {
    return (getNamespace().has() ? m_absoluteNamespace + "::" : "") +  m_name;
}

void NamedElement::updateQualifiedName(std::string absoluteNamespace) {
    m_absoluteNamespace = absoluteNamespace;
}

NamespacePtr NamedElement::getNamespace() const {
    return m_namespace->get();
}

Set<Dependency, NamedElement>& NamedElement::getClientDependencies() {
    return *m_clientDependencies;
}

VisibilityKind NamedElement::getVisibility() {
    return m_visibility;
}

void NamedElement::setVisibility(VisibilityKind visibility) {
    if (m_visibility != visibility) {
        if (visibility == VisibilityKind::PRIVATE) {
            std::vector<Classifier*> clazzs;
            for (auto& pair : m_node->m_references) {
                // find use as inherited member through references and remove
                if (pair.second.node->m_managerElementMemory && pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_managerElementMemory->as<Classifier>().m_inheritedMembers.contains(m_id)) {
                        clazzs.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
                    }
                }
            }
            for (auto& clazz : clazzs) {
                clazz->m_inheritedMembers.innerRemove(m_id);
            }
        }
    }
    m_visibility = visibility;
}

bool NamedElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}
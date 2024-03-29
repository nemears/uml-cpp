#include "uml/types/namedElement.h"
#include "uml/umlPtr.h"
#include "uml/types/namespace.h"
#include "uml/types/dependency.h"
#include "uml/types/behavior.h"
#include "uml/types/generalization.h"
#include "uml/types/property.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void NamedElement::UpdateQualifiedNamePolicy::apply(Namespace& el, NamedElement& me) {
    me.updateQualifiedName(el.getQualifiedName());
}

void NamedElement::RemoveQualifiedNamePolicy::apply(__attribute__((unused))Namespace& el, NamedElement& me) {
    me.updateQualifiedName("");
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
                if (pair.second.node->m_element && pair.second.node->m_element->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(m_id)) {
                        clazzs.push_back(&pair.second.node->m_element->as<Classifier>());
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

void NamedElement::setNamespace(ID id) {
    m_namespace->innerAdd(id);
}
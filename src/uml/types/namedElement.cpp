#include "uml/uml-stable.h"
#include <vector>

using namespace UML;

typedef UmlPtr<Classifier> ClassifierPtr;

void NamedElement::UpdateQualifiedNamePolicy::elementAdded(Namespace& el, NamedElement& me) {
    me.updateQualifiedName(el.getQualifiedName());
}

void NamedElement::UpdateQualifiedNamePolicy::elementRemoved(__attribute__((unused))Namespace& el, NamedElement& me) {
    me.updateQualifiedName("");
}

ReadOnlySingleton<Namespace, NamedElement, NamedElement::UpdateQualifiedNamePolicy>& NamedElement::getNamespaceSingleton() {
    return m_namespace;
}

NamedElement::NamedElement(std::size_t elementType, AbstractManager& manager) : Element(elementType, manager) {
    m_namespace.subsets(m_owner);
    m_namespace.opposite(&Namespace::getOwnedMembers);
    // m_clientDependencies.opposite(&Dependency::getClients);
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
    return m_namespace.get();
}

// Set<Dependency, NamedElement>& NamedElement::getClientDependencies() {
//     return m_clientDependencies;
// }

VisibilityKind NamedElement::getVisibility() {
    return m_visibility;
}

void NamedElement::setVisibility(VisibilityKind visibility) {
    // if (m_visibility != visibility) {
    //     if (visibility == VisibilityKind::PRIVATE) {
    //         std::vector<ClassifierPtr> clazzs;
    //         for (auto& pair : m_node->m_references) {
    //             // find use as inherited member through references and remove
    //             if (pair.second.node->m_element && pair.second.node->m_element->is(ElementType::CLASSIFIER)) {
    //                 if (pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(m_id)) {
    //                     clazzs.push_back(&pair.second.node->m_element->as<Classifier>());
    //                 }
    //             }
    //         }
    //         for (auto& clazz : clazzs) {
    //             clazz->m_inheritedMembers.innerRemove(ElementPtr(this));
    //         }
    //     }
    // }
    m_visibility = visibility;
}

void NamedElement::setNamespace(ID id) {
    m_namespace.nonOppositeAdd(m_manager.createPtr(id));
}

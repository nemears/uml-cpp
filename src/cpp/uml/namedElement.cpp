#include "uml/namedElement.h"
#include "uml/namespace.h"
#include "uml/uml-stable.h"

using namespace UML;

void NamedElement::UpdateQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName(el.as<NamedElement>().getQualifiedName());
}

void NamedElement::RemoveQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName("");
}

void NamedElement::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_clientDependencies.reindex(oldID, newID);
}

void NamedElement::reindexName(std::string oldName, std::string newName) {
    Element::reindexName(oldName, newName);
    m_clientDependencies.reindexName(oldName, newName);
}

void NamedElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_clientDependencies.release(id);
}

void NamedElement::referenceErased(ID id) {
    Element::referenceErased(id);
    m_clientDependencies.eraseElement(id);
}

Set<Namespace, NamedElement>& NamedElement::getNamespaceSingleton() {
    return m_namespace;
}

void NamedElement::init() {
    m_namespace.subsets(*m_owner);
    m_namespace.opposite(&Namespace::getOwnedMembers);
    m_namespace.m_signature = &NamedElement::getNamespaceSingleton;
    m_namespace.m_readOnly = true;
    m_namespace.m_addFunctors.insert(new UpdateQualifiedNameFunctor(this));
    m_namespace.m_removeFunctors.insert(new RemoveQualifiedNameFunctor(this));
    m_clientDependencies.opposite(&Dependency::getClient);
    m_clientDependencies.m_signature = &NamedElement::getClientDependencies;
}

void NamedElement::copy(const NamedElement& rhs) {
    m_name = rhs.m_name;
    m_visibility = rhs.m_visibility;
    m_namespace = rhs.m_namespace;
    m_namespace.m_el = this;
    m_clientDependencies = rhs.m_clientDependencies;
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

void NamedElement::setName(const std::string &name) {
    for (auto& pair : m_node->m_references) {
        if (!pair.second) {
            m_manager->get<>(pair.first);
        }
        pair.second->m_managerElementMemory->reindexName(m_name, name);
    }
    m_name = name;
    updateCopiesScalar(name, &NamedElement::m_name);
}

std::string NamedElement::getName() {
    return m_name;
}

std::string NamedElement::getQualifiedName() {
    return (hasNamespace() ? m_absoluteNamespace + "::" : "") +  m_name;
}

void NamedElement::updateQualifiedName(std::string absoluteNamespace) {
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

Set<Dependency, NamedElement>& NamedElement::getClientDependencies() {
    return m_clientDependencies;
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
                if (pair.second->m_managerElementMemory && pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second->m_managerElementMemory->as<Classifier>().m_inheritedMembers.contains(m_id)) {
                        clazzs.push_back(&pair.second->m_managerElementMemory->as<Classifier>());
                    }
                }
            }
            for (auto& clazz : clazzs) {
                clazz->m_inheritedMembers.nonOppositeRemove(m_id);
            }
        }
    }
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
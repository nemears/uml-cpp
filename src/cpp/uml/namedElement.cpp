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

void NamedElement::UpdateQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName(el.as<NamedElement>().getQualifiedName());
}

void NamedElement::RemoveQualifiedNameFunctor::operator()(Element& el) const {
    m_el.as<NamedElement>().updateQualifiedName("");
}

void NamedElement::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_clientDependencies->reindex(oldID, newID);
}

void NamedElement::reindexName(ID id, std::string newName) {
    Element::reindexName(id, newName);
    m_clientDependencies->reindexName(id, newName);
}

void NamedElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_clientDependencies->release(id);
}

void NamedElement::referenceErased(ID id) {
    Element::referenceErased(id);
    m_clientDependencies->eraseElement(id);
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
    m_clientDependencies = new Set<Dependency, NamedElement>(this);
    m_clientDependencies->opposite(&Dependency::getClients);
    m_clientDependencies->m_signature = &NamedElement::getClientDependencies;
}

NamedElement::NamedElement() : Element(ElementType::NAMED_ELEMENT) {
    init();
}

NamedElement::~NamedElement() {
    delete m_clientDependencies;
}

void NamedElement::setName(const std::string &name) {
    for (auto& pair : m_node->m_references) {
        if (!pair.second) {
            m_manager->get(pair.first);
        }
        if (pair.second) {  // TODO: don't really like this if statement
            pair.second->m_managerElementMemory->reindexName(m_id, name); 
        }
    }
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
}

bool NamedElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::NAMED_ELEMENT;
    }

    return ret;
}
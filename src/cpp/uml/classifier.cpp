#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/generalizationSet.h"
#include "uml/uml-stable.h"

using namespace UML;

void Classifier::AddGeneralizationFunctor::operator()(Element& el) const {
    if (el.as<Generalization>().hasGeneral() && !m_el.as<Classifier>().m_generals.contains(el.as<Generalization>().getGeneralID())) {
        m_el.as<Classifier>().m_generals.add(el.as<Generalization>().getGeneralRef());
    }
}

void Classifier::RemoveGeneralizationFunctor::operator()(Element& el) const {
    if (el.as<Generalization>().hasGeneral() && m_el.as<Classifier>().m_generals.contains(el.as<Generalization>().getGeneralID())) {
        m_el.as<Classifier>().m_generals.remove(el.as<Generalization>().getGeneralID());
    }
}

void Classifier::AddGeneralFunctor::operator()(Element& el) const {
    bool createGeneralization = true;
    for (auto& generalization : m_el.as<Classifier>().m_generalizations) {
        if (generalization.getGeneralID() == el.getID()) {
            createGeneralization = false;
            break;
        }
    }
    if (createGeneralization) {
        Generalization& newGeneralization = m_el.m_manager->create<Generalization>();
        newGeneralization.setGeneral(el.as<Classifier>());
        m_el.as<Classifier>().getGeneralizations().add(newGeneralization);
    }
    for (auto& mem : el.as<Classifier>().m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE) {
            m_el.as<Class>().m_inheritedMembers.nonOppositeAdd(mem);
        }
    }
    el.setReference(&m_el);
}

void Classifier::RemoveGeneralFunctor::operator()(Element& el) const {
    for (auto& mem : el.as<Classifier>().m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE && m_el.as<Classifier>().m_inheritedMembers.contains(mem.getID())) {
            m_el.as<Classifier>().m_inheritedMembers.nonOppositeRemove(mem.getID());
        }
    }
    el.removeReference(m_el.getID());
}

void Classifier::AddOwnedMemberFunctor::operator()(Element& el) const {
    if (el.as<NamedElement>().getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : m_el.m_node->m_references) {
            if (pair.second->m_managerElementMemory && pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && pair.second->m_managerElementMemory->as<Classifier>().m_generals.contains(m_el.getID())) {
                pair.second->m_managerElementMemory->as<Classifier>().getInheritedMembers().nonOppositeAdd(el.as<NamedElement>());
            }
        }
    }
}

void Classifier::RemoveOwnedMemberFunctor::operator()(Element& el) const {
    if (el.as<NamedElement>().getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : m_el.m_node->m_references) {
            if (pair.second->m_managerElementMemory && pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && pair.second->m_managerElementMemory->as<Classifier>().m_generals.contains(m_el.getID())) {
                pair.second->m_managerElementMemory->as<Classifier>().m_inheritedMembers.nonOppositeRemove(el.getID());
            }
        }
    }
}

void Classifier::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    m_generals.release(id);
    m_powerTypeExtent.release(id);
}

void Classifier::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID); // todo non super call meth
    m_generals.reindex(oldID, newID);
    m_powerTypeExtent.reindex(oldID, newID);
}

void Classifier::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    for (auto& generalization : m_generalizations) {
        // load through loop
    }
}

void Classifier::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Type::referenceErased(id);
    RedefinableElement::referenceErased(id);
    TemplateableElement::referenceErased(id);
    m_generals.eraseElement(id);
    m_powerTypeExtent.eraseElement(id);
}

void Classifier::init() {
    m_ownedMembers.m_addFunctors.insert(new AddOwnedMemberFunctor(this));
    m_ownedMembers.m_removeFunctors.insert(new RemoveOwnedMemberFunctor(this));
    m_features.subsets(m_members);
    m_features.opposite(&Feature::getFeaturingClassifierSingleton);
    m_features.m_signature = &Classifier::getFeatures;
    m_features.m_readOnly = true;
    m_attributes.subsets(m_features);
    m_attributes.m_signature = &Classifier::getAttributes;
    m_attributes.m_readOnly = true;
    m_generalizations.subsets(*m_ownedElements);
    m_generalizations.opposite(&Generalization::getSpecificSingleton);
    m_generalizations.m_signature = &Classifier::getGeneralizations;
    m_generalizations.m_addFunctors.insert(new AddGeneralizationFunctor(this));
    m_generalizations.m_removeFunctors.insert(new RemoveGeneralizationFunctor(this));
    m_generals.m_signature = &Classifier::getGenerals;
    m_generals.m_addFunctors.insert(new AddGeneralFunctor(this));
    m_generals.m_removeFunctors.insert(new RemoveGeneralFunctor(this));
    m_inheritedMembers.subsets(m_members);
    m_inheritedMembers.m_signature = &Classifier::getInheritedMembers;
    m_inheritedMembers.m_readOnly = true;
    m_powerTypeExtent.opposite(&GeneralizationSet::getPowerTypeSingleton);
    m_powerTypeExtent.m_signature = &Classifier::getPowerTypeExtent;
}

void Classifier::copy(const Classifier& rhs) {
    m_features = rhs.m_features;
    m_attributes = rhs.m_attributes;
    m_generalizations = rhs.m_generalizations;
    m_inheritedMembers = rhs.m_inheritedMembers;
    m_powerTypeExtent = rhs.m_powerTypeExtent;
}

Classifier::Classifier() : Element(ElementType::CLASSIFIER) {
    init();
}

Classifier::~Classifier() {

}

Classifier::Classifier(const Classifier& rhs) : Element(ElementType::CLASSIFIER) {
    // abstract
}

void Classifier::reindexName(std::string oldName, std::string newName) {
    Namespace::reindexName(oldName, newName);
}

std::string Classifier::getName() {
    return Namespace::getName();
}

void Classifier::setName(const std::string& name) {
    Namespace::setName(name);
}

Set<Feature, Classifier>& Classifier::getFeatures() {
    return m_features;
}

Set<Property, Classifier>& Classifier::getAttributes() {
    return m_attributes;
}

Set<Generalization, Classifier>& Classifier::getGeneralizations() {
    return m_generalizations;
}

Set<Classifier, Classifier>& Classifier::getGenerals() {
    return m_generals;
}

Set<NamedElement, Classifier>& Classifier::getInheritedMembers() {
    return m_inheritedMembers;
}

Set<GeneralizationSet, Classifier>& Classifier::getPowerTypeExtent() {
    return m_powerTypeExtent;
}

bool Classifier::isSubClassOf(ElementType eType) const {
    bool ret = Namespace::isSubClassOf(eType);

    if (!ret) {
        ret = Type::isSubClassOf(eType);
    }

    if (!ret) {
        ret = RedefinableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASSIFIER;
    }

    return ret;
}
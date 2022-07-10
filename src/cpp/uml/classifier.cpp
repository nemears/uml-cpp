#include "uml/classifier.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/umlPtr.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void Classifier::AddGeneralizationFunctor::operator()(Element& el) const {
    if (el.as<Generalization>().getGeneral() && !m_el.as<Classifier>().m_generals.contains(el.as<Generalization>().getGeneral().id())) {
        if (el.as<Generalization>().getGeneral().loaded()) {
            m_el.as<Classifier>().m_generals.add(*el.as<Generalization>().getGeneral());
        } else {
            m_el.as<Classifier>().m_generals.add(el.as<Generalization>().getGeneral().id());
        }
    }
}

void Classifier::RemoveGeneralizationFunctor::operator()(Element& el) const {
    if (el.as<Generalization>().getGeneral() && m_el.as<Classifier>().m_generals.contains(el.as<Generalization>().getGeneral().id())) {
        m_el.as<Classifier>().m_generals.remove(el.as<Generalization>().getGeneral().id());
    }
}

void Classifier::AddGeneralFunctor::operator()(Element& el) const {
    bool createGeneralization = true;
    for (auto& generalization : m_el.as<Classifier>().m_generalizations) {
        if (generalization.getGeneral().id() == el.getID()) {
            createGeneralization = false;
            break;
        }
    }
    if (createGeneralization) {
        Generalization& newGeneralization = *m_el.m_manager->create<Generalization>();
        newGeneralization.setGeneral(el.as<Classifier>());
        m_el.as<Classifier>().getGeneralizations().add(newGeneralization);
    }
    for (auto& mem : el.as<Classifier>().m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE) {
            m_el.as<Classifier>().m_inheritedMembers.nonOppositeAdd(mem);
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
            if (pair.second.node && 
                pair.second.node->m_managerElementMemory && 
                pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_managerElementMemory->as<Classifier>().m_generals.contains(m_el.getID())) {
                    pair.second.node->m_managerElementMemory->as<Classifier>().getInheritedMembers().nonOppositeAdd(el.as<NamedElement>());
            }
        }
    }
}

void Classifier::RemoveOwnedMemberFunctor::operator()(Element& el) const {
    if (el.as<NamedElement>().getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : m_el.m_node->m_references) {
            if (!pair.second.node && m_el.m_manager->loaded(pair.first)) {
                pair.second.node = m_el.m_manager->UmlManager::get(pair.first).m_node;
            }
            if (pair.second.node->m_managerElementMemory && 
                pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_managerElementMemory->as<Classifier>().m_generals.contains(m_el.getID())) {
                    pair.second.node->m_managerElementMemory->as<Classifier>().m_inheritedMembers.nonOppositeRemove(el.getID());
            }
        }
    }
}

Set<RedefinableTemplateSignature, Classifier>& Classifier::getOwnedTemplateSignatureSingleton() {
    return m_classifierOwnedTemplateSignature;
}

Set<ClassifierTemplateParameter, Classifier>& Classifier::getTemplateParameterSingleton() {
    return m_classifierTemplateParameter;
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

void Classifier::reindexName(ID id, std::string newName) {
    Namespace::reindexName(id, newName);
    PackageableElement::reindexName(id, newName);
    m_generals.reindexName(id, newName);
    m_powerTypeExtent.reindexName(id, newName);
}

void Classifier::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    for (auto& _generalization : m_generalizations) {
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
    m_features.m_readOnly = true;
    m_attributes.subsets(m_features);
    m_attributes.m_readOnly = true;
    m_generalizations.subsets(*m_ownedElements);
    m_generalizations.opposite(&Generalization::getSpecificSingleton);
    m_generalizations.m_addFunctors.insert(new AddGeneralizationFunctor(this));
    m_generalizations.m_removeFunctors.insert(new RemoveGeneralizationFunctor(this));
    m_generals.m_addFunctors.insert(new AddGeneralFunctor(this));
    m_generals.m_removeFunctors.insert(new RemoveGeneralFunctor(this));
    m_inheritedMembers.subsets(m_members);
    m_inheritedMembers.m_readOnly = true;
    m_powerTypeExtent.opposite(&GeneralizationSet::getPowerTypeSingleton);
    m_classifierOwnedTemplateSignature.redefines(m_ownedTemplateSignature);
    m_classifierOwnedTemplateSignature.opposite(&RedefinableTemplateSignature::getClassifierSingleton);
    m_classifierTemplateParameter.redefines(m_templateParameter);
    m_classifierTemplateParameter.opposite(&ClassifierTemplateParameter::getParameteredElementSingleton);
}

Classifier::Classifier() : Element(ElementType::CLASSIFIER) {
    init();
}

Classifier::~Classifier() {

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

RedefinableTemplateSignaturePtr Classifier::getOwnedTemplateSignature() const {
    return m_classifierOwnedTemplateSignature.get();
}

void Classifier::setOwnedTemplateSignature(RedefinableTemplateSignature* signature) {
    m_classifierOwnedTemplateSignature.set(signature);
}

void Classifier::setOwnedTemplateSignature(RedefinableTemplateSignature& signature) {
    m_classifierOwnedTemplateSignature.set(signature);
}

void Classifier::setOwnedTemplateSignature(RedefinableTemplateSignaturePtr signature) {
    m_classifierOwnedTemplateSignature.set(signature);
}

void Classifier::setOwnedTemplateSignature(ID id) {
    m_classifierOwnedTemplateSignature.set(id);
}

ClassifierTemplateParameterPtr Classifier::getTemplateParameter() const {
    return m_classifierTemplateParameter.get();
}

void Classifier::setTemplateParameter(ClassifierTemplateParameter* templateParameter) {
    m_classifierTemplateParameter.set(templateParameter);
}

void Classifier::setTemplateParameter(ClassifierTemplateParameter& templateParameter) {
    m_classifierTemplateParameter.set(templateParameter);
}

void Classifier::setTemplateParameter(ClassifierTemplateParameterPtr templateParameter) {
    m_classifierTemplateParameter.set(templateParameter);
}

void Classifier::setTemplateParameter(ID id) {
    m_classifierTemplateParameter.set(id);
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
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

void Classifier::AddGeneralizationPolicy::apply(Generalization& el, Classifier& me) {
    if (el.getGeneral() && !me.m_generals.contains(el.getGeneral().id())) {
        if (el.as<Generalization>().getGeneral().loaded()) {
            me.m_generals.add(el.getGeneral());
        } else {
            me.m_generals.add(el.getGeneral().id());
        }
    }
}

void Classifier::RemoveGeneralizationPolicy::apply(Generalization& el, Classifier& me) {
    if (el.getGeneral() && me.m_generals.contains(el.getGeneral().id())) {
        me.m_generals.remove(el.getGeneral().id());
    }
}

void Classifier::AddGeneralPolicy::apply(Classifier& el, Classifier& me) {
    bool createGeneralization = true;
    for (auto& generalization : me.m_generalizations) {
        if (generalization.getGeneral().id() == el.getID()) {
            createGeneralization = false;
            break;
        }
    }
    if (createGeneralization) {
        Generalization& newGeneralization = *me.m_manager->create<Generalization>();
        newGeneralization.setGeneral(el);
        me.getGeneralizations().add(newGeneralization);
    }
    for (auto& mem : el.m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE) {
            [[maybe_unused]] SetLock memLck = me.lockEl(mem);
            me.m_inheritedMembers.innerAdd(mem);
            me.m_node->setReference(mem);
            mem.m_node->setReference(me);
        }
    }
}

void Classifier::RemoveGeneralPolicy::apply(Classifier& el, Classifier& me) {
    for (NamedElement& mem : el.m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE && me.m_inheritedMembers.contains(mem)) {
            [[maybe_unused]] SetLock memLck = me.lockEl(mem);
            me.m_inheritedMembers.innerRemove(mem.getID());
            me.m_node->removeReference(mem);
            mem.m_node->removeReference(me);
        }
    }
}

void Classifier::AddOwnedMemberPolicy::apply(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : me.m_node->m_references) {
            if (pair.second.node && 
                pair.second.node->m_managerElementMemory && 
                pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_managerElementMemory->as<Classifier>().m_generals.contains(me.getID())) {
                    pair.second.node->m_managerElementMemory->as<Classifier>().m_inheritedMembers.innerAdd(el);
            }
        }
    }
}

void Classifier::RemoveOwnedMemberPolicy::apply(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : me.m_node->m_references) {
            if (!pair.second.node && me.m_manager->loaded(pair.first)) {
                pair.second.node = me.m_manager->get(pair.first).ptr()->m_node;
            }
            if (!pair.second.node) {
                continue;
            }
            if (pair.second.node->m_managerElementMemory && 
                pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_managerElementMemory->as<Classifier>().m_generals.contains(me.getID())) {
                    pair.second.node->m_managerElementMemory->as<Classifier>().m_inheritedMembers.innerRemove(el.getID());
            }
        }
    }
}

TypedSet<RedefinableTemplateSignature, Classifier>& Classifier::getOwnedTemplateSignatureSingleton() {
    return m_classifierOwnedTemplateSignature;
}

TypedSet<ClassifierTemplateParameter, Classifier>& Classifier::getTemplateParameterSingleton() {
    return m_classifierTemplateParameter;
}


void Classifier::referenceReindexed(ID newID) {
    Namespace::referenceReindexed(newID);
    PackageableElement::referenceReindexed(newID); // todo non super call meth
    m_generals.reindex(newID);
    m_powerTypeExtent.reindex(newID);
}

void Classifier::restoreReferences() {
    // Namespace::restoreReferences();
    // PackageableElement::restoreReferences();
    for ([[maybe_unused]] auto& _generalization : m_generalizations) {
        // load through loop
    }
}

void Classifier::restoreReference(Element* el) {
    Type::restoreReference(el);
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
    m_features.subsets(m_members);
    m_features.opposite(&Feature::getFeaturingClassifierSingleton);
    m_features.m_readOnly = true;
    m_attributes.subsets(m_features);
    m_attributes.m_readOnly = true;
    m_generalizations.subsets(*m_ownedElements);
    m_generalizations.opposite(&Generalization::getSpecificSingleton);
    m_inheritedMembers.subsets(m_members);
    m_inheritedMembers.m_readOnly = true;
    m_powerTypeExtent.opposite(&GeneralizationSet::getPowerTypeSingleton);
    m_classifierOwnedTemplateSignature.redefines(m_ownedTemplateSignature);
    m_classifierOwnedTemplateSignature.opposite(&RedefinableTemplateSignature::getClassifierSingleton);
    m_classifierTemplateParameter.redefines(m_templateParameter);
    m_classifierTemplateParameter.opposite(&ClassifierTemplateParameter::getParameteredElementSingleton);
    m_ownedMembers.redefines(m_classifierOwnedMembers);
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
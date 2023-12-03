#include "uml/types/classifier.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/umlPtr.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

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
        }
    }
}

void Classifier::RemoveGeneralPolicy::apply(Classifier& el, Classifier& me) {
    for (NamedElement& mem : el.m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE && me.m_inheritedMembers.contains(mem)) {
            [[maybe_unused]] SetLock memLck = me.lockEl(mem);
            me.m_inheritedMembers.innerRemove(mem.getID());
        }
    }
}

void Classifier::AddOwnedMemberPolicy::apply(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : me.m_node->m_references) {
            if (pair.second.node && 
                pair.second.node->m_element && 
                pair.second.node->m_element->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_element->as<Classifier>().m_generals.contains(me.getID()) &&
                !pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(el.getID())) {
                    pair.second.node->m_element->as<Classifier>().m_inheritedMembers.innerAdd(el);
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
            if (pair.second.node->m_element && 
                pair.second.node->m_element->isSubClassOf(ElementType::CLASSIFIER) && 
                pair.second.node->m_element->as<Classifier>().m_generals.contains(me.getID()) &&
                pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(me.getID())) {
                    pair.second.node->m_element->as<Classifier>().m_inheritedMembers.innerRemove(el.getID());
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

void Classifier::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    for (auto& generalization : m_generalizations) {
        if (generalization.getGeneral() && !m_generals.contains(generalization.getGeneral().id())) {
            m_generals.add(*generalization.getGeneral());
        }
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

Classifier::Classifier() : Element(ElementType::CLASSIFIER) {
    m_features.subsets(m_members);
    m_features.opposite(&Feature::getFeaturingClassifierSingleton);
    m_attributes.subsets(m_features);
    m_generalizations.subsets(*m_ownedElements);
    m_generalizations.opposite(&Generalization::getSpecificSingleton);
    m_inheritedMembers.subsets(m_members);
    m_powerTypeExtent.opposite(&GeneralizationSet::getPowerTypeSingleton);
    m_classifierOwnedTemplateSignature.redefines(m_ownedTemplateSignature);
    m_classifierOwnedTemplateSignature.opposite(&RedefinableTemplateSignature::getClassifierSingleton);
    m_classifierTemplateParameter.redefines(m_templateParameter);
    m_classifierTemplateParameter.opposite(&ClassifierTemplateParameter::getParameteredElementSingleton);
    m_classifierOwnedMembers.redefines(m_ownedMembers);
}

Classifier::~Classifier() {

}

std::string Classifier::getName() {
    return Namespace::getName();
}

void Classifier::setName(const std::string& name) {
    Namespace::setName(name);
}

ReadOnlySet<Feature, Classifier>& Classifier::getFeatures() {
    return m_features;
}

ReadOnlySet<Property, Classifier>& Classifier::getAttributes() {
    return m_attributes;
}

Set<Generalization, Classifier>& Classifier::getGeneralizations() {
    return m_generalizations;
}

Set<Classifier, Classifier>& Classifier::getGenerals() {
    return m_generals;
}

ReadOnlySet<NamedElement, Classifier>& Classifier::getInheritedMembers() {
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
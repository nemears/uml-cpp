#include "uml/set/indexableSet.h"
#include "uml/uml-stable.h"

using namespace UML;

void Classifier::GeneralizationPolicy::elementAdded(Generalization& el, Classifier& me) {
    if (el.getGeneral() && !me.m_generals.contains(el.getGeneral().id())) {
        if (el.as<Generalization>().getGeneral().loaded()) {
            me.m_generals.add(el.getGeneral());
        } else {
            me.m_generals.add(el.getGeneral());
        }
    }
}

void Classifier::GeneralizationPolicy::elementRemoved(Generalization& el, Classifier& me) {
    if (el.getGeneral() && me.m_generals.contains(el.getGeneral().id())) {
        me.m_generals.remove(el.getGeneral());
    }
}

void Classifier::GeneralPolicy::elementAdded(Classifier& el, Classifier& me) {
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
        if (mem.getVisibility() != VisibilityKind::PRIVATE && !me.m_inheritedMembers.contains(mem)) {
            me.m_inheritedMembers.innerAdd(ElementPtr(&mem));
        }
    }
}

void Classifier::GeneralPolicy::elementRemoved(Classifier& el, Classifier& me) {
    for (NamedElement& mem : el.m_members) {
        if (mem.getVisibility() != VisibilityKind::PRIVATE && me.m_inheritedMembers.contains(mem)) {
            me.m_inheritedMembers.innerRemove(ElementPtr(&mem));
        }
    }
}

void Classifier::OwnedMemberPolicy::elementAdded(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : me.m_node->m_references) {
            if (pair.second.node && 
                pair.second.node->m_element && 
                pair.second.node->m_element->is(ElementType::CLASSIFIER) && 
                pair.second.node->m_element->as<Classifier>().m_generals.contains(me.getID()) &&
                !pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(el.getID())) {
                    pair.second.node->m_element->as<Classifier>().m_inheritedMembers.innerAdd(ElementPtr(&el));
            }
        }
    }
}

void Classifier::OwnedMemberPolicy::elementRemoved(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& pair : me.m_node->m_references) {
            if (!pair.second.node && me.m_manager->loaded(pair.first)) {
                pair.second.node = me.m_manager->get(pair.first).ptr()->m_node;
            }
            if (!pair.second.node) {
                continue;
            }
            if (pair.second.node->m_element && 
                pair.second.node->m_element->is(ElementType::CLASSIFIER) && 
                pair.second.node->m_element->as<Classifier>().m_generals.contains(me.getID()) &&
                pair.second.node->m_element->as<Classifier>().m_inheritedMembers.contains(me.getID())) {
                    pair.second.node->m_element->as<Classifier>().m_inheritedMembers.innerRemove(&el);
            }
        }
    }
}

Singleton<RedefinableTemplateSignature, Classifier>& Classifier::getOwnedTemplateSignatureSingleton() {
    return m_classifierOwnedTemplateSignature;
}

Singleton<ClassifierTemplateParameter, Classifier>& Classifier::getTemplateParameterSingleton() {
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
    eraseFromSet(id, m_generals);
    eraseFromSet(id, m_powerTypeExtent);
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

ReadOnlyIndexableSet<Feature, Classifier>& Classifier::getFeatures() {
    return m_features;
}

ReadOnlyIndexableSet<Property, Classifier>& Classifier::getAttributes() {
    return m_attributes;
}

Set<Generalization, Classifier, Classifier::GeneralizationPolicy>& Classifier::getGeneralizations() {
    return m_generalizations;
}

IndexableSet<Classifier, Classifier, Classifier::GeneralPolicy>& Classifier::getGenerals() {
    return m_generals;
}

ReadOnlyIndexableSet<NamedElement, Classifier>& Classifier::getInheritedMembers() {
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

bool Classifier::is(ElementType eType) const {
    bool ret = Namespace::is(eType);

    if (!ret) {
        ret = Type::is(eType);
    }

    if (!ret) {
        ret = RedefinableElement::is(eType);
    }

    if (!ret) {
        ret = TemplateableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASSIFIER;
    }

    return ret;
}

#include "uml/uml-stable.h"

using namespace UML;

void Classifier::GeneralizationPolicy::elementAdded(Generalization& el, Classifier& me) {
    if (el.getGeneral() && !me.m_generals.contains(el.getGeneral().id())) {
        if (el.as<Generalization>().getGeneral().loaded()) {
            me.m_generals.add(el.getGeneral());
        } else {
            me.m_generals.add(el.getGeneral().id());
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
        GeneralizationPtr newGeneralization = me.m_manager.create(Generalization::Info::elementType);
        newGeneralization->setGeneral(el);
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
        for (auto& referencePair : me.m_node.lock()->m_references) {
            auto& reference = referencePair.second;
            if (
                reference.m_node.lock()->m_ptr && 
                std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->is<Classifier>()
             ) {
                Classifier& clazz = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->as<Classifier>();
                if (
                    clazz.m_generals.contains(me.getID()) &&
                    !clazz.m_inheritedMembers.contains(el.getID())
                ) {
                    me.addToReadonlySet(clazz.m_inheritedMembers, el);
                }
             }
        }
    }
}

void Classifier::OwnedMemberPolicy::elementRemoved(NamedElement& el, Classifier& me) {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& referencePair : me.m_node.lock()->m_references) {
            auto& reference = referencePair.second;
            if (
                reference.m_node.lock()->m_ptr &&
                std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->is<Classifier>()
            ) {
                Classifier& clazz = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr)->as<Classifier>();
                if (
                    clazz.m_generals.contains(me.getID()) &&
                    clazz.m_inheritedMembers.contains(me.getID())
                ) {
                    me.removeFromReadonlySet(clazz.m_inheritedMembers, el);
                }
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

Classifier::Classifier(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Namespace(elementType, manager),
    Type(elementType, manager),
    RedefinableElement(elementType, manager),
    TemplateableElement(elementType, manager)
{
    m_features.subsets(m_members);
    m_features.opposite(&Feature::getFeaturingClassifierSingleton);
    m_attributes.subsets(m_features);
    m_generalizations.subsets(m_ownedElements);
    m_generalizations.opposite(&Generalization::getSpecificSingleton);
    m_inheritedMembers.subsets(m_members);
    m_powerTypeExtent.opposite(&GeneralizationSet::getPowerTypeSingleton);
    m_classifierOwnedTemplateSignature.redefines(m_ownedTemplateSignature);
    m_classifierOwnedTemplateSignature.opposite(&RedefinableTemplateSignature::getClassifierSingleton);
    m_classifierTemplateParameter.redefines(m_templateParameter);
    m_classifierTemplateParameter.opposite(&ClassifierTemplateParameter::getParameteredElementSingleton);
    m_classifierOwnedMembers.redefines(m_ownedMembers);
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

void Classifier::setTemplateParameter(ClassifierTemplateParameter& templateParameter) {
    m_classifierTemplateParameter.set(templateParameter);
}

void Classifier::setTemplateParameter(ClassifierTemplateParameterPtr templateParameter) {
    m_classifierTemplateParameter.set(templateParameter);
}

void Classifier::setTemplateParameter(ID id) {
    m_classifierTemplateParameter.set(id);
}

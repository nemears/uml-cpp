#include "uml/uml-stable.h"

using namespace UML;

void RedefinableTemplateSignature::ExtendedSignaturePolicy::elementAdded(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me) {
    el.m_redefinableTemplateSignatureParameters.addSignatures.insert(UmlPtr<RedefinableTemplateSignature>(&me));
    el.m_redefinableTemplateSignatureParameters.removeSignatures.insert(UmlPtr<RedefinableTemplateSignature>(&me));
    for (auto& param : el.getParameters()) {
        if (!me.m_inheritedParameters.contains(param)) {
            me.m_inheritedParameters.innerAdd(&param);
        }
    }
}

void RedefinableTemplateSignature::ExtendedSignaturePolicy::elementRemoved(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me) {
    el.m_redefinableTemplateSignatureParameters.addSignatures.erase(UmlPtr<RedefinableTemplateSignature>(&me));
    el.m_redefinableTemplateSignatureParameters.removeSignatures.erase(UmlPtr<RedefinableTemplateSignature>(&me));
    std::unordered_set<ID> parametersToKeep;
    for (auto& sig : me.getExtendedSignatures()) {
        if (sig != el) {
            for (ID id : sig.getParameters().ids()) {
                parametersToKeep.insert(id);
            }
        }
    }
    for (auto& param : el.getParameters()) {
        if (me.m_inheritedParameters.contains(param) && !parametersToKeep.count(param.getID())) {
            me.m_inheritedParameters.innerRemove(&param);
        }
    }
}

void RedefinableTemplateSignature::ParameterPolicy::elementAdded(TemplateParameter& el, RedefinableTemplateSignature& me) {
    for (auto sig : addSignatures) {
        if (!sig->getInheritedParameters().contains(el.getID())) {
            sig->m_inheritedParameters.innerAdd(&el);
        }   
    }
}

void RedefinableTemplateSignature::ParameterPolicy::elementRemoved(TemplateParameter& el, RedefinableTemplateSignature& me) {
    for (auto ogSig : removeSignatures) {
        std::unordered_set<ID> parametersToKeep;
        for (auto& sig : ogSig->getExtendedSignatures()) {
            if (sig != me) {
                for (ID id : sig.getParameters().ids()) {
                    parametersToKeep.insert(id);
                }
            }
        }
        if (ogSig->getInheritedParameters().contains(el.getID()) && !parametersToKeep.count(el.getID())) {
            ogSig->m_inheritedParameters.innerRemove(&el);
        }
    }
}

Singleton<Classifier, RedefinableTemplateSignature>& RedefinableTemplateSignature::getClassifierSingleton() {
    return m_classifier;
}

RedefinableTemplateSignature::RedefinableTemplateSignature(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    TemplateSignature(elementType, manager)
{
    m_classifier.redefines(m_template);
    m_classifier.opposite(&Classifier::getOwnedTemplateSignatureSingleton);
    m_extendedSignatures.subsets(m_redefinedElement);
    m_inheritedParameters.subsets(m_redefinableTemplateSignatureParameters);
    m_parameters.redefines(m_redefinableTemplateSignatureParameters);
}

ClassifierPtr RedefinableTemplateSignature::getClassifier() const {
    return m_classifier.get();
}

void RedefinableTemplateSignature::setClassifier(Classifier* classifier) {
    m_classifier.set(classifier);
}

void RedefinableTemplateSignature::setClassifier(Classifier& classifier) {
    m_classifier.set(classifier);
}

void RedefinableTemplateSignature::setClassifier(ClassifierPtr classifier) {
    m_classifier.set(classifier);
}

void RedefinableTemplateSignature::setClassifier(ID id) {
    m_classifier.set(id);
}

Set<RedefinableTemplateSignature, RedefinableTemplateSignature, RedefinableTemplateSignature::ExtendedSignaturePolicy>& RedefinableTemplateSignature::getExtendedSignatures() {
    return m_extendedSignatures;
}

ReadOnlySet<TemplateParameter, RedefinableTemplateSignature>& RedefinableTemplateSignature::getInheritedParameters() {
    return m_inheritedParameters;
}

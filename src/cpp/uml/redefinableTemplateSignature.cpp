#include "uml/redefinableTemplateSignature.h"
#include "uml/templateableElement.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void RedefinableTemplateSignature::AddExtendedSignaturePolicy::apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me) {
    el.m_redefinableTemplateSignatureParameters.addSignatures.insert(UmlPtr<RedefinableTemplateSignature>(&me));
    el.m_redefinableTemplateSignatureParameters.removeSignatures.insert(UmlPtr<RedefinableTemplateSignature>(&me));
    for (auto& param : el.getParameters()) {
        if (!me.m_inheritedParameters.contains(param)) {
            [[maybe_unused]] SetLock paramLck = me.lockEl(param);
            me.m_inheritedParameters.innerAdd(param);
            me.m_node->setReference(param);
            param.m_node->setReference(me);
        }
    }
}

void RedefinableTemplateSignature::RemoveExtendedSignaturePolicy::apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me) {
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
            [[maybe_unused]] SetLock paramLck = me.lockEl(param);
            me.m_inheritedParameters.innerRemove(param.getID());
            me.m_node->removeReference(param);
            param.m_node->removeReference(me);
        }
    }
}

void RedefinableTemplateSignature::AddParameterPolicy::apply(TemplateParameter& el, RedefinableTemplateSignature& me) {
    for (auto sig : addSignatures) {
        if (!sig->getInheritedParameters().contains(el.getID())) {
            [[maybe_unused]] SetLock sigLock = me.lockEl(*sig);
            sig->m_inheritedParameters.innerAdd(el.as<TemplateParameter>());
            sig->m_node->setReference(me);
            me.m_node->setReference(*sig);
        }   
    }
}

void RedefinableTemplateSignature::RemoveParameterPolicy::apply(TemplateParameter& el, RedefinableTemplateSignature& me) {
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
            [[maybe_unused]] SetLock ogSigLock = me.lockEl(*ogSig);
            ogSig->m_inheritedParameters.innerRemove(el.getID());
            ogSig->m_node->removeReference(me);
            me.m_node->removeReference(*ogSig);
        }
    }
}

TypedSet<Classifier, RedefinableTemplateSignature>& RedefinableTemplateSignature::getClassifierSingleton() {
    return m_classifier;
}

void RedefinableTemplateSignature::referenceReindexed(ID newID) {
    RedefinableElement::referenceReindexed(newID);
    TemplateSignature::referenceReindexed(newID);
}

void RedefinableTemplateSignature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    TemplateSignature::referenceErased(id);
}

void RedefinableTemplateSignature::init() {
    m_classifier.redefines(m_template);
    m_classifier.opposite(&Classifier::getOwnedTemplateSignatureSingleton);
    m_extendedSignatures.subsets(m_redefinedElement);
    m_inheritedParameters.subsets(m_redefinableTemplateSignatureParameters);
    m_parameters.redefines(m_redefinableTemplateSignatureParameters);
    m_inheritedParameters.m_readOnly = true;
}

RedefinableTemplateSignature::RedefinableTemplateSignature() : Element(ElementType::REDEFINABLE_TEMPLATE_SIGNATURE) {
    init();
}

RedefinableTemplateSignature::~RedefinableTemplateSignature() {

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

Set<RedefinableTemplateSignature, RedefinableTemplateSignature>& RedefinableTemplateSignature::getExtendedSignatures() {
    return m_extendedSignatures;
}

Set<TemplateParameter, RedefinableTemplateSignature>& RedefinableTemplateSignature::getInheritedParameters() {
    return m_inheritedParameters;
}

bool RedefinableTemplateSignature::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = TemplateSignature::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
    }

    return ret;
}
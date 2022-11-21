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
    // AddParameterInExtendedSignatureFunctor* addFunctor = new AddParameterInExtendedSignatureFunctor(&el);
    // addFunctor->m_sig = &m_el.as<RedefinableTemplateSignature>();
    // el.as<RedefinableTemplateSignature>().getParameters().m_addFunctors.insert(addFunctor);
    // RemoveParameterInExtendedSignatureFunctor* removeFunctor = new RemoveParameterInExtendedSignatureFunctor(&el);
    // removeFunctor->m_sig = &m_el.as<RedefinableTemplateSignature>();
    // el.as<RedefinableTemplateSignature>().getParameters().m_removeFunctors.insert(removeFunctor);
    // for (auto& param : el.as<RedefinableTemplateSignature>().getParameters()) {
    //     if (!m_el.as<RedefinableTemplateSignature>().m_inheritedParameters.contains(param)) {
    //         m_el.as<RedefinableTemplateSignature>().m_inheritedParameters.addReadOnly(param);
    //     }
    // }
}

void RedefinableTemplateSignature::RemoveExtendedSignaturePolicy::apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me) {
    // for (auto& functor : el.as<RedefinableTemplateSignature>().getParameters().m_addFunctors) {
    //     if (sizeof(functor) == sizeof(AddParameterInExtendedSignatureFunctor)) { // compare type
    //         el.as<RedefinableTemplateSignature>().getParameters().m_addFunctors.erase(functor);
    //         delete functor;
    //         break;
    //     }
    // }
    // for (auto& functor : el.as<RedefinableTemplateSignature>().getParameters().m_removeFunctors) {
    //     if (sizeof(functor) == sizeof(RemoveParameterInExtendedSignatureFunctor)) { // compare type
    //         el.as<RedefinableTemplateSignature>().getParameters().m_removeFunctors.erase(functor);
    //         delete functor;
    //         break;
    //     }
    // }
    // std::unordered_set<ID> parametersToKeep;
    // for (auto& sig : m_el.as<RedefinableTemplateSignature>().getExtendedSignatures()) {
    //     if (sig != el) {
    //         for (ID id : sig.getParameters().ids()) {
    //             parametersToKeep.insert(id);
    //         }
    //     }
    // }
    // for (auto& param : el.as<RedefinableTemplateSignature>().getParameters()) {
    //     if (m_el.as<RedefinableTemplateSignature>().m_inheritedParameters.contains(param) && !parametersToKeep.count(param.getID())) {
    //         m_el.as<RedefinableTemplateSignature>().m_inheritedParameters.removeReadOnly(param.getID());
    //     }
    // }
}

// void RedefinableTemplateSignature::AddParameterInExtendedSignatureFunctor::operator()(Element& el) const {
//     if (!m_sig->getInheritedParameters().contains(el.getID())) {
//         m_sig->getInheritedParameters().addReadOnly(el.as<TemplateParameter>());
//     }
// }

// void RedefinableTemplateSignature::RemoveParameterInExtendedSignatureFunctor::operator()(Element& el) const {
//     std::unordered_set<ID> parametersToKeep;
//     for (auto& sig : m_sig->getExtendedSignatures()) {
//         if (sig != m_el) {
//             for (ID id : sig.getParameters().ids()) {
//                 parametersToKeep.insert(id);
//             }
//         }
//     }
//     if (m_sig->getInheritedParameters().contains(el.getID()) && !parametersToKeep.count(el.getID())) {
//         m_sig->getInheritedParameters().removeReadOnly(el.getID());
//     }
// }

TypedSet<Classifier, RedefinableTemplateSignature>& RedefinableTemplateSignature::getClassifierSingleton() {
    return m_classifier;
}

void RedefinableTemplateSignature::referenceReindexed(ID oldID, ID newID) {
    RedefinableElement::referenceReindexed(oldID, newID);
    TemplateSignature::referenceReindexed(oldID, newID);
}

void RedefinableTemplateSignature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    TemplateSignature::referenceErased(id);
}

void RedefinableTemplateSignature::init() {
    m_classifier.redefines(m_template);
    m_classifier.opposite(&Classifier::getOwnedTemplateSignatureSingleton);
    m_extendedSignatures.subsets(m_redefinedElement);
    m_inheritedParameters.subsets(m_parameters);
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
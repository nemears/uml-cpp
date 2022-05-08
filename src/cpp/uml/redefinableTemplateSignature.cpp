#include "uml/redefinableTemplateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"
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

Set<Classifier, RedefinableTemplateSignature>& RedefinableTemplateSignature::getClassifierSingleton() {
    return m_classifier;
}

void RedefinableTemplateSignature::referencingReleased(ID id) {
    RedefinableElement::referencingReleased(id);
    TemplateSignature::referencingReleased(id);
}

void RedefinableTemplateSignature::referenceReindexed(ID oldID, ID newID) {
    RedefinableElement::referenceReindexed(oldID, newID);
    TemplateSignature::referenceReindexed(oldID, newID);
}

void RedefinableTemplateSignature::reindexName(ID id, std::string newName) {
    RedefinableElement::reindexName(id, newName);
    TemplateSignature::reindexName(id, newName);
}

void RedefinableTemplateSignature::referenceErased(ID id) {
    RedefinableElement::referenceErased(id);
    TemplateSignature::referenceErased(id);
}

void RedefinableTemplateSignature::init() {
    m_classifier.redefines(m_template);
    m_classifier.opposite(&Classifier::getOwnedTemplateSignatureSingleton);
    m_classifier.m_signature = &RedefinableTemplateSignature::getClassifierSingleton;
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
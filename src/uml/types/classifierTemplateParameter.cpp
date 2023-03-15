#include "uml/types/classifierTemplateParameter.h"
#include "uml/types/templateableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getParameteredElementSingleton() {
    return m_classifierParameteredElement;
}

void ClassifierTemplateParameter::referenceReindexed(ID newID) {
    TemplateParameter::referenceReindexed(newID);
    m_constrainingClassifiers.reindex(newID);
}

void ClassifierTemplateParameter::referenceErased(ID id) {
    TemplateParameter::referenceErased(id);
    m_constrainingClassifiers.eraseElement(id);
}

ClassifierTemplateParameter::ClassifierTemplateParameter() : Element(ElementType::CLASSIFIER_TEMPLATE_PARAMETER) {
    m_classifierParameteredElement.redefines(m_parameteredElement);
    m_classifierParameteredElement.opposite(&Classifier::getTemplateParameterSingleton);
}

ClassifierTemplateParameter::~ClassifierTemplateParameter() {

}

ClassifierPtr ClassifierTemplateParameter::getParameteredElement() const {
    return m_classifierParameteredElement.get();
}

void ClassifierTemplateParameter::setParameteredElement(Classifier* parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(Classifier& parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(ClassifierPtr parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(ID id) {
    m_classifierParameteredElement.set(id);
}

Set<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getConstrainingClassifiers() {
    return m_constrainingClassifiers;
}

bool ClassifierTemplateParameter::isAllowSubstitutable() const {
    return m_allowSubstitutable;
}

void ClassifierTemplateParameter::setAllowSubstitutable(bool allowSubstitutable) {
    m_allowSubstitutable = allowSubstitutable;
}

bool ClassifierTemplateParameter::isSubClassOf(ElementType eType) const {
    bool ret = false;
    if ((ret = TemplateParameter::isSubClassOf(eType))) {
        return ret;
    }

    ret = eType == ElementType::CLASSIFIER_TEMPLATE_PARAMETER;

    return ret;
}
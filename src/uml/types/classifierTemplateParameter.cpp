#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getParameteredElementSingleton() {
    return m_classifierParameteredElement;
}

void ClassifierTemplateParameter::referenceErased(ID id) {
    TemplateParameter::referenceErased(id);
    eraseFromSet(id, m_constrainingClassifiers);
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

bool ClassifierTemplateParameter::is(ElementType eType) const {
    bool ret = false;
    if ((ret = TemplateParameter::is(eType))) {
        return ret;
    }

    ret = eType == ElementType::CLASSIFIER_TEMPLATE_PARAMETER;

    return ret;
}

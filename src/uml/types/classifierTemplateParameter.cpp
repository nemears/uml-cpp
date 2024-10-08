#include "uml/managers/abstractManager.h"
#include "uml/set/singleton.h"
#include "uml/types/templateParameter.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getParameteredElementSingleton() {
    return m_classifierParameteredElement;
}

ClassifierTemplateParameter::ClassifierTemplateParameter(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    TemplateParameter(elementType, manager)
    
{
    m_classifierParameteredElement.redefines(m_parameteredElement);
    m_classifierParameteredElement.opposite(&Classifier::getTemplateParameterSingleton);
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

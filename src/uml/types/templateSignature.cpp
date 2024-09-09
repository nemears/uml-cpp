#include "uml/uml-stable.h"

using namespace UML;

Singleton<TemplateableElement, TemplateSignature>& TemplateSignature::getTemplateSingleton() {
    return m_template;
}

TemplateSignature::TemplateSignature(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager)
{
    m_template.subsets(m_owner);
    m_template.opposite(&TemplateableElement::getOwnedTemplateSignatureSingleton);
    m_ownedParameters.subsets(m_parameters);
    m_ownedParameters.subsets(m_ownedElements);
    m_ownedParameters.opposite(&TemplateParameter::getSignatureSingleton);
}

TemplateableElementPtr TemplateSignature::getTemplate() const {
    return m_template.get();
}

void TemplateSignature::setTemplate(TemplateableElementPtr temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(TemplateableElement& temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(ID id) {
    m_template.set(id);
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getOwnedParameters() {
    return m_ownedParameters;
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getParameters() {
    return m_parameters;
}

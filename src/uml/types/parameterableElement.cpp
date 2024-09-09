#include "uml/uml-stable.h"

using namespace UML;

Singleton<TemplateParameter, ParameterableElement>& ParameterableElement::getOwningTemplateParameterSingleton() {
    return m_owningTemplateParameter;
}

Singleton<TemplateParameter, ParameterableElement>& ParameterableElement::getTemplateParameterSingleton() {
    return m_templateParameter;
}

ParameterableElement::ParameterableElement(std::size_t elementType, AbstractManager& manager) : Element(elementType, manager) {
    m_templateParameter.opposite(&TemplateParameter::getParameteredElementSingleton);
    m_owningTemplateParameter.subsets(m_templateParameter);
    m_owningTemplateParameter.subsets(m_owner);
    m_owningTemplateParameter.opposite(&TemplateParameter::getOwnedParameteredElementSingleton);
}

TemplateParameterPtr ParameterableElement::getOwningTemplateParameter() const {
    return m_owningTemplateParameter.get();
}

void ParameterableElement::setOwningTemplateParameter(TemplateParameterPtr parameter) {
    m_owningTemplateParameter.set(parameter);
}

void ParameterableElement::setOwningTemplateParameter(TemplateParameter& parameter) {
    m_owningTemplateParameter.set(parameter);
}

void ParameterableElement::setOwningTemplateParameter(ID id) {
    m_owningTemplateParameter.set(id);
}

TemplateParameterPtr ParameterableElement::getTemplateParameter() const {
    return m_templateParameter.get();
}

void ParameterableElement::setTemplateParameter(TemplateParameterPtr parameter) {
    m_templateParameter.set(parameter);
}

void ParameterableElement::setTemplateParameter(TemplateParameter& parameter) {
    m_templateParameter.set(parameter);
}

void ParameterableElement::setTemplateParameter(ID id) {
    m_templateParameter.set(id);
}

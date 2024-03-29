#include "uml/types/parameterableElement.h"
#include "uml/types/templateableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void ParameterableElement::referenceErased(ID id) {
    m_templateParameter.eraseElement(id);
}

TypedSet<TemplateParameter, ParameterableElement>& ParameterableElement::getOwningTemplateParameterSingleton() {
    return m_owningTemplateParameter;
}

TypedSet<TemplateParameter, ParameterableElement>& ParameterableElement::getTemplateParameterSingleton() {
    return m_templateParameter;
}

ParameterableElement::ParameterableElement() : Element(ElementType::PARAMETERABLE_ELEMENT) {
    m_templateParameter.opposite(&TemplateParameter::getParameteredElementSingleton);
    m_owningTemplateParameter.subsets(m_templateParameter);
    m_owningTemplateParameter.subsets(*m_owner);
    m_owningTemplateParameter.opposite(&TemplateParameter::getOwnedParameteredElementSingleton);
}

ParameterableElement::~ParameterableElement() {

}

TemplateParameterPtr ParameterableElement::getOwningTemplateParameter() const {
    return m_owningTemplateParameter.get();
}

void ParameterableElement::setOwningTemplateParameter(TemplateParameter* parameter) {
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

void ParameterableElement::setTemplateParameter(TemplateParameter* parameter) {
    m_templateParameter.set(parameter);
}

void ParameterableElement::setTemplateParameter(TemplateParameter& parameter) {
    m_templateParameter.set(parameter);
}

void ParameterableElement::setTemplateParameter(ID id) {
    m_templateParameter.set(id);
}

bool ParameterableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PARAMETERABLE_ELEMENT;
    }

    return ret;
}
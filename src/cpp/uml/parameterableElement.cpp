#include "uml/parameterableElement.h"
#include "uml/templateableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void ParameterableElement::referencingReleased(ID id) {
    m_templateParameter.release(id);
}

void ParameterableElement::referenceReindexed(ID oldID, ID newID) {
    m_templateParameter.reindex(oldID, newID);
}

void ParameterableElement::reindexName(std::string oldName, std::string newName) {
    m_templateParameter.reindexName(oldName, newName);
}

void ParameterableElement::restoreReference(Element* el) {
    m_templateParameter.restore(el);
    if (el->isSubClassOf(ElementType::TEMPLATE_PARAMETER) && el->as<TemplateParameter>().m_ownedDefault.get().id() == m_id) {
        if (m_templateParameter.empty()) {
            m_templateParameter.set(el->getID());
        }
    }
}

void ParameterableElement::referenceErased(ID id) {
    m_templateParameter.eraseElement(id);
}

Set<TemplateParameter, ParameterableElement>& ParameterableElement::getOwningTemplateParameterSingleton() {
    return m_owningTemplateParameter;
}

Set<TemplateParameter, ParameterableElement>& ParameterableElement::getTemplateParameterSingleton() {
    return m_templateParameter;
}

void ParameterableElement::init() {
    m_templateParameter.opposite(&TemplateParameter::getParameteredElementSingleton);
    m_templateParameter.m_signature = &ParameterableElement::getTemplateParameterSingleton;
    m_owningTemplateParameter.subsets(m_templateParameter);
    m_owningTemplateParameter.subsets(*m_owner);
    m_owningTemplateParameter.opposite(&TemplateParameter::getOwnedParameteredElementSingleton);
    m_owningTemplateParameter.m_signature = &ParameterableElement::getOwningTemplateParameterSingleton;
}


ParameterableElement::ParameterableElement() : Element(ElementType::PARAMETERABLE_ELEMENT) {
    init();
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
#include "uml/parameterableElement.h"
#include "uml/templateParameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void ParameterableElement::referencingReleased(ID id) {
    m_owningTemplateParameter.release(id);
    m_templateParameter.release(id);
}

void ParameterableElement::referenceReindexed(ID oldID, ID newID) {
    m_owningTemplateParameter.reindex(oldID, newID);
    m_templateParameter.reindex(oldID, newID);
}

void ParameterableElement::restoreReference(Element* el) {
    if (el->isSubClassOf(ElementType::TEMPLATE_PARAMETER) && el->as<TemplateParameter>().m_ownedDefault.id() == m_id) {
        if (m_templateParameter.empty()) {
            m_templateParameter.set(el->getID());
        }
        m_templateParameter.restore(el);
    }
}

void ParameterableElement::restoreReferences() {
    // m_owningTemplateParameter.restoreReference();
    // m_templateParameter.restoreReference();
}

void ParameterableElement::referenceErased(ID id) {
    m_owningTemplateParameter.eraseElement(id);
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

void ParameterableElement::copy(const ParameterableElement& rhs) {
    m_templateParameter = rhs.m_templateParameter;
    m_owningTemplateParameter = rhs.m_owningTemplateParameter;
}

ParameterableElement::ParameterableElement() : Element(ElementType::PARAMETERABLE_ELEMENT) {
    init();
}

ParameterableElement::ParameterableElement(const ParameterableElement& el) : Element(el, ElementType::PARAMETERABLE_ELEMENT) {
    // abstract
}

ParameterableElement::~ParameterableElement() {

}

TemplateParameter* ParameterableElement::getOwningTemplateParameter() {
    return m_owningTemplateParameter.get();
}

TemplateParameter& ParameterableElement::getOwningTemplateParameterRef() {
    return m_owningTemplateParameter.getRef();
}

ID ParameterableElement::getOwningTemplateParameterID() const {
    return m_owningTemplateParameter.id();
}

bool ParameterableElement::hasOwningTemplateParameter() const {
    return m_owningTemplateParameter.has();
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

TemplateParameter* ParameterableElement::getTemplateParameter() {
    return m_templateParameter.get();
}

TemplateParameter& ParameterableElement::getTemplateParameterRef() {
    return m_templateParameter.getRef();
}

ID ParameterableElement::getTemplateParameterID() const {
    return m_templateParameter.id();
}

bool ParameterableElement::hasTemplateParameter() const {
    return m_templateParameter.has();
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
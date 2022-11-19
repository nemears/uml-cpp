#include "uml/parameterableElement.h"
#include "uml/templateableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void ParameterableElement::referenceReindexed(ID oldID, ID newID) {
    m_templateParameter.reindex(newID);
}

// void ParameterableElement::restoreReference(Element* el) {
//     m_templateParameter.restore(el);
//     if (el->isSubClassOf(ElementType::TEMPLATE_PARAMETER) && el->as<TemplateParameter>().m_ownedDefault.get().id() == m_id) {
//         if (m_templateParameter.empty()) {
//             m_templateParameter.set(el->getID());
//         }
//     }
// }

void ParameterableElement::referenceErased(ID id) {
    m_templateParameter.eraseElement(id);
}

TypedSet<TemplateParameter, ParameterableElement>& ParameterableElement::getOwningTemplateParameterSingleton() {
    return m_owningTemplateParameter;
}

TypedSet<TemplateParameter, ParameterableElement>& ParameterableElement::getTemplateParameterSingleton() {
    return m_templateParameter;
}

void ParameterableElement::init() {
    m_templateParameter.opposite(&TemplateParameter::getParameteredElementSingleton);
    m_owningTemplateParameter.subsets(m_templateParameter);
    m_owningTemplateParameter.subsets(*m_owner);
    m_owningTemplateParameter.opposite(&TemplateParameter::getOwnedParameteredElementSingleton);
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
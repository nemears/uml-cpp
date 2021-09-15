#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"
#include "uml/templateParameter.h"

using namespace UML;

void TemplateSignature::RemoveTemplateProcedure::operator()(TemplateableElement* el) const {
    if (el->m_ownedTemplateSignatureID == m_me->getID()) {
        el->setOwnedTemplateSignature(0);
    }
}

void TemplateSignature::AddTemplateProcedure::operator()(TemplateableElement* el) const {
    if (el->m_ownedTemplateSignatureID != m_me->getID()) {
        el->setOwnedTemplateSignature(m_me);
    }
}

void TemplateSignature::AddOwnedParameterFunctor::operator()(TemplateParameter& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    if (el.getSignature() != m_el) {
        el.setSignature(m_el);
    }

    if (!m_el->getParameter().count(el.getID())) {
        m_el->getParameter().add(el);
    }
    updateCopiedSequenceAddedTo(el, &TemplateSignature::getOwnedParameter);
}

void TemplateSignature::RemoveOwnedParameterFunctor::operator()(TemplateParameter& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    if (el.getSignature() == m_el) {
        el.setSignature(0);
    }

    if (m_el->getParameter().count(el.getID())) {
        m_el->getParameter().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &TemplateSignature::getOwnedParameter);
}

void TemplateSignature::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_ownedParameter.m_manager = manager;
    m_parameter.m_manager = manager;
}

void TemplateSignature::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_template.id() == id) {
        m_template.release();
    }
    m_ownedParameter.elementReleased(id, &TemplateSignature::getOwnedParameter);
    m_parameter.elementReleased(id, &TemplateSignature::getParameter);
}

void TemplateSignature::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    if (m_template.id() == oldID) {
        m_template.reindex(oldID, newID);
    }
    if (m_ownedParameter.count(oldID)) {
        m_ownedParameter.reindex(oldID, newID, &TemplateSignature::getOwnedParameter);
    }
    if (m_parameter.count(oldID)) {
        m_parameter.reindex(oldID, newID, &TemplateSignature::getParameter);
    }
}

TemplateSignature::TemplateSignature() {
    m_template.m_signature = &TemplateSignature::m_template;
    m_template.m_removeProcedures.push_back(new RemoveTemplateProcedure(this));
    m_template.m_addProcedures.push_back(new AddTemplateProcedure(this));
    m_ownedParameter.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameter.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
}

TemplateSignature::TemplateSignature(const TemplateSignature& el) {
    m_template = el.m_template;
    m_template.m_me = this;
    m_template.m_removeProcedures.clear();
    m_template.m_addProcedures.clear();
    m_template.m_removeProcedures.push_back(new RemoveTemplateProcedure(this));
    m_template.m_addProcedures.push_back(new AddTemplateProcedure(this));
    m_ownedParameter = el.m_ownedParameter;
    m_ownedParameter.addProcedures.clear();
    m_ownedParameter.removeProcedures.clear();
    m_ownedParameter.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameter.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
}

TemplateSignature::~TemplateSignature() {

}

TemplateableElement* TemplateSignature::getTemplate() {
    return m_template.get();
}

TemplateableElement& TemplateSignature::getTemplateRef() {
    return m_template.getRef();
}

bool TemplateSignature::hasTemplate() const {
    return m_template.has();
}

void TemplateSignature::setTemplate(TemplateableElement* temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(TemplateableElement& temp) {
    m_template.set(temp);
}

Sequence<TemplateParameter>& TemplateSignature::getOwnedParameter() {
    return m_ownedParameter;
}

Sequence<TemplateParameter>& TemplateSignature::getParameter() {
    return m_parameter;
}

ElementType TemplateSignature::getElementType() const {
    return ElementType::TEMPLATE_SIGNATURE;
}

bool TemplateSignature::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}
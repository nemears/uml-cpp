#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"
#include "uml/templateParameter.h"

using namespace UML;

void TemplateSignature::AddOwnedParameterFunctor::operator()(Element& el) const {
    if (!dynamic_cast<TemplateSignature*>(m_el)->getOwnedElements().count(el.getID())) {
        dynamic_cast<TemplateSignature*>(m_el)->getOwnedElements().internalAdd(el);
    }

    if (dynamic_cast<TemplateParameter&>(el).getSignature() != dynamic_cast<TemplateSignature*>(m_el)) {
        dynamic_cast<TemplateParameter&>(el).setSignature(dynamic_cast<TemplateSignature*>(m_el));
    }

    if (!dynamic_cast<TemplateSignature*>(m_el)->getParameter().count(el.getID())) {
        dynamic_cast<TemplateSignature*>(m_el)->getParameter().add(dynamic_cast<TemplateParameter&>(el));
    }
}

void TemplateSignature::RemoveOwnedParameterFunctor::operator()(Element& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    if (dynamic_cast<TemplateParameter&>(el).getSignature() == m_el) {
        dynamic_cast<TemplateParameter&>(el).setSignature(0);
    }

    if (dynamic_cast<TemplateSignature*>(m_el)->getParameter().count(el.getID())) {
        dynamic_cast<TemplateSignature*>(m_el)->getParameter().remove(dynamic_cast<TemplateParameter&>(el));
    }
}

void TemplateSignature::setManager(UmlManager* manager) {
    Element::setManager(manager);
    m_ownedParameter.m_manager = manager;
}

TemplateSignature::TemplateSignature() {
    m_templatePtr = 0;
    m_ownedParameter.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameter.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
}

TemplateSignature::TemplateSignature(const TemplateSignature& el) {
    m_templateID = el.m_templateID;
    m_ownedParameter = el.m_ownedParameter;
    m_ownedParameter.addProcedures.clear();
    m_ownedParameter.removeProcedures.clear();
    m_ownedParameter.addProcedures.push_back(new AddOwnedParameterFunctor(this));
    m_ownedParameter.removeProcedures.push_back(new RemoveOwnedParameterFunctor(this));
}

TemplateSignature::~TemplateSignature() {

}

TemplateableElement* TemplateSignature::getTemplate() {
    if (!m_templateID.isNull()) {
        if (!m_templatePtr) {
            m_templatePtr = &m_manager->get<TemplateableElement>(m_templateID);
        }
        return m_templatePtr;
    }
    return 0;
}

void TemplateSignature::setTemplate(TemplateableElement* temp) {
    if (!m_templateID.isNull()) {
        if (!m_templatePtr) {
            m_templatePtr = &m_manager->get<TemplateableElement>(m_templateID);
        }

        TemplateableElement* temp = m_templatePtr;

        m_templateID = ID::nullID();
        m_templatePtr = 0;

        if (temp->getOwnedTemplateSignature() == this) {
            temp->setOwnedTemplateSignature(0);
        }
    }

    if (temp) {
        m_templateID = temp->getID();
    }

    if (!m_manager) {
        m_templatePtr = temp;
    }

    if (temp) {
        if (temp->getOwnedTemplateSignature() != this) {
            temp->setOwnedTemplateSignature(this);
        }
    }
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
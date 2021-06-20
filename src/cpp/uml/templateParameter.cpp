#include "uml/templateParameter.h"
#include "uml/templateSignature.h"

using namespace UML;

TemplateParameter::TemplateParameter() {
    m_signaturePtr = 0;
    m_ownedParameteredElementPtr = 0;
}

TemplateParameter::TemplateParameter(const TemplateParameter& el) {
    m_signatureID = el.m_signatureID;
    m_ownedParameteredElementID = el.m_ownedParameteredElementID;
}

TemplateParameter::~TemplateParameter() {

}

TemplateSignature* TemplateParameter::getSignature() {
    if (!m_signatureID.isNull()) {
        if (!m_signaturePtr) {
            m_signaturePtr = &m_manager->get<TemplateSignature>(m_signatureID);
        }
        return m_signaturePtr;
    }
    return 0;
}

void TemplateParameter::setSignature(TemplateSignature* signature) {
    if (!isSameOrNull(m_signatureID, signature)) {
        if (!m_signaturePtr) {
            m_signaturePtr = &m_manager->get<TemplateSignature>(m_signatureID);
        }

        if (m_signaturePtr->getOwnedParameter().count(m_id)) {
            m_signaturePtr->getOwnedParameter().remove(*this);
        }

        m_signatureID = ID::nullID();
        m_signaturePtr = 0;
    }

    if (signature) {
        m_signatureID = signature->getID();
    }

    if (!m_manager) {
        m_signaturePtr = signature;
    }

    if (signature) {
        if (!signature->getOwnedParameter().count(m_id)) {
            signature->getOwnedParameter().add(*this);
        }
    }
}

ParameterableElement* TemplateParameter::getOwnedParameterableElement() {
    if (!m_ownedParameteredElementID.isNull()) {
        if (!m_ownedParameteredElementPtr) {
            m_ownedParameteredElementPtr = &m_manager->get<ParameterableElement>(m_ownedParameteredElementID);
        }
        return m_ownedParameteredElementPtr;
    }
    return 0;
}

void TemplateParameter::setOwnedParameterableElement(ParameterableElement* el) {
    if (!isSameOrNull(m_ownedParameteredElementID, el)) {
        if (!m_ownedParameteredElementPtr) {
            m_ownedParameteredElementPtr = &m_manager->get<ParameterableElement>(m_ownedParameteredElementID);
        }

        if (m_ownedElements->count(m_ownedParameteredElementID)) {
            m_ownedElements->internalRemove(*m_ownedParameteredElementPtr);
        }

        m_ownedParameteredElementID = ID::nullID();
        m_ownedParameteredElementPtr = 0;
    }

    if (el) {
        m_ownedParameteredElementID = el->getID();
    }

    if (!m_manager) {
        m_ownedParameteredElementPtr = el;
    }

    if (el) {
        if (!m_ownedElements->count(el->getID())) {
            m_ownedElements->internalAdd(*el);
        }
    }
}

ElementType TemplateParameter::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER;
}

bool TemplateParameter::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}
#include "uml/templateParameter.h"
#include "uml/templateSignature.h"

using namespace UML;

TemplateParameter::TemplateParameter() {
    m_signaturePtr = 0;
}

TemplateParameter::TemplateParameter(const TemplateParameter& el) {
    m_signatureID = el.m_signatureID;
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

ElementType TemplateParameter::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER;
}

bool TemplateParameter::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}
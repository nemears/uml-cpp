#include "uml/templateParameter.h"
#include "uml/templateSignature.h"

using namespace UML;

TemplateParameter::TemplateParameter() {
    m_signaturePtr = 0;
    m_ownedParameteredElementPtr = 0;
    m_parameteredElementPtr = 0;
    m_defaultPtr = 0;
    m_ownedDefaultPtr = 0;
}

TemplateParameter::TemplateParameter(const TemplateParameter& el) {
    m_signatureID = el.m_signatureID;
    m_ownedParameteredElementID = el.m_ownedParameteredElementID;
    m_parameteredElementID = el.m_parameteredElementID;
    m_defaultID = el.m_defaultID;
    m_ownedDefaultID = el.m_ownedDefaultID;
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

ParameterableElement* TemplateParameter::getOwnedParameteredElement() {
    if (!m_ownedParameteredElementID.isNull()) {
        if (!m_ownedParameteredElementPtr) {
            m_ownedParameteredElementPtr = &m_manager->get<ParameterableElement>(m_ownedParameteredElementID);
        }
        return m_ownedParameteredElementPtr;
    }
    return 0;
}

void TemplateParameter::setOwnedParameteredElement(ParameterableElement* el) {
    if (!isSameOrNull(m_ownedParameteredElementID, el)) {
        if (!m_ownedParameteredElementPtr) {
            m_ownedParameteredElementPtr = &m_manager->get<ParameterableElement>(m_ownedParameteredElementID);
        }

        if (m_parameteredElementID != el->getID()) {
            setParameteredElement(el);
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
        if (m_parameteredElementID != el->getID()) {
            setParameteredElement(el);
        }

        if (!m_ownedElements->count(el->getID())) {
            m_ownedElements->internalAdd(*el);
        }
    }
}

ParameterableElement* TemplateParameter::getParameteredElement() {
    if (!m_parameteredElementID.isNull()) {
        if (!m_parameteredElementPtr) {
            m_parameteredElementPtr = &m_manager->get<ParameterableElement>(m_parameteredElementID);
        }
        return m_parameteredElementPtr;
    }
    return 0;
}

void TemplateParameter::setParameteredElement(ParameterableElement* el) {
    if (!isSameOrNull(m_parameteredElementID, el)) {
        m_parameteredElementPtr = 0;
        m_parameteredElementID = ID::nullID();
    }

    if (el) {
        m_parameteredElementID = el->getID();
    }

    if (!m_manager) {
        m_parameteredElementPtr = el;
    }
}

ParameterableElement* TemplateParameter::getDefault() {
    if (!m_defaultID.isNull()) {
        if (!m_defaultPtr) {
            m_defaultPtr = &m_manager->get<ParameterableElement>(m_defaultID);
        }
        return m_defaultPtr;
    }
    return 0;
}

void TemplateParameter::setDefault(ParameterableElement* el) {
    if (!isSameOrNull(m_defaultID, el)) {
        m_defaultID = ID::nullID();
        m_defaultPtr = 0;
    }

    if (el) {
        m_defaultID = el->getID();
    }

    if (!m_manager) {
        m_defaultPtr = el;
    }
}

ParameterableElement* TemplateParameter::getOwnedDefault() {
    if (!m_ownedDefaultID.isNull()) {
        if (!m_ownedDefaultPtr) {
            m_ownedDefaultPtr = &m_manager->get<ParameterableElement>(m_ownedDefaultID);
        }

        return m_ownedDefaultPtr;
    }
    return 0;
}

void TemplateParameter::setOwnedDefault(ParameterableElement* el) {
    if (!isSameOrNull(m_ownedDefaultID, el)) {
        if (!m_ownedDefaultPtr) {
            m_ownedDefaultPtr = &m_manager->get<ParameterableElement>(m_ownedDefaultID);
        }

        if (m_defaultID == m_ownedDefaultID) {
            setDefault(0);
        }

        if (m_ownedElements->count(m_ownedDefaultID)) {
            m_ownedElements->internalRemove(*m_ownedDefaultPtr);
        }

        m_ownedDefaultID = ID::nullID();
        m_ownedDefaultPtr = 0;
    }

    if (el) {
        m_ownedDefaultID = el->getID();
    }

    if (!m_manager) {
        m_ownedDefaultPtr = el;
    }

    if (el) {
        if (m_defaultID != el->getID()) {
            setDefault(el);
        }

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
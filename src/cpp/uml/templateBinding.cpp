#include "uml/templateBinding.h"

using namespace UML;

TemplateBinding::TemplateBinding() {
    m_boundElementPtr = 0;
    m_signaturePtr = 0;
}

TemplateBinding::TemplateBinding(const TemplateBinding& bind) {
    m_boundElementID = bind.m_boundElementID;
    m_signatureID = bind.m_signatureID;
}

TemplateBinding::~TemplateBinding() {

}

TemplateableElement* TemplateBinding::getBoundElement() {
    if (!m_boundElementID.isNull()) {
        if (!m_boundElementPtr) {
            m_boundElementPtr = &m_manager->get<TemplateableElement>(m_boundElementID);
        }
        return m_boundElementPtr;
    }
    return 0;
}

void TemplateBinding::setBoundElement(TemplateableElement* el) {
    if (!isSameOrNull(m_boundElementID, el)) {
        if (!m_boundElementPtr) {
            m_boundElementPtr = &m_manager->get<TemplateableElement>(m_boundElementID);
        }

        TemplateableElement* temp = m_boundElementPtr;

        if (m_sources.count(m_boundElementID)) {
            m_sources.remove(*m_boundElementPtr);
        }

        m_boundElementID = ID::nullID();
        m_boundElementPtr = 0;

        if (temp->getTemplateBinding() == this) {
            temp->setTemplateBinding(0);
        }
    }

    if (el) {
        m_boundElementID = el->getID();
    }

    if (!m_manager) {
        m_boundElementPtr = 0;
    }

    if (el) {
        if (el->getTemplateBinding() != this) {
            el->setTemplateBinding(this);
        }

        if (!m_sources.count(el->getID())) {
            m_sources.add(*el);
        }
    }
}

TemplateSignature* TemplateBinding::getSignature() {
    if (!m_signatureID.isNull()) {
        if (!m_signaturePtr) {
            m_signaturePtr = &m_manager->get<TemplateSignature>(m_signatureID);
        }
        return m_signaturePtr;
    }
    return 0;
}

void TemplateBinding::setSignature(TemplateSignature* signature) {
    if (!isSameOrNull(m_signatureID, signature)) {
        if (!m_signaturePtr) {
            m_signaturePtr = &m_manager->get<TemplateSignature>(m_signatureID);
        }

        if (m_targets.count(m_signatureID)) {
            m_targets.remove(*m_signaturePtr);
        }

        m_signatureID = ID::nullID();
        m_signaturePtr = 0;
    }

    if (signature) {
        m_signatureID = signature->getID();
    }

    if (!m_manager) {
        m_signaturePtr = 0;
    }

    if (signature) {
        if (!m_targets.count(signature->getID())) {
            m_targets.add(*signature);
        }
    }
}

ElementType TemplateBinding::getElementType() const {
    return ElementType::TEMPLATE_BINDING;
}

bool TemplateBinding::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_BINDING;
    }

    return ret;
}
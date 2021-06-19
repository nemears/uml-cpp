#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"

using namespace UML;

TemplateSignature::TemplateSignature() {
    m_templatePtr = 0;
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

ElementType TemplateSignature::getElementType() const {
    return ElementType::TEMPLATE_SIGNATURE;
}

bool TemplateSignature::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}
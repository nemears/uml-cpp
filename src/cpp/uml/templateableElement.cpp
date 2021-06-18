#include "uml/templateableElement.h"
#include "uml/umlManager.h"

using namespace UML;

TemplateableElement::~TemplateableElement() {

}

TemplateSignature* TemplateableElement::getOwnedTemplateSignature() {
    if (!m_ownedTemplateSignatureID.isNull()) {
        if (!m_ownedTemplateSignaturePtr) {
            m_ownedTemplateSignaturePtr = &m_manager->get<TemplateSignature>(m_ownedTemplateSignatureID);
        }
        return m_ownedTemplateSignaturePtr;
    }
    return 0;
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature* signature) {
    if (!isSameOrNull(m_ownedTemplateSignatureID, signature)) {
        if (!m_ownedTemplateSignaturePtr) {
            m_ownedTemplateSignaturePtr = &m_manager->get<TemplateSignature>(m_ownedTemplateSignatureID);
        }
        // TODO add to TemplateSignature template field
        m_ownedTemplateSignatureID = ID::nullID();
        m_ownedTemplateSignaturePtr = 0;
    }

    if (signature) {
        m_ownedTemplateSignatureID = signature->getID();
    }

    if (!m_manager) {
        m_ownedTemplateSignaturePtr = signature;
    }

    if (signature) {
        // todo
    }
}

ElementType TemplateableElement::getElementType() const {
    return ElementType::TEMPLATEABLE_ELEMENT;
}

bool TemplateableElement::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}
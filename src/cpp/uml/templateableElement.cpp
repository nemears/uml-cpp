#include "uml/templateableElement.h"
#include "uml/umlManager.h"
#include "uml/sequence.h"

using namespace UML;

TemplateableElement::TemplateableElement() {
    m_ownedTemplateSignaturePtr = 0;
}

TemplateableElement::TemplateableElement(const TemplateableElement& el){
    m_ownedTemplateSignatureID = el.m_ownedTemplateSignatureID;
    m_ownedTemplateSignaturePtr = 0;
}

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
    if (!m_ownedTemplateSignatureID.isNull()) {
        if (!m_ownedTemplateSignaturePtr) {
            m_ownedTemplateSignaturePtr = &m_manager->get<TemplateSignature>(m_ownedTemplateSignatureID);
        }
        
        if (m_ownedTemplateSignaturePtr->getTemplate() == this) {
            m_ownedTemplateSignaturePtr->setTemplate(0);
        }

        if (m_ownedElements->count(m_ownedTemplateSignatureID)) {
            m_ownedElements->internalRemove(*m_ownedTemplateSignaturePtr);
        }

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
       if (!m_ownedElements->count(signature->getID())) {
           m_ownedElements->internalAdd(*signature);
       }

       if (signature->getTemplate() != this) {
           signature->setTemplate(this);
       }
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
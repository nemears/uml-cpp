#include "uml/templateableElement.h"
// #include "uml/templateSignature.h"

using namespace UML;

// Set<TemplateSignature, TemplateableElement>& TemplateableElement::getOwnedTemplateSignatureSingleton() {
//     return m_ownedTemplateSignature;
// }

void TemplateableElement::init() {
    // m_ownedTemplateSignature.subsets(*m_ownedElements);
    // m_ownedTemplateSignature.opposite(&TemplateSignature::getTemplateSingleton);
    // m_ownedTemplateSignature.m_signature = &TemplateableElement::getOwnedTemplateSignatureSingleton;
    // m_templateBindings.subsets(*m_ownedElements);
    // m_templateBindings.opposite(&TemplateBinding::getBoundElementSingleton);
    // m_templateBindings.m_signature = &TemplateableElement::getTemplateBindings;
}

TemplateableElement::TemplateableElement() : Element(ElementType::TEMPLATEABLE_ELEMENT) {
    init();
}

TemplateableElement::~TemplateableElement() {

}

// TemplateSignature* TemplateableElement::getOwnedTemplateSignature() {
//     return m_ownedTemplateSignature.get();
// }

// TemplateSignature& TemplateableElement::getOwnedTemplateSignatureRef() {
//     return m_ownedTemplateSignature.getRef();
// }

// ID TemplateableElement::getOwnedTemplateSignatureID() const {
//     return m_ownedTemplateSignature.id();
// }

// bool TemplateableElement::hasOwnedTemplateSignature() const {
//     return m_ownedTemplateSignature.has();
// }

// void TemplateableElement::setOwnedTemplateSignature(TemplateSignature* signature) {
//     m_ownedTemplateSignature.set(signature);
// }

// void TemplateableElement::setOwnedTemplateSignature(TemplateSignature& signature) {
//     m_ownedTemplateSignature.set(signature);
// }

// void TemplateableElement::setOwnedTemplateSignature(ID id) {
//     m_ownedTemplateSignature.set(id);
// }

// Set<TemplateBinding, TemplateableElement>& TemplateableElement::getTemplateBindings() {
//     return m_templateBindings;
// }

bool TemplateableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}
#include "uml/uml-stable.h"

using namespace UML;

Singleton<TemplateSignature, TemplateableElement>& TemplateableElement::getOwnedTemplateSignatureSingleton() {
    return m_ownedTemplateSignature;
}

TemplateableElement::TemplateableElement() : Element(ElementType::TEMPLATEABLE_ELEMENT) {
    m_ownedTemplateSignature.subsets(*m_ownedElements);
    m_ownedTemplateSignature.opposite(&TemplateSignature::getTemplateSingleton);
    m_templateBindings.subsets(*m_ownedElements);
    m_templateBindings.opposite(&TemplateBinding::getBoundElementSingleton);
}

TemplateableElement::~TemplateableElement() {

}

TemplateSignaturePtr TemplateableElement::getOwnedTemplateSignature() const {
    return m_ownedTemplateSignature.get();
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignaturePtr signature) {
    m_ownedTemplateSignature.set(signature);
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature& signature) {
    m_ownedTemplateSignature.set(signature);
}

void TemplateableElement::setOwnedTemplateSignature(ID id) {
    m_ownedTemplateSignature.set(id);
}

Set<TemplateBinding, TemplateableElement>& TemplateableElement::getTemplateBindings() {
    return m_templateBindings;
}

bool TemplateableElement::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}

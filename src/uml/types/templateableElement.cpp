#include "uml/types/templateableElement.h"
#include "uml/types/templateSignature.h"
#include "uml/types/templateBinding.h"
#include "uml/types/parameterableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<TemplateSignature, TemplateableElement>& TemplateableElement::getOwnedTemplateSignatureSingleton() {
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

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature* signature) {
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

bool TemplateableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}
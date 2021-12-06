#include "uml/templateableElement.h"
#include "uml/templateSignature.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateableElement::referencingReleased(ID id) {
    m_ownedTemplateSignature.release(id);
    // m_templateBindings.elementReleased(id, &TemplateableElement::getTemplateBindings);
}

void TemplateableElement::referenceReindexed(ID oldID, ID newID) {
    m_ownedTemplateSignature.reindex(oldID, newID);
    // m_templateBindings.reindex(oldID, newID, &TemplateableElement::getTemplateBindings);
}

void TemplateableElement::restoreReferences() {
    // m_ownedTemplateSignature.restoreReference();
    // m_templateBindings.restoreReferences();
}

void TemplateableElement::referenceErased(ID id) {
    m_ownedTemplateSignature.eraseElement(id);
    // m_templateBindings.elementErased(id);
}

Set<TemplateSignature, TemplateableElement>& TemplateableElement::getOwnedTemplateSignatureSingleton() {
    return m_ownedTemplateSignature;
}

void TemplateableElement::init() {
    m_ownedTemplateSignature.subsets(*m_ownedElements);
    m_ownedTemplateSignature.opposite(&TemplateSignature::getTemplateSingleton);
    m_ownedTemplateSignature.m_signature = &TemplateableElement::getOwnedTemplateSignatureSingleton;
}

void TemplateableElement::copy(const TemplateableElement& rhs) {
    m_ownedTemplateSignature = rhs.m_ownedTemplateSignature;
}

TemplateableElement::TemplateableElement() : Element(ElementType::TEMPLATEABLE_ELEMENT) {
    init();
}

TemplateableElement::TemplateableElement(const TemplateableElement& el) : Element(el, ElementType::TEMPLATEABLE_ELEMENT) {
    // anstract
}

TemplateableElement::~TemplateableElement() {

}

TemplateSignature* TemplateableElement::getOwnedTemplateSignature() {
    return m_ownedTemplateSignature.get();
}

TemplateSignature& TemplateableElement::getOwnedTemplateSignatureRef() {
    return m_ownedTemplateSignature.getRef();
}

ID TemplateableElement::getOwnedTemplateSignatureID() const {
    return m_ownedTemplateSignature.id();
}

bool TemplateableElement::hasOwnedTemplateSignature() const {
    return m_ownedTemplateSignature.has();
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature* signature) {
    m_ownedTemplateSignature.set(signature);
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature& signature) {
    m_ownedTemplateSignature.set(signature);
}

// Sequence<TemplateBinding>& TemplateableElement::getTemplateBindings() {
//     return m_templateBindings;
// }

bool TemplateableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}
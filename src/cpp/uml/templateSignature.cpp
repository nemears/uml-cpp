#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"
// #include "uml/templateParameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateSignature::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_template.release(id);
    // m_ownedParameter.elementReleased(id, &TemplateSignature::getOwnedParameter);
    // m_parameter.elementReleased(id, &TemplateSignature::getParameter);
}

void TemplateSignature::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_template.reindex(oldID, newID);
    // m_ownedParameter.reindex(oldID, newID, &TemplateSignature::getOwnedParameter);
    // m_parameter.reindex(oldID, newID, &TemplateSignature::getParameter);
}

void TemplateSignature::restoreReferences() {
    Element::restoreReferences();
    // m_template.restoreReference();
    // m_ownedParameter.restoreReferences();
    // m_parameter.restoreReferences();
}

void TemplateSignature::referenceErased(ID id) {
    Element::referenceErased(id);
    m_template.eraseElement(id);
    // m_ownedParameter.elementErased(id);
    // m_parameter.elementErased(id);
}

Set<TemplateableElement, TemplateSignature>& TemplateSignature::getTemplateSingleton() {
    return m_template;
}

void TemplateSignature::init() {
    m_template.subsets(*m_owner);
    m_template.opposite(&TemplateableElement::getOwnedTemplateSignatureSingleton);
    m_template.m_signature = &TemplateSignature::getTemplateSingleton;
}

void TemplateSignature::copy(const TemplateSignature& rhs) {
    m_template = rhs.m_template;
}

TemplateSignature::TemplateSignature() : Element(ElementType::TEMPLATE_SIGNATURE) {
    init();
}

TemplateSignature::TemplateSignature(const TemplateSignature& rhs) : Element(rhs, ElementType::TEMPLATE_SIGNATURE) {
    init();
    Element::copy(rhs);
    copy(rhs);
}

TemplateSignature::~TemplateSignature() {

}

TemplateableElement* TemplateSignature::getTemplate() {
    return m_template.get();
}

TemplateableElement& TemplateSignature::getTemplateRef() {
    return m_template.getRef();
}

ID TemplateSignature::getTemplateID() const {
    return m_template.id();
}

bool TemplateSignature::hasTemplate() const {
    return m_template.has();
}

void TemplateSignature::setTemplate(TemplateableElement* temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(TemplateableElement& temp) {
    m_template.set(temp);
}

// Sequence<TemplateParameter>& TemplateSignature::getOwnedParameter() {
//     return m_ownedParameter;
// }

// Sequence<TemplateParameter>& TemplateSignature::getParameter() {
//     return m_parameter;
// }

bool TemplateSignature::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}
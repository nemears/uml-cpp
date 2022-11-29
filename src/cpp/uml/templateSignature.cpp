#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void TemplateSignature::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_parameters.reindex(newID);
}

void TemplateSignature::referenceErased(ID id) {
    Element::referenceErased(id);
    m_parameters.eraseElement(id);
}

TypedSet<TemplateableElement, TemplateSignature>& TemplateSignature::getTemplateSingleton() {
    return m_template;
}

void TemplateSignature::init() {
    m_template.subsets(*m_owner);
    m_template.opposite(&TemplateableElement::getOwnedTemplateSignatureSingleton);
    m_ownedParameters.subsets(m_parameters);
    m_ownedParameters.subsets(*m_ownedElements);
    m_ownedParameters.opposite(&TemplateParameter::getSignatureSingleton);
}

TemplateSignature::TemplateSignature() : Element(ElementType::TEMPLATE_SIGNATURE) {
    init();
}

TemplateSignature::~TemplateSignature() {
    mountAndRelease();
}

TemplateableElementPtr TemplateSignature::getTemplate() const {
    return m_template.get();
}

void TemplateSignature::setTemplate(TemplateableElement* temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(TemplateableElement& temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(ID id) {
    m_template.set(id);
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getOwnedParameters() {
    return m_ownedParameters;
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getParameters() {
    return m_parameters;
}

bool TemplateSignature::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}
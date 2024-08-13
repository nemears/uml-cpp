#include "uml/uml-stable.h"

using namespace UML;

void TemplateSignature::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_parameters);
}

Singleton<TemplateableElement, TemplateSignature>& TemplateSignature::getTemplateSingleton() {
    return m_template;
}

TemplateSignature::TemplateSignature() : Element(ElementType::TEMPLATE_SIGNATURE) {
    m_template.subsets(*m_owner);
    m_template.opposite(&TemplateableElement::getOwnedTemplateSignatureSingleton);
    m_ownedParameters.subsets(m_parameters);
    m_ownedParameters.subsets(*m_ownedElements);
    m_ownedParameters.opposite(&TemplateParameter::getSignatureSingleton);
}

TemplateSignature::~TemplateSignature() {
    
}

TemplateableElementPtr TemplateSignature::getTemplate() const {
    return m_template.get();
}

void TemplateSignature::setTemplate(TemplateableElementPtr temp) {
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

bool TemplateSignature::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}

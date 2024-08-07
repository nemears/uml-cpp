#include "uml/uml-stable.h"

using namespace UML;

void TemplateParameter::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_parameteredElement);
    eraseFromSet(id, m_default);
}

Singleton<TemplateSignature, TemplateParameter>& TemplateParameter::getSignatureSingleton() {
    return m_signature;
}

Singleton<ParameterableElement, TemplateParameter>& TemplateParameter::getParameteredElementSingleton() {
    return m_parameteredElement;
}

Singleton<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedParameteredElementSingleton() {
    return m_ownedParameteredElement;
}

Singleton<ParameterableElement, TemplateParameter>& TemplateParameter::getDefaultSingleton() {
    return m_default;
}

Singleton<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedDefaultSingleton() {
    return m_ownedDefault;
}

TemplateParameter::TemplateParameter() : Element(ElementType::TEMPLATE_PARAMETER) {
    m_signature.subsets(*m_owner);
    m_signature.opposite(&TemplateSignature::getOwnedParameters);
    m_parameteredElement.opposite(&ParameterableElement::getTemplateParameterSingleton);
    m_ownedParameteredElement.subsets(m_parameteredElement);
    m_ownedParameteredElement.subsets(*m_ownedElements);
    m_ownedParameteredElement.opposite(&ParameterableElement::getOwningTemplateParameterSingleton);
    m_ownedDefault.subsets(m_default);
    m_ownedDefault.subsets(*m_ownedElements);
}

TemplateParameter::~TemplateParameter() {
    
}

TemplateSignaturePtr TemplateParameter::getSignature() const {
    return m_signature.get();
}

void TemplateParameter::setSignature(TemplateSignature* signature) {
    m_signature.set(signature);
}

void TemplateParameter::setSignature(TemplateSignature& signature) {
    m_signature.set(signature);
}

void TemplateParameter::setSignature(ID id) {
    m_signature.set(id);
}

ParameterableElementPtr TemplateParameter::getOwnedParameteredElement() const {
    return m_ownedParameteredElement.get();
}

void TemplateParameter::setOwnedParameteredElement(ParameterableElement* el) {
    m_ownedParameteredElement.set(el);
}

void TemplateParameter::setOwnedParameteredElement(ParameterableElement& el) {
    m_ownedParameteredElement.set(el);
}

void TemplateParameter::setOwnedParameteredElement(ID id) {
    m_ownedParameteredElement.set(id);
}

ParameterableElementPtr TemplateParameter::getParameteredElement() const {
    return m_parameteredElement.get();
}

void TemplateParameter::setParameteredElement(ParameterableElement* el) {
    m_parameteredElement.set(el);
}

void TemplateParameter::setParameteredElement(ParameterableElement& el) {
    m_parameteredElement.set(el);
}

void TemplateParameter::setParameteredElement(ID id) {
    m_parameteredElement.set(id);
}

ParameterableElementPtr TemplateParameter::getDefault() const {
    return m_default.get();
}

void TemplateParameter::setDefault(ParameterableElement* el) {
    m_default.set(el);
}

void TemplateParameter::setDefault(ParameterableElement& el) {
    m_default.set(el);
}

void TemplateParameter::setDefault(ID id) {
    m_default.set(id);
}

ParameterableElementPtr TemplateParameter::getOwnedDefault() const {
    return m_ownedDefault.get();
}

void TemplateParameter::setOwnedDefault(ParameterableElement* el) {
    m_ownedDefault.set(el);
}

void TemplateParameter::setOwnedDefault(ParameterableElement& el) {
    m_ownedDefault.set(el);
}

void TemplateParameter::setOwnedDefault(ID id) {
    m_ownedDefault.set(id);
}

bool TemplateParameter::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}

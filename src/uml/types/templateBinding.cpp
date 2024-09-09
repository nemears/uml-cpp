#include "uml/uml-stable.h"

using namespace UML;

Singleton<TemplateableElement, TemplateBinding>& TemplateBinding::getBoundElementSingleton() {
    return m_boundElement;
}

Singleton<TemplateSignature, TemplateBinding>& TemplateBinding::getSignatureSingleton() {
    return m_signature;
}

TemplateBinding::TemplateBinding(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_boundElement.subsets(m_sources);
    m_boundElement.subsets(m_owner);
    m_boundElement.opposite(&TemplateableElement::getTemplateBindings);
    m_signature.subsets(m_targets);
    m_parameterSubstitutions.subsets(m_ownedElements);
    m_parameterSubstitutions.opposite(&TemplateParameterSubstitution::getTemplateBindingSingleton);
}

TemplateableElementPtr TemplateBinding::getBoundElement() const {
    return m_boundElement.get();
}

void TemplateBinding::setBoundElement(TemplateableElementPtr el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(TemplateableElement& el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(ID id) {
    m_boundElement.set(id);
}

TemplateSignaturePtr TemplateBinding::getSignature() const {
    return m_signature.get();
}

void TemplateBinding::setSignature(TemplateSignaturePtr signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(TemplateSignature& signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(ID id) {
    m_signature.set(id);
}

Set<TemplateParameterSubstitution, TemplateBinding>& TemplateBinding::getParameterSubstitutions() {
    return m_parameterSubstitutions;
}

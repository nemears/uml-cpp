#include "uml/templateParameter.h"
#include "uml/templateSignature.h"
#include "uml/parameterableElement.h"
#include "uml/templateableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void TemplateParameter::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_parameteredElement.reindex(newID);
    m_default.reindex(newID);
}

void TemplateParameter::referenceErased(ID id) {
    Element::referenceErased(id);
    m_parameteredElement.eraseElement(id);
    m_default.eraseElement(id);
}

TypedSet<TemplateSignature, TemplateParameter>& TemplateParameter::getSignatureSingleton() {
    return m_signature;
}

TypedSet<ParameterableElement, TemplateParameter>& TemplateParameter::getParameteredElementSingleton() {
    return m_parameteredElement;
}

TypedSet<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedParameteredElementSingleton() {
    return m_ownedParameteredElement;
}

TypedSet<ParameterableElement, TemplateParameter>& TemplateParameter::getDefaultSingleton() {
    return m_default;
}

TypedSet<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedDefaultSingleton() {
    return m_ownedDefault;
}

TemplateParameter::TemplateParameter() : Element(ElementType::TEMPLATE_PARAMETER) {
    m_signature.subsets(*m_owner);
    m_signature.opposite(&TemplateSignature::getOwnedParameters);
    m_parameteredElement.opposite(&ParameterableElement::getTemplateParameterSingleton);
    m_ownedParameteredElement.subsets(m_parameteredElement);
    m_ownedParameteredElement.subsets(*m_ownedElements);
    m_ownedParameteredElement.opposite(&ParameterableElement::getOwningTemplateParameterSingleton);
    m_default.opposite(&ParameterableElement::getTemplateParameterSingleton);
    m_ownedDefault.subsets(m_default);
    m_ownedDefault.subsets(*m_ownedElements);
    m_ownedDefault.opposite(&ParameterableElement::getOwningTemplateParameterSingleton); //?
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

bool TemplateParameter::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}
#include "uml/templateParameter.h"
#include "uml/templateSignature.h"
#include "uml/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateParameter::referencingReleased(ID id) {
    m_signature.release(id);
    m_ownedParameteredElement.release(id);
    m_parameteredElement.release(id);
    m_default.release(id);
    m_ownedDefault.release(id);
}

void TemplateParameter::referenceReindexed(ID oldID, ID newID) {
    m_signature.reindex(oldID, newID);
    m_ownedParameteredElement.reindex(oldID, newID);
    m_parameteredElement.reindex(oldID, newID);
    m_default.reindex(oldID, newID);
    m_ownedDefault.reindex(oldID, newID);
}

void TemplateParameter::restoreReferences() {
    // Element::restoreReferences();
    // m_signature.restoreReference();
    // m_ownedParameteredElement.restoreReference();
    // m_parameteredElement.restoreReference();
    // m_default.restoreReference();
    // m_ownedDefault.restoreReference();
}

void TemplateParameter::referenceErased(ID id) {
    m_signature.eraseElement(id);
    m_ownedParameteredElement.eraseElement(id);
    m_parameteredElement.eraseElement(id);
    m_default.eraseElement(id);
    m_ownedDefault.eraseElement(id);
}

Set<TemplateSignature, TemplateParameter>& TemplateParameter::getSignatureSingleton() {
    return m_signature;
}

Set<ParameterableElement, TemplateParameter>& TemplateParameter::getParameteredElementSingleton() {
    return m_parameteredElement;
}

Set<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedParameteredElementSingleton() {
    return m_ownedParameteredElement;
}

Set<ParameterableElement, TemplateParameter>& TemplateParameter::getDefaultSingleton() {
    return m_default;
}

Set<ParameterableElement, TemplateParameter>& TemplateParameter::getOwnedDefaultSingleton() {
    return m_ownedDefault;
}

void TemplateParameter::init() {
    m_signature.subsets(*m_owner);
    m_signature.opposite(&TemplateSignature::getOwnedParametersSet);
    m_signature.m_signature = &TemplateParameter::getSignatureSingleton;
    m_parameteredElement.opposite(&ParameterableElement::getTemplateParameterSingleton);
    m_parameteredElement.m_signature = &TemplateParameter::getParameteredElementSingleton;
    m_ownedParameteredElement.subsets(m_parameteredElement);
    m_ownedParameteredElement.subsets(*m_ownedElements);
    m_ownedParameteredElement.opposite(&ParameterableElement::getOwningTemplateParameterSingleton);
    m_ownedParameteredElement.m_signature = &TemplateParameter::getOwnedParameteredElementSingleton;
    m_default.m_signature = &TemplateParameter::getDefaultSingleton;
    m_ownedDefault.subsets(m_default);
    m_ownedDefault.subsets(*m_ownedElements);
    m_ownedDefault.m_signature = &TemplateParameter::getOwnedDefaultSingleton;
}

void TemplateParameter::copy(const TemplateParameter& rhs) {
    m_signature = rhs.m_signature;
    m_parameteredElement = rhs.m_parameteredElement;
    m_ownedParameteredElement = rhs.m_ownedParameteredElement;
    m_default = rhs.m_default;
    m_ownedDefault = rhs.m_ownedDefault;
}

TemplateParameter::TemplateParameter() : Element(ElementType::TEMPLATE_PARAMETER) {
    init();
}

TemplateParameter::TemplateParameter(const TemplateParameter& rhs) : Element(rhs, ElementType::TEMPLATE_PARAMETER) {
    init();
    Element::copy(rhs);
    copy(rhs);
}

TemplateParameter::~TemplateParameter() {

}

TemplateSignature* TemplateParameter::getSignature() {
    return m_signature.get();
}

TemplateSignature& TemplateParameter::getSignatureRef() {
    return m_signature.getRef();
}

ID TemplateParameter::getSignatureID() const {
    return m_signature.id();
}

bool TemplateParameter::hasSignature() const {
    return m_signature.has();
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

ParameterableElement* TemplateParameter::getOwnedParameteredElement() {
    return m_ownedParameteredElement.get();
}

ParameterableElement& TemplateParameter::getOwnedParameteredElementRef() {
    return m_ownedParameteredElement.getRef();
}

ID TemplateParameter::getOwnedParameteredElementID() const {
    return m_ownedParameteredElement.id();
}

bool TemplateParameter::hasOwnedParameteredElement() const {
    return m_ownedParameteredElement.has();
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

ParameterableElement* TemplateParameter::getParameteredElement() {
    return m_parameteredElement.get();
}

ParameterableElement& TemplateParameter::getParameteredElementRef() {
    return m_parameteredElement.getRef();
}

ID TemplateParameter::getParameteredElementID() const {
    return m_parameteredElement.id();
}

bool TemplateParameter::hasParameteredElement() const {
    return m_parameteredElement.has();
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

ParameterableElement* TemplateParameter::getDefault() {
    return m_default.get();
}

ParameterableElement& TemplateParameter::getDefaultRef() {
    return m_default.getRef();
}

ID TemplateParameter::getDefaultID() const {
    return m_default.id();
}

bool TemplateParameter::hasDefault() const {
    return m_default.has();
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

ParameterableElement* TemplateParameter::getOwnedDefault() {
    return m_ownedDefault.get();
}

ParameterableElement& TemplateParameter::getOwnedDefaultRef() {
    return m_ownedDefault.getRef();
}

ID TemplateParameter::getOwnedDefaultID() const {
    return m_ownedDefault.id();
}

bool TemplateParameter::hasOwnedDefault() const {
    return m_ownedDefault.has();
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
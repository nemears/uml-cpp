#include "uml/templateParameter.h"
#include "uml/templateSignature.h"
#include "uml/parameterableElement.h"
#include "uml/templateableElement.h"
#include "uml/umlPtr.h"

using namespace UML;

void TemplateParameter::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_parameteredElement.release(id);
    m_default.release(id);
}

void TemplateParameter::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_parameteredElement.reindex(oldID, newID);
    m_default.reindex(oldID, newID);
}

void TemplateParameter::reindexName(ID id, std::string newName) {
    Element::reindexName(id, newName);
    m_parameteredElement.reindexName(id, newName);
    m_default.reindexName(id, newName);
}

void TemplateParameter::referenceErased(ID id) {
    Element::referenceErased(id);
    m_parameteredElement.eraseElement(id);
    m_default.eraseElement(id);
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
    m_default.opposite(&ParameterableElement::getTemplateParameterSingleton);
    m_default.m_signature = &TemplateParameter::getDefaultSingleton;
    m_ownedDefault.subsets(m_default);
    m_ownedDefault.subsets(*m_ownedElements);
    m_ownedDefault.opposite(&ParameterableElement::getOwningTemplateParameterSingleton);
    m_ownedDefault.m_signature = &TemplateParameter::getOwnedDefaultSingleton;
}

TemplateParameter::TemplateParameter() : Element(ElementType::TEMPLATE_PARAMETER) {
    init();
}

TemplateParameter::~TemplateParameter() {
    mountAndRelease();
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
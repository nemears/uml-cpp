#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateableElement.h"
#include "uml/templateSignature.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateBinding::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_boundElement.release(id);
    m_signature.release(id);
    m_parameterSubstitution.release(id);
}

void TemplateBinding::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_boundElement.reindex(oldID, newID);
    m_signature.reindex(oldID, newID);
    m_parameterSubstitution.reindex(oldID, newID);
}

void TemplateBinding::restoreReferences() {
    DirectedRelationship::restoreReferences();
    // m_boundElement.restoreReference();
    // m_signature.restoreReference();
}

void TemplateBinding::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_boundElement.eraseElement(id);
    m_signature.eraseElement(id);
    m_parameterSubstitution.eraseElement(id);
}

Set<TemplateableElement, TemplateBinding>& TemplateBinding::getBoundElementSingleton() {
    return m_boundElement;
}

Set<TemplateSignature, TemplateBinding>& TemplateBinding::getSignatureSingleton() {
    return m_signature;
}

void TemplateBinding::init() {
    m_boundElement.subsets(m_sources);
    m_boundElement.subsets(*m_owner);
    m_boundElement.opposite(&TemplateableElement::getTemplateBindings);
    m_boundElement.m_signature = &TemplateBinding::getBoundElementSingleton;
    m_signature.subsets(m_targets);
    m_signature.m_signature = &TemplateBinding::getSignatureSingleton;
    m_parameterSubstitution.subsets(*m_ownedElements);
    m_parameterSubstitution.opposite(&TemplateParameterSubstitution::getTemplateBindingSingleton);
    m_parameterSubstitution.m_signature = &TemplateBinding::getParameterSubstitution;
}

void TemplateBinding::copy(const TemplateBinding& rhs) {
    m_boundElement = rhs.m_boundElement;
    m_signature = rhs.m_signature;
    m_parameterSubstitution = rhs.m_parameterSubstitution;
}

TemplateBinding::TemplateBinding() : Element(ElementType::TEMPLATE_BINDING) {
    init();
}

TemplateBinding::TemplateBinding(const TemplateBinding& rhs) : Element(rhs, ElementType::TEMPLATE_BINDING) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    copy(rhs);
}

TemplateBinding::~TemplateBinding() {

}

TemplateableElement* TemplateBinding::getBoundElement() {
    return m_boundElement.get();
}

TemplateableElement& TemplateBinding::getBoundElementRef() {
    return m_boundElement.getRef();
}

ID TemplateBinding::getBoundElementID() const {
    return m_boundElement.id();
}

bool TemplateBinding::hasBoundElement() const {
    return m_boundElement.has();
}

void TemplateBinding::setBoundElement(TemplateableElement* el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(TemplateableElement& el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(ID id) {
    m_boundElement.set(id);
}

TemplateSignature* TemplateBinding::getSignature() {
    return m_signature.get();
}

TemplateSignature& TemplateBinding::getSignatureRef() {
    return m_signature.getRef();
}

ID TemplateBinding::getSignatureID() const {
    return m_signature.id();
}

bool TemplateBinding::hasSignature() const {
    return m_signature.has();
}

void TemplateBinding::setSignature(TemplateSignature* signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(TemplateSignature& signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(ID id) {
    m_signature.set(id);
}

Set<TemplateParameterSubstitution, TemplateBinding>& TemplateBinding::getParameterSubstitution() {
    return m_parameterSubstitution;
}

bool TemplateBinding::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_BINDING;
    }

    return ret;
}
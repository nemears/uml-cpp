#include "uml/templateParameterSubstitution.h"
#include "uml/umlManager.h"
#include "uml/templateBinding.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateParameterSubstitution::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_formal.release(id);
    m_templateBinding.release(id);
    m_actual.release(id);
    m_ownedActual.release(id);
}

void TemplateParameterSubstitution::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_formal.reindex(oldID, newID);
    m_templateBinding.reindex(oldID, newID);
    m_actual.reindex(oldID, newID);
    m_ownedActual.reindex(oldID, newID);
}

void TemplateParameterSubstitution::restoreReferences() {
    Element::restoreReferences();
    // m_formal.restoreReference();
    // m_templateBinding.restoreReference();
    // m_actual.restoreReference();
    // m_ownedActual.restoreReference();
}

void TemplateParameterSubstitution::referenceErased(ID id) {
    Element::referenceErased(id);
    m_formal.eraseElement(id);
    m_templateBinding.eraseElement(id);
    m_actual.eraseElement(id);
    m_ownedActual.eraseElement(id);
}

Set<TemplateParameter, TemplateParameterSubstitution>& TemplateParameterSubstitution::getFormalSingleton() {
    return m_formal;
}

Set<TemplateBinding, TemplateParameterSubstitution>& TemplateParameterSubstitution::getTemplateBindingSingleton() {
    return m_templateBinding;
}

Set<ParameterableElement, TemplateParameterSubstitution>& TemplateParameterSubstitution::getActualSingleton() {
    return m_actual;
}

Set<ParameterableElement, TemplateParameterSubstitution>& TemplateParameterSubstitution::getOwnedActualSingleton() {
    return m_ownedActual;
}

void TemplateParameterSubstitution::init() {
    m_formal.m_signature = &TemplateParameterSubstitution::getFormalSingleton;
    m_templateBinding.subsets(*m_owner);
    m_templateBinding.opposite(&TemplateBinding::getParameterSubstitution);
    m_templateBinding.m_signature = &TemplateParameterSubstitution::getTemplateBindingSingleton;
    m_actual.m_signature = &TemplateParameterSubstitution::getActualSingleton;
    m_ownedActual.subsets(*m_ownedElements);
    m_ownedActual.subsets(m_actual);
    m_ownedActual.m_signature = &TemplateParameterSubstitution::getOwnedActualSingleton;
}

void TemplateParameterSubstitution::copy(const TemplateParameterSubstitution& rhs) {
    m_formal = rhs.m_formal;
    m_templateBinding = rhs.m_templateBinding;
    m_actual = rhs.m_actual;
    m_ownedActual = rhs.m_ownedActual;
}

TemplateParameterSubstitution::TemplateParameterSubstitution() : Element(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    init();
}

TemplateParameterSubstitution::TemplateParameterSubstitution(const TemplateParameterSubstitution& rhs) : Element(rhs, ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    init();
    Element::copy(rhs);
    copy(rhs);
}

TemplateParameterSubstitution::~TemplateParameterSubstitution() {

}

TemplateParameter* TemplateParameterSubstitution::getFormal() {
    return m_formal.get();
}

TemplateParameter& TemplateParameterSubstitution::getFormalRef() {
    return m_formal.getRef();
}

ID TemplateParameterSubstitution::getFormalID() const {
    return m_formal.id();
}

bool TemplateParameterSubstitution::hasFormal() const {
    return m_formal.has();
}

void TemplateParameterSubstitution::setFormal(TemplateParameter* formal) {
    m_formal.set(formal);
}

void TemplateParameterSubstitution::setFormal(TemplateParameter& formal) {
    m_formal.set(formal);
}

TemplateBinding* TemplateParameterSubstitution::getTemplateBinding() {
    return m_templateBinding.get();
}

TemplateBinding& TemplateParameterSubstitution::getTemplateBindingRef() {
    return m_templateBinding.getRef();
}

ID TemplateParameterSubstitution::getTemplateBindingID() const {
    return m_templateBinding.id();
}

bool TemplateParameterSubstitution::hasTemplateBinding() const {
    return m_templateBinding.has();
}

void TemplateParameterSubstitution::setTemplateBinding(TemplateBinding* bind) {
    m_templateBinding.set(bind);
}

void TemplateParameterSubstitution::setTemplateBinding(TemplateBinding& bind) {
    m_templateBinding.set(bind);
}

ParameterableElement* TemplateParameterSubstitution::getActual() {
    return m_actual.get();
}

ParameterableElement& TemplateParameterSubstitution::getActualRef() {
    return m_actual.getRef();
}

ID TemplateParameterSubstitution::getActualID() const {
    return m_actual.id();
}

bool TemplateParameterSubstitution::hasActual() const {
    return m_actual.has();
}

void TemplateParameterSubstitution::setActual(ParameterableElement* actual) {
    m_actual.set(actual);
}

void TemplateParameterSubstitution::setActual(ParameterableElement& actual) {
    m_actual.set(actual);
}

ParameterableElement* TemplateParameterSubstitution::getOwnedActual() {
    return m_ownedActual.get();
}

ParameterableElement& TemplateParameterSubstitution::getOwnedActualRef() {
    return m_ownedActual.getRef();
}

ID TemplateParameterSubstitution::getOwnedActualID() const {
    return m_ownedActual.id();
}

bool TemplateParameterSubstitution::hasOwnedActual() const {
    return m_ownedActual.has();
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement* actual) {
    m_ownedActual.set(actual);
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement& actual) {
    m_ownedActual.set(actual);
}

bool TemplateParameterSubstitution::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    }

    return ret;
}
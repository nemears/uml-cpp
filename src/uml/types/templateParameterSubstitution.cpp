#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateParameterSubstitution::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_formal);
    eraseFromSet(id, m_actual);
}

Singleton<TemplateParameter, TemplateParameterSubstitution>& TemplateParameterSubstitution::getFormalSingleton() {
    return m_formal;
}

Singleton<TemplateBinding, TemplateParameterSubstitution>& TemplateParameterSubstitution::getTemplateBindingSingleton() {
    return m_templateBinding;
}

Singleton<ParameterableElement, TemplateParameterSubstitution>& TemplateParameterSubstitution::getActualSingleton() {
    return m_actual;
}

Singleton<ParameterableElement, TemplateParameterSubstitution>& TemplateParameterSubstitution::getOwnedActualSingleton() {
    return m_ownedActual;
}

TemplateParameterSubstitution::TemplateParameterSubstitution() : Element(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    m_templateBinding.subsets(*m_owner);
    m_templateBinding.opposite(&TemplateBinding::getParameterSubstitutions);
    m_ownedActual.subsets(*m_ownedElements);
    m_ownedActual.subsets(m_actual);
}

TemplateParameterSubstitution::~TemplateParameterSubstitution() {
    
}

TemplateParameterPtr TemplateParameterSubstitution::getFormal() const {
    return m_formal.get();
}

void TemplateParameterSubstitution::setFormal(TemplateParameterPtr formal) {
    m_formal.set(formal);
}

void TemplateParameterSubstitution::setFormal(TemplateParameter& formal) {
    m_formal.set(formal);
}

void TemplateParameterSubstitution::setFormal(ID id) {
    m_formal.set(id);
}

TemplateBindingPtr TemplateParameterSubstitution::getTemplateBinding() const {
    return m_templateBinding.get();
}

void TemplateParameterSubstitution::setTemplateBinding(TemplateBindingPtr bind) {
    m_templateBinding.set(bind);
}

void TemplateParameterSubstitution::setTemplateBinding(TemplateBinding& bind) {
    m_templateBinding.set(bind);
}

void TemplateParameterSubstitution::setTemplateBinding(ID id) {
    m_templateBinding.set(id);
}

ParameterableElementPtr TemplateParameterSubstitution::getActual() const {
    return m_actual.get();
}

void TemplateParameterSubstitution::setActual(ParameterableElementPtr actual) {
    m_actual.set(actual);
}

void TemplateParameterSubstitution::setActual(ParameterableElement& actual) {
    m_actual.set(actual);
}

void TemplateParameterSubstitution::setActual(ID id) {
    m_actual.set(id);
}

ParameterableElementPtr TemplateParameterSubstitution::getOwnedActual() const {
    return m_ownedActual.get();
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElementPtr actual) {
    m_ownedActual.set(actual);
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement& actual) {
    m_ownedActual.set(actual);
}

void TemplateParameterSubstitution::setOwnedActual(ID id) {
    m_ownedActual.set(id);
}

bool TemplateParameterSubstitution::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    }

    return ret;
}

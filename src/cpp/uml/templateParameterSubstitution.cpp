#include "uml/templateParameterSubstitution.h"
#include "uml/umlManager.h"
#include "uml/templateBinding.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"
#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/setReferenceFunctor.h"
#include "uml/umlPtr.h"

using namespace UML;

void TemplateParameterSubstitution::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_formal.release(id);
    m_actual.release(id);
}

void TemplateParameterSubstitution::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_formal.reindex(oldID, newID);
    m_actual.reindex(oldID, newID);
}

void TemplateParameterSubstitution::reindexName(ID id, std::string newName) {
    Element::reindexName(id, newName);
    m_formal.reindexName(id, newName);
    m_actual.reindexName(id, newName);
}

void TemplateParameterSubstitution::restoreReference(Element* el) {
    Element::restoreReference(el);
    m_formal.restore(el);
    m_actual.restore(el);
    if (m_formal.get().id() == el->getID()) {
        el->setReference(this);
    }
    if (m_actual.get().id() == el->getID()) {
        el->setReference(this);
    }
}

void TemplateParameterSubstitution::referenceErased(ID id) {
    Element::referenceErased(id);
    m_formal.eraseElement(id);
    m_actual.eraseElement(id);
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
    m_formal.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_formal.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_templateBinding.subsets(*m_owner);
    m_templateBinding.opposite(&TemplateBinding::getParameterSubstitution);
    m_templateBinding.m_signature = &TemplateParameterSubstitution::getTemplateBindingSingleton;
    m_actual.m_signature = &TemplateParameterSubstitution::getActualSingleton;
    m_actual.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_actual.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_ownedActual.subsets(*m_ownedElements);
    m_ownedActual.subsets(m_actual);
    m_ownedActual.m_signature = &TemplateParameterSubstitution::getOwnedActualSingleton;
}

TemplateParameterSubstitution::TemplateParameterSubstitution() : Element(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    init();
}

TemplateParameterSubstitution::~TemplateParameterSubstitution() {
    mountAndRelease();
}

TemplateParameterPtr TemplateParameterSubstitution::getFormal() const {
    return m_formal.get();
}

void TemplateParameterSubstitution::setFormal(TemplateParameter* formal) {
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

void TemplateParameterSubstitution::setTemplateBinding(TemplateBinding* bind) {
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

void TemplateParameterSubstitution::setActual(ParameterableElement* actual) {
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

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement* actual) {
    m_ownedActual.set(actual);
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement& actual) {
    m_ownedActual.set(actual);
}

void TemplateParameterSubstitution::setOwnedActual(ID id) {
    m_ownedActual.set(id);
}

bool TemplateParameterSubstitution::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    }

    return ret;
}
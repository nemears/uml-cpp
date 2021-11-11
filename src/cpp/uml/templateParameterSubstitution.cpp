#include "uml/templateParameterSubstitution.h"
#include "uml/umlManager.h"
#include "uml/templateBinding.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"

using namespace UML;

void TemplateParameterSubstitution::RemoveFormalProcedure::operator()(TemplateParameter* el) const {
    el->removeReference(m_me->getID());
}

void TemplateParameterSubstitution::AddFormalProcedure::operator()(TemplateParameter* el) const {
    if (!el->m_node->m_references.count(m_me->getID())) {
        el->setReference(m_me);
    }
}

void TemplateParameterSubstitution::RemoveTemplateBindingProcedure::operator()(TemplateBinding* el) const {
    if (el->getParameterSubstitution().count(m_me->getID())) {
        el->getParameterSubstitution().remove(*m_me);
    }
}

void TemplateParameterSubstitution::AddTemplateBindingProcedure::operator()(TemplateBinding* el) const {
    if (!el->getParameterSubstitution().count(m_me->getID())) {
        el->getParameterSubstitution().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void TemplateParameterSubstitution::AddTemplateBindingProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void TemplateParameterSubstitution::RemoveActualProcedure::operator()(ParameterableElement* el) const {
    el->removeReference(m_me->getID());
}

void TemplateParameterSubstitution::AddActualProcedure::operator()(ParameterableElement* el) const {
    el->setReference(m_me);
}

void TemplateParameterSubstitution::RemoveOwnedActualProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasActual()) {
        m_me->setActual(0);
    }
    if (m_me->getOwnedElements().contains(el->getID())) {
        m_me->getOwnedElements().remove(*el);
    }
}

void TemplateParameterSubstitution::AddOwnedActualProcedure::operator()(ParameterableElement* el) const {
    if (m_me->getActualID() != el->getID()) {
        m_me->setActual(el);
    }
    if (!m_me->getOwnedElements().contains(el->getID())) {
        m_me->getOwnedElements().add(*el);
    }
    if (el->getOwnerID() != m_me->getID()) {
        el->setOwner(m_me);
    }
}

void TemplateParameterSubstitution::AddOwnedActualProcedure::operator()(ID id) const {
    if (m_me->getActualID() != id) {
        m_me->m_actual.setByID(id);
    }
    if (!m_me->getOwnedElements().contains(id)) {
        m_me->getOwnedElements().add(id);
    }
}

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
    m_formal.restoreReference();
    m_templateBinding.restoreReference();
    m_actual.restoreReference();
    m_ownedActual.restoreReference();
}

void TemplateParameterSubstitution::referenceErased(ID id) {
    Element::referenceErased(id);
    m_formal.elementErased(id);
    m_templateBinding.elementErased(id);
    m_actual.elementErased(id);
    m_ownedActual.elementErased(id);
}

TemplateParameterSubstitution::TemplateParameterSubstitution() : Element(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    m_formal.m_signature = &TemplateParameterSubstitution::m_formal;
    m_formal.m_addProcedures.push_back(new AddFormalProcedure(this));
    m_formal.m_removeProcedures.push_back(new RemoveFormalProcedure(this));
    m_templateBinding.m_signature = &TemplateParameterSubstitution::m_templateBinding;
    m_templateBinding.m_addProcedures.push_back(new AddTemplateBindingProcedure(this));
    m_templateBinding.m_removeProcedures.push_back(new RemoveTemplateBindingProcedure(this));
    m_actual.m_signature = &TemplateParameterSubstitution::m_actual;
    m_actual.m_addProcedures.push_back(new AddActualProcedure(this));
    m_actual.m_removeProcedures.push_back(new RemoveActualProcedure(this));
    m_ownedActual.m_signature = &TemplateParameterSubstitution::m_ownedActual;
    m_ownedActual.m_addProcedures.push_back(new AddOwnedActualProcedure(this));
    m_ownedActual.m_removeProcedures.push_back(new RemoveOwnedActualProcedure(this));
}

TemplateParameterSubstitution::TemplateParameterSubstitution(const TemplateParameterSubstitution& temp) : Element(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
    m_formal = temp.m_formal;
    m_formal.m_me = this;
    m_formal.m_addProcedures.clear();
    m_formal.m_removeProcedures.clear();
    m_formal.m_addProcedures.push_back(new AddFormalProcedure(this));
    m_formal.m_removeProcedures.push_back(new RemoveFormalProcedure(this));
    m_templateBinding = temp.m_templateBinding;
    m_templateBinding.m_me = this;
    m_templateBinding.m_addProcedures.clear();
    m_templateBinding.m_removeProcedures.clear();
    m_templateBinding.m_addProcedures.push_back(new AddTemplateBindingProcedure(this));
    m_templateBinding.m_removeProcedures.push_back(new RemoveTemplateBindingProcedure(this));
    m_actual = temp.m_actual;
    m_actual.m_me = this;
    m_actual.m_addProcedures.clear();
    m_actual.m_removeProcedures.clear();
    m_actual.m_addProcedures.push_back(new AddActualProcedure(this));
    m_actual.m_removeProcedures.push_back(new RemoveActualProcedure(this));
    m_ownedActual = temp.m_ownedActual;
    m_ownedActual.m_me = this;
    m_ownedActual.m_removeProcedures.clear();
    m_ownedActual.m_addProcedures.clear();
    m_ownedActual.m_addProcedures.push_back(new AddOwnedActualProcedure(this));
    m_ownedActual.m_removeProcedures.push_back(new RemoveOwnedActualProcedure(this));
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
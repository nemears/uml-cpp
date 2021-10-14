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
    el->setReference(m_me);
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
}

void TemplateParameterSubstitution::RemoveActualProcedure::operator()(ParameterableElement* el) const {
    el->removeReference(el->getID());
}

void TemplateParameterSubstitution::AddActualProcedure::operator()(ParameterableElement* el) const {
    el->setReference(el);
}

void TemplateParameterSubstitution::RemoveOwnedActualProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasActual()) {
        m_me->setActual(0);
    }
    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void TemplateParameterSubstitution::AddOwnedActualProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasActual()) {
        if (m_me->getActualRef() != *el) {
            m_me->setActual(el);
        }
    }
    else {
        m_me->setActual(el);
    }
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

void TemplateParameterSubstitution::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_formal.id() == id) {
        m_formal.release();
    }
    if (m_templateBinding.id() == id) {
        m_templateBinding.release();
    }
    if (m_actual.id() == id) {
        m_actual.release();
    }
    if (m_ownedActual.id() == id) {
        m_ownedActual.release();
    }
}

void TemplateParameterSubstitution::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    if (m_formal.id() == oldID) {
        m_formal.reindex(oldID, newID);
    }
    if (m_templateBinding.id() == oldID) {
        m_templateBinding.reindex(oldID, newID);
    }
    if (m_actual.id() == oldID) {
        m_actual.reindex(oldID, newID);
    }
    if (m_ownedActual.id() == oldID) {
        m_ownedActual.reindex(oldID, newID);
    }
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

TemplateParameterSubstitution::TemplateParameterSubstitution() {
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

TemplateParameterSubstitution::TemplateParameterSubstitution(const TemplateParameterSubstitution& temp) {
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

bool TemplateParameterSubstitution::hasActual() const {
    return m_actual.has();
}

void TemplateParameterSubstitution::setActual(ParameterableElement* actual) {
    m_actual.set(actual);
}

ParameterableElement* TemplateParameterSubstitution::getOwnedActual() {
    return m_ownedActual.get();
}

ParameterableElement& TemplateParameterSubstitution::getOwnedActualRef() {
    return m_ownedActual.getRef();
}

bool TemplateParameterSubstitution::hasOwnedActual() const {
    return m_ownedActual.has();
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement* actual) {
    m_ownedActual.set(actual);
}

ElementType TemplateParameterSubstitution::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
}

bool TemplateParameterSubstitution::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    }

    return ret;
}
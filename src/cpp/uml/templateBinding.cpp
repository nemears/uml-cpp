#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateableElement.h"
#include "uml/templateSignature.h"

using namespace UML;

void TemplateBinding::RemoveBoundElementProcedure::operator()(TemplateableElement* el) const {
    if (m_me->getSources().count(el->getID())) {
        m_me->getSources().remove(*el);
    }
    if (el->getTemplateBindings().count(m_me->getID())) {
        el->getTemplateBindings().remove(*m_me);
    }
    if (m_me->getOwnerID() == el->getID()) {
        m_me->setOwner(0);
    }
}

void TemplateBinding::AddBoundElementProcedure::operator()(TemplateableElement* el) const {
    if (!m_me->getSources().count(el->getID())) {
        m_me->getSources().add(*el);
    }
    if (!el->getTemplateBindings().count(m_me->getID())) {
        el->getTemplateBindings().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void TemplateBinding::AddBoundElementProcedure::operator()(ID id) const {
    if (!m_me->getSources().count(id)) {
        m_me->getSources().addByID(id);
    }
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void TemplateBinding::RemoveSignatureProcedure::operator()(TemplateSignature* el) const {
    if (m_me->getTargets().count(el->getID())) {
        m_me->getTargets().remove(*el);
    }
}

void TemplateBinding::AddSignatureProcedure::operator()(TemplateSignature* el) const {
    if (!m_me->getTargets().count(el->getID())) {
        m_me->getTargets().add(*el);
    }
}

void TemplateBinding::AddParameterSubstitutionFunctor::operator()(TemplateParameterSubstitution& el) const {
    if(el.getTemplateBinding() != m_el) {
        el.setTemplateBinding(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    updateCopiedSequenceAddedTo(el, &TemplateBinding::getParameterSubstitution);
}

void TemplateBinding::AddParameterSubstitutionFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
}

void TemplateBinding::RemoveParameterSubstitutionFunctor::operator()(TemplateParameterSubstitution& el) const {
    if(el.getTemplateBinding() == m_el) {
        el.setTemplateBinding(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &TemplateBinding::getParameterSubstitution);
}

void TemplateBinding::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_boundElement.release(id);
    m_signature.release(id);
}

void TemplateBinding::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_boundElement.reindex(oldID, newID);
    m_signature.reindex(oldID, newID);
}

void TemplateBinding::restoreReferences() {
    DirectedRelationship::restoreReferences();
    m_boundElement.restoreReference();
    m_signature.restoreReference();
}

void TemplateBinding::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_boundElement.elementErased(id);
    m_signature.elementErased(id);
}

TemplateBinding::TemplateBinding() {
    m_boundElement.m_signature = &TemplateBinding::m_boundElement;
    m_boundElement.m_removeProcedures.push_back(new RemoveBoundElementProcedure(this));
    m_boundElement.m_addProcedures.push_back(new AddBoundElementProcedure(this));
    m_signature.m_signature = &TemplateBinding::m_signature;
    m_signature.m_removeProcedures.push_back(new RemoveSignatureProcedure(this));
    m_signature.m_addProcedures.push_back(new AddSignatureProcedure(this));
    m_parameterSubstitution.addProcedures.push_back(new AddParameterSubstitutionFunctor(this));
    m_parameterSubstitution.removeProcedures.push_back(new RemoveParameterSubstitutionFunctor(this));
}

TemplateBinding::TemplateBinding(const TemplateBinding& bind) : DirectedRelationship(bind), Element(bind) {
    m_boundElement = bind.m_boundElement;
    m_boundElement.m_me = this;
    m_boundElement.m_removeProcedures.clear();
    m_boundElement.m_addProcedures.clear();
    m_signature.m_removeProcedures.push_back(new RemoveSignatureProcedure(this));
    m_signature.m_addProcedures.push_back(new AddSignatureProcedure(this));
    m_signature = bind.m_signature;
    m_signature.m_me = this;
    m_signature.m_removeProcedures.clear();
    m_signature.m_addProcedures.clear();
    m_signature.m_removeProcedures.push_back(new RemoveSignatureProcedure(this));
    m_signature.m_addProcedures.push_back(new AddSignatureProcedure(this));
    m_parameterSubstitution = bind.m_parameterSubstitution;
    m_parameterSubstitution.addProcedures.clear();
    m_parameterSubstitution.addProcedures.push_back(new AddParameterSubstitutionFunctor(this));
    m_parameterSubstitution.removeProcedures.clear();
    m_parameterSubstitution.removeProcedures.push_back(new RemoveParameterSubstitutionFunctor(this));
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

Sequence<TemplateParameterSubstitution>& TemplateBinding::getParameterSubstitution() {
    return m_parameterSubstitution;
}

ElementType TemplateBinding::getElementType() const {
    return ElementType::TEMPLATE_BINDING;
}

bool TemplateBinding::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_BINDING;
    }

    return ret;
}
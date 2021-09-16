#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateableElement.h"
#include "uml/templateSignature.h"

using namespace UML;

void TemplateBinding::RemoveBoundElementProcedure::operator()(TemplateableElement* el) const {
    if (m_me->getSources().count(el->getID())) {
        m_me->getSources().remove(*el);
    }
    if (el->hasTemplateBinding() && !m_me->m_setFlag) {
        m_me->m_setFlag = true;
        el->setTemplateBinding(0);
        m_me->m_setFlag = false;
    }
}

void TemplateBinding::AddBoundElementProcedure::operator()(TemplateableElement* el) const {
    if (!m_me->getSources().count(el->getID())) {
        m_me->getSources().add(*el);
    }
    if (el->hasTemplateBinding()) {
        if (el->getTemplateBindingRef() != *m_me) {
            el->setTemplateBinding(m_me);
        }
    } else {
        el->setTemplateBinding(m_me);
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

void TemplateBinding::RemoveParameterSubstitutionFunctor::operator()(TemplateParameterSubstitution& el) const {
    if(el.getTemplateBinding() == m_el) {
        el.setTemplateBinding(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &TemplateBinding::getParameterSubstitution);
}

void TemplateBinding::setManager(UmlManager* manager) {
    DirectedRelationship::setManager(manager);
    m_parameterSubstitution.m_manager = manager;
}

void TemplateBinding::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    if (m_boundElement.id() == id) {
        m_boundElement.release();
    }
    if (m_signature.id() == id) {
        m_signature.release();
    }
}

void TemplateBinding::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    if (m_boundElement.id() == oldID) {
        m_boundElement.reindex(oldID, newID);
    }
    if (m_boundElement.id() == oldID) {
        m_boundElement.reindex(oldID, newID);
    }
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
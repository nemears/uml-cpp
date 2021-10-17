#include "uml/templateableElement.h"
#include "uml/umlManager.h"
#include "uml/sequence.h"
#include "uml/templateBinding.h"
#include "uml/templateSignature.h"

using namespace UML;

void TemplateableElement::RemoveOwnedTemplateSignatureProcedure::operator()(TemplateSignature* el) const {
    if (el->hasTemplate() && !m_me->m_setFlag) {
        m_me->m_setFlag = true;
        el->setTemplate(0);
        m_me->m_setFlag = false;
    }
    if (m_me->m_ownedElements->count(el->getID())) {
        m_me->m_ownedElements->internalRemove(*el);
    }
}

void TemplateableElement::AddOwnedTemplateSignatureProcedure::operator()(TemplateSignature* el) const {
    if (el->hasTemplate()) {
        if (el->getTemplateRef() != *m_me) {
            el->setTemplate(m_me);
        }
    } else {
        el->setTemplate(m_me);
    }
    if (!m_me->m_ownedElements->count(el->getID())) {
        m_me->m_ownedElements->internalAdd(*el);
    }
}

void TemplateableElement::AddOwnedTemplateSignatureProcedure::operator()(ID id) const {
    if (!m_me->m_ownedElements->count(id)) {
        m_me->m_ownedElements->addByID(id);
    }
}

void TemplateableElement::RemoveTemplateBindingFunctor::operator()(TemplateBinding& el) const {
    if (m_el->m_ownedElements->count(el.getID())) {
        m_el->m_ownedElements->internalRemove(el);
    }
    if (el.hasBoundElement() && !m_el->m_setFlag) {
        m_el->m_setFlag = true;
        el.setBoundElement(0);
        m_el->m_setFlag = false;
    }
}

void TemplateableElement::AddTemplateBindingFunctor::operator()(TemplateBinding& el) const {
    if (!m_el->m_ownedElements->count(el.getID())) {
        m_el->m_ownedElements->internalAdd(el);
    }
    if (el.getBoundElementID() != m_el->getID()) {
        el.setBoundElement(m_el);
    }
    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }
}

void TemplateableElement::AddTemplateBindingFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
    if (!m_el->getDirectedRelationships().count(id)) {
        m_el->getDirectedRelationships().addByID(id);
    }
}

void TemplateableElement::referencingReleased(ID id) {
    m_ownedTemplateSignature.release(id);
    m_templateBindings.elementReleased(id, &TemplateableElement::getTemplateBindings);
}

void TemplateableElement::referenceReindexed(ID oldID, ID newID) {
    m_ownedTemplateSignature.reindex(oldID, newID);
    m_templateBindings.reindex(oldID, newID, &TemplateableElement::getTemplateBindings);
}

void TemplateableElement::restoreReferences() {
    m_ownedTemplateSignature.restoreReference();
    m_templateBindings.restoreReferences();
}

void TemplateableElement::referenceErased(ID id) {
    m_ownedTemplateSignature.elementErased(id);
    m_templateBindings.elementErased(id);
}

TemplateableElement::TemplateableElement() {
    m_ownedTemplateSignature.m_signature = &TemplateableElement::m_ownedTemplateSignature;
    m_ownedTemplateSignature.m_removeProcedures.push_back(new RemoveOwnedTemplateSignatureProcedure(this));
    m_ownedTemplateSignature.m_addProcedures.push_back(new AddOwnedTemplateSignatureProcedure(this));
    m_templateBindings.addProcedures.push_back(new AddTemplateBindingFunctor(this));
    m_templateBindings.removeProcedures.push_back(new RemoveTemplateBindingFunctor(this));
}

TemplateableElement::TemplateableElement(const TemplateableElement& el){
    m_ownedTemplateSignature = el.m_ownedTemplateSignature;
    m_ownedTemplateSignature.m_me = this;
    m_ownedTemplateSignature.m_removeProcedures.clear();
    m_ownedTemplateSignature.m_addProcedures.clear();
    m_ownedTemplateSignature.m_removeProcedures.push_back(new RemoveOwnedTemplateSignatureProcedure(this));
    m_ownedTemplateSignature.m_addProcedures.push_back(new AddOwnedTemplateSignatureProcedure(this));
    m_templateBindings = el.m_templateBindings;
    m_templateBindings.m_el = this;
    m_templateBindings.addProcedures.clear();
    m_templateBindings.removeProcedures.clear();
    m_templateBindings.addProcedures.push_back(new AddTemplateBindingFunctor(this));
    m_templateBindings.removeProcedures.push_back(new RemoveTemplateBindingFunctor(this));
}

TemplateableElement::~TemplateableElement() {

}

TemplateSignature* TemplateableElement::getOwnedTemplateSignature() {
    return m_ownedTemplateSignature.get();
}

TemplateSignature& TemplateableElement::getOwnedTemplateSignatureRef() {
    return m_ownedTemplateSignature.getRef();
}

ID TemplateableElement::getOwnedTemplateSignatureID() const {
    return m_ownedTemplateSignature.id();
}

bool TemplateableElement::hasOwnedTemplateSignature() const {
    return m_ownedTemplateSignature.has();
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature* signature) {
    m_ownedTemplateSignature.set(signature);
}

void TemplateableElement::setOwnedTemplateSignature(TemplateSignature& signature) {
    m_ownedTemplateSignature.set(signature);
}

Sequence<TemplateBinding>& TemplateableElement::getTemplateBindings() {
    return m_templateBindings;
}

ElementType TemplateableElement::getElementType() const {
    return ElementType::TEMPLATEABLE_ELEMENT;
}

bool TemplateableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}
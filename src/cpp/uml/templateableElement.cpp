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

void TemplateableElement::RemoveTemplateBindingProcedure::operator()(TemplateBinding* el) const {
    if (m_me->m_ownedElements->count(el->getID())) {
        m_me->m_ownedElements->internalRemove(*el);
    }
    if (el->hasBoundElement() && !m_me->m_setFlag) {
        m_me->m_setFlag = true;
        el->setBoundElement(0);
        m_me->m_setFlag = false;
    }
}

void TemplateableElement::AddTemplateBindingProcedure::operator()(TemplateBinding* el) const {
    if (!m_me->m_ownedElements->count(el->getID())) {
        m_me->m_ownedElements->internalAdd(*el);
    }
    if (el->hasBoundElement()) {
        if (el->getBoundElementRef() != *m_me) {
            el->setBoundElement(m_me);
        }
    } else {
        el->setBoundElement(m_me);
    }
}

void TemplateableElement::referencingReleased(ID id) {
    if (m_ownedTemplateSignature.id() == id) {
        m_ownedTemplateSignature.release();
    }
    if (m_templateBinding.id() == id) {
        m_templateBinding.release();
    }
}

void TemplateableElement::referenceReindexed(ID oldID, ID newID) {
    if (m_ownedTemplateSignature.id() == oldID) {
        m_ownedTemplateSignature.reindex(oldID, newID);
    }
    if (m_templateBinding.id() == oldID) {
        m_templateBinding.reindex(oldID, newID);
    }
}

void TemplateableElement::restoreReferences() {
    m_ownedTemplateSignature.restoreReference();
    m_templateBinding.restoreReference();
}

void TemplateableElement::referenceErased(ID id) {
    m_ownedTemplateSignature.elementErased(id);
    m_templateBinding.elementErased(id);
}

TemplateableElement::TemplateableElement() {
    m_ownedTemplateSignature.m_signature = &TemplateableElement::m_ownedTemplateSignature;
    m_ownedTemplateSignature.m_removeProcedures.push_back(new RemoveOwnedTemplateSignatureProcedure(this));
    m_ownedTemplateSignature.m_addProcedures.push_back(new AddOwnedTemplateSignatureProcedure(this));
    m_templateBinding.m_signature = &TemplateableElement::m_templateBinding;
    m_templateBinding.m_removeProcedures.push_back(new RemoveTemplateBindingProcedure(this));
    m_templateBinding.m_addProcedures.push_back(new AddTemplateBindingProcedure(this));
}

TemplateableElement::TemplateableElement(const TemplateableElement& el){
    m_ownedTemplateSignature = el.m_ownedTemplateSignature;
    m_ownedTemplateSignature.m_me = this;
    m_ownedTemplateSignature.m_removeProcedures.clear();
    m_ownedTemplateSignature.m_addProcedures.clear();
    m_ownedTemplateSignature.m_removeProcedures.push_back(new RemoveOwnedTemplateSignatureProcedure(this));
    m_ownedTemplateSignature.m_addProcedures.push_back(new AddOwnedTemplateSignatureProcedure(this));
    m_templateBinding = el.m_templateBinding;
    m_templateBinding.m_me = this;
    m_templateBinding.m_removeProcedures.clear();
    m_templateBinding.m_addProcedures.clear();
    m_templateBinding.m_removeProcedures.push_back(new RemoveTemplateBindingProcedure(this));
    m_templateBinding.m_addProcedures.push_back(new AddTemplateBindingProcedure(this));
}

TemplateableElement::~TemplateableElement() {

}

TemplateSignature* TemplateableElement::getOwnedTemplateSignature() {
    return m_ownedTemplateSignature.get();
}

TemplateSignature& TemplateableElement::getOwnedTemplateSignatureRef() {
    return m_ownedTemplateSignature.getRef();
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

TemplateBinding* TemplateableElement::getTemplateBinding() {
    return m_templateBinding.get();
}

TemplateBinding& TemplateableElement::getTemplateBindingRef() {
    return m_templateBinding.getRef();
}

bool TemplateableElement::hasTemplateBinding() const {
    return m_templateBinding.has();
}

void TemplateableElement::setTemplateBinding(TemplateBinding* binding) {
    m_templateBinding.set(binding);
}

void TemplateableElement::setTemplateBinding(TemplateBinding& binding) {
    m_templateBinding.set(binding);
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
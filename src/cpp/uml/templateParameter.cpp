#include "uml/templateParameter.h"
#include "uml/templateSignature.h"
#include "uml/parameterableElement.h"

using namespace UML;

void TemplateParameter::RemoveSignatureProcedure::operator()(TemplateSignature* el) const {
    if (el->getOwnedParameter().count(m_me->getID())) {
        el->getOwnedParameter().remove(*m_me);
    }
}

void TemplateParameter::AddSignatureProcedure::operator()(TemplateSignature* el) const {
    if (!el->getOwnedParameter().count(m_me->getID())) {
        el->getOwnedParameter().add(*m_me);
    }
}

void TemplateParameter::RemoveOwnedParameteredElementProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasParameteredElement() && !m_me->m_setFlag) {
        m_me->m_setFlag = true;
        m_me->setParameteredElement(0);
        m_me->m_setFlag = false;
    }
    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void TemplateParameter::AddOwnedParameteredElementProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasParameteredElement()) {
        if (m_me->getParameteredElementRef() != *el) {
            m_me->setParameteredElement(el);
        }
    } else {
        m_me->setParameteredElement(el);
    }
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

void TemplateParameter::RemoveParameteredElementProcedure::operator()(ParameterableElement* el) const {
    el->removeReference(m_me->getID());
}

void TemplateParameter::AddParameteredElementProcedure::operator()(ParameterableElement* el) const {
    el->setReference(m_me);
}

void TemplateParameter::RemoveDefaultProcedure::operator()(ParameterableElement* el) const {
    el->removeReference(m_me->getID());
}

void TemplateParameter::AddDefaultProcedure::operator()(ParameterableElement* el) const {
    el->setReference(m_me);
}

void TemplateParameter::RemoveOwnedDefaultProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasDefault()) {
        m_me->setDefault(0);
    }
    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void TemplateParameter::AddOwnedDefaultProcedure::operator()(ParameterableElement* el) const {
    if (m_me->hasDefault()) {
        if (m_me->getDefaultRef() != *el) {
            m_me->setDefault(el);
        }
    } else {
        m_me->setDefault(el);
    }
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

void TemplateParameter::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_signature.release(id);
    m_ownedParameteredElement.release(id);
    m_parameteredElement.release(id);
    m_default.release(id);
    m_ownedDefault.release(id);
}

void TemplateParameter::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_signature.reindex(oldID, newID);
    m_ownedParameteredElement.reindex(oldID, newID);
    m_parameteredElement.reindex(oldID, newID);
    m_default.reindex(oldID, newID);
    m_ownedDefault.reindex(oldID, newID);
}

void TemplateParameter::restoreReferences() {
    Element::restoreReferences();
    m_signature.restoreReference();
    m_ownedParameteredElement.restoreReference();
    m_parameteredElement.restoreReference();
    m_default.restoreReference();
    m_ownedDefault.restoreReference();
}

void TemplateParameter::referenceErased(ID id) {
    Element::referenceErased(id);
    m_signature.elementErased(id);
    m_ownedParameteredElement.elementErased(id);
    m_parameteredElement.elementErased(id);
    m_default.elementErased(id);
    m_ownedDefault.elementErased(id);
}

TemplateParameter::TemplateParameter() {
    m_signature.m_signature = &TemplateParameter::m_signature;
    m_signature.m_addProcedures.push_back(new AddSignatureProcedure(this));
    m_signature.m_removeProcedures.push_back(new RemoveSignatureProcedure(this));
    m_ownedParameteredElement.m_signature = &TemplateParameter::m_ownedParameteredElement;
    m_ownedParameteredElement.m_addProcedures.push_back(new AddOwnedParameteredElementProcedure(this));
    m_ownedParameteredElement.m_removeProcedures.push_back(new RemoveOwnedParameteredElementProcedure(this));
    m_parameteredElement.m_signature = &TemplateParameter::m_parameteredElement;
    m_parameteredElement.m_addProcedures.push_back(new AddParameteredElementProcedure(this));
    m_parameteredElement.m_removeProcedures.push_back(new RemoveOwnedParameteredElementProcedure(this));
    m_default.m_signature = &TemplateParameter::m_default;
    m_default.m_addProcedures.push_back(new AddDefaultProcedure(this));
    m_default.m_removeProcedures.push_back(new RemoveDefaultProcedure(this));
    m_ownedDefault.m_signature = &TemplateParameter::m_ownedDefault;
    m_ownedDefault.m_addProcedures.push_back(new AddOwnedDefaultProcedure(this));
    m_ownedDefault.m_removeProcedures.push_back(new RemoveOwnedDefaultProcedure(this));
}

TemplateParameter::TemplateParameter(const TemplateParameter& el) {
    m_signature = el.m_signature;
    m_signature.m_me = this;
    m_signature.m_removeProcedures.clear();
    m_signature.m_addProcedures.clear();
    m_signature.m_addProcedures.push_back(new AddSignatureProcedure(this));
    m_signature.m_removeProcedures.push_back(new RemoveSignatureProcedure(this));
    m_ownedParameteredElement = el.m_ownedParameteredElement;
    m_ownedParameteredElement.m_me = this;
    m_ownedParameteredElement.m_addProcedures.clear();
    m_ownedParameteredElement.m_removeProcedures.clear();
    m_ownedParameteredElement.m_addProcedures.push_back(new AddOwnedParameteredElementProcedure(this));
    m_ownedParameteredElement.m_removeProcedures.push_back(new RemoveOwnedParameteredElementProcedure(this));
    m_parameteredElement = el.m_parameteredElement;
    m_parameteredElement.m_me = this;
    m_parameteredElement.m_removeProcedures.clear();
    m_parameteredElement.m_addProcedures.clear();
    m_parameteredElement.m_addProcedures.push_back(new AddParameteredElementProcedure(this));
    m_parameteredElement.m_removeProcedures.push_back(new RemoveOwnedParameteredElementProcedure(this));
    m_default = el.m_default;
    m_default.m_me = this;
    m_default.m_addProcedures.clear();
    m_default.m_removeProcedures.clear();
    m_default.m_addProcedures.push_back(new AddDefaultProcedure(this));
    m_default.m_removeProcedures.push_back(new RemoveDefaultProcedure(this));
    m_ownedDefault = el.m_ownedDefault;
    m_ownedDefault.m_me = this;
    m_ownedDefault.m_addProcedures.clear();
    m_ownedDefault.m_removeProcedures.clear();
}

TemplateParameter::~TemplateParameter() {

}

TemplateSignature* TemplateParameter::getSignature() {
    return m_signature.get();
}

TemplateSignature& TemplateParameter::getSignatureRef() {
    return m_signature.getRef();
}

ID TemplateParameter::getSignatureID() const {
    return m_signature.id();
}

bool TemplateParameter::hasSignature() const {
    return m_signature.has();
}

void TemplateParameter::setSignature(TemplateSignature* signature) {
    m_signature.set(signature);
}

void TemplateParameter::setSignature(TemplateSignature& signature) {
    m_signature.set(signature);
}

ParameterableElement* TemplateParameter::getOwnedParameteredElement() {
    return m_ownedParameteredElement.get();
}

ParameterableElement& TemplateParameter::getOwnedParameteredElementRef() {
    return m_ownedParameteredElement.getRef();
}

ID TemplateParameter::getOwnedParameteredElementID() const {
    return m_ownedParameteredElement.id();
}

bool TemplateParameter::hasOwnedParameteredElement() const {
    return m_ownedParameteredElement.has();
}

void TemplateParameter::setOwnedParameteredElement(ParameterableElement* el) {
    m_ownedParameteredElement.set(el);
}

void TemplateParameter::setOwnedParameteredElement(ParameterableElement& el) {
    m_ownedParameteredElement.set(el);
}

ParameterableElement* TemplateParameter::getParameteredElement() {
    return m_parameteredElement.get();
}

ParameterableElement& TemplateParameter::getParameteredElementRef() {
    return m_parameteredElement.getRef();
}

ID TemplateParameter::getParameteredElementID() const {
    return m_parameteredElement.id();
}

bool TemplateParameter::hasParameteredElement() const {
    return m_parameteredElement.has();
}

void TemplateParameter::setParameteredElement(ParameterableElement* el) {
    m_parameteredElement.set(el);
}

void TemplateParameter::setParameteredElement(ParameterableElement& el) {
    m_parameteredElement.set(el);
}

ParameterableElement* TemplateParameter::getDefault() {
    return m_default.get();
}

ParameterableElement& TemplateParameter::getDefaultRef() {
    return m_default.getRef();
}

ID TemplateParameter::getDefaultID() const {
    return m_default.id();
}

bool TemplateParameter::hasDefault() const {
    return m_default.has();
}

void TemplateParameter::setDefault(ParameterableElement* el) {
    m_default.set(el);
}

void TemplateParameter::setDefault(ParameterableElement& el) {
    m_default.set(el);
}

ParameterableElement* TemplateParameter::getOwnedDefault() {
    return m_ownedDefault.get();
}

ParameterableElement& TemplateParameter::getOwnedDefaultRef() {
    return m_ownedDefault.getRef();
}

ID TemplateParameter::getOwnedDefaultID() const {
    return m_ownedDefault.id();
}

bool TemplateParameter::hasOwnedDefault() const {
    return m_ownedDefault.has();
}

void TemplateParameter::setOwnedDefault(ParameterableElement* el) {
    m_ownedDefault.set(el);
}

void TemplateParameter::setOwnedDefault(ParameterableElement& el) {
    m_ownedDefault.set(el);
}

ElementType TemplateParameter::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER;
}

bool TemplateParameter::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}
#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateableElement.h"
#include "uml/templateSignature.h"
#include "uml/universalFunctions.h"

using namespace UML;

void TemplateBinding::AddParameterSubstitutionFunctor::operator()(TemplateParameterSubstitution& el) const {
    if(el.getTemplateBinding() != m_el) {
        el.setTemplateBinding(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void TemplateBinding::RemoveParameterSubstitutionFunctor::operator()(TemplateParameterSubstitution& el) const {
    if(el.getTemplateBinding() == m_el) {
        el.setTemplateBinding(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void TemplateBinding::setManager(UmlManager* manager) {
    DirectedRelationship::setManager(manager);
    m_parameterSubstitution.m_manager = manager;
}

TemplateBinding::TemplateBinding() {
    m_boundElementPtr = 0;
    m_signaturePtr = 0;
    m_parameterSubstitution.addProcedures.push_back(new AddParameterSubstitutionFunctor(this));
    m_parameterSubstitution.removeProcedures.push_back(new RemoveParameterSubstitutionFunctor(this));
}

TemplateBinding::TemplateBinding(const TemplateBinding& bind) {
    m_boundElementID = bind.m_boundElementID;
    m_signatureID = bind.m_signatureID;
    m_parameterSubstitution.addProcedures.clear();
    m_parameterSubstitution.addProcedures.push_back(new AddParameterSubstitutionFunctor(this));
    m_parameterSubstitution.removeProcedures.clear();
    m_parameterSubstitution.removeProcedures.push_back(new RemoveParameterSubstitutionFunctor(this));
}

TemplateBinding::~TemplateBinding() {

}

TemplateableElement* TemplateBinding::getBoundElement() {
    return universalGet<TemplateableElement>(m_boundElementID, m_boundElementPtr, m_manager);
}

void TemplateBinding::setBoundElement(TemplateableElement* el) {
    if (!isSameOrNull(m_boundElementID, el)) {
        if (!m_boundElementPtr) {
            m_boundElementPtr = &m_manager->get<TemplateableElement>(m_boundElementID);
        }

        TemplateableElement* temp = m_boundElementPtr;

        if (m_sources.count(m_boundElementID)) {
            m_sources.remove(*m_boundElementPtr);
        }

        m_boundElementID = ID::nullID();
        m_boundElementPtr = 0;

        if (temp->getTemplateBinding() == this) {
            temp->setTemplateBinding(0);
        }
    }

    if (el) {
        m_boundElementID = el->getID();
    }

    if (!m_manager) {
        m_boundElementPtr = 0;
    }

    if (el) {
        if (el->getTemplateBinding() != this) {
            el->setTemplateBinding(this);
        }

        if (!m_sources.count(el->getID())) {
            m_sources.add(*el);
        }
    }
}

TemplateSignature* TemplateBinding::getSignature() {
    return universalGet<TemplateSignature>(m_signatureID, m_signaturePtr, m_manager);
}

void TemplateBinding::setSignature(TemplateSignature* signature) {
    if (!isSameOrNull(m_signatureID, signature)) {
        if (!m_signaturePtr) {
            m_signaturePtr = &m_manager->get<TemplateSignature>(m_signatureID);
        }

        if (m_targets.count(m_signatureID)) {
            m_targets.remove(*m_signaturePtr);
        }

        m_signatureID = ID::nullID();
        m_signaturePtr = 0;
    }

    if (signature) {
        m_signatureID = signature->getID();
    }

    if (!m_manager) {
        m_signaturePtr = 0;
    }

    if (signature) {
        if (!m_targets.count(signature->getID())) {
            m_targets.add(*signature);
        }
    }
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
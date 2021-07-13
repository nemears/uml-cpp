#include "uml/templateParameterSubstitution.h"
#include "uml/umlManager.h"
#include "uml/templateBinding.h"
#include "uml/templateParameter.h"
#include "uml/parameterableElement.h"

using namespace UML;

TemplateParameterSubstitution::TemplateParameterSubstitution() {
    m_formalPtr = 0;
    m_templateBindingPtr = 0;
    m_actualPtr = 0;
    m_ownedActualPtr = 0;
}

TemplateParameterSubstitution::TemplateParameterSubstitution(const TemplateParameterSubstitution& temp) {

}

TemplateParameterSubstitution::~TemplateParameterSubstitution() {

}

TemplateParameter* TemplateParameterSubstitution::getFormal() {
    if (!m_formalID.isNull()) {
        if (!m_formalPtr) {
            m_formalPtr = &m_manager->get<TemplateParameter>(m_formalID);
        }
        return m_formalPtr;
    }
    return 0;
}

void TemplateParameterSubstitution::setFormal(TemplateParameter* formal) {
    if (!m_formalID.isNull()) {
        m_formalID = ID::nullID();
        m_formalPtr = 0;
    }

    if (formal) {
        m_formalID = formal->getID();
    }

    if (!m_manager) {
        m_formalPtr = formal;
    }
}

TemplateBinding* TemplateParameterSubstitution::getTemplateBinding() {
    if (!m_templateBindingID.isNull()) {
        if (!m_templateBindingPtr) {
            m_templateBindingPtr = &m_manager->get<TemplateBinding>(m_templateBindingID);
        }
        return m_templateBindingPtr;
    }
    return 0;
}

void TemplateParameterSubstitution::setTemplateBinding(TemplateBinding* bind) {
    if (!isSameOrNull(m_templateBindingID, bind)) {
        if (!m_templateBindingPtr) {
            m_templateBindingPtr = &m_manager->get<TemplateBinding>(m_templateBindingID);
        }

        TemplateBinding* temp = m_templateBindingPtr;

        m_templateBindingID = ID::nullID();
        m_templateBindingPtr = 0;

        if (temp->getParameterSubstitution().count(m_id)) {
            temp->getParameterSubstitution().remove(*this);
        }
    }

    if (bind) {
        m_templateBindingID = bind->getID();
    }

    if (!m_manager) {
        m_templateBindingPtr = bind;
    }

    if (bind) {
        if (!bind->getParameterSubstitution().count(m_id)) {
            bind->getParameterSubstitution().add(*this);
        }
    }
}

ParameterableElement* TemplateParameterSubstitution::getActual() {
    if (!m_actualID.isNull()) {
        if (!m_actualPtr) {
            m_actualPtr = &m_manager->get<ParameterableElement>(m_actualID);
        }
        return m_actualPtr;
    }
    return 0;
}

void TemplateParameterSubstitution::setActual(ParameterableElement* actual) {
    if (actual) {
        m_actualID = actual->getID();
    } else {
        m_actualID = ID::nullID();
    }

    if (!m_manager) {
        m_actualPtr = actual;
    } else {
        m_actualPtr = 0;
    }
}

ParameterableElement* TemplateParameterSubstitution::getOwnedActual() {
    if (!m_ownedActualID.isNull()) {
        if (!m_ownedActualPtr) {
            m_ownedActualPtr = &m_manager->get<ParameterableElement>(m_ownedActualID);
        }
        return m_ownedActualPtr;
    }
    return 0;
}

void TemplateParameterSubstitution::setOwnedActual(ParameterableElement* actual) {
    if (!isSameOrNull(m_ownedActualID, actual)) {
        if (!m_ownedActualPtr) {
            m_ownedActualPtr = &m_manager->get<ParameterableElement>(m_ownedActualID);
        }

        if (m_ownedElements->count(m_ownedActualID)) {
            m_ownedElements->internalRemove(*m_ownedActualPtr);
        }

        setActual(0);

        m_ownedActualID = ID::nullID();
        m_ownedActualPtr = 0;
    }

    if (actual) {
        m_ownedActualID = actual->getID();
    }

    if (!m_manager) {
        m_ownedActualPtr = actual;
    }

    if (actual) {
        if (!m_ownedElements->count(actual->getID())) {
            m_ownedElements->internalAdd(*actual);
        }
        if (m_actualID != actual->getID()) {
            setActual(actual);
        }
    }
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
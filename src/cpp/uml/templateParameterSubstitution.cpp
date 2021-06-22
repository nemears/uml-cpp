#include "uml/templateParameterSubstitution.h"
#include "uml/umlManager.h"
#include "uml/templateBinding.h"

using namespace UML;

TemplateParameterSubstitution::TemplateParameterSubstitution() {
    m_formalPtr = 0;
    m_templateBindingPtr = 0;
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

        if (temp->getParameterSubstitution() == this) {
            temp->setParameterSubstitution(0);
        }
    }

    if (bind) {
        m_templateBindingID = bind->getID();
    }

    if (!m_manager) {
        m_templateBindingPtr = bind;
    }

    if (bind) {
        if (bind->getParameterSubstitution() != this) {
            bind->setParameterSubstitution(0);
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
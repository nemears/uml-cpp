#include "uml/generalization.h"

using namespace UML;

Generalization::Generalization() {
    m_general = 0;
    m_specific = 0;
}

Classifier* Generalization::getGeneral() {
    return m_general;
}

void Generalization::setGeneral(Classifier* general) {
    if (m_general) {
        if (m_general->getRelationships().count(m_id)) {
            m_general->getRelationships().remove(*this);
        }
        if (m_specific) {
            if (m_specific->getGenerals().count(m_general->getID())) {
                m_specific->getGenerals().remove(*m_general);
            }
        }
    }
    m_general = general;
    if (!getTargets().count(m_general->getID())) {
        getTargets().add(*m_general);
    }
    if (m_specific) {
        if (!m_specific->getGenerals().count(m_general->getID())) {
            m_specific->getGenerals().add(*m_general);
        }
    }
}

Classifier* Generalization::getSpecific() {
    return m_specific;
}

void Generalization::setSpecific(Classifier* specific) {
    m_specific = specific;
    if (!getSources().count(m_specific->getID())) {
        getSources().add(*m_specific);
    }
    if (!m_specific->getGeneralizations().count(m_id)) {
        m_specific->getGeneralizations().add(*this);
    }
}

ElementType Generalization::getElementType() {
    return ElementType::GENERALIZATION;
}
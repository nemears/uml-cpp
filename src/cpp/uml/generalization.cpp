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
            for (auto const& member: m_general->getMembers()) {
                if (m_specific->getInheritedMembers().count(member->getID())) {
                    m_specific->getInheritedMembers().remove(*member);
                }
            }
            if (m_specific->getGenerals().count(m_general->getID())) {
                m_specific->getGenerals().remove(*m_general);
            }
        }
    }
    m_general = general;
    if (m_general) {
        if (!getTargets().count(m_general->getID())) {
            getTargets().add(*m_general);
        }
        if (m_specific) {
            if (!m_specific->getGenerals().count(m_general->getID())) {
                m_specific->getGenerals().add(*m_general);
            }
            for (auto const& member: m_general->getMembers()) {
                if (!m_specific->getInheritedMembers().count(member->getID()) && member->getVisibility() != VisibilityKind::PRIVATE) {
                    m_specific->getInheritedMembers().add(*member);
                }
            }
        }
    }
}

Classifier* Generalization::getSpecific() {
    return m_specific;
}

void Generalization::setSpecific(Classifier* specific) {
    if (m_specific) {
        if (m_specific->getRelationships().count(m_id)) {
            m_specific->getRelationships().remove(*this);
        }
        if (m_specific->getGeneralizations().count(m_id)) {
            m_specific->getGeneralizations().remove(*this);
        }
        if (m_general) {
            for (auto const& member: m_general->getMembers()) {
                if (m_specific->getInheritedMembers().count(member->getID())) {
                    m_specific->getInheritedMembers().remove(*member);
                }
            }
        }
    }
    m_specific = specific;
    if (m_specific) {
        if (!getSources().count(m_specific->getID())) {
            getSources().add(*m_specific);
        }
        if (!m_specific->getGeneralizations().count(m_id)) {
            m_specific->getGeneralizations().add(*this);
        }
        if (m_general) {
            for (auto const& member: m_general->getMembers()) {
                if (!m_specific->getInheritedMembers().count(member->getID()) && member->getVisibility() != VisibilityKind::PRIVATE) {
                    m_specific->getInheritedMembers().add(*member);
                }
            }
        }
    }
}

ElementType Generalization::getElementType() const {
    return ElementType::GENERALIZATION;
}

bool Generalization::isSubClassOf(ElementType eType) {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION;
    }

    return ret;
}
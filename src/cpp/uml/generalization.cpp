#include "uml/generalization.h"
#include "uml/classifier.h"

using namespace UML;

Generalization::Generalization() {
    m_generalPtr = 0;
    m_specificPtr = 0;
}

Classifier* Generalization::getGeneral() {
    if (!m_generalID.isNull()) {
        if (!m_generalPtr) {
            m_generalPtr = &m_manager->get<Classifier>(m_generalID);
        }
        return m_generalPtr;
    }
    return 0;
}

void Generalization::setGeneral(Classifier* general) {
    if (!m_generalID.isNull()) {
        if (!m_generalPtr) {
            m_generalPtr = &m_manager->get<Classifier>(m_generalID);
        }
        if (m_generalPtr->getRelationships().count(m_id)) {
            m_generalPtr->getRelationships().internalRemove(*this);
        }
        if (!m_specificID.isNull()) {
            if(!m_specificPtr) {
                m_specificPtr = &m_manager->get<Classifier>(m_specificID);
            }
            for (auto& member : m_generalPtr->getMembers()) {
                if (m_specificPtr->getInheritedMembers().count(member.getID())) {
                    m_specificPtr->getInheritedMembers().remove(member);
                }
            }
            if (m_specificPtr->getGenerals().count(m_generalPtr->getID())) {
                m_specificPtr->getGenerals().remove(*m_generalPtr);
            }
        }
        m_generalID = ID::nullID();
        m_generalPtr = 0;
    }

    if (general) {
        m_generalID = general->getID();
    }

    if (!m_manager) {
        m_generalPtr = general;
    }

    if (general) {
        if (!getTargets().count(general->getID())) {
            getTargets().add(*general);
        }
        if (!m_specificID.isNull()) {
            if (!m_specificPtr) {
                m_specificPtr = &m_manager->get<Classifier>(m_specificID);
            }
            if (!m_specificPtr->getGenerals().count(m_generalID)) {
                m_specificPtr->getGenerals().add(*general);
            }
            for (auto& member : general->getMembers()) {
                if (!m_specificPtr->getInheritedMembers().count(member.getID()) && member.getVisibility() != VisibilityKind::PRIVATE) {
                    m_specificPtr->getInheritedMembers().add(member);
                }
            }
        }
    }
}

Classifier* Generalization::getSpecific() {
    if (!m_specificID.isNull()) {
        if (!m_specificPtr) {
            m_specificPtr = &m_manager->get<Classifier>(m_specificID);
        }
        return m_specificPtr;
    }
    return 0;
}

void Generalization::setSpecific(Classifier* specific) {
    if (!m_specificID.isNull()) {
        if (!m_specificPtr) {
            m_specificPtr = &m_manager->get<Classifier>(m_specificID);
        }
        if (m_specificPtr->getRelationships().count(m_id)) {
            m_specificPtr->getRelationships().internalRemove(*this);
        }
        if (m_specificPtr->getGeneralizations().count(m_id)) {
            m_specificPtr->getGeneralizations().remove(*this);
        }
        if (!m_generalID.isNull()) {
            if (!m_generalPtr) {
                m_generalPtr = &m_manager->get<Classifier>(m_generalID);
            }
            for (auto& member: m_generalPtr->getMembers()) {
                if (m_specificPtr->getInheritedMembers().count(member.getID())) {
                    m_specificPtr->getInheritedMembers().remove(member);
                }
            }
        }
        m_specificPtr = 0;
        m_specificID = ID::nullID();
    }

    if (specific) {
        m_specificID = specific->getID();
    }

    if (!m_manager) {
        m_specificPtr = specific;
    }

    if (specific) {
        if (!getSources().count(specific->getID())) {
            getSources().add(*specific);
        }
        if (!specific->getGeneralizations().count(m_id)) {
            specific->getGeneralizations().add(*this);
        }
        if (!m_generalID.isNull()) {
            if (!m_generalPtr) {
                m_generalPtr = &m_manager->get<Classifier>(m_generalID);
            }
            for (auto& member: m_generalPtr->getMembers()) {
                if (!specific->getInheritedMembers().count(member.getID()) && member.getVisibility() != VisibilityKind::PRIVATE) {
                    specific->getInheritedMembers().add(member);
                }
            }
        }
    }
}

ElementType Generalization::getElementType() const {
    return ElementType::GENERALIZATION;
}

bool Generalization::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION;
    }

    return ret;
}
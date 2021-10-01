#include "uml/generalization.h"
#include "uml/classifier.h"

using namespace UML;

void Generalization::RemoveGeneralProcedure::operator()(Classifier* el) const {
    if (m_me->getTargets().count(el->getID())) {
        m_me->getTargets().remove(*el);
    }
    if (m_me->m_specific.has()) {
        for (auto& member : el->getMembers()) {
            if (m_me->m_specific.getRef().getInheritedMembers().count(member.getID())) {
                m_me->m_specific.getRef().getInheritedMembers().remove(member);
            }
        }
        if (m_me->m_specific.getRef().getGenerals().count(el->getID())) {
            m_me->m_specific.getRef().getGenerals().remove(*el);
        }
    }
}

void Generalization::AddGeneralProcedure::operator()(Classifier* el) const {
    if (!m_me->getTargets().count(el->getID())) {
        m_me->getTargets().add(*el);
    }
    if (m_me->m_specific.has()) {
        for (auto& member : el->getMembers()) {
            if (!m_me->m_specific.getRef().getInheritedMembers().count(member.getID())) {
                m_me->m_specific.getRef().getInheritedMembers().add(member);
            }
        }
        if (!m_me->m_specific.getRef().getGenerals().count(el->getID())) {
            m_me->m_specific.getRef().getGenerals().add(*el);
        }
    }
}

void Generalization::RemoveSpecificProcedure::operator()(Classifier* el) const {
    if (m_me->getSources().count(el->getID())) {
        m_me->getSources().remove(*el);
    }
    if (el->getGeneralizations().count(m_me->getID())) {
        el->getGeneralizations().remove(*m_me);
    }
    /**if (m_me->getOwner()) {
        if (*m_me->getOwner() == *el) {
            m_me->setOwner(0);
        }
    }**/
    if (m_me->m_general.has()) {
        for (auto& member : m_me->m_general.getRef().getMembers()) {
            if (el->getInheritedMembers().count(member.getID())) {
                el->getInheritedMembers().remove(member);
            }
        }
        if (el->getGenerals().count(m_me->m_specific.id())) {
            el->getGenerals().remove(m_me->m_specific.getRef());
        }
    }
}

void Generalization::AddSpecificProcedure::operator()(Classifier* el) const {
    if (!m_me->getSources().count(el->getID())) {
        m_me->getSources().add(*el);
    }
    if (!el->getGeneralizations().count(m_me->getID())) {
        el->getGeneralizations().add(*m_me);
    }
    /**bool setOwner = false;
    if (m_me->getOwner()) {
        setOwner = *m_me->getOwner() == *el;
    }
    if (!m_me->getOwner() || setOwner) {
        m_me->setOwner(el);
    }**/
    if (m_me->m_general.has()) {
        for (auto& member : m_me->m_general.getRef().getMembers()) {
            if (member.getVisibility() != VisibilityKind::PRIVATE) {
                if (!el->getInheritedMembers().count(member.getID())) {
                    el->getInheritedMembers().add(member);
                }
            }
        }
        if (!el->getGenerals().count(m_me->m_general.id())) {
            el->getGenerals().add(m_me->m_general.getRef());
        }
    }
}

void Generalization::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    if (m_general.id() == oldID) {
        m_general.reindex(oldID, newID);
    }
    if (m_specific.id() == oldID) {
        m_specific.reindex(oldID, newID);
    }
}

void Generalization::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    if (m_general.id() == id) {
        m_general.release();
    }
    if (m_specific.id() == id) {
        m_specific.release();
    }
}

Generalization::Generalization() {
    m_general.m_signature = &Generalization::m_general;
    m_general.m_removeProcedures.push_back(new RemoveGeneralProcedure(this));
    m_general.m_addProcedures.push_back(new AddGeneralProcedure(this));
    m_specific.m_signature = &Generalization::m_specific;
    m_specific.m_removeProcedures.push_back(new RemoveSpecificProcedure(this));
    m_specific.m_addProcedures.push_back(new AddSpecificProcedure(this));
}

Generalization::Generalization(const Generalization& rhs) : DirectedRelationship(rhs), Element(rhs) {
    m_general = rhs.m_general;
    m_general.m_me = this;
    m_general.m_removeProcedures.clear();
    m_general.m_addProcedures.clear();
    m_general.m_removeProcedures.push_back(new RemoveGeneralProcedure(this));
    m_general.m_addProcedures.push_back(new AddGeneralProcedure(this));
    m_specific = rhs.m_specific;
    m_specific.m_me = this;
    m_specific.m_addProcedures.clear();
    m_specific.m_removeProcedures.clear();
    m_specific.m_removeProcedures.push_back(new RemoveSpecificProcedure(this));
    m_specific.m_addProcedures.push_back(new AddSpecificProcedure(this));
}

Generalization::~Generalization() {
    
}

Classifier* Generalization::getGeneral() {
    return m_general.get();
}

Classifier& Generalization::getGeneralRef() {
    return m_general.getRef();
}

ID Generalization::getGeneralID() const {
    return m_general.id();
}

bool Generalization::hasGeneral() const {
    return m_general.has();
}

void Generalization::setGeneral(Classifier* general) {
    m_general.set(general);
}

void Generalization::setGeneral(Classifier& general) {
    m_general.set(general);
}

Classifier* Generalization::getSpecific() {
    return m_specific.get();
}

Classifier& Generalization::getSpecificRef() {
    return m_specific.getRef();
}

ID Generalization::getSpecificID() const {
    return m_specific.id();
}

bool Generalization::hasSpecific() const {
    return m_specific.has();
}

void Generalization::setSpecific(Classifier* specific) {
    m_specific.set(specific);
}

void Generalization::setSpecific(Classifier& specific) {
    m_specific.set(specific);
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
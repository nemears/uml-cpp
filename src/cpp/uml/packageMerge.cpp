#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/universalFunctions.h"

using namespace UML;

void PackageMerge::RemoveReceivingPackageProcedure::operator()(Package* el) const {
    if (el->getPackageMerge().count(m_me->getID())) {
        el->getPackageMerge().remove(*m_me);
    }
    if (m_me->getSources().count(el->getID())) {
        m_me->getSources().remove(*el);
    }
}

void PackageMerge::AddReceivingPackageProcedure::operator()(Package* el) const {
    if (!el->getPackageMerge().count(m_me->getID())) {
        el->getPackageMerge().add(*m_me);
    }
    if (!m_me->getSources().count(el->getID())) {
        m_me->getSources().add(*el);
    }
}

void PackageMerge::RemoveMergedPackageProcedure::operator()(Package* el) const {
    if (m_me->getTargets().count(el->getID())) {
        m_me->getTargets().remove(*el);
    }
}

void PackageMerge::AddMergedPackageProcedure::operator()(Package* el) const {
    if (!m_me->getTargets().count(el->getID())) {
        m_me->getTargets().add(*el);
    }
}

PackageMerge::PackageMerge() {
    m_receivingPackage.m_signature = &PackageMerge::m_receivingPackage;
    m_receivingPackage.m_removeProcedures.push_back(new RemoveReceivingPackageProcedure(this));
    m_receivingPackage.m_addProcedures.push_back(new AddReceivingPackageProcedure(this));
    m_mergedPackage.m_signature = &PackageMerge::m_mergedPackage;
    m_mergedPackage.m_removeProcedures.push_back(new RemoveMergedPackageProcedure(this));
    m_mergedPackage.m_addProcedures.push_back(new AddMergedPackageProcedure(this));
}

PackageMerge::PackageMerge(const PackageMerge& merge) : DirectedRelationship(merge), Element(merge) {
    m_receivingPackage = merge.m_receivingPackage;
    m_receivingPackage.m_me = this;
    m_receivingPackage.m_removeProcedures.clear();
    m_receivingPackage.m_addProcedures.clear();
    m_receivingPackage.m_removeProcedures.push_back(new RemoveReceivingPackageProcedure(this));
    m_receivingPackage.m_addProcedures.push_back(new AddReceivingPackageProcedure(this));
    m_mergedPackage = merge.m_mergedPackage;
    m_mergedPackage.m_me = this;
    m_mergedPackage.m_removeProcedures.clear();
    m_mergedPackage.m_addProcedures.clear();
    m_mergedPackage.m_removeProcedures.push_back(new RemoveMergedPackageProcedure(this));
    m_mergedPackage.m_addProcedures.push_back(new AddMergedPackageProcedure(this));
}

PackageMerge::~PackageMerge() {

}

Package* PackageMerge::getReceivingPackage() {
    return m_receivingPackage.get();
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    m_receivingPackage.set(receiving);
}

Package* PackageMerge::getMergedPackage() {
    return m_mergedPackage.get();
}

void PackageMerge::setMergedPackage(Package* merge) {
    m_mergedPackage.set(merge);
}

ElementType PackageMerge::getElementType() const {
    return ElementType::PACKAGE_MERGE;
}

bool PackageMerge::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}

void PackageMerge::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
}

void PackageMerge::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_mergedPackage.id() == id) {
        m_mergedPackage.release();
    }
    if (m_receivingPackage.id() == id) {
        m_receivingPackage.release();
    }
}
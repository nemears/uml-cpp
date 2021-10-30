#include "uml/packageMerge.h"
#include "uml/package.h"

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

void PackageMerge::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_mergedPackage.release(id);
    m_receivingPackage.release(id);
}

void PackageMerge::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_mergedPackage.reindex(oldID, newID);
    m_receivingPackage.reindex(oldID, newID);
}

void PackageMerge::restoreReferences() {
    DirectedRelationship::restoreReferences();
    m_mergedPackage.restoreReference();
    m_receivingPackage.restoreReference();
}

void PackageMerge::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_mergedPackage.elementErased(id);
    m_receivingPackage.elementErased(id);
}

PackageMerge::PackageMerge() : Element(ElementType::PACKAGE_MERGE) {
    m_receivingPackage.m_signature = &PackageMerge::m_receivingPackage;
    m_receivingPackage.m_removeProcedures.push_back(new RemoveReceivingPackageProcedure(this));
    m_receivingPackage.m_addProcedures.push_back(new AddReceivingPackageProcedure(this));
    m_mergedPackage.m_signature = &PackageMerge::m_mergedPackage;
    m_mergedPackage.m_removeProcedures.push_back(new RemoveMergedPackageProcedure(this));
    m_mergedPackage.m_addProcedures.push_back(new AddMergedPackageProcedure(this));
}

PackageMerge::PackageMerge(const PackageMerge& merge) : 
DirectedRelationship(merge), 
Element(merge, ElementType::PACKAGE_MERGE) {
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

Package& PackageMerge::getReceivingPackageRef() {
    return m_receivingPackage.getRef();
}

ID PackageMerge::getReceivingPackageID() const {
    return m_receivingPackage.id();
}

bool PackageMerge::hasReceivingPackage() const {
    return m_receivingPackage.has();
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    m_receivingPackage.set(receiving);
}

void PackageMerge::setReceivingPackage(Package& receiving) {
    m_receivingPackage.set(receiving);
}

Package* PackageMerge::getMergedPackage() {
    return m_mergedPackage.get();
}

Package& PackageMerge::getMergedPackageRef() {
    return m_mergedPackage.getRef();
}

ID PackageMerge::getMergedPackageID() const {
    return m_mergedPackage.id();
}

bool PackageMerge::hasMergedPackage() const {
    return m_mergedPackage.has();
}

void PackageMerge::setMergedPackage(Package* merge) {
    m_mergedPackage.set(merge);
}

void PackageMerge::setMergedPackage(Package& merge) {
    m_mergedPackage.set(merge);
}

bool PackageMerge::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}
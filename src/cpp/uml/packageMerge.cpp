#include "uml/packageMerge.h"
#include "uml/package.h"

using namespace UML;

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
    // m_mergedPackage.restoreReference();
    // m_receivingPackage.restoreReference();
}

void PackageMerge::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_mergedPackage.eraseElement(id);
    m_receivingPackage.eraseElement(id);
}

Set<Package, PackageMerge>& PackageMerge::getReceivingPackageSingleton() {
    return m_receivingPackage;
}

Set<Package, PackageMerge>& PackageMerge::getMergedPackageSingleton() {
    return m_mergedPackage;
}

void PackageMerge::init() {
    m_receivingPackage.subsets(*m_owner);
    m_receivingPackage.subsets(m_sources);
    m_receivingPackage.m_signature = &PackageMerge::getReceivingPackageSingleton;
    m_mergedPackage.subsets(m_targets);
    m_mergedPackage.m_signature = &PackageMerge::getMergedPackageSingleton;
}

void PackageMerge::copy(const PackageMerge& rhs) {
    m_receivingPackage = Singleton2<Package, PackageMerge>(rhs.m_receivingPackage);
    m_receivingPackage.m_el = this;
    m_mergedPackage = Singleton2<Package, PackageMerge>(rhs.m_mergedPackage);
    m_mergedPackage.m_el = this;
}

PackageMerge::PackageMerge() : Element(ElementType::PACKAGE_MERGE) {
    
}

PackageMerge::PackageMerge(const PackageMerge& merge) : Element(ElementType::PACKAGE_MERGE) {
    Relationship::copy(merge);
    DirectedRelationship::copy(merge);
    copy(merge);
    Relationship::init();
    DirectedRelationship::init();
    init();
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
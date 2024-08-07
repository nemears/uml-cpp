#include "uml/uml-stable.h"

using namespace UML;

Singleton<Package, PackageMerge>& PackageMerge::getReceivingPackageSingleton() {
    return m_receivingPackage;
}

Singleton<Package, PackageMerge>& PackageMerge::getMergedPackageSingleton() {
    return m_mergedPackage;
}

PackageMerge::PackageMerge() : Element(ElementType::PACKAGE_MERGE) {
    m_receivingPackage.subsets(*m_owner);
    m_receivingPackage.subsets(m_sources);
    m_receivingPackage.opposite(&Package::getPackageMerge);
    m_mergedPackage.subsets(m_targets);
}

PackageMerge::~PackageMerge() {
    
}

PackagePtr PackageMerge::getReceivingPackage() const {
    return m_receivingPackage.get();
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    m_receivingPackage.set(receiving);
}

void PackageMerge::setReceivingPackage(Package& receiving) {
    m_receivingPackage.set(receiving);
}

void PackageMerge::setReceivingPackage(ID id) {
    m_receivingPackage.set(id);
}

PackagePtr PackageMerge::getMergedPackage() const {
    return m_mergedPackage.get();
}

void PackageMerge::setMergedPackage(Package* merge) {
    m_mergedPackage.set(merge);
}

void PackageMerge::setMergedPackage(Package& merge) {
    m_mergedPackage.set(merge);
}

void PackageMerge::setMergedPackage(ID id) {
    m_mergedPackage.set(id);
}

bool PackageMerge::is(ElementType eType) const {
    bool ret = DirectedRelationship::is(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}

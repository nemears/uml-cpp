#include "uml/uml-stable.h"

using namespace UML;

Singleton<Package, PackageMerge>& PackageMerge::getReceivingPackageSingleton() {
    return m_receivingPackage;
}

Singleton<Package, PackageMerge>& PackageMerge::getMergedPackageSingleton() {
    return m_mergedPackage;
}

PackageMerge::PackageMerge(std::size_t elementType, AbstractManager&manager) : 
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_receivingPackage.subsets(m_owner);
    m_receivingPackage.subsets(m_sources);
    m_receivingPackage.opposite(&Package::getPackageMerge);
    m_mergedPackage.subsets(m_targets);
}

PackagePtr PackageMerge::getReceivingPackage() const {
    return m_receivingPackage.get();
}

void PackageMerge::setReceivingPackage(PackagePtr receiving) {
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

void PackageMerge::setMergedPackage(PackagePtr merge) {
    m_mergedPackage.set(merge);
}

void PackageMerge::setMergedPackage(Package& merge) {
    m_mergedPackage.set(merge);
}

void PackageMerge::setMergedPackage(ID id) {
    m_mergedPackage.set(id);
}

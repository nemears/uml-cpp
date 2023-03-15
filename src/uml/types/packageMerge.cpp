#include "uml/types/packageMerge.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/packageMerge.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Package, PackageMerge>& PackageMerge::getReceivingPackageSingleton() {
    return m_receivingPackage;
}

TypedSet<Package, PackageMerge>& PackageMerge::getMergedPackageSingleton() {
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

bool PackageMerge::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}
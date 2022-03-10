#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void PackageMerge::MergedPackageReferenceFunctor::operator()(Element& el) const {
    el.setReference(&m_el);
}

void PackageMerge::RemoveMergedPackageReferenceFunctor::operator()(Element& el) const {
    el.removeReference(m_el.getID());
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
    m_mergedPackage.m_addFunctors.insert(new MergedPackageReferenceFunctor(this));
    m_mergedPackage.m_removeFunctors.insert(new RemoveMergedPackageReferenceFunctor(this));
    m_mergedPackage.subsets(m_targets);
    m_mergedPackage.m_signature = &PackageMerge::getMergedPackageSingleton;
}

PackageMerge::PackageMerge() : Element(ElementType::PACKAGE_MERGE) {
    init();
}

PackageMerge::~PackageMerge() {
    mountAndRelease();
}

PackagePtr PackageMerge::getReceivingPackage() {
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

PackagePtr PackageMerge::getMergedPackage() {
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
#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/uml-stable.h"

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

void PackageMerge::copy(const PackageMerge& rhs) {
    m_receivingPackage = Singleton<Package, PackageMerge>(rhs.m_receivingPackage);
    m_receivingPackage.m_el = this;
    m_mergedPackage = Singleton<Package, PackageMerge>(rhs.m_mergedPackage);
    m_mergedPackage.m_el = this;
}

PackageMerge::PackageMerge() : Element(ElementType::PACKAGE_MERGE) {
    init();
}

PackageMerge::PackageMerge(const PackageMerge& rhs) : Element(rhs, ElementType::PACKAGE_MERGE) {
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    copy(rhs);
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

void PackageMerge::setReceivingPackage(ID id) {
    m_receivingPackage.set(id);
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
#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/universalFunctions.h"

using namespace UML;

PackageMerge::PackageMerge() {
    m_receivingPackagePtr = 0;
    m_mergedPackagePtr = 0;
}

PackageMerge::PackageMerge(const PackageMerge& merge) {
    m_receivingPackageID = merge.m_receivingPackageID;
    m_receivingPackagePtr = merge.m_receivingPackagePtr;
    m_mergedPackageID = merge.m_mergedPackageID;
    m_mergedPackagePtr = merge.m_mergedPackagePtr;
}

PackageMerge::~PackageMerge() {

}

Package* PackageMerge::getReceivingPackage() {
    return universalGet<Package>(m_receivingPackageID, m_receivingPackagePtr, m_manager);
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    if (!isSameOrNull(m_receivingPackageID, receiving)) {
        if (!m_receivingPackagePtr) {
            m_receivingPackagePtr = m_manager->get<Package>(this, m_receivingPackageID, &PackageMerge::m_receivingPackagePtr);
        }
        if (m_manager) {
            removeReference(m_receivingPackageID);
        }
        m_receivingPackageID = ID::nullID();
        if (m_receivingPackagePtr->getPackageMerge().count(m_id)) {
            m_receivingPackagePtr->getPackageMerge().remove(*this);
        }
        if (m_sources.count(m_receivingPackagePtr->getID())) {
            m_sources.remove(*m_receivingPackagePtr);
        }
        m_receivingPackagePtr = 0;
    }

    if (receiving) {
        m_receivingPackageID = receiving->getID();
    }

    if (!m_manager) {
        m_receivingPackagePtr = receiving;
    }

    if (receiving) {
        if (m_manager) {
            setReference(receiving);
        }
        if (!receiving->getPackageMerge().count(m_id)) {
            receiving->getPackageMerge().add(*this);
        }
        if (!m_sources.count(receiving->getID())) {
            m_sources.add(*receiving);
        }
    }
}

Package* PackageMerge::getMergedPackage() {
    return universalGet<Package>(m_mergedPackageID, m_mergedPackagePtr, m_manager);
}

void PackageMerge::setMergedPackage(Package* merge) {
    if (!m_mergedPackageID.isNull()) {
        if (!m_mergedPackagePtr) {
            m_mergedPackagePtr = &m_manager->get<Package>(m_mergedPackageID);
        }
        if (m_targets.count(m_mergedPackageID)) {
            m_targets.remove(*m_mergedPackagePtr);
        }
        m_mergedPackagePtr = 0;
        m_mergedPackageID = ID::nullID();
    }

    if (merge) {
        m_mergedPackageID = merge->getID();
    }

    if (!m_manager) {
        m_mergedPackagePtr = merge;
    }

    if (merge) {
        if (!m_targets.count(m_mergedPackageID)) {
            m_targets.add(*merge);
        }
    }
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
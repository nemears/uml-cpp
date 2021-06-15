#include "uml/packageMerge.h"
#include "uml/package.h"

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

Package* PackageMerge::getReceivingPackage() {
    if (!m_receivingPackageID.isNull()) {
        if (!m_receivingPackagePtr) {
            m_receivingPackagePtr = &m_manager->get<Package>(m_receivingPackageID);
        }
        return m_receivingPackagePtr;
    }
    return 0;
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    if (!isSameOrNull(m_receivingPackageID, receiving)) {
        if (!m_receivingPackagePtr) {
            m_receivingPackagePtr = &m_manager->get<Package>(m_receivingPackageID);
        }
        if (m_receivingPackagePtr->getPackageMerge().count(m_id)) {
            m_receivingPackagePtr->getPackageMerge().remove(*this);
        }
        if (m_sources.count(m_receivingPackagePtr->getID())) {
            m_sources.remove(*m_receivingPackagePtr);
        }
        m_receivingPackagePtr = 0;
        m_receivingPackageID = ID::nullID();
    }

    if (receiving) {
        m_receivingPackageID = receiving->getID();
    }

    if (!m_manager) {
        m_receivingPackagePtr = receiving;
    }

    if (receiving) {
        if (!receiving->getPackageMerge().count(m_id)) {
            receiving->getPackageMerge().add(*this);
        }
        if (!m_sources.count(receiving->getID())) {
            m_sources.add(*receiving);
        }
    }
}

Package* PackageMerge::getMergedPackage() {
    if (!m_mergedPackageID.isNull()) {
        if (!m_mergedPackagePtr) {
            m_mergedPackagePtr = &m_manager->get<Package>(m_mergedPackageID);
        }
        return m_mergedPackagePtr;
    }
    return 0;
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


    // if (m_mergedPackage) {
    //     if (m_targets.count(m_mergedPackage->getID())) {
    //         m_targets.remove(*m_mergedPackage);
    //     }
    // }
    // m_mergedPackage = merge;
    // if (m_mergedPackage) {
    //     if (!m_targets.count(m_mergedPackage->getID())) {
    //         m_targets.add(*m_mergedPackage);
    //     }
    // }
}

ElementType PackageMerge::getElementType() const {
    return ElementType::PACKAGE_MERGE;
}

bool PackageMerge::isSubClassOf(ElementType eType) {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}
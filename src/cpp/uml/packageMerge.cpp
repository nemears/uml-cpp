#include "uml/packageMerge.h"
#include "uml/package.h"

using namespace UML;

PackageMerge::PackageMerge() {
    m_receivingPackage = 0;
    m_mergedPackage = 0;
}

PackageMerge::PackageMerge(const PackageMerge& merge) {
    m_receivingPackage = merge.m_receivingPackage;
    m_mergedPackage = merge.m_mergedPackage;
}

Package* PackageMerge::getReceivingPackage() {
    return m_receivingPackage;
}

void PackageMerge::setReceivingPackage(Package* receiving) {
    if (m_receivingPackage) {
        if (m_receivingPackage->getPackageMerge().count(m_id)) {
            m_receivingPackage->getPackageMerge().remove(*this);
        }
        if (m_sources.count(m_receivingPackage->getID())) {
            m_sources.remove(*m_receivingPackage);
        }
    }
    m_receivingPackage = receiving;
    if (m_receivingPackage) {
        if (!m_receivingPackage->getPackageMerge().count(m_id)) {
            m_receivingPackage->getPackageMerge().add(*this);
        }
        if (!m_sources.count(m_receivingPackage->getID())) {
            m_sources.add(*m_receivingPackage);
        }
    }
}

Package* PackageMerge::getMergedPackage() {
    return m_mergedPackage;
}

void PackageMerge::setMergedPackage(Package* merge) {
    if (m_mergedPackage) {
        if (m_targets.count(m_mergedPackage->getID())) {
            m_targets.remove(*m_mergedPackage);
        }
    }
    m_mergedPackage = merge;
    if (m_mergedPackage) {
        if (!m_targets.count(m_mergedPackage->getID())) {
            m_targets.add(*m_mergedPackage);
        }
    }
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
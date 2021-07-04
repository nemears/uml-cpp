#include "uml/profileApplication.h"
#include "uml/profile.h"

using namespace UML;

ProfileApplication::ProfileApplication() {
    m_appliedProfilePtr = 0;
    m_applyingPackagePtr = 0;
}

ProfileApplication::ProfileApplication(const ProfileApplication& profileApplication) {

}

ProfileApplication::~ProfileApplication() {

}

Profile* ProfileApplication::getAppliedProfile() {
    if (!m_appliedProfileID.isNull()) {
        if (!m_appliedProfilePtr) {
            m_appliedProfilePtr = &m_manager->get<Profile>(m_appliedProfileID);
        }
        return m_appliedProfilePtr;
    }

    return 0;
}

void ProfileApplication::setAppliedProfile(Profile* profile) {
    if (!isSameOrNull(m_appliedProfileID, profile)) {
        if (!m_appliedProfilePtr) {
            m_appliedProfilePtr = &m_manager->get<Profile>(m_appliedProfileID);
        }

        if (m_targets.count(m_appliedProfileID)) {
            m_targets.remove(*m_appliedProfilePtr);
        }

        m_appliedProfilePtr = 0;
        m_appliedProfileID = ID::nullID();
    }

    if (profile) {
        m_appliedProfileID = profile->getID();
    }

    if (!m_manager) {
        m_appliedProfilePtr = profile;
    }

    if (profile) {
        if (!m_targets.count(profile->getID())) {
            m_targets.add(*profile);
        }
    }
}

Package* ProfileApplication::getApplyingPackage() {
    if (!m_applyingPackageID.isNull()) {
        if (!m_applyingPackagePtr) {
            m_applyingPackagePtr = &m_manager->get<Package>(m_applyingPackageID);
        }
        return m_applyingPackagePtr;
    }
    return 0;
}

void ProfileApplication::setApplyingPackage(Package* pckg) {
    if (!isSameOrNull(m_applyingPackageID, pckg)) {
        if (!m_applyingPackagePtr) {
            m_applyingPackagePtr = &m_manager->get<Package>(m_applyingPackageID);
        }

        if (m_sources.count(m_applyingPackageID)) {
            m_sources.remove(*m_applyingPackagePtr);
        }

        m_applyingPackagePtr = 0;
        m_applyingPackageID = ID::nullID();
    }

    if (pckg) {
        m_applyingPackageID = pckg->getID();
    }

    if (!m_manager) {
        m_applyingPackagePtr = pckg;
    }

    if (pckg) {
        if (!m_sources.count(pckg->getID())) {
            m_sources.add(*pckg);
        }
    }
}

ElementType ProfileApplication::getElementType() const {
    return ElementType::PROFILE_APPLICATION;
}

bool ProfileApplication::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE_APPLICATION;
    }

    return ret;
}
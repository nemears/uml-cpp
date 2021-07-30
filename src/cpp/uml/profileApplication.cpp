#include "uml/profileApplication.h"
#include "uml/profile.h"
#include "uml/universalFunctions.h"

using namespace UML;

ProfileApplication::ProfileApplication() {
    m_appliedProfilePtr = 0;
    m_applyingPackagePtr = 0;
}

ProfileApplication::ProfileApplication(const ProfileApplication& profileApplication) {
    m_appliedProfileID = profileApplication.m_appliedProfileID;
    m_applyingPackageID = profileApplication.m_applyingPackageID;
    if (!profileApplication.m_manager) {
        m_applyingPackagePtr = profileApplication.m_applyingPackagePtr;
        m_appliedProfilePtr = profileApplication.m_appliedProfilePtr;
    }
}

ProfileApplication::~ProfileApplication() {

}

Profile* ProfileApplication::getAppliedProfile() {
    return universalGet<Profile>(m_appliedProfileID, m_appliedProfilePtr, m_manager);
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
    return universalGet<Package>(m_applyingPackageID, m_applyingPackagePtr, m_manager);
}

void ProfileApplication::setApplyingPackage(Package* pckg) {
    if (!isSameOrNull(m_applyingPackageID, pckg)) {
        if (!m_applyingPackagePtr) {
            m_applyingPackagePtr = &m_manager->get<Package>(m_applyingPackageID);
        }

        if (m_sources.count(m_applyingPackageID)) {
            m_sources.remove(*m_applyingPackagePtr);
        }

        if (m_applyingPackagePtr->getProfileApplications().count(m_id)) {
            m_applyingPackagePtr->getProfileApplications().remove(*this);
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

        if (!pckg->getProfileApplications().count(m_id)) {
            pckg->getProfileApplications().add(*this);
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
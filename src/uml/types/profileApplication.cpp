#include "uml/types/profileApplication.h"
#include "uml/types/profile.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Profile, ProfileApplication>& ProfileApplication::getAppliedProfileSingleton() {
    return m_appliedProfile;
}

TypedSet<Package, ProfileApplication>& ProfileApplication::getApplyingPackageSingleton() {
    return m_applyingPackage;
}

ProfileApplication::ProfileApplication() : Element(ElementType::PROFILE_APPLICATION) {
    m_appliedProfile.subsets(m_targets);
    m_applyingPackage.subsets(*m_owner);
    m_applyingPackage.subsets(m_sources);
    m_applyingPackage.opposite(&Package::getProfileApplications);
}

ProfileApplication::~ProfileApplication() {
    
}

ProfilePtr ProfileApplication::getAppliedProfile() const {
    return m_appliedProfile.get();
}

void ProfileApplication::setAppliedProfile(Profile* profile) {
    m_appliedProfile.set(profile);
}

void ProfileApplication::setAppliedProfile(Profile& profile) {
    m_appliedProfile.set(profile);
}

void ProfileApplication::setAppliedProfile(ID id) {
    m_appliedProfile.set(id);
}

PackagePtr ProfileApplication::getApplyingPackage() const {
    return m_applyingPackage.get();
}

void ProfileApplication::setApplyingPackage(Package* pckg) {
    m_applyingPackage.set(pckg);
}

void ProfileApplication::setApplyingPackage(Package& pckg) {
    m_applyingPackage.set(pckg);
}

void ProfileApplication::setApplyingPackage(ID id) {
    m_applyingPackage.set(id);
}

bool ProfileApplication::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE_APPLICATION;
    }

    return ret;
}
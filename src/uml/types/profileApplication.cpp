#include "uml/uml-stable.h"

using namespace UML;

Singleton<Profile, ProfileApplication>& ProfileApplication::getAppliedProfileSingleton() {
    return m_appliedProfile;
}

Singleton<Package, ProfileApplication>& ProfileApplication::getApplyingPackageSingleton() {
    return m_applyingPackage;
}

ProfileApplication::ProfileApplication(std::size_t elementType, AbstractManager& manager) :
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_appliedProfile.subsets(m_targets);
    m_applyingPackage.subsets(m_owner);
    m_applyingPackage.subsets(m_sources);
    m_applyingPackage.opposite(&Package::getProfileApplications);
}

ProfilePtr ProfileApplication::getAppliedProfile() const {
    return m_appliedProfile.get();
}

void ProfileApplication::setAppliedProfile(ProfilePtr profile) {
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

void ProfileApplication::setApplyingPackage(PackagePtr pckg) {
    m_applyingPackage.set(pckg);
}

void ProfileApplication::setApplyingPackage(Package& pckg) {
    m_applyingPackage.set(pckg);
}

void ProfileApplication::setApplyingPackage(ID id) {
    m_applyingPackage.set(id);
}

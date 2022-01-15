#include "uml/profileApplication.h"
#include "uml/profile.h"
#include "uml/uml-stable.h"

using namespace UML;

void ProfileApplication::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_appliedProfile.release(id);
    m_applyingPackage.release(id);
}

void ProfileApplication::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_appliedProfile.reindex(oldID, newID);
    m_applyingPackage.reindex(oldID, newID);
}

void ProfileApplication::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_appliedProfile.eraseElement(id);
    m_applyingPackage.eraseElement(id);
}

Set<Profile, ProfileApplication>& ProfileApplication::getAppliedProfileSingleton() {
    return m_appliedProfile;
}

Set<Package, ProfileApplication>& ProfileApplication::getApplyingPackageSingleton() {
    return m_applyingPackage;
}

void ProfileApplication::init() {
    m_appliedProfile.subsets(m_targets);
    m_appliedProfile.m_signature = &ProfileApplication::getAppliedProfileSingleton;
    m_applyingPackage.subsets(*m_owner);
    m_applyingPackage.subsets(m_sources);
    m_applyingPackage.opposite(&Package::getProfileApplications);
    m_applyingPackage.m_signature = &ProfileApplication::getApplyingPackageSingleton;
}

void ProfileApplication::copy(const ProfileApplication& rhs) {
    m_appliedProfile = rhs.m_appliedProfile;
    m_applyingPackage = rhs.m_applyingPackage;
}

ProfileApplication::ProfileApplication() : Element(ElementType::PROFILE_APPLICATION) {
    init();
}

ProfileApplication::ProfileApplication(const ProfileApplication& rhs) : Element(rhs, ElementType::PROFILE_APPLICATION) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    copy(rhs);
}

ProfileApplication::~ProfileApplication() {

}

Profile* ProfileApplication::getAppliedProfile() {
    return m_appliedProfile.get();
}

Profile& ProfileApplication::getAppliedProfileRef() {
    return m_appliedProfile.getRef();
}

ID ProfileApplication::getAppliedProfileID() const {
    return m_appliedProfile.id();
}

bool ProfileApplication::hasAppliedProfile() const {
    return m_appliedProfile.has();
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

Package* ProfileApplication::getApplyingPackage() {
    return m_applyingPackage.get();
}

Package& ProfileApplication::getApplyingPackageRef() {
    return m_applyingPackage.getRef();
}

ID ProfileApplication::getApplyingPackageID() const {
    return m_applyingPackage.id();
}

bool ProfileApplication::hasApplyingPackage() const {
    return m_applyingPackage.has();
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
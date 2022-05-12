#include "uml/profileApplication.h"
#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<Profile, ProfileApplication>& ProfileApplication::getAppliedProfileSingleton() {
    return m_appliedProfile;
}

Set<Package, ProfileApplication>& ProfileApplication::getApplyingPackageSingleton() {
    return m_applyingPackage;
}

void ProfileApplication::init() {
    m_appliedProfile.subsets(m_targets);
    m_appliedProfile.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_appliedProfile.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_applyingPackage.subsets(*m_owner);
    m_applyingPackage.subsets(m_sources);
    m_applyingPackage.opposite(&Package::getProfileApplications);
}

ProfileApplication::ProfileApplication() : Element(ElementType::PROFILE_APPLICATION) {
    init();
}

ProfileApplication::~ProfileApplication() {
    mountAndRelease();
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
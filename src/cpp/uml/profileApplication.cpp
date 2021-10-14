#include "uml/profileApplication.h"
#include "uml/profile.h"

using namespace UML;

void ProfileApplication::RemoveAppliedProfileProcedure::operator()(Profile* el) const {
    if (m_me->getTargets().count(el->getID())) {
        m_me->getTargets().remove(*el);
    }
}

void ProfileApplication::AddAppliedProfileProcedure::operator()(Profile* el) const {
    if (!m_me->getTargets().count(el->getID())) {
        m_me->getTargets().add(*el);
    }
}

void ProfileApplication::RemoveApplyingPackageProcedure::operator()(Package* el) const {
    if (m_me->getSources().count(el->getID())) {
        m_me->getSources().remove(*el);
    }
    if (el->getProfileApplications().count(m_me->getID())) {
        el->getProfileApplications().remove(*m_me);
    }
}

void ProfileApplication::AddApplyingPackageProcedure::operator()(Package* el) const {
    if (!m_me->getSources().count(el->getID())) {
        m_me->getSources().add(*el);
    }
    if (!el->getProfileApplications().count(m_me->getID())) {
        el->getProfileApplications().add(*m_me);
    }
}

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

void ProfileApplication::restoreReferences() {
    DirectedRelationship::restoreReferences();
    m_appliedProfile.restoreReference();
    m_applyingPackage.restoreReference();
}

void ProfileApplication::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_appliedProfile.elementErased(id);
    m_applyingPackage.elementErased(id);
}

ProfileApplication::ProfileApplication() {
    m_appliedProfile.m_signature = &ProfileApplication::m_appliedProfile;
    m_appliedProfile.m_addProcedures.push_back(new AddAppliedProfileProcedure(this));
    m_appliedProfile.m_removeProcedures.push_back(new RemoveAppliedProfileProcedure(this));
    m_applyingPackage.m_signature = &ProfileApplication::m_applyingPackage;
    m_applyingPackage.m_addProcedures.push_back(new AddApplyingPackageProcedure(this));
    m_applyingPackage.m_removeProcedures.push_back(new RemoveApplyingPackageProcedure(this));
}

ProfileApplication::ProfileApplication(const ProfileApplication& profileApplication) {
    m_appliedProfile = profileApplication.m_appliedProfile;
    m_appliedProfile.m_me = this;
    m_appliedProfile.m_addProcedures.clear();
    m_applyingPackage.m_removeProcedures.clear();
    m_appliedProfile.m_addProcedures.push_back(new AddAppliedProfileProcedure(this));
    m_appliedProfile.m_removeProcedures.push_back(new RemoveAppliedProfileProcedure(this));
    m_applyingPackage = profileApplication.m_applyingPackage;
    m_applyingPackage.m_me = this;
    m_applyingPackage.m_addProcedures.clear();
    m_applyingPackage.m_removeProcedures.clear();
    m_applyingPackage.m_addProcedures.push_back(new AddApplyingPackageProcedure(this));
    m_applyingPackage.m_removeProcedures.push_back(new RemoveApplyingPackageProcedure(this));
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
#include "gtest/gtest.h"
#include "uml/profileApplication.h"
#include "uml/profile.h"

using namespace UML;

class ProfileApplicationTest : public ::testing::Test {};

TEST_F(ProfileApplicationTest, setProfileAndPackageTest) {
    UmlManager m;
    Profile& profile = m.create<Profile>();
    Package& pckg = m.create<Package>();
    ProfileApplication& pa = m.create<ProfileApplication>();
    pa.setAppliedProfile(&profile);
    pa.setApplyingPackage(&pckg);
    ASSERT_TRUE(pa.getAppliedProfile() != 0);
    ASSERT_EQ(pa.getAppliedProfile()->getID(), profile.getID());
    ASSERT_EQ(pa.getTargets().size(), 1);
    ASSERT_EQ(pa.getTargets().front()->getID(), profile.getID());
    ASSERT_TRUE(pa.getApplyingPackage() != 0);
    ASSERT_EQ(pa.getApplyingPackage()->getID(), pckg.getID());
    ASSERT_EQ(pa.getSources().size(), 1);
    ASSERT_EQ(pa.getSources().front()->getID(), pckg.getID());
    ASSERT_TRUE(pa.getOwner() != 0);
    ASSERT_EQ(pa.getOwner()->getID(), pckg.getID());
    ASSERT_EQ(pckg.getProfileApplications().size(), 1);
    ASSERT_EQ(pckg.getProfileApplications().front()->getID(), pa.getID());
    ASSERT_EQ(pckg.getDirectedRelationships().size(), 1);
    ASSERT_EQ(pckg.getDirectedRelationships().front()->getID(), pa.getID());
    ASSERT_EQ(pckg.getOwnedElements().size(), 1);
    ASSERT_EQ(pckg.getOwnedElements().front()->getID(), pa.getID());
}

TEST_F(ProfileApplicationTest, removeProfileApplication) {
    UmlManager m;
    Profile& profile = m.create<Profile>();
    Package& pckg = m.create<Package>();
    ProfileApplication& pa = m.create<ProfileApplication>();
    pa.setAppliedProfile(&profile);
    pa.setApplyingPackage(&pckg);
    pckg.getProfileApplications().remove(pa);
    ASSERT_TRUE(pa.getAppliedProfile() != 0);
    ASSERT_EQ(pa.getAppliedProfile()->getID(), profile.getID());
    ASSERT_EQ(pa.getTargets().size(), 1);
    ASSERT_EQ(pa.getTargets().front()->getID(), profile.getID());
    ASSERT_TRUE(pa.getApplyingPackage() == 0);
    ASSERT_EQ(pa.getSources().size(), 0);
    ASSERT_TRUE(pa.getOwner() == 0);
    ASSERT_EQ(pckg.getProfileApplications().size(), 0);
    ASSERT_EQ(pckg.getDirectedRelationships().size(), 0);
    ASSERT_EQ(pckg.getOwnedElements().size(), 0);
}

TEST_F(ProfileApplicationTest, setApplyingPackageNull) {
    UmlManager m;
    Profile& profile = m.create<Profile>();
    Package& pckg = m.create<Package>();
    ProfileApplication& pa = m.create<ProfileApplication>();
    pa.setAppliedProfile(&profile);
    pa.setApplyingPackage(&pckg);
    pa.setApplyingPackage(0);
    ASSERT_TRUE(pa.getAppliedProfile() != 0);
    ASSERT_EQ(pa.getAppliedProfile()->getID(), profile.getID());
    ASSERT_EQ(pa.getTargets().size(), 1);
    ASSERT_EQ(pa.getTargets().front()->getID(), profile.getID());
    ASSERT_TRUE(pa.getApplyingPackage() == 0);
    ASSERT_EQ(pa.getSources().size(), 0);
    ASSERT_TRUE(pa.getOwner() == 0);
    ASSERT_EQ(pckg.getProfileApplications().size(), 0);
    ASSERT_EQ(pckg.getDirectedRelationships().size(), 0);
    ASSERT_EQ(pckg.getOwnedElements().size(), 0);
}
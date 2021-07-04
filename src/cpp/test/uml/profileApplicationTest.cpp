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
}
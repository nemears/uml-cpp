#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(StereotypeProfile, Profile, Stereotype, &Stereotype::getProfile, &Stereotype::setProfile);

class StereotypeTest : public ::testing::Test {};

TEST_F(StereotypeTest, setProfileTest) {
    BasicManager m;
    Stereotype& s = *m.create<Stereotype>();
    Profile& p = *m.create<Profile>();
    s.setProfile(p);
    ASSERT_TRUE(s.getProfile());
    ASSERT_EQ(s.getProfile()->getID(), p.getID());
}

TEST_F(StereotypeTest, overrideProfileTest) {
    BasicManager m;
    Stereotype& s = *m.create<Stereotype>();
    Profile& p = *m.create<Profile>();
    Profile& p2 = *m.create<Profile>();
    s.setProfile(p);
    s.setProfile(p2);
    ASSERT_TRUE(s.getProfile());
    ASSERT_EQ(s.getProfile()->getID(), p2.getID());
    s.setProfile(0);
    ASSERT_FALSE(s.getProfile());
}
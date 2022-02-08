#include "gtest/gtest.h"
#include "uml/stereotype.h"
#include "uml/profile.h"
#include "uml/uml-stable.h"

using namespace UML;

class StereotypeTest : public ::testing::Test {};

TEST_F(StereotypeTest, setProfileTest) {
    UmlManager m;
    Stereotype& s = m.create<Stereotype>();
    Profile& p = m.create<Profile>();
    s.setProfile(&p);
    ASSERT_TRUE(s.getProfile() != 0);
    ASSERT_EQ(s.getProfile()->getID(), p.getID());
}

TEST_F(StereotypeTest, overrideProfileTest) {
    UmlManager m;
    Stereotype& s = m.create<Stereotype>();
    Profile& p = m.create<Profile>();
    Profile& p2 = m.create<Profile>();
    s.setProfile(&p);
    s.setProfile(&p2);
    ASSERT_TRUE(s.getProfile() != 0);
    ASSERT_EQ(s.getProfile()->getID(), p2.getID());
    s.setProfile(0);
    ASSERT_TRUE(s.getProfile() == 0);
}
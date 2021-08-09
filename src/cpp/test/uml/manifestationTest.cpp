#include "gtest/gtest.h"
#include "uml/manifestation.h"

using namespace UML;

class ManifestationTest : public ::testing::Test {};

TEST_F(ManifestationTest, setUtilizedElementTest) {
    UmlManager m;
    Manifestation& man = m.create<Manifestation>();
    Dependency& d = m.create<Dependency>();
    man.setUtilizedElement(&d);
    ASSERT_TRUE(man.getUtilizedElement() != 0);
    ASSERT_EQ(man.getUtilizedElement(), &d);
    Dependency& d2 = m.create<Dependency>();
    man.setUtilizedElement(&d2);
    ASSERT_TRUE(man.getUtilizedElement() != 0);
    ASSERT_EQ(man.getUtilizedElement(), & d2);
    man.setUtilizedElement(0);
    ASSERT_TRUE(man.getUtilizedElement() == 0);
}
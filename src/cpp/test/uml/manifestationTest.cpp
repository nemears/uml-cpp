#include "gtest/gtest.h"
#include "uml/manifestation.h"
#include "uml/artifact.h"
#include "uml/uml-stable.h"

using namespace UML;

class ManifestationTest : public ::testing::Test {};

TEST_F(ManifestationTest, setUtilizedElementTest) {
    UmlManager m;
    Manifestation& man = *m.create<Manifestation>();
    Dependency& d = *m.create<Dependency>();
    man.setUtilizedElement(d);
    ASSERT_TRUE(man.getUtilizedElement());
    ASSERT_EQ(*man.getUtilizedElement(), d);
    ASSERT_EQ(man.getSupplier().size(), 1);
    ASSERT_EQ(man.getSupplier().front().getID(), d.getID());
    Dependency& d2 = *m.create<Dependency>();
    man.setUtilizedElement(d2);
    ASSERT_TRUE(man.getUtilizedElement());
    ASSERT_EQ(*man.getUtilizedElement(), d2);
    ASSERT_EQ(man.getSupplier().size(), 1);
    ASSERT_EQ(man.getSupplier().front().getID(), d2.getID());
    man.setUtilizedElement(0);
    ASSERT_FALSE(man.getUtilizedElement());
    ASSERT_EQ(man.getSupplier().size(), 0);
}
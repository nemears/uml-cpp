#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(ManifestationUtilizedElement, Package, Manifestation, &Manifestation::getUtilizedElement, &Manifestation::setUtilizedElement)

class ManifestationTest : public ::testing::Test {};

TEST_F(ManifestationTest, setUtilizedElementTest) {
    UmlManager m;
    Manifestation& man = *m.create<Manifestation>();
    Dependency& d = *m.create<Dependency>();
    man.setUtilizedElement(d);
    ASSERT_TRUE(man.getUtilizedElement());
    ASSERT_EQ(*man.getUtilizedElement(), d);
    ASSERT_EQ(man.getSuppliers().size(), 1);
    ASSERT_EQ(man.getSuppliers().front()->getID(), d.getID());
    Dependency& d2 = *m.create<Dependency>();
    man.setUtilizedElement(d2);
    ASSERT_TRUE(man.getUtilizedElement());
    ASSERT_EQ(*man.getUtilizedElement(), d2);
    ASSERT_EQ(man.getSuppliers().size(), 1);
    ASSERT_EQ(man.getSuppliers().front()->getID(), d2.getID());
    man.setUtilizedElement(0);
    ASSERT_FALSE(man.getUtilizedElement());
    ASSERT_EQ(man.getSuppliers().size(), 0);
}

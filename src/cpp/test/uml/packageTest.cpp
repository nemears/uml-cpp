#include "gtest/gtest.h"
#include "uml/package.h"
#include "uml/packageableElement.h"

using namespace UML;

class PackageTest : public ::testing::Test {
   
};

TEST_F(PackageTest, addPackagedElementTest) {
    Package p;
    PackageableElement e;
    ASSERT_NO_THROW(p.getPackagedElements().add(e));
    ASSERT_TRUE(p.getPackagedElements().size() == 1);
    ASSERT_TRUE(p.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p);
    ASSERT_TRUE(p.getMembers().size() == 1);
    ASSERT_TRUE(p.getMembers().front() == &e);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().front() == &e);
    ASSERT_TRUE(e.getNamespace() == &p);
    ASSERT_TRUE(e.getOwner() == &p);
}

TEST_F(PackageTest, setOwningPackageTest) {
    Package p;
    PackageableElement e;
    ASSERT_NO_THROW(e.setOwningPackage(&p));
    ASSERT_TRUE(p.getPackagedElements().size() == 1);
    ASSERT_TRUE(p.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p);
    ASSERT_TRUE(p.getMembers().size() == 1);
    ASSERT_TRUE(p.getMembers().front() == &e);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().front() == &e);
    ASSERT_TRUE(e.getNamespace() == &p);
    ASSERT_TRUE(e.getOwner() == &p);
}

TEST_F(PackageTest, overwriteOwningPackageTest) {
    Package p1;
    Package p2;
    PackageableElement e;
    p1.getPackagedElements().add(e);
    ASSERT_NO_THROW(e.setOwningPackage(&p2));
    ASSERT_TRUE(p2.getPackagedElements().size() == 1);
    ASSERT_TRUE(p2.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p2);
    ASSERT_TRUE(p2.getMembers().size() == 1);
    ASSERT_TRUE(p2.getMembers().front() == &e);
    ASSERT_TRUE(p2.getOwnedElements().size() == 1);
    ASSERT_TRUE(p2.getOwnedElements().front() == &e);
    ASSERT_TRUE(e.getNamespace() == &p2);
    ASSERT_TRUE(e.getOwner() == &p2);
    ASSERT_TRUE(p1.getPackagedElements().size() == 0);
    ASSERT_TRUE(p1.getMembers().size() == 0);
    ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}
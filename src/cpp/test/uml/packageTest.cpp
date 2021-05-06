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

TEST_F(PackageTest, overwritePackagedElementsTest) {
    // TODO Error or overwrite when we do p2.getPackagedElements().add(e) instead of e.setowningPackage(&p2) in above test
}

TEST_F(PackageTest, copyPackageTest) {
    Package s;
    Package p;
    s.getPackagedElements().add(p);
    p.setName("test");
    Package c;
    p.getPackagedElements().add(c);
    Package p2 = p;
    ASSERT_TRUE(p2.getID() == p.getID());
    ASSERT_TRUE(p2.getName().compare("test") == 0);
    ASSERT_TRUE(p2.getPackagedElements().size() == 1);
    ASSERT_TRUE(p2.getPackagedElements().front() == &c);
    ASSERT_TRUE(p2.getOwningPackage() == &s);
}

TEST_F(PackageTest, packageMergeTest) {
    Package p;
    PackageMerge m;
    Package mp;
    ASSERT_NO_THROW(m.setMergedPackage(&mp));
    ASSERT_NO_THROW(p.getPackageMerge().add(m));
    ASSERT_TRUE(p.getPackageMerge().size() == 1);
    ASSERT_TRUE(p.getPackageMerge().front() == &m);
    ASSERT_TRUE(p.getDirectedRelationships().size() == 1);
    ASSERT_TRUE(p.getDirectedRelationships().front() == &m);
    ASSERT_TRUE(p.getRelationships().size() == 1);
    ASSERT_TRUE(p.getRelationships().front() == &m);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().front() == &m);

    ASSERT_TRUE(m.getMergedPackage() == &mp);
    ASSERT_TRUE(m.getReceivingPackage() == &p);
    ASSERT_TRUE(m.getSources().size() == 1);
    ASSERT_TRUE(m.getSources().front() == &p);
    ASSERT_TRUE(m.getTargets().size() == 1);
    ASSERT_TRUE(m.getTargets().front() == &mp);
    ASSERT_TRUE(m.getTargets().front() == &mp);
}
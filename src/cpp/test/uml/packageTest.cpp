#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class PackageTest : public ::testing::Test {
   
};

TEST_F(PackageTest, addPackagedElementTest) {
    UmlManager m;
    Package& p = *m.create<Package>();
    Package& e = *m.create<Package>();
    ASSERT_NO_THROW(p.getPackagedElements().add(e));
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getPackagedElements().front(), e);
    ASSERT_TRUE(e.getOwningPackage());
    ASSERT_EQ(*e.getOwningPackage(), p);
    ASSERT_EQ(p.getMembers().size(), 1);
    ASSERT_EQ(p.getMembers().front(), e);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().front(), e);
    ASSERT_TRUE(e.getNamespace());
    ASSERT_EQ(*e.getNamespace(), p);
    ASSERT_EQ(*e.getOwner(), p);
}

TEST_F(PackageTest, setOwningPackageTest) {
    UmlManager m;
    Package& p = *m.create<Package>();
    PackageableElement& e = *m.create<Package>();
    ASSERT_NO_THROW(e.setOwningPackage(&p));
    ASSERT_TRUE(p.getPackagedElements().size() == 1);
    ASSERT_TRUE(&p.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p);
    ASSERT_TRUE(p.getMembers().size() == 1);
    ASSERT_TRUE(&p.getMembers().front() == &e);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(&p.getOwnedElements().get(e.getID()) == &e);
    ASSERT_TRUE(e.getNamespace() == &p);
    ASSERT_TRUE(e.getOwner() == &p);
}

TEST_F(PackageTest, overwriteOwningPackageTest) {
    UmlManager m;
    Package& p1 = *m.create<Package>();
    Package& p2 = *m.create<Package>();
    PackageableElement& e = *m.create<Package>();
    p1.getPackagedElements().add(e);
    ASSERT_NO_THROW(e.setOwningPackage(&p2));
    ASSERT_TRUE(p2.getPackagedElements().size() == 1);
    ASSERT_TRUE(&p2.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p2);
    ASSERT_TRUE(p2.getMembers().size() == 1);
    ASSERT_TRUE(&p2.getMembers().front() == &e);
    ASSERT_TRUE(p2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&p2.getOwnedElements().get(e.getID()) == &e);
    ASSERT_TRUE(e.getNamespace() == &p2);
    ASSERT_TRUE(e.getOwner() == &p2);
    ASSERT_TRUE(p1.getPackagedElements().size() == 0);
    ASSERT_TRUE(p1.getMembers().size() == 0);
    ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(PackageTest, packageMergeTest) {
    UmlManager mm;
    Package& p = *mm.create<Package>();
    PackageMerge& m = *mm.create<PackageMerge>();
    Package& mp = *mm.create<Package>();
    m.setMergedPackage(&mp);
    p.getPackageMerge().add(m);
    ASSERT_EQ(p.getPackageMerge().size(), 1);
    ASSERT_EQ(p.getPackageMerge().front(), m);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().get(m.getID()), m);

    ASSERT_EQ(*m.getMergedPackage(), mp);
    ASSERT_EQ(*m.getReceivingPackage(), p);
    ASSERT_EQ(m.getSources().size(), 1);
    ASSERT_EQ(m.getSources().front(), p);
    ASSERT_EQ(m.getTargets().size(), 1);
    ASSERT_EQ(m.getTargets().front(), mp);
    ASSERT_EQ(m.getTargets().front(), mp);
}

TEST_F(PackageTest, removePackageMergeTest) {
    UmlManager mm;
    Package& p = *mm.create<Package>();
    PackageMerge& m = *mm.create<PackageMerge>();
    p.getPackageMerge().add(m);
    ASSERT_NO_THROW(p.getPackageMerge().remove(m));
    ASSERT_TRUE(p.getPackageMerge().size() == 0);
    ASSERT_TRUE(p.getOwnedElements().size() == 0);
    ASSERT_FALSE(m.getReceivingPackage());
    ASSERT_TRUE(m.getSources().size() == 0);
    ASSERT_TRUE(m.getRelatedElements().size() == 0);
}

// TEST_F(PackageTest, addOwnedStereotype) {
//     UmlManager m;
//     Profile& p = m.create<Profile>();
//     p.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");
//     Stereotype& s = m.create<Stereotype>();
//     s.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
//     p.getOwnedStereotypes().add(s);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
//     ASSERT_EQ(p.getOwnedStereotypes().front().getID(), s.getID());
//     ASSERT_EQ(p.getPackagedElements().size(), 1);
//     ASSERT_EQ(p.getPackagedElements().front().getID(), s.getID());
//     ASSERT_EQ(p.getOwnedMembers().size(), 1);
//     ASSERT_TRUE(p.getOwnedMembers().contains(s));
//     ASSERT_EQ(p.getMembers().size(), 1);
//     ASSERT_TRUE(p.getMembers().contains(s));
//     ASSERT_EQ(p.getOwnedElements().size(), 1);
//     ASSERT_TRUE(p.getOwnedElements().contains(s));
    
//     // now with another element in packagedElements
//     Profile& p2 = m.create<Profile>();
//     Stereotype& s2 = m.create<Stereotype>();
//     s2.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
//     Package& pp = m.create<Package>();
//     pp.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAF");
//     p2.getPackagedElements().add(pp);
//     p2.getOwnedStereotypes().add(s2);
//     ASSERT_EQ(p2.getOwnedStereotypes().size(), 1);
//     ASSERT_EQ(p2.getPackagedElements().size(), 2);
//     ASSERT_TRUE(p2.getOwnedStereotypes().contains(s2));
//     ASSERT_FALSE(p2.getOwnedStereotypes().contains(pp.getID()));
//     ASSERT_TRUE(p2.getPackagedElements().contains(s2));
//     ASSERT_TRUE(p2.getPackagedElements().contains(pp));
//     ASSERT_EQ(p2.getOwnedMembers().size(), 2);
//     ASSERT_TRUE(p2.getOwnedMembers().contains(s2));
//     ASSERT_TRUE(p2.getOwnedMembers().contains(pp));
//     ASSERT_EQ(p2.getMembers().size(), 2);
//     ASSERT_TRUE(p2.getMembers().contains(s2));
//     ASSERT_TRUE(p2.getMembers().contains(pp));
//     ASSERT_EQ(p2.getOwnedElements().size(), 2);
//     ASSERT_TRUE(p2.getOwnedElements().contains(s2));
//     ASSERT_TRUE(p2.getOwnedElements().contains(pp));
// }

// TEST_F(PackageTest, removeOwnedStereotype) {
//     UmlManager m;
//     Profile& p = m.create<Profile>();
//     Stereotype& s = m.create<Stereotype>();
//     p.getOwnedStereotypes().add(s);
//     p.getOwnedStereotypes().remove(s);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
//     ASSERT_EQ(p.getPackagedElements().size(), 0);
//     Package& pp = m.create<Package>();
//     s.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
//     p.getPackagedElements().add(pp);
//     p.getOwnedStereotypes().add(s);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
//     ASSERT_EQ(p.getPackagedElements().size(), 2);
//     ASSERT_TRUE(p.getOwnedStereotypes().contains(s));
//     ASSERT_TRUE(p.getPackagedElements().contains(s));
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(pp.getID()));
//     ASSERT_TRUE(p.getPackagedElements().contains(pp));
//     p.getOwnedStereotypes().remove(s);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
//     ASSERT_EQ(p.getPackagedElements().size(), 1);
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(s));
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(pp.getID()));
//     ASSERT_FALSE(p.getPackagedElements().contains(s));
//     ASSERT_TRUE(p.getPackagedElements().contains(pp));
// }

// TEST_F(PackageTest, inTreeRemoveOwnedStereotype) {
//     UmlManager m;
//     Package& p = m.create<Package>();
//     Stereotype& s = m.create<Stereotype>();
//     PackageMerge& pm = m.create<PackageMerge>();
//     p.getOwnedStereotypes().add(s);
//     p.getPackageMerge().add(pm);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
//     ASSERT_EQ(p.getPackagedElements().size(), 1);
//     ASSERT_EQ(p.getOwnedMembers().size(), 1);
//     ASSERT_EQ(p.getMembers().size(), 1);
//     ASSERT_EQ(p.getPackageMerge().size(), 1);
//     ASSERT_EQ(p.getOwnedElements().size(), 2);
//     ASSERT_TRUE(p.getOwnedStereotypes().contains(s));
//     ASSERT_TRUE(p.getPackagedElements().contains(s));
//     ASSERT_TRUE(p.getOwnedMembers().contains(s));
//     ASSERT_TRUE(p.getMembers().contains(s));
//     ASSERT_TRUE(p.getOwnedElements().contains(s));
//     ASSERT_FALSE(p.getPackageMerge().contains(s.getID()));
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(pm.getID()));
//     ASSERT_FALSE(p.getPackagedElements().contains(pm.getID()));
//     ASSERT_FALSE(p.getOwnedMembers().contains(pm.getID()));
//     ASSERT_FALSE(p.getMembers().contains(pm.getID()));
//     ASSERT_TRUE(p.getOwnedElements().contains(pm));
//     ASSERT_TRUE(p.getPackageMerge().contains(pm));
//     p.getOwnedStereotypes().remove(s);
//     ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
//     ASSERT_EQ(p.getPackagedElements().size(), 0);
//     ASSERT_EQ(p.getOwnedMembers().size(), 0);
//     ASSERT_EQ(p.getMembers().size(), 0);
//     ASSERT_EQ(p.getPackageMerge().size(), 1);
//     ASSERT_EQ(p.getOwnedElements().size(), 1);
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(s));
//     ASSERT_FALSE(p.getPackagedElements().contains(s));
//     ASSERT_FALSE(p.getOwnedMembers().contains(s));
//     ASSERT_FALSE(p.getMembers().contains(s));
//     ASSERT_FALSE(p.getOwnedElements().contains(s));
//     ASSERT_FALSE(p.getPackageMerge().contains(s.getID()));
//     ASSERT_FALSE(p.getOwnedStereotypes().contains(pm.getID()));
//     ASSERT_FALSE(p.getPackagedElements().contains(pm.getID()));
//     ASSERT_FALSE(p.getOwnedMembers().contains(pm.getID()));
//     ASSERT_FALSE(p.getMembers().contains(pm.getID()));
//     ASSERT_TRUE(p.getOwnedElements().contains(pm));
//     ASSERT_TRUE(p.getPackageMerge().contains(pm));
// }

TEST_F(PackageTest, erasePackagedElementTest) {
    UmlManager m;
    Package& parent = *m.create<Package>();
    Package& child = *m.create<Package>();
    parent.getPackagedElements().add(child);
    m.erase(child);
    ASSERT_TRUE(parent.getPackagedElements().empty());
    ASSERT_TRUE(parent.getOwnedMembers().empty());
    ASSERT_TRUE(parent.getMembers().empty());
    ASSERT_TRUE(parent.getOwnedElements().empty());
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

class PackageTest : public ::testing::Test {
   
};

TEST_F(PackageTest, addPackagedElementTest) {
    UmlManager m;
    Package p = m.create<Package>();
    Package e = m.create<Package>();
    ASSERT_NO_THROW(p.getPackagedElements().add(e));
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getPackagedElements().front(), e);
    ASSERT_TRUE(e.hasOwningPackage());
    ASSERT_TRUE(e.getOwningPackage());
    ASSERT_EQ(e.getOwningPackageRef(), p);
    ASSERT_EQ(p.getMembers().size(), 1);
    ASSERT_EQ(p.getMembers().front(), e);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().get(e.getID()), e);
    ASSERT_TRUE(e.hasNamespace());
    ASSERT_TRUE(e.getNamespace());
    ASSERT_EQ(e.getNamespaceRef(), p);
    ASSERT_EQ(*e.getOwner(), p);
}

TEST_F(PackageTest, setOwningPackageTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    PackageableElement& e = m.create<Package>();
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
    Package& p1 = m.create<Package>();
    Package& p2 = m.create<Package>();
    PackageableElement& e = m.create<Package>();
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

TEST_F(PackageTest, overwritePackagedElementsTest) {
    // TODO Error or overwrite when we do p2.getPackagedElements().add(e) instead of e.setowningPackage(&p2) in above test
}

TEST_F(PackageTest, copyPackageTest) {
    UmlManager m;
    Package& s = m.create<Package>();
    Package& p = m.create<Package>();
    s.getPackagedElements().add(p);
    p.setName("test");
    Package& c = m.create<Package>();
    p.getPackagedElements().add(c);
    Package p2 = p;
    ASSERT_TRUE(p2.getID() == p.getID());
    ASSERT_TRUE(p2.getName().compare("test") == 0);
    ASSERT_TRUE(p2.getPackagedElements().size() == 1);
    ASSERT_TRUE(&p2.getPackagedElements().front() == &c);
    ASSERT_TRUE(p2.getOwningPackage() == &s);
}

TEST_F(PackageTest, packageMergeTest) {
    UmlManager mm;
    Package p = mm.create<Package>();
    PackageMerge m = mm.create<PackageMerge>();
    Package mp = mm.create<Package>();
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
    Package p = mm.create<Package>();
    PackageMerge m = mm.create<PackageMerge>();
    p.getPackageMerge().add(m);
    ASSERT_NO_THROW(p.getPackageMerge().remove(m));
    ASSERT_TRUE(p.getPackageMerge().size() == 0);
    // ASSERT_TRUE(p.getDirectedRelationships().size() == 0);
    ASSERT_TRUE(p.getOwnedElements().size() == 0);
    // ASSERT_TRUE(p.getRelationships().size() == 0);
    ASSERT_TRUE(m.getReceivingPackage() == 0);
    ASSERT_TRUE(m.getSources().size() == 0);
    ASSERT_TRUE(m.getRelatedElements().size() == 0);
}

TEST_F(PackageTest, addOwnedStereotype) {
    UmlManager m;
    Profile& p = m.create<Profile>();
    Stereotype& s = m.create<Stereotype>();
    p.getOwnedStereotypes().add(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(p.getOwnedStereotypes().front().getID(), s.getID());
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getPackagedElements().front().getID(), s.getID());
}

TEST_F(PackageTest, removeOwnedStereotype) {
    UmlManager m;
    Profile& p = m.create<Profile>();
    Stereotype& s = m.create<Stereotype>();
    p.getOwnedStereotypes().add(s);
    p.getOwnedStereotypes().remove(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
    ASSERT_EQ(p.getPackagedElements().size(), 0);
}

// TEST_F(PackageTest, packageFullCopyAndEditTest) {
//     UmlManager m;
//     Package& root = m.create<Package>();
//     Package& c1 = m.create<Package>();
//     Package& merged = m.create<Package>();
//     Profile& profile = m.create<Profile>();
//     PackageMerge& merge = m.create<PackageMerge>();
//     ProfileApplication& profileApplication = m.create<ProfileApplication>();
//     Stereotype& stereotype = m.create<Stereotype>();
//     c1.getOwnedStereotypes().add(stereotype);
//     merge.setMergedPackage(&merged);
//     c1.getPackageMerge().add(merge);
//     profileApplication.setAppliedProfile(&profile);
//     c1.getProfileApplications().add(profileApplication);
//     root.getPackagedElements().add(c1, merged, profile);
//     Package& copy = c1;
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c1, copy, &Package::getPackagedElements, &Package::getPackageMerge, &Package::getProfileApplications, &Package::getOwnedStereotypes, &Namespace::getOwnedMembers, &Namespace::getMembers, &NamedElement::getMemberNamespace, &Element::getOwnedElements));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c1, copy, &NamedElement::getNamespace, &Element::getOwner));
//     copy.getOwnedStereotypes().remove(stereotype);
//     copy.getPackageMerge().remove(merge);
//     copy.getProfileApplications().remove(profileApplication);
//     root.getPackagedElements().remove(copy);
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c1, copy, &Package::getPackagedElements, &Package::getPackageMerge, &Package::getProfileApplications, &Package::getOwnedStereotypes, &Namespace::getOwnedMembers, &Namespace::getMembers, &NamedElement::getMemberNamespace, &Element::getOwnedElements));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c1, copy, &NamedElement::getNamespace, &Element::getOwner));
//     copy.getOwnedStereotypes().add(stereotype);
//     copy.getPackageMerge().add(merge);
//     copy.getProfileApplications().add(profileApplication);
//     root.getPackagedElements().add(copy);
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c1, copy, &Package::getPackagedElements, &Package::getPackageMerge, &Package::getProfileApplications, &Package::getOwnedStereotypes, &Namespace::getOwnedMembers, &Namespace::getMembers, &NamedElement::getMemberNamespace, &Element::getOwnedElements));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c1, copy, &NamedElement::getNamespace, &Element::getOwner));
//     c1.getOwnedStereotypes().remove(stereotype);
//     c1.getPackageMerge().remove(merge);
//     c1.getProfileApplications().remove(profileApplication);
//     c1.setOwningPackage(0);
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c1, copy, &Package::getPackagedElements, &Package::getPackageMerge, &Package::getProfileApplications, &Package::getOwnedStereotypes, &Namespace::getOwnedMembers, &Namespace::getMembers, &NamedElement::getMemberNamespace, &Element::getOwnedElements));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c1, copy, &NamedElement::getNamespace, &Element::getOwner));
// }

TEST_F(PackageTest, erasePackagedElementTest) {
    UmlManager m;
    Package& parent = m.create<Package>();
    Package& child = m.create<Package>();
    parent.getPackagedElements().add(child);
    m.erase(child);
    ASSERT_TRUE(parent.getPackagedElements().empty());
    ASSERT_TRUE(parent.getOwnedMembers().empty());
    ASSERT_TRUE(parent.getMembers().empty());
    ASSERT_TRUE(parent.getOwnedElements().empty());
}
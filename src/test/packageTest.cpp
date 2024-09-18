#include "gtest/gtest.h"
#include "uml/types/extension.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

class PackageTest : public ::testing::Test {
   public:
        std::string ymlPath = YML_FILES_PATH;
};

UML_SET_INTEGRATION_TEST(PackagePackagedElements, DataType, Package, &Package::getPackagedElements)
UML_SINGLETON_INTEGRATION_TEST(PackageableElementOwningPackage, Package, Package, &PackageableElement::getOwningPackage, &PackageableElement::setOwningPackage)
UML_SET_INTEGRATION_TEST(PackagePackageMerges, PackageMerge, Package, &Package::getPackageMerge)
UML_SINGLETON_INTEGRATION_TEST(PackageMergeReceivingPackage, Package, PackageMerge, &PackageMerge::getReceivingPackage, &PackageMerge::setReceivingPackage)
UML_SINGLETON_INTEGRATION_TEST(PackageMergeMergedPackage, Package, PackageMerge, &PackageMerge::getMergedPackage, &PackageMerge::setMergedPackage)
UML_SINGLETON_INTEGRATION_TEST(ExtensionOwnedEnd, ExtensionEnd, Extension, &Extension::getOwnedEnd, &Extension::setOwnedEnd)

TEST_F(PackageTest, addPackagedElementTest) {
    UmlManager m;
    Package& p = *m.create<Package>();
    Package& e = *m.create<Package>();
    ASSERT_NO_THROW(p.getPackagedElements().add(e));
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getPackagedElements().front(), &e);
    ASSERT_TRUE(e.getOwningPackage());
    ASSERT_EQ(*e.getOwningPackage(), p);
    ASSERT_EQ(p.getMembers().size(), 1);
    ASSERT_EQ(p.getMembers().front(), &e);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().front(), &e);
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
    ASSERT_TRUE(p.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p);
    ASSERT_TRUE(p.getMembers().size() == 1);
    ASSERT_TRUE(p.getMembers().front() == &e);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().get(e.getID()) == &e);
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
    ASSERT_TRUE(p2.getPackagedElements().front() == &e);
    ASSERT_TRUE(e.getOwningPackage() == &p2);
    ASSERT_TRUE(p2.getMembers().size() == 1);
    ASSERT_TRUE(p2.getMembers().front() == &e);
    ASSERT_TRUE(p2.getOwnedElements().size() == 1);
    ASSERT_TRUE(p2.getOwnedElements().get(e.getID()) == &e);
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
    ASSERT_EQ(p.getPackageMerge().front(), &m);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().get(m.getID()), &m);

    ASSERT_EQ(*m.getMergedPackage(), mp);
    ASSERT_EQ(*m.getReceivingPackage(), p);
    ASSERT_EQ(m.getSources().size(), 1);
    ASSERT_EQ(m.getSources().front(), &p);
    ASSERT_EQ(m.getTargets().size(), 1);
    ASSERT_EQ(m.getTargets().front(), &mp);
    ASSERT_EQ(m.getTargets().front(), &mp);
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

TEST_F(PackageTest, simpleStereotypeTest) {
    UmlManager m;
    Profile& p = *m.create<Profile>();
    p.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAD"));
    Stereotype& s = *m.create<Stereotype>();
    s.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAC"));
    p.getPackagedElements().add(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(p.getOwnedStereotypes().front()->getID(), s.getID());
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getPackagedElements().front()->getID(), s.getID());
    ASSERT_EQ(p.getOwnedMembers().size(), 1);
    ASSERT_TRUE(p.getOwnedMembers().contains(s));
    ASSERT_EQ(p.getMembers().size(), 1);
    ASSERT_TRUE(p.getMembers().contains(s));
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_TRUE(p.getOwnedElements().contains(s));
}

TEST_F(PackageTest, addOwnedStereotype) {
    UmlManager m;
    Profile& p2 = *m.create<Profile>();
    Stereotype& s2 = *m.create<Stereotype>();
    s2.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAB"));
    Package& pp = *m.create<Package>();
    pp.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAF"));
    p2.getPackagedElements().add(pp);
    p2.getPackagedElements().add(s2);
    ASSERT_EQ(p2.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(p2.getPackagedElements().size(), 2);
    ASSERT_TRUE(p2.getOwnedStereotypes().contains(s2));
    ASSERT_FALSE(p2.getOwnedStereotypes().contains(pp.getID()));
    ASSERT_TRUE(p2.getPackagedElements().contains(s2));
    ASSERT_TRUE(p2.getPackagedElements().contains(pp));
    ASSERT_EQ(p2.getOwnedMembers().size(), 2);
    ASSERT_TRUE(p2.getOwnedMembers().contains(s2));
    ASSERT_TRUE(p2.getOwnedMembers().contains(pp));
    ASSERT_EQ(p2.getMembers().size(), 2);
    ASSERT_TRUE(p2.getMembers().contains(s2));
    ASSERT_TRUE(p2.getMembers().contains(pp));
    ASSERT_EQ(p2.getOwnedElements().size(), 2);
    ASSERT_TRUE(p2.getOwnedElements().contains(s2));
    ASSERT_TRUE(p2.getOwnedElements().contains(pp));
}

TEST_F(PackageTest, removeOwnedStereotype) {
    UmlManager m;
    Profile& p = *m.create<Profile>();
    Stereotype& s = *m.create<Stereotype>();
    s.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAB"));
    p.getPackagedElements().add(s);
    p.getPackagedElements().remove(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
    ASSERT_EQ(p.getPackagedElements().size(), 0);
    Package& pp = *m.create<Package>();
    p.getPackagedElements().add(pp);
    p.getPackagedElements().add(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(p.getPackagedElements().size(), 2);
    ASSERT_TRUE(p.getOwnedStereotypes().contains(s));
    ASSERT_TRUE(p.getPackagedElements().contains(s));
    ASSERT_FALSE(p.getOwnedStereotypes().contains(pp.getID()));
    ASSERT_TRUE(p.getPackagedElements().contains(pp));
    p.getPackagedElements().remove(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_FALSE(p.getOwnedStereotypes().contains(s));
    ASSERT_FALSE(p.getOwnedStereotypes().contains(pp.getID()));
    ASSERT_FALSE(p.getPackagedElements().contains(s));
    ASSERT_TRUE(p.getPackagedElements().contains(pp));
}

TEST_F(PackageTest, inTreeRemoveOwnedStereotype) {
    UmlManager m;
    Package& p = *m.create<Package>();
    Stereotype& s = *m.create<Stereotype>();
    PackageMerge& pm = *m.create<PackageMerge>();
    p.getPackagedElements().add(s);
    p.getPackageMerge().add(pm);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(p.getPackagedElements().size(), 1);
    ASSERT_EQ(p.getOwnedMembers().size(), 1);
    ASSERT_EQ(p.getMembers().size(), 1);
    ASSERT_EQ(p.getPackageMerge().size(), 1);
    ASSERT_EQ(p.getOwnedElements().size(), 2);
    ASSERT_TRUE(p.getOwnedStereotypes().contains(s));
    ASSERT_TRUE(p.getPackagedElements().contains(s));
    ASSERT_TRUE(p.getOwnedMembers().contains(s));
    ASSERT_TRUE(p.getMembers().contains(s));
    ASSERT_TRUE(p.getOwnedElements().contains(s));
    ASSERT_FALSE(p.getPackageMerge().contains(s.getID()));
    ASSERT_FALSE(p.getOwnedStereotypes().contains(pm.getID()));
    ASSERT_FALSE(p.getPackagedElements().contains(pm.getID()));
    ASSERT_FALSE(p.getOwnedMembers().contains(pm.getID()));
    ASSERT_FALSE(p.getMembers().contains(pm.getID()));
    ASSERT_TRUE(p.getOwnedElements().contains(pm));
    ASSERT_TRUE(p.getPackageMerge().contains(pm));
    p.getPackagedElements().remove(s);
    ASSERT_EQ(p.getOwnedStereotypes().size(), 0);
    ASSERT_EQ(p.getPackagedElements().size(), 0);
    ASSERT_EQ(p.getOwnedMembers().size(), 0);
    ASSERT_EQ(p.getMembers().size(), 0);
    ASSERT_EQ(p.getPackageMerge().size(), 1);
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_FALSE(p.getOwnedStereotypes().contains(s));
    ASSERT_FALSE(p.getPackagedElements().contains(s));
    ASSERT_FALSE(p.getOwnedMembers().contains(s));
    ASSERT_FALSE(p.getMembers().contains(s));
    ASSERT_FALSE(p.getOwnedElements().contains(s));
    ASSERT_FALSE(p.getPackageMerge().contains(s.getID()));
    ASSERT_FALSE(p.getOwnedStereotypes().contains(pm.getID()));
    ASSERT_FALSE(p.getPackagedElements().contains(pm.getID()));
    ASSERT_FALSE(p.getOwnedMembers().contains(pm.getID()));
    ASSERT_FALSE(p.getMembers().contains(pm.getID()));
    ASSERT_TRUE(p.getOwnedElements().contains(pm));
    ASSERT_TRUE(p.getPackageMerge().contains(pm));
}

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

TEST_F(PackageTest, parse3PackagesTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "packageParserTests/3packages.yml"));
    ASSERT_TRUE(m.getRoot()->is<Package>());
    Package* pckg1 = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg1->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg1->getPackagedElements().front()->is<Package>());
    ASSERT_TRUE((pckg1->getPackagedElements().begin()++)->is<Package>());
    PackagePtr pckg2 = pckg1->getPackagedElements().front();
    PackagePtr pckg3 = &((pckg1->getPackagedElements().begin()++)->as<Package>());
    ASSERT_TRUE(pckg2->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg3->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg1->getMembers().size() == 2);
    ASSERT_TRUE(pckg1->getMembers().front() == pckg2);
    ASSERT_TRUE(*(pckg1->getMembers().begin()++) == *pckg3);
    ASSERT_TRUE(pckg2->getNamespace() == pckg1);
    ASSERT_TRUE(pckg3->getNamespace() == pckg1);
    ASSERT_TRUE(pckg1->getOwnedElements().size() == 2);
    auto it = pckg1->getOwnedElements().begin();
    ASSERT_EQ(*it, *pckg2);
    ++it;
    ASSERT_EQ(*it, *pckg3);
    ASSERT_TRUE(pckg2->getOwner() == pckg1);
    ASSERT_TRUE(pckg3->getOwner() == pckg1);
    ASSERT_TRUE(pckg2->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg2->getPackagedElements().front()->is<Class>());
    ClassPtr act1 = pckg2->getPackagedElements().front();
    ASSERT_TRUE(act1->getOwningPackage() == pckg2);
    ASSERT_TRUE(act1->getNamespace() == pckg2);
    ASSERT_TRUE(act1->getOwner() == pckg2);
    ASSERT_TRUE(pckg3->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg3->getPackagedElements().front()->is<Class>());
    ClassPtr act2 = pckg3->getPackagedElements().front();
    ASSERT_TRUE(act2->getOwningPackage() == pckg3);
    ASSERT_TRUE(act2->getNamespace() == pckg3);
    ASSERT_TRUE(act2->getOwner() == pckg3);
}

// TEST_F(PackageTest, NamedElementFeaturesTest) {
//     Manager* m;
//     ASSERT_NO_THROW(m = Parsers::parse(ymlPath + "packageParserTests/packageWithName.yml"));
//     el = & m->get(ID::fromString("8q2Rht9aAZlY0EnMGtEKlw5Odr_u"));
//     ASSERT_TRUE(el->getElementType() == Package::Info::elementType);
//     Package* pckg = dynamic_cast<Package*>(el);
//     ASSERT_TRUE(pckg->getName().compare("test") == 0);
//     delete m;
// }

TEST_F(PackageTest, ElementFeaturesTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "packageParserTests/packagewID.yml"));
    ASSERT_TRUE(m.getRoot()->is<Package>());
    ASSERT_TRUE(m.getRoot()->getID() == ID::fromString("Hmq5HbrypUzqPYovV8oo3wnFE6Jl"));
}

TEST_F(PackageTest, ElementParserExceptionTest) {
    UmlManager m;
    ASSERT_THROW(m.open(ymlPath + "packageParserTests/improperID.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "packageParserTests/otherImproperID.yml"), InvalidUmlID_Exception);
}

TEST_F(PackageTest, NamedElementParserExceptionTest) {
    UmlManager m;
    ASSERT_THROW(m.open(ymlPath + "packageParserTests/improperName.yml"), SerializationError);
}

TEST_F(PackageTest, properExceptions) {
    UmlManager m;
    ASSERT_THROW(m.open(ymlPath + "packageParserTests/improperPackagedElement.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "packageParserTests/invalidPackagedElements.yml"), SerializationError);
}

TEST_F(PackageTest, basicPackageMerge) {
    UmlManager m2;
    ASSERT_NO_THROW(m2.open(ymlPath + "packageParserTests/basicPackageMerge.yml"));
    ASSERT_TRUE(m2.getRoot()->is<Package>());
    Package* bPckg = &m2.getRoot()->as<Package>();
    ASSERT_TRUE(bPckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(bPckg->getPackagedElements().front()->is<Package>());
    PackagePtr pckg1 = bPckg->getPackagedElements().get(ID::fromString("CLZW&6KK6mcu4qLEXNULDidsDpkX"));
    ASSERT_TRUE((bPckg->getPackagedElements().begin()++)->is<Package>());
    PackagePtr pckg2 = bPckg->getPackagedElements().get(ID::fromString("NrvESaGrSk1vI2aksOyHy8cJ8221"));
    ASSERT_TRUE(pckg2->getPackageMerge().size() == 1);
    PackageMergePtr m = pckg2->getPackageMerge().front();
    ASSERT_TRUE(m->getMergedPackage() == pckg1);
    ASSERT_TRUE(m->getReceivingPackage() == pckg2);
}

TEST_F(PackageTest, externalMergedPackageTest) {
    UmlManager mm;
    mm.open(ymlPath + "uml/primitiveTypes.yml");
    mm.open(ymlPath + "packageParserTests/3packages.yml");
    ASSERT_NO_THROW(mm.open(ymlPath + "packageParserTests/mergedPackage.yml"));
    ASSERT_TRUE(mm.getRoot()->is<Package>());
    Package* pckg = &mm.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackageMerge().size() == 2);
    PackageMergePtr m = *(pckg->getPackageMerge().ptrs().begin()++);
    PackageMergePtr m2 = pckg->getPackageMerge().front();
    ASSERT_TRUE(m->getMergedPackage());
    PackagePtr p2 = m->getMergedPackage();
    ASSERT_TRUE(p2->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front()->is<Class>());
    ClassPtr c = pckg->getPackagedElements().front();
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    PropertyPtr p = c->getOwnedAttributes().front();
    PackagePtr primPack = m2->getMergedPackage();
    ASSERT_TRUE(primPack->getPackagedElements().front()->is<PrimitiveType>());
    PrimitiveType& b = primPack->getPackagedElements().get("bool")->as<PrimitiveType>();
    ASSERT_TRUE(*p->getType() == b);
}

TEST_F(PackageTest, emitVerySimplePackageTest) {
    UmlManager m;
    Package& p = *m.create<Package>();
    p.setID(ID::fromString("_SljVdCSVuBAkmgXqFcopy8&D9oN"));
    p.setName("package");
    p.setVisibility(VisibilityKind::PACKAGE);
    std::string expectedEmit = R""""(Package:
  id: "_SljVdCSVuBAkmgXqFcopy8&D9oN"
  name: package
  visibility: package)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(p));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

// TEST_F(PackageTest, parsePackagedElementInDifferentFileTest) {
//     UmlManager m;
//     ASSERT_NO_THROW(m.open(ymlPath + "packageParserTests/subFolderTest.yml"));
//     ASSERT_EQ(m.getRoot()->getElementType(), Package::Info::elementType);
//     Package& pckg = m.getRoot()->as<Package>();
//     ASSERT_EQ(pckg.getPackagedElements().size(), 1);
//     ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
//     ASSERT_EQ(pckg.getPackagedElements().front().getID(), ID::fromString("4tcg0slbMiorhD6UUNfSGw6hHTV3"));
// }

TEST_F(PackageTest, emitMergedPackageTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    PackageMerge& merge = *m.create<PackageMerge>();
    Package& mergin = *m.create<Package>();
    Package& merged = *m.create<Package>();
    pckg.setID(ID::fromString("la_AO7XKQcEsH1P2LHcSk4ELzoEV"));
    mergin.setID(ID::fromString("SXE9QBb0rYOmBFbahGIQLhMxlYNq"));
    merge.setID(ID::fromString("I7c2Z27FF1w&WX4NHKdfIkbNuhDA"));
    merged.setID(ID::fromString("orUaM2sY9dz&YP83TqfGaHO5SBY4"));
    mergin.getPackageMerge().add(merge);
    merge.setMergedPackage(&merged);
    pckg.getPackagedElements().add(mergin);
    pckg.getPackagedElements().add(merged);
    std::string expectedEmit = R""""(Package:
  id: la_AO7XKQcEsH1P2LHcSk4ELzoEV
  packagedElements:
    - Package:
        id: "orUaM2sY9dz&YP83TqfGaHO5SBY4"
    - Package:
        id: SXE9QBb0rYOmBFbahGIQLhMxlYNq
        packageMerges:
          - PackageMerge:
              id: "I7c2Z27FF1w&WX4NHKdfIkbNuhDA"
              mergedPackage: "orUaM2sY9dz&YP83TqfGaHO5SBY4")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(pckg));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

void ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(DirectedRelationship& dr, Element& source, Element& target) {
    ASSERT_EQ(dr.getSources().size(), 1);
    ASSERT_EQ(dr.getSources().front(), &source);
    ASSERT_EQ(dr.getRelatedElements().contains(source.getID()), 1);
    ASSERT_EQ(dr.getRelatedElements().get(source.getID()), &source);
    ASSERT_EQ(dr.getTargets().size(), 1);
    ASSERT_EQ(dr.getTargets().front(), &target);
    ASSERT_EQ(dr.getRelatedElements().contains(target.getID()), 1);
    ASSERT_EQ(dr.getRelatedElements().get(target.getID()), &target);
}

TEST_F(PackageTest, mountAndEditPackageTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Package& c1 = *m.create<Package>();
    Package& merged = *m.create<Package>();
    Profile& profile = *m.create<Profile>();
    PackageMerge& merge = *m.create<PackageMerge>();
    ProfileApplication& profileApplication = *m.create<ProfileApplication>();
    Stereotype& stereotype = *m.create<Stereotype>();
    c1.getPackagedElements().add(stereotype);
    merge.setMergedPackage(merged);
    c1.getPackageMerge().add(merge);
    profileApplication.setAppliedProfile(profile);
    c1.getProfileApplications().add(profileApplication);
    root.getPackagedElements().add(c1, merged, profile);
    m.setRoot(&root);
    m.mount(ymlPath + "packageParserTests");
    ID cID = c1.getID();
    m.release(c1);
    Package& c2 = root.getPackagedElements().get(cID)->as<Package>();
    ASSERT_TRUE(c2.getOwningPackage());
    ASSERT_EQ(c2.getOwningPackage(), &root);
    ASSERT_EQ(c2.getPackageMerge().size(), 1);
    ASSERT_EQ(c2.getPackageMerge().front(), &merge);
    ASSERT_TRUE(c2.getOwnedElements().contains(merge.getID()));
    ASSERT_EQ(c2.getOwnedElements().get(merge.getID()), &merge);
    ASSERT_TRUE(merge.getReceivingPackage());
    ASSERT_EQ(merge.getReceivingPackage(), &c2);
    ASSERT_TRUE(merge.getOwner());
    ASSERT_EQ(merge.getOwner(), &c2);
    ASSERT_EQ(merge.getSources().size(), 1);
    ASSERT_EQ(merge.getSources().front(), &c2);
    ASSERT_EQ(c2.getProfileApplications().size(), 1);
    ASSERT_EQ(c2.getProfileApplications().front(), &profileApplication);
    ASSERT_TRUE(c2.getOwnedElements().contains(profileApplication.getID()));
    ASSERT_EQ(c2.getOwnedElements().get(profileApplication.getID()), &profileApplication);
    ASSERT_TRUE(profileApplication.getApplyingPackage());
    ASSERT_EQ(*profileApplication.getApplyingPackage(), c2);
    ASSERT_TRUE(profileApplication.getOwner());
    ASSERT_EQ(*profileApplication.getOwner(), c2);
    ASSERT_EQ(profileApplication.getSources().size(), 1);
    ASSERT_EQ(profileApplication.getSources().front(), &c2);
    ASSERT_EQ(c2.getPackagedElements().size(), 1);
    ASSERT_EQ(c2.getPackagedElements().front(), &stereotype);
    ASSERT_EQ(c2.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(c2.getOwnedStereotypes().front(), &stereotype);
    ASSERT_TRUE(stereotype.getOwningPackage());
    ASSERT_EQ(*stereotype.getOwningPackage(), c2);
    ASSERT_EQ(c2.getOwnedMembers().size(), 1);
    ASSERT_EQ(c2.getOwnedMembers().front(), &stereotype);
    ASSERT_EQ(c2.getMembers().size(), 1);
    ASSERT_EQ(c2.getMembers().front(), &stereotype);
    ASSERT_EQ(c2.getOwnedElements().size(), 3);
    ASSERT_EQ(c2.getOwnedElements().get(stereotype.getID()), &stereotype);
    ASSERT_TRUE(stereotype.getNamespace());
    ASSERT_EQ(*stereotype.getNamespace(), c2);
    ASSERT_TRUE(stereotype.getOwner());
    ASSERT_EQ(*stereotype.getOwner(), c2);

    m.release(merge);
    ASSERT_EQ(c2.getPackageMerge().size(), 1);
    PackageMerge& merge2 = *c2.getPackageMerge().front();
    ASSERT_TRUE(merge2.getReceivingPackage());
    ASSERT_EQ(*merge2.getReceivingPackage(), c2);
    ASSERT_TRUE(merge2.getMergedPackage());
    ASSERT_EQ(merge2.getMergedPackage(), &merged);
    ASSERT_NO_FATAL_FAILURE(ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(merge2, c2, merged));

    m.release(profileApplication);
    ASSERT_EQ(c2.getProfileApplications().size(), 1);
    ProfileApplication& profileApplication2 = *c2.getProfileApplications().front();
    ASSERT_TRUE(profileApplication2.getApplyingPackage());
    ASSERT_EQ(profileApplication2.getApplyingPackage(), &c2);
    ASSERT_TRUE(profileApplication2.getAppliedProfile());
    ASSERT_EQ(profileApplication2.getAppliedProfile(), &profile);
    ASSERT_NO_FATAL_FAILURE(ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(profileApplication2, c2, profile));

    m.release(stereotype);
    ASSERT_EQ(c2.getOwnedStereotypes().size(), 1);
    Stereotype& stereotype2 = *c2.getOwnedStereotypes().front();
    ASSERT_TRUE(stereotype2.getOwningPackage());
    ASSERT_EQ(stereotype2.getOwningPackage(), &c2);
    ASSERT_TRUE(stereotype2.getNamespace());
    ASSERT_EQ(stereotype2.getNamespace(), &c2);
    ASSERT_TRUE(stereotype2.getOwner());
    ASSERT_EQ(stereotype2.getOwner(), &c2);

    m.release(merge2, c2);
    ASSERT_EQ(*profileApplication2.getApplyingPackage()->getPackageMerge().front()->getMergedPackage(), merged);
}

// TEST_F(PackageTest, parseStringTest) {
//     UmlManager m;
//     Package& p = *m.create<Package>();
//     std::string ps = Parsers::emit(p);
//     ID pid = p.getID();
//     m.erase(p);
//     Parsers::ParserMetaData data(&m);
//     data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
//     Package& p2 = Parsers::parseString(ps, data)->as<Package>();
//     ASSERT_EQ(p2.getID(), pid);
// }

TEST_F(PackageTest, parseJsonTest) {
    UmlManager m;
    ElementPtr parsed = m.open(ymlPath + "packageParserTests/jsonFileTest.json");
    ASSERT_TRUE(parsed->is<Package>());
    PackagePtr root = parsed;
    ASSERT_EQ(root->getPackagedElements().size(), 2);
    ASSERT_EQ(root->getName(), "JSON_pckg");
}

TEST_F(PackageTest, stereotypeWithExtensionTest) {
  UmlManager m;
  m.open(ymlPath + "profileTests/stereotypeW_Extension.yml");
  ASSERT_TRUE(m.getRoot()->is<Profile>());
  Profile& profile = dynamic_cast<Profile&>(*m.getRoot());
  ASSERT_EQ(profile.getOwnedStereotypes().size(), 1);
  Stereotype& s = *profile.getOwnedStereotypes().front();
  ASSERT_EQ(profile.getPackagedElements().size(), 3);
  //ASSERT_EQ(profile.getpackagedelements().get("ext")->getElementType(), Extension::Info::elementType);
  ASSERT_TRUE(profile.getPackagedElements().get("ext")->is<Extension>());
  Extension& ext = dynamic_cast<Extension&>(*profile.getPackagedElements().get("ext"));
  ASSERT_TRUE(ext.getOwnedEnd());
  ExtensionEnd& end = *ext.getOwnedEnd();
  ASSERT_TRUE(end.getType());
  ASSERT_EQ(end.getType()->getID(), s.getID());
  Class& metaClass = profile.getPackagedElements().get("meta class")->as<Class>();
  Property& memberEnd = *ext.getMemberEnds().get("member end");
  ASSERT_EQ(*memberEnd.getType(), metaClass);
  ASSERT_EQ(*ext.getMetaClass(), metaClass);
}

TEST_F(PackageTest, internalProfileapplication) {
  UmlManager m;
  ASSERT_NO_THROW(m.open(ymlPath + "profileTests/internalProfileApplication.yml"));
  // ASSERT_EQ(m.getRoot()->getElementType(), Package::Info::elementType);
  ASSERT_TRUE(m.getRoot()->is<Package>());
  Package& pckg = m.getRoot()->as<Package>();
  ASSERT_EQ(pckg.getPackagedElements().size(), 2);
  // ASSERT_EQ(pckg.getPackagedElements().get(ID::fromString("3l8cYkYl_KVsMsQtR5ax2IE76CsM"))->getElementType(), Package::Info::elementType);
  ASSERT_TRUE(pckg.getPackagedElements().get(ID::fromString("3l8cYkYl_KVsMsQtR5ax2IE76CsM"))->is<Package>());
  Package& applying = pckg.getPackagedElements().get(ID::fromString("3l8cYkYl_KVsMsQtR5ax2IE76CsM"))->as<Package>();
  ASSERT_EQ(applying.getProfileApplications().size(), 1);
  // ASSERT_EQ(pckg.getPackagedElements().get(ID::fromString("ZjvD1bCxVklVQI7SsmtE_kS3oIIc"))->getElementType(), Profile::Info::elementType);
  ASSERT_TRUE(pckg.getPackagedElements().get(ID::fromString("ZjvD1bCxVklVQI7SsmtE_kS3oIIc"))->is<Profile>());
  Profile& profile = pckg.getPackagedElements().get(ID::fromString("ZjvD1bCxVklVQI7SsmtE_kS3oIIc"))->as<Profile>();
  ProfileApplication& application = *applying.getProfileApplications().front();
  ASSERT_EQ(application.getAppliedProfile()->getID(), profile.getID());
}

TEST_F(PackageTest, externalProfileApplicationTest) {
  UmlManager m;
  m.open(ymlPath + "profileTests/testProfile.yml");
  ASSERT_NO_THROW(m.open(ymlPath + "profileTests/externalProfileApplication.yml"));
  // ASSERT_EQ(m.getRoot()->getElementType(), Package::Info::elementType);
  ASSERT_TRUE(m.getRoot()->is<Package>());
  Package& pckg = m.getRoot()->as<Package>();
  ASSERT_EQ(pckg.getProfileApplications().size(), 1);
  ProfileApplication& application = *pckg.getProfileApplications().front();
  ASSERT_EQ(application.getAppliedProfile()->getID(), ID::fromString("XIf5yPHTzLz4NDkVLLwDamOWscKb"));
  //lazy
}

TEST_F(PackageTest, emitProfileTest) {
    UmlManager m;
    Profile& profile = *m.create<Profile>();
    Stereotype& stereotype = *m.create<Stereotype>();
    Extension& extension = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    Class& metaClass = *m.create<Class>();
    Property& baseClass = *m.create<Property>();
    profile.setID(ID::fromString("83lphS&gucqvJwW&KSzVmTSMMG1z"));
    extension.setID(ID::fromString("&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ"));
    end.setID(ID::fromString("t&ZWitKKpMcvG9Dzwh23wSbP1hr5"));
    stereotype.setID(ID::fromString("7PJxQhyjuuWylik9y2fgpNDXmMdv"));
    metaClass.setID(ID::fromString("JHMJw4rDUtZrQUJ1JP1rMynWvEsK"));
    baseClass.setID(ID::fromString("cEoEHKDqYcoIOtYwIqrMbz&WG1G_"));
    end.setType(&stereotype);
    extension.setOwnedEnd(&end);
    baseClass.setType(metaClass);
    extension.getMemberEnds().add(baseClass);
    stereotype.getOwnedAttributes().add(baseClass);
    profile.getPackagedElements().add(stereotype);
    profile.getPackagedElements().add(extension);
    profile.getPackagedElements().add(metaClass);
    std::string expectedEmit = R""""(Profile:
  id: "83lphS&gucqvJwW&KSzVmTSMMG1z"
  packagedElements:
    - Class:
        id: JHMJw4rDUtZrQUJ1JP1rMynWvEsK
    - Extension:
        id: "&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ"
        memberEnds:
          - "cEoEHKDqYcoIOtYwIqrMbz&WG1G_"
        ownedEnd:
          ExtensionEnd:
            id: "t&ZWitKKpMcvG9Dzwh23wSbP1hr5"
            type: 7PJxQhyjuuWylik9y2fgpNDXmMdv
    - Stereotype:
        id: 7PJxQhyjuuWylik9y2fgpNDXmMdv
        ownedAttributes:
          - Property:
              id: "cEoEHKDqYcoIOtYwIqrMbz&WG1G_"
              type: JHMJw4rDUtZrQUJ1JP1rMynWvEsK
              association: "&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(profile));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PackageTest, emitProfileApplication) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Package& applying = *m.create<Package>();
    Profile& profile = *m.create<Profile>();
    ProfileApplication& application = *m.create<ProfileApplication>();
    root.setID(ID::fromString("BW5iaVf_WdBebuIH3yi9beXpG5Yi"));
    applying.setID(ID::fromString("BtO&7RDq4sOe2Cb3hl_bByknWtDU"));
    application.setID(ID::fromString("QbTzWJmjUCFjrufpPQc9qyeQdK3R"));
    profile.setID(ID::fromString("R12X_VJHWWUKmJS_F8JotXJZzsNB"));
    application.setAppliedProfile(&profile);
    applying.getProfileApplications().add(application);
    root.getPackagedElements().add(applying);
    root.getPackagedElements().add(profile);
    std::string expectedEmit = R""""(Package:
  id: BW5iaVf_WdBebuIH3yi9beXpG5Yi
  packagedElements:
    - Profile:
        id: R12X_VJHWWUKmJS_F8JotXJZzsNB
    - Package:
        id: "BtO&7RDq4sOe2Cb3hl_bByknWtDU"
        profileApplications:
          - ProfileApplication:
              id: QbTzWJmjUCFjrufpPQc9qyeQdK3R
              appliedProfile: R12X_VJHWWUKmJS_F8JotXJZzsNB)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PackageTest, parseAppliedStereotypeTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "profileTests/appliedStereotype.yml"));
    // ASSERT_EQ(m.getRoot()->getElementType(), Package::Info::elementType);
    ASSERT_TRUE(m.getRoot()->is<Package>());
    Package& root = m.getRoot()->as<Package>();
    ASSERT_EQ(root.getPackagedElements().size(), 3);
    // ASSERT_EQ(root.getPackagedElements().get(ID::fromString("J5Y0janY19dgKxqwQ1YYfFgMgXmD"))->getElementType(), Package::Info::elementType);
    ASSERT_TRUE(root.getPackagedElements().get(ID::fromString("J5Y0janY19dgKxqwQ1YYfFgMgXmD"))->is<Package>());
    Package& applying = dynamic_cast<Package&>(*root.getPackagedElements().get(ID::fromString("J5Y0janY19dgKxqwQ1YYfFgMgXmD")));
    ASSERT_EQ(applying.getProfileApplications().size(), 1);
    ProfileApplication& application = *applying.getProfileApplications().front();
    ASSERT_TRUE(application.getAppliedProfile());
    Profile& profile = dynamic_cast<Profile&>(*root.getPackagedElements().get(ID::fromString("PPkaUpbLRkxoN76k6_qR9xv6FruX")));
    ASSERT_EQ(application.getAppliedProfile()->getID(), profile.getID());
    ASSERT_EQ(applying.getPackagedElements().size(), 1);
    // ASSERT_EQ(applying.getPackagedElements().front()->getElementType(), Package::Info::elementType);
    ASSERT_TRUE(applying.getPackagedElements().front()->is<Package>());
    Package& typed = dynamic_cast<Package&>(*applying.getPackagedElements().front());
    ASSERT_EQ(typed.getAppliedStereotypes().size(), 1);
    InstanceSpecification& stereotypeInst = *typed.getAppliedStereotypes().front();
    ASSERT_FALSE(stereotypeInst.getClassifiers().empty());
    ASSERT_EQ(stereotypeInst.getClassifiers().size(), 1);
    // ASSERT_EQ(stereotypeInst.getClassifiers().front()->getElementType(), Stereotype::Info::elementType);
    ASSERT_TRUE(stereotypeInst.getClassifiers().front()->is<Stereotype>());
    ASSERT_EQ(profile.getOwnedStereotypes().size(), 1);
    Stereotype& stereotype = *profile.getOwnedStereotypes().front();
    ASSERT_EQ(stereotypeInst.getClassifiers().front()->getID(), stereotype.getID());
}

TEST_F(PackageTest, emitAppliedStereotypeTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Package& applying = *m.create<Package>();
    ProfileApplication& application = *m.create<ProfileApplication>();
    Package& stereotypedEl = *m.create<Package>();
    InstanceSpecification& stereotypeInst = *m.create<InstanceSpecification>();
    Profile& profile = *m.create<Profile>();
    Stereotype& stereotype = *m.create<Stereotype>();
    Extension& extension = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    Class& metaClass = *m.create<Class>();
    Property& stereotypeBaseClassProperty = *m.create<Property>();
    root.setID(ID::fromString("jswJELYwKd_wleha5klF&GJFcU_0"));
    applying.setID(ID::fromString("Lf963Dxo5MFIqi9ip7&Nj4l1f1yj"));
    application.setID(ID::fromString("MUiSKR6gArugHOb1RqZtF5_uhflV"));
    profile.setID(ID::fromString("I3QrZblFek6tdX&j70kCP8u4QNAh"));
    stereotypedEl.setID(ID::fromString("wX&KNwgtwFYOQ0B4eIweaaRz&QC1"));
    stereotypeInst.setID(ID::fromString("l3q&INpC6kqcdavsgIMSrSNNpGEt"));
    extension.setID(ID::fromString("jjf&mHlwFSAjJXsBqng4IlxfYIJh"));
    end.setID(ID::fromString("FK1SGxJ2lV&5RtbRhiGU9jR0zAsw"));
    stereotype.setID(ID::fromString("x5r8XggyW2DI5c3RyAS8r_arWh79"));
    metaClass.setID(ID::fromString("JHMJw4rDUtZrQUJ1JP1rMynWvEsK"));
    stereotypeBaseClassProperty.setID(ID::fromString("cEoEHKDqYcoIOtYwIqrMbz&WG1G_"));
    profile.getPackagedElements().add(stereotype);
    end.setType(&stereotype);
    extension.setOwnedEnd(&end);
    profile.getPackagedElements().add(extension, metaClass);
    extension.getMemberEnds().add(stereotypeBaseClassProperty);
    stereotypeBaseClassProperty.setType(metaClass);
    stereotype.getOwnedAttributes().add(stereotypeBaseClassProperty);
    application.setAppliedProfile(&profile);
    applying.getProfileApplications().add(application);
    stereotypeInst.getClassifiers().add(stereotype);
    stereotypedEl.getAppliedStereotypes().add(stereotypeInst);
    applying.getPackagedElements().add(stereotypedEl);
    applying.getPackagedElements().add(stereotypeInst);
    root.getPackagedElements().add(applying);
    root.getPackagedElements().add(profile);
    std::string expectedEmit = R""""(Package:
  id: "jswJELYwKd_wleha5klF&GJFcU_0"
  packagedElements:
    - Profile:
        id: "I3QrZblFek6tdX&j70kCP8u4QNAh"
        packagedElements:
          - Class:
              id: JHMJw4rDUtZrQUJ1JP1rMynWvEsK
          - Extension:
              id: "jjf&mHlwFSAjJXsBqng4IlxfYIJh"
              memberEnds:
                - "cEoEHKDqYcoIOtYwIqrMbz&WG1G_"
              ownedEnd:
                ExtensionEnd:
                  id: "FK1SGxJ2lV&5RtbRhiGU9jR0zAsw"
                  type: x5r8XggyW2DI5c3RyAS8r_arWh79
          - Stereotype:
              id: x5r8XggyW2DI5c3RyAS8r_arWh79
              ownedAttributes:
                - Property:
                    id: "cEoEHKDqYcoIOtYwIqrMbz&WG1G_"
                    type: JHMJw4rDUtZrQUJ1JP1rMynWvEsK
                    association: "jjf&mHlwFSAjJXsBqng4IlxfYIJh"
    - Package:
        id: "Lf963Dxo5MFIqi9ip7&Nj4l1f1yj"
        packagedElements:
          - InstanceSpecification:
              id: "l3q&INpC6kqcdavsgIMSrSNNpGEt"
              classifiers:
                - x5r8XggyW2DI5c3RyAS8r_arWh79
          - Package:
              id: "wX&KNwgtwFYOQ0B4eIweaaRz&QC1"
              appliedStereotypes:
                - "l3q&INpC6kqcdavsgIMSrSNNpGEt"
        profileApplications:
          - ProfileApplication:
              id: MUiSKR6gArugHOb1RqZtF5_uhflV
              appliedProfile: "I3QrZblFek6tdX&j70kCP8u4QNAh")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PackageTest, extensionMemberEndsTest) {
    UmlManager m;
    ExtensionPtr e = m.create<Extension>();
    ExtensionEndPtr end = m.create<ExtensionEnd>();
    PropertyPtr memberEnd = m.create<Property>();
    e->setOwnedEnd(end);
    e->getMemberEnds().add(memberEnd);
    ASSERT_EQ(e->getMemberEnds().size(), 2);
    ASSERT_EQ(e->Association::getMemberEnds().size(), 2);
    ASSERT_EQ(e->getOwnedEnds().size(), 1);
    ASSERT_EQ(e->getMemberEnds().front(), end);
    ASSERT_EQ(e->getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->Association::getMemberEnds().front(), end);
    ASSERT_EQ(e->Association::getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->getOwnedEnds().front(), end);
    ASSERT_EQ(e->getOwnedEnds().back(), end);
    ASSERT_EQ(e->getOwnedEnd(), end);
    ASSERT_EQ(e->getOwnedMembers().size(), 1);
    ASSERT_EQ(e->getOwnedMembers().front(), end);
    ASSERT_EQ(e->getMembers().size(), 2);
    ASSERT_TRUE(e->getMembers().contains(end));
    ASSERT_TRUE(e->getMembers().contains(memberEnd));
    e->getMemberEnds().clear();
    ASSERT_EQ(e->getMemberEnds().front(), 0);
    ASSERT_EQ(e->getMemberEnds().back(), 0);
    ASSERT_EQ(e->Association::getMemberEnds().front(), 0);
    ASSERT_EQ(e->Association::getMemberEnds().back(), 0);
    ASSERT_EQ(e->getOwnedEnds().front(), 0);
    ASSERT_EQ(e->getOwnedEnds().back(), 0);
    ASSERT_EQ(e->getOwnedEnd(), 0);

    e->getMemberEnds().add(memberEnd);
    e->setOwnedEnd(end);
    ASSERT_EQ(e->getMemberEnds().size(), 2);
    ASSERT_EQ(e->Association::getMemberEnds().size(), 2);
    ASSERT_EQ(e->getOwnedEnds().size(), 1);
    ASSERT_EQ(e->getMemberEnds().front(), end);
    ASSERT_EQ(e->getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->Association::getMemberEnds().front(), end);
    ASSERT_EQ(e->Association::getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->getOwnedEnds().front(), end);
    ASSERT_EQ(e->getOwnedEnds().back(), end);
    ASSERT_EQ(e->getOwnedEnd(), end);
    ASSERT_EQ(e->getOwnedMembers().size(), 1);
    ASSERT_EQ(e->getOwnedMembers().front(), end);
    ASSERT_EQ(e->getMembers().size(), 2);
    ASSERT_TRUE(e->getMembers().contains(end));
    ASSERT_TRUE(e->getMembers().contains(memberEnd));
}

TEST_F(PackageTest, mountProfileTest) {
  UmlManager m;
  Profile& profile = *m.create<Profile>();
  Stereotype& stereotype = *m.create<Stereotype>();
  Extension& extension = *m.create<Extension>();
  ExtensionEnd& end = *m.create<ExtensionEnd>();
  Class& applying = *m.create<Class>();
  InstanceSpecification& stereotypeInst = *m.create<InstanceSpecification>();
  Package& root = *m.create<Package>();
  Package& pckg = *m.create<Package>();
  ProfileApplication& profileApplication = *m.create<ProfileApplication>();
  Class& metaClass = *m.create<Class>();
  Property& stereotypePropertyBaseClass = *m.create<Property>();
  profile.getPackagedElements().add(stereotype);
  profile.getPackagedElements().add(extension);
  profile.getPackagedElements().add(metaClass);
  extension.setOwnedEnd(end);
  extension.getMemberEnds().add(stereotypePropertyBaseClass);
  stereotypePropertyBaseClass.setType(metaClass);
  stereotype.getOwnedAttributes().add(stereotypePropertyBaseClass);
  end.setType(stereotype);
  profileApplication.setAppliedProfile(profile);
  pckg.getProfileApplications().add(profileApplication);
  stereotypeInst.getClassifiers().add(stereotype);
  applying.getAppliedStereotypes().add(stereotypeInst);
  pckg.getPackagedElements().add(applying);
  root.getPackagedElements().add(pckg, profile);
  m.setRoot(&root);
  m.mount(ymlPath + "profileTests");

  ID profileID = profile.getID();
  m.release(profile);
  ASSERT_FALSE(m.loaded(profileID));
  Profile& profile2 = m.get(profileID)->as<Profile>();
  ASSERT_EQ(profile2.getPackagedElements().size(), 3);
  ASSERT_TRUE(profile2.getPackagedElements().contains(extension));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(extension, profile2));
  ASSERT_TRUE(profile2.getPackagedElements().contains(stereotype));
  ASSERT_EQ(profile2.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile2.getOwnedStereotypes().front(), &stereotype);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype, profile2));

  ID stereotypeID = stereotype.getID();
  m.release(stereotype, profile2);
  ASSERT_FALSE(m.loaded(profileID));
  ASSERT_FALSE(m.loaded(stereotypeID));
  Profile& profile3 = m.get(profileID)->as<Profile>();
  ASSERT_EQ(profile3.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile3.getOwnedStereotypes().ids().front(), stereotypeID);
  ASSERT_EQ(profile3.getPackagedElements().size(), 3);
  ASSERT_TRUE(profile3.getPackagedElements().contains(stereotypeID));
  Stereotype& stereotype2 = m.get(stereotypeID)->as<Stereotype>();
  ASSERT_TRUE(stereotype2.getProfile());
  ASSERT_EQ(*stereotype2.getProfile(), profile3);
  ASSERT_TRUE(stereotype2.getOwningPackage());
  ASSERT_EQ(*stereotype2.getOwningPackage(), profile3);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype2, profile3));

  m.release(stereotype2, profile3);
  ASSERT_FALSE(m.loaded(profileID));
  ASSERT_FALSE(m.loaded(stereotypeID));
  Stereotype& stereotype3 = m.get(stereotypeID)->as<Stereotype>();
  ASSERT_TRUE(stereotype3.getProfile());
  ASSERT_EQ(stereotype3.getProfile().id(), profileID);
  ASSERT_TRUE(stereotype3.getOwningPackage());
  ASSERT_EQ(stereotype3.getOwningPackage().id(), profileID);
  ASSERT_TRUE(stereotype3.getNamespace());
  ASSERT_EQ(stereotype3.getNamespace().id(), profileID);
  ASSERT_TRUE(stereotype3.getOwner());
  ASSERT_EQ(stereotype3.getOwner().id(), profileID);
  Profile& profile4 = m.get(profileID)->as<Profile>();
  ASSERT_EQ(profile4.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile4.getOwnedStereotypes().front(), &stereotype3);
  ASSERT_EQ(profile4.getPackagedElements().size(), 3);
  ASSERT_TRUE(profile4.getPackagedElements().contains(stereotype3));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype3, profile4));

  ID extensionID = extension.getID();
  m.release(extension, profile4);
  ASSERT_FALSE(m.loaded(extensionID));
  ASSERT_FALSE(m.loaded(profileID));
  Extension& extension2 = m.get(extensionID)->as<Extension>();
  ASSERT_TRUE(extension2.getOwningPackage());
  ASSERT_EQ(extension2.getOwningPackage().id(), profileID);
  Profile& profile5 = m.get(profileID)->as<Profile>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(extension2, profile5));
  ASSERT_TRUE(extension2.getOwnedEnd());
  ASSERT_EQ(*extension2.getOwnedEnd(), end);
  ASSERT_EQ(extension2.getOwnedEnds().size(), 1);
  ASSERT_EQ(extension2.getOwnedEnds().front(), &end);
  ASSERT_EQ(extension2.getMemberEnds().size(), 2);
  ASSERT_TRUE(extension2.getMemberEnds().contains(end));
  ASSERT_TRUE(extension2.getMemberEnds().contains(stereotypePropertyBaseClass));

  ID endID = end.getID();
  m.release(end, extension2);
  ASSERT_FALSE(m.loaded(endID));
  ASSERT_FALSE(m.loaded(extensionID));
  Extension& extension3 = m.get(extensionID)->as<Extension>();
  ASSERT_TRUE(extension3.getOwnedEnd());
  ASSERT_EQ(extension3.getOwnedEnd().id(), endID);
  ASSERT_EQ(extension3.getOwnedEnds().size(), 1);
  ASSERT_EQ(extension3.getOwnedEnds().ids().front(), endID);
  ASSERT_EQ(extension3.getMemberEnds().size(), 2);
  ASSERT_TRUE(extension3.getMemberEnds().contains(endID));
  ASSERT_TRUE(extension3.getMemberEnds().contains(stereotypePropertyBaseClass));
  ASSERT_EQ(extension3.getOwnedMembers().size(), 1);
  ASSERT_TRUE(extension3.getOwnedMembers().contains(endID));
  ASSERT_EQ(extension3.getMembers().size(), 2);
  ASSERT_TRUE(extension3.getMembers().contains(endID));
  ASSERT_EQ(extension3.getOwnedElements().size(), 1);
  ASSERT_EQ(extension3.getOwnedElements().ids().front(), endID);
  ASSERT_EQ(extension3.getEndTypes().size(), 2);
  ASSERT_TRUE(extension3.getEndTypes().contains(stereotypeID));
  ExtensionEnd& end2 = m.get(endID)->as<ExtensionEnd>();
  ASSERT_TRUE(end2.getOwningAssociation());
  ASSERT_EQ(*end2.getOwningAssociation(), extension3);
  ASSERT_TRUE(end2.getAssociation());
  ASSERT_EQ(*end2.getAssociation(), extension3);
  ASSERT_TRUE(end2.getFeaturingClassifier());
  ASSERT_EQ(*end2.getFeaturingClassifier(), extension3);
  ASSERT_TRUE(end2.getNamespace());
  ASSERT_EQ(*end2.getNamespace(), extension3);
  ASSERT_TRUE(end2.getOwner());
  ASSERT_EQ(*end2.getOwner(), extension3);

  m.release(end2, extension3);
  ASSERT_FALSE(m.loaded(endID));
  ASSERT_FALSE(m.loaded(extensionID));
  ExtensionEnd& end3 = m.get(endID)->as<ExtensionEnd>();
  ASSERT_TRUE(end3.getOwningAssociation());
  ASSERT_EQ(end3.getOwningAssociation().id(), extensionID);
  ASSERT_TRUE(end3.getAssociation());
  ASSERT_EQ(end3.getAssociation().id(), extensionID);
  ASSERT_TRUE(end3.getFeaturingClassifier());
  ASSERT_EQ(end3.getFeaturingClassifier().id(), extensionID);
  ASSERT_TRUE(end3.getNamespace());
  ASSERT_EQ(end3.getNamespace().id(), extensionID);
  ASSERT_TRUE(end3.getOwner());
  ASSERT_EQ(end3.getOwner().id(), extensionID);
  Extension& extension4 = m.get(extensionID)->as<Extension>();
  ASSERT_TRUE(extension4.getOwnedEnd());
  ASSERT_EQ(extension4.getOwnedEnd(), &end3);
  ASSERT_EQ(extension4.getOwnedEnds().size(), 1);
  ASSERT_EQ(extension4.getOwnedEnds().front(), &end3);
  ASSERT_EQ(extension4.getMemberEnds().size(), 2);
  ASSERT_TRUE(extension4.getMemberEnds().contains(end3));
  ASSERT_TRUE(extension4.getMemberEnds().contains(stereotypePropertyBaseClass));
  ASSERT_EQ(extension4.getOwnedMembers().size(), 1);
  ASSERT_EQ(extension4.getOwnedMembers().front(), &end3);
  ASSERT_EQ(extension4.getMembers().size(), 2);
  ASSERT_TRUE(extension4.getMembers().contains(end3));
  ASSERT_TRUE(extension4.getMembers().contains(stereotypePropertyBaseClass));
  ASSERT_EQ(extension4.getOwnedElements().size(), 1);
  ASSERT_EQ(*extension4.getOwnedElements().begin(), end3);
  ASSERT_EQ(extension4.getEndTypes().size(), 2);
  ASSERT_TRUE(extension4.getEndTypes().contains(stereotype3));
  ASSERT_TRUE(extension4.getEndTypes().contains(metaClass));
  ASSERT_EQ(*end3.getOwningAssociation(), extension4);
  ASSERT_EQ(*end3.getAssociation(), extension4);
  ASSERT_EQ(*end3.getFeaturingClassifier(), extension4);
  ASSERT_EQ(*end3.getNamespace(), extension4);
  ASSERT_EQ(*end3.getOwner(), extension4);

  ID stereotypeInstID = stereotypeInst.getID();
  ID applyingID = applying.getID();
  m.release(applying, stereotypeInst);
  ASSERT_FALSE(m.loaded(stereotypeInstID));
  ASSERT_FALSE(m.loaded(applyingID));
  Class& applying2 = m.get(applyingID)->as<Class>();
  ASSERT_EQ(applying2.getAppliedStereotypes().size(), 1);
  ASSERT_EQ(applying2.getAppliedStereotypes().ids().front(), stereotypeInstID);
//   ASSERT_EQ(applying2.getOwnedElements().size(), 1);
//   ASSERT_EQ(applying2.getOwnedElements().ids().front(), stereotypeInstID);
  InstanceSpecification& stereotypeInst2 = m.get(stereotypeInstID)->as<InstanceSpecification>();
  ASSERT_EQ(applying2.getAppliedStereotypes().front(), &stereotypeInst2);
//   ASSERT_TRUE(stereotypeInst2.getOwner());
//   ASSERT_EQ(*stereotypeInst2.getOwner(), applying2);
//   ASSERT_EQ(applying2.getOwnedElements().front(), stereotypeInst2);

  m.release(stereotypeInst2, applying2);
  ASSERT_FALSE(m.loaded(stereotypeInstID));
  ASSERT_FALSE(m.loaded(applyingID));
  InstanceSpecification& stereotypeInst3 = m.get(stereotypeInstID)->as<InstanceSpecification>();
//   ASSERT_TRUE(stereotypeInst3.getOwner());
//   ASSERT_EQ(stereotypeInst3.getOwner().id(), applyingID);
  Class& applying3 = m.get(applyingID)->as<Class>();
  ASSERT_EQ(applying3.getAppliedStereotypes().size(), 1);
  ASSERT_EQ(applying3.getAppliedStereotypes().front(), &stereotypeInst3);
//   ASSERT_EQ(applying3.getOwnedElements().size(), 1);
//   ASSERT_EQ(applying3.getOwnedElements().front(), stereotypeInst3);
//   ASSERT_EQ(*stereotypeInst3.getOwner(), applying3);
}

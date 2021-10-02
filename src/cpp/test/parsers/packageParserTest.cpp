#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"
#include "uml/activity.h"
#include "uml/packageMerge.h"
#include "uml/primitiveType.h"
#include "uml/property.h"
#include "uml/profile.h"
#include "uml/profileApplication.h"
#include "uml/stereotype.h"

using namespace std;
using namespace UML;

class PackageParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(PackageParserTest, parsePackageWithActivityTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "packageParserTests/packageWithActivity.yml"));
    ASSERT_TRUE(el);
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::ACTIVITY);
    Activity* act = &dynamic_cast<Activity&>(pckg->getPackagedElements().front());
    ASSERT_TRUE(pckg->getMembers().size() == 1);
    ASSERT_TRUE(&pckg->getMembers().front() == act);
    ASSERT_TRUE(pckg->getOwnedElements().size() == 1);
    ASSERT_TRUE(&pckg->getOwnedElements().front() == act);
    ASSERT_TRUE(act->getOwningPackage() == pckg);
    ASSERT_TRUE(act->getNamespace() == pckg);
    ASSERT_TRUE(act->getOwner() == pckg);
}

TEST_F(PackageParserTest, parse3PackagesTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "packageParserTests/3packages.yml"));
    ASSERT_TRUE(el);
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg1 = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg1->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg1->getPackagedElements().front().getElementType() == ElementType::PACKAGE);
    ASSERT_TRUE(pckg1->getPackagedElements().back().getElementType() == ElementType::PACKAGE);
    Package* pckg2 = dynamic_cast<Package*>(&pckg1->getPackagedElements().front());
    Package* pckg3 = dynamic_cast<Package*>(&pckg1->getPackagedElements().back());
    ASSERT_TRUE(pckg2->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg3->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg1->getMembers().size() == 2);
    ASSERT_TRUE(&pckg1->getMembers().front() == pckg2);
    ASSERT_TRUE(&pckg1->getMembers().back() == pckg3);
    ASSERT_TRUE(pckg2->getNamespace() == pckg1);
    ASSERT_TRUE(pckg3->getNamespace() == pckg1);
    ASSERT_TRUE(pckg1->getOwnedElements().size() == 2);
    ASSERT_TRUE(&pckg1->getOwnedElements().front() == pckg2);
    ASSERT_TRUE(&pckg1->getOwnedElements().back() == pckg3);
    ASSERT_TRUE(pckg2->getOwner() == pckg1);
    ASSERT_TRUE(pckg3->getOwner() == pckg1);
    ASSERT_TRUE(pckg2->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg2->getPackagedElements().front().getElementType() == ElementType::ACTIVITY);
    Activity* act1 = dynamic_cast<Activity*>(&pckg2->getPackagedElements().front());
    ASSERT_TRUE(act1->getOwningPackage() == pckg2);
    ASSERT_TRUE(act1->getNamespace() == pckg2);
    ASSERT_TRUE(act1->getOwner() == pckg2);
    ASSERT_TRUE(pckg3->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg3->getPackagedElements().front().getElementType() == ElementType::ACTIVITY);
    Activity* act2 = dynamic_cast<Activity*>(&pckg3->getPackagedElements().front());
    ASSERT_TRUE(act2->getOwningPackage() == pckg3);
    ASSERT_TRUE(act2->getNamespace() == pckg3);
    ASSERT_TRUE(act2->getOwner() == pckg3);
}

TEST_F(PackageParserTest, NamedElementFeaturesTest) {
    Element* el;
    UmlManager* m;
    ASSERT_NO_THROW(m = Parsers::parse(ymlPath + "packageParserTests/packageWithName.yml"));
    el = & m->get<>(ID::fromString("8q2Rht9aAZlY0EnMGtEKlw5Odr_u"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getName().compare("test") == 0);
    delete m;
}

TEST_F(PackageParserTest, ElementFeaturesTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "packageParserTests/packagewID.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    ASSERT_TRUE(el->getID() == ID::fromString("Hmq5HbrypUzqPYovV8oo3wnFE6Jl"));
}

TEST_F(PackageParserTest, ElementParserExceptionTest) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "packageParserTests/improperID.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "packageParserTests/otherImproperID.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, NamedElementParserExceptionTest) {
    UmlManager m;
    Element* el;
    ASSERT_THROW(el = m.parse(ymlPath + "packageParserTests/improperName.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, properExceptions) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "packageParserTests/improperPackagedElement.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "packageParserTests/invalidPackagedElements.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, basicPackageMerge) {
    Element* el;
    UmlManager m2;
    ASSERT_NO_THROW(el = m2.parse(ymlPath + "packageParserTests/basicPackageMerge.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* bPckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(bPckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(bPckg->getPackagedElements().front().getElementType() == ElementType::PACKAGE);
    Package* pckg1 = dynamic_cast<Package*>(&bPckg->getPackagedElements().front());
    ASSERT_TRUE(bPckg->getPackagedElements().back().getElementType() == ElementType::PACKAGE);
    Package* pckg2 = dynamic_cast<Package*>(&bPckg->getPackagedElements().back());
    ASSERT_TRUE(pckg2->getPackageMerge().size() == 1);
    PackageMerge* m = &pckg2->getPackageMerge().front();
    ASSERT_TRUE(m->getMergedPackage() == pckg1);
    ASSERT_TRUE(m->getReceivingPackage() == pckg2);
}

TEST_F(PackageParserTest, externalMergedPackageTest) {
    Element* el;
    UmlManager mm;
    ASSERT_NO_THROW(el = mm.parse(ymlPath + "packageParserTests/mergedPackage.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 2);
    PackageMerge* m = &pckg->getPackageMerge().front();
    PackageMerge* m2 = &pckg->getPackageMerge().back();
    ASSERT_TRUE(m->getMergedPackage() != 0);
    Package* p2 = m->getMergedPackage();
    ASSERT_TRUE(p2->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = &c->getOwnedAttributes().front();
    Package* primPack = m2->getMergedPackage();
    ASSERT_TRUE(primPack->getPackagedElements().front().getElementType() == ElementType::PRIMITIVE_TYPE);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&primPack->getPackagedElements().front());
    ASSERT_TRUE(p->getType() == b);
}

TEST_F(PackageParserTest, emitVerySimplePackageTest) {
    UmlManager m;
    Package p = m.create<Package>();
    p.setID("_SljVdCSVuBAkmgXqFcopy8&D9oN");
    p.setName("package");
    p.setVisibility(VisibilityKind::PACKAGE);
    string expectedEmit = R""""(package:
  id: _SljVdCSVuBAkmgXqFcopy8&D9oN
  name: package
  visibility: PACKAGE)"""";
    string generatedEmit = Parsers::emit(p);
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PackageParserTest, parsePackagedElementInDifferentFileTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "packageParserTests/subFolderTest.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 1);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    ASSERT_EQ(pckg.getPackagedElements().front().getID(), ID::fromString("4tcg0slbMiorhD6UUNfSGw6hHTV3"));
}

TEST_F(PackageParserTest, emitMergedPackageTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    PackageMerge& merge = m.create<PackageMerge>();
    Package& mergin = m.create<Package>();
    Package& merged = m.create<Package>();
    pckg.setID("la_AO7XKQcEsH1P2LHcSk4ELzoEV");
    mergin.setID("SXE9QBb0rYOmBFbahGIQLhMxlYNq");
    merge.setID("I7c2Z27FF1w&WX4NHKdfIkbNuhDA");
    merged.setID("orUaM2sY9dz&YP83TqfGaHO5SBY4");
    mergin.getPackageMerge().add(merge);
    merge.setMergedPackage(&merged);
    pckg.getPackagedElements().add(mergin);
    pckg.getPackagedElements().add(merged);
    string expectedEmit = R""""(package:
  id: la_AO7XKQcEsH1P2LHcSk4ELzoEV
  packagedElements:
    - package:
        id: SXE9QBb0rYOmBFbahGIQLhMxlYNq
        packageMerge:
          - packageMerge:
              id: I7c2Z27FF1w&WX4NHKdfIkbNuhDA
              mergedPackage: orUaM2sY9dz&YP83TqfGaHO5SBY4
    - package:
        id: orUaM2sY9dz&YP83TqfGaHO5SBY4)"""";
    string generatedEmit = Parsers::emit(pckg);
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

void ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(DirectedRelationship& dr, Element& source, Element& target) {
    ASSERT_EQ(dr.getSources().size(), 1);
    ASSERT_EQ(&dr.getSources().front(), &source);
    ASSERT_EQ(dr.getRelatedElements().count(source.getID()), 1);
    ASSERT_EQ(&dr.getRelatedElements().get(source.getID()), &source);
    ASSERT_EQ(dr.getTargets().size(), 1);
    ASSERT_EQ(&dr.getTargets().front(), &target);
    ASSERT_EQ(dr.getRelatedElements().count(target.getID()), 1);
    ASSERT_EQ(&dr.getRelatedElements().get(target.getID()), &target);
}

TEST_F(PackageParserTest, mountAndEditPackageTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Package& c1 = m.create<Package>();
    Package& merged = m.create<Package>();
    Profile& profile = m.create<Profile>();
    PackageMerge& merge = m.create<PackageMerge>();
    ProfileApplication& profileApplication = m.create<ProfileApplication>();
    Stereotype& stereotype = m.create<Stereotype>();
    c1.getOwnedStereotypes().add(stereotype);
    merge.setMergedPackage(&merged);
    c1.getPackageMerge().add(merge);
    profileApplication.setAppliedProfile(&profile);
    c1.getProfileApplications().add(profileApplication);
    root.getPackagedElements().add(c1, merged, profile);
    m.setRoot(&root);
    m.mount(ymlPath + "packageParserTests");
    m.release(c1);
    Package& c2 = root.getPackagedElements().front().as<Package>();
    ASSERT_TRUE(c2.getOwningPackage() != 0);
    ASSERT_EQ(c2.getOwningPackage(), &root);
    ASSERT_EQ(c2.getPackageMerge().size(), 1);
    ASSERT_EQ(&c2.getPackageMerge().front(), &merge);
    ASSERT_TRUE(c2.getOwnedElements().count(merge.getID()));
    ASSERT_EQ(c2.getOwnedElements().get(merge.getID()), merge);
    ASSERT_TRUE(c2.getDirectedRelationships().count(merge.getID()));
    ASSERT_EQ(c2.getDirectedRelationships().get(merge.getID()), merge);
    ASSERT_TRUE(merge.hasReceivingPackage());
    ASSERT_EQ(merge.getReceivingPackageRef(), c2);
    ASSERT_TRUE(merge.hasOwner());
    ASSERT_EQ(merge.getOwnerRef(), c2);
    ASSERT_EQ(merge.getSources().size(), 1);
    ASSERT_EQ(merge.getSources().front(), c2);
    ASSERT_EQ(c2.getProfileApplications().size(), 1);
    ASSERT_EQ(&c2.getProfileApplications().front(), &profileApplication);
    ASSERT_TRUE(c2.getOwnedElements().count(profileApplication.getID()));
    ASSERT_EQ(c2.getOwnedElements().get(profileApplication.getID()), profileApplication);
    ASSERT_TRUE(c2.getDirectedRelationships().count(profileApplication.getID()));
    ASSERT_EQ(c2.getDirectedRelationships().get(profileApplication.getID()), profileApplication);
    ASSERT_TRUE(profileApplication.hasApplyingPackage());
    ASSERT_EQ(profileApplication.getApplyingPackageRef(), c2);
    ASSERT_TRUE(profileApplication.hasOwner());
    ASSERT_EQ(profileApplication.getOwnerRef(), c2);
    ASSERT_EQ(profileApplication.getSources().size(), 1);
    ASSERT_EQ(profileApplication.getSources().front(), c2);
    ASSERT_EQ(c2.getPackagedElements().size(), 1);
    ASSERT_EQ(&c2.getPackagedElements().front(), &stereotype);
    ASSERT_EQ(c2.getOwnedStereotypes().size(), 1);
    ASSERT_EQ(&c2.getOwnedStereotypes().front(), &stereotype);
    ASSERT_TRUE(stereotype.hasOwningPackage());
    ASSERT_EQ(stereotype.getOwningPackageRef(), c2);
    ASSERT_EQ(c2.getOwnedMembers().size(), 1);
    ASSERT_EQ(c2.getOwnedMembers().front(), stereotype);
    ASSERT_EQ(c2.getMembers().size(), 1);
    ASSERT_EQ(c2.getMembers().front(), stereotype);
    ASSERT_EQ(c2.getOwnedElements().size(), 3);
    ASSERT_EQ(c2.getOwnedElements().get(stereotype.getID()), stereotype);
    ASSERT_TRUE(stereotype.hasNamespace());
    ASSERT_EQ(stereotype.getNamespaceRef(), c2);
    ASSERT_EQ(stereotype.getMemberNamespace().size(), 1);
    ASSERT_EQ(stereotype.getMemberNamespace().front(), c2);
    ASSERT_TRUE(stereotype.hasOwner());
    ASSERT_EQ(stereotype.getOwnerRef(), c2);

    m.release(merge);
    ASSERT_EQ(c2.getPackageMerge().size(), 1);
    PackageMerge& merge2 = c2.getPackageMerge().front();
    ASSERT_TRUE(merge2.getReceivingPackage() != 0);
    ASSERT_EQ(merge2.getReceivingPackage(), &c2);
    ASSERT_TRUE(merge2.getMergedPackage() != 0);
    ASSERT_EQ(merge2.getMergedPackage(), &merged);
    ASSERT_NO_FATAL_FAILURE(ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(merge2, c2, merged));

    m.release(profileApplication);
    ASSERT_EQ(c2.getProfileApplications().size(), 1);
    ProfileApplication& profileApplication2 = c2.getProfileApplications().front();
    ASSERT_TRUE(profileApplication2.getApplyingPackage() != 0);
    ASSERT_EQ(profileApplication2.getApplyingPackage(), &c2);
    ASSERT_TRUE(profileApplication2.getAppliedProfile() != 0);
    ASSERT_EQ(profileApplication2.getAppliedProfile(), &profile);
    ASSERT_NO_FATAL_FAILURE(ASSERT_PROPER_DIRECTED_RELATIONSHIP_AQUIRE(profileApplication2, c2, profile));

    m.release(stereotype);
    ASSERT_EQ(c2.getOwnedStereotypes().size(), 1);
    Stereotype& stereotype2 = c2.getOwnedStereotypes().front();
    ASSERT_TRUE(stereotype2.getOwningPackage() != 0);
    ASSERT_EQ(stereotype2.getOwningPackage(), &c2);
    ASSERT_TRUE(stereotype2.getNamespace() != 0);
    ASSERT_EQ(stereotype2.getNamespace(), &c2);
    ASSERT_EQ(stereotype2.getMemberNamespace().size(), 1);
    ASSERT_EQ(&stereotype2.getMemberNamespace().front(), &c2);
    ASSERT_TRUE(stereotype2.getOwner() != 0);
    ASSERT_EQ(stereotype2.getOwner(), &c2);

    m.release(merge2, c2);
    ASSERT_EQ(profileApplication2.getApplyingPackageRef().getProfileApplications().front(), merged);
}
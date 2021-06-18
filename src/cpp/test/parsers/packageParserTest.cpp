#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"
#include "uml/activity.h"

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
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::ACTIVITY);
    Activity* act = dynamic_cast<Activity*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(pckg->getMembers().size() == 1);
    ASSERT_TRUE(pckg->getMembers().front() == act);
    ASSERT_TRUE(pckg->getOwnedElements().size() == 1);
    ASSERT_TRUE(pckg->getOwnedElements().front() == act);
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
    ASSERT_TRUE(pckg1->getPackagedElements().front()->getElementType() == ElementType::PACKAGE);
    ASSERT_TRUE(pckg1->getPackagedElements().back()->getElementType() == ElementType::PACKAGE);
    Package* pckg2 = dynamic_cast<Package*>(pckg1->getPackagedElements().front());
    Package* pckg3 = dynamic_cast<Package*>(pckg1->getPackagedElements().back());
    ASSERT_TRUE(pckg2->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg3->getOwningPackage() == pckg1);
    ASSERT_TRUE(pckg1->getMembers().size() == 2);
    ASSERT_TRUE(pckg1->getMembers().front() == pckg2);
    ASSERT_TRUE(pckg1->getMembers().back() == pckg3);
    ASSERT_TRUE(pckg2->getNamespace() == pckg1);
    ASSERT_TRUE(pckg3->getNamespace() == pckg1);
    ASSERT_TRUE(pckg1->getOwnedElements().size() == 2);
    ASSERT_TRUE(pckg1->getOwnedElements().front() == pckg2);
    ASSERT_TRUE(pckg1->getOwnedElements().back() == pckg3);
    ASSERT_TRUE(pckg2->getOwner() == pckg1);
    ASSERT_TRUE(pckg3->getOwner() == pckg1);
    ASSERT_TRUE(pckg2->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg2->getPackagedElements().front()->getElementType() == ElementType::ACTIVITY);
    Activity* act1 = dynamic_cast<Activity*>(pckg2->getPackagedElements().front());
    ASSERT_TRUE(act1->getOwningPackage() == pckg2);
    ASSERT_TRUE(act1->getNamespace() == pckg2);
    ASSERT_TRUE(act1->getOwner() == pckg2);
    ASSERT_TRUE(pckg3->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg3->getPackagedElements().front()->getElementType() == ElementType::ACTIVITY);
    Activity* act2 = dynamic_cast<Activity*>(pckg3->getPackagedElements().front());
    ASSERT_TRUE(act2->getOwningPackage() == pckg3);
    ASSERT_TRUE(act2->getNamespace() == pckg3);
    ASSERT_TRUE(act2->getOwner() == pckg3);
}

TEST_F(PackageParserTest, NamedElementFeaturesTest) {
    Element* el;
    UmlManager* m;
    ASSERT_NO_THROW(m = Parsers::parse2(ymlPath + "packageParserTests/packageWithName.yml"));
    el = & m->get<>(ID::fromString("8q2Rht9aAZlY0EnMGtEKlw5Odr_u"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getName().compare("test") == 0);
}

TEST_F(PackageParserTest, ElementFeaturesTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "packageParserTests/packagewID.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    ASSERT_TRUE(el->getID() == ID::fromString("54e8f139-9581-48a4-8021-32ff00606c93"));
    Parsers::deleteParsedElement(el);
}

TEST_F(PackageParserTest, ElementParserExceptionTest) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "packageParserTests/improperID.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "packageParserTests/otherImproperID.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, NamedElementParserExceptionTest) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "packageParserTests/improperName.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, properExceptions) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "packageParserTests/improperPackagedElement.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "packageParserTests/invalidPackagedElements.yml"), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, basicPackageMerge) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "packageParserTests/basicPackageMerge.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* bPckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(bPckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(bPckg->getPackagedElements().front()->getElementType() == ElementType::PACKAGE);
    Package* pckg1 = dynamic_cast<Package*>(bPckg->getPackagedElements().front());
    ASSERT_TRUE(bPckg->getPackagedElements().back()->getElementType() == ElementType::PACKAGE);
    Package* pckg2 = dynamic_cast<Package*>(bPckg->getPackagedElements().back());
    ASSERT_TRUE(pckg2->getPackageMerge().size() == 1);
    PackageMerge* m = pckg2->getPackageMerge().front();
    ASSERT_TRUE(m->getMergedPackage() == pckg1);
    ASSERT_TRUE(m->getReceivingPackage() == pckg2);
    Parsers::deleteParsedElement(el);
}

TEST_F(PackageParserTest, externalMergedPackageTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "packageParserTests/mergedPackage.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 2);
    PackageMerge* m = pckg->getPackageMerge().front();
    PackageMerge* m2 = pckg->getPackageMerge().back();
    ASSERT_TRUE(m->getMergedPackage() != 0);
    Package* p2 = m->getMergedPackage();
    ASSERT_TRUE(p2->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = c->getOwnedAttributes().front();
    Package* primPack = m2->getMergedPackage();
    ASSERT_TRUE(primPack->getPackagedElements().front()->getElementType() == ElementType::PRIMITIVE_TYPE);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(primPack->getPackagedElements().front());
    ASSERT_TRUE(p->getType() == b);
    Parsers::deleteParsedElement(el);
}

TEST_F(PackageParserTest, emitVerySimplePackageTest) {
    Package p;
    p.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    p.setName("package");
    p.setVisibility(VisibilityKind::PACKAGE);
    string expectedEmit = R""""(package:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
  name: package
  visibility: PACKAGE)"""";
    string generatedEmit = Parsers::emit(p);
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
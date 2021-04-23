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
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/packageWithActivity.yml")));
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
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/3packages.yml")));
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
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/packageWithName.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getName().compare("test") == 0);
}

TEST_F(PackageParserTest, ElementFeaturesTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/packagewID.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    ASSERT_TRUE(el->getID() == boost::lexical_cast<boost::uuids::uuid>("54e8f139-9581-48a4-8021-32ff00606c93"));
}

TEST_F(PackageParserTest, ElementParserExceptionTest) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/improperID.yml")), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/otherImproperID.yml")), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, NamedElementParserExceptionTest) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/improperName.yml")), Parsers::UmlParserException);
}

TEST_F(PackageParserTest, properExceptions) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/improperPackagedElement.yml")), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/invalidPackagedElements.yml")), Parsers::UmlParserException);
}
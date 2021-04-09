#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"
#include "uml/activity.h"

using namespace UML;

class PackageParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(PackageParserTest, parsePackageWithActivityTest) {
    Element* el = Parsers::parse(YAML::LoadFile(ymlPath + "packageParserTests/packageWithActivity.yml"));
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
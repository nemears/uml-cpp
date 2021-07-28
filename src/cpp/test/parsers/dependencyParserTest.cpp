#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/dependency.h"
#include "uml/package.h"

using namespace std;
using namespace UML;

class DependencyParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DependencyParserTest, basicDependencyTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "dependencyTests/basicDependency.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::DEPENDENCY);
    Dependency& dep = pckg.getPackagedElements().front()->as<Dependency>();
    ASSERT_EQ(dep.getClient().size(), 1);
    ASSERT_EQ(dep.getClient().front()->getID(), pckg.getPackagedElements().back()->getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front()->getID(), pckg.getPackagedElements().get(1)->getID());
}
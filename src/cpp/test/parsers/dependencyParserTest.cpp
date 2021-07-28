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

TEST_F(DependencyParserTest, basicDependencyEmitTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Dependency& dependency = m.create<Dependency>();
    Package& client = m.create<Package>();
    Package& supplier = m.create<Package>();
    pckg.setID("oT59r8w9_ZlGzo2NFpN&vJgH_4YJ");
    dependency.setID("tAps&UBn21dKnQ5z7qaAzKBZqR7S");
    client.setID("zMVDkDbSoENGrPr&JLyOGzYo&_D0");
    supplier.setID("uONNU0sKPVjLALJuw2pHcNqljgkg");
    pckg.getPackagedElements().add(dependency);
    pckg.getPackagedElements().add(client);
    pckg.getPackagedElements().add(supplier);
    dependency.setName("test");
    dependency.getClient().add(client);
    dependency.getSupplier().add(supplier);
    string expectedEmit = R""""(package:
  id: oT59r8w9_ZlGzo2NFpN&vJgH_4YJ
  packagedElements:
    - dependency:
        id: tAps&UBn21dKnQ5z7qaAzKBZqR7S
        name: test
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: zMVDkDbSoENGrPr&JLyOGzYo&_D0
    - package:
        id: uONNU0sKPVjLALJuw2pHcNqljgkg)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
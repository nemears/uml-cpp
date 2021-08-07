#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/dependency.h"
#include "uml/package.h"
#include "uml/realization.h"
#include "uml/usage.h"

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
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::DEPENDENCY);
    Dependency& dep = pckg.getPackagedElements().front().as<Dependency>();
    ASSERT_EQ(dep.getClient().size(), 1);
    ASSERT_EQ(dep.getClient().front().getID(), pckg.getPackagedElements().back().getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front().getID(), pckg.getPackagedElements().get(1).getID());
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

TEST_F(DependencyParserTest, parseAllTheSubclassesTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "dependencyTests/allSubClasses.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 5);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::ABSTRACTION);
    Abstraction& abs = pckg.getPackagedElements().front().as<Abstraction>();
    ASSERT_EQ(abs.getClient().size(), 1);
    ASSERT_EQ(abs.getClient().front().getID(), pckg.getPackagedElements().get(3).getID());
    ASSERT_EQ(abs.getSupplier().size(), 1);
    ASSERT_EQ(abs.getSupplier().front().getID(), pckg.getPackagedElements().get(4).getID());

    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::REALIZATION);
    Realization& real = pckg.getPackagedElements().get(1).as<Realization>();
    ASSERT_EQ(real.getClient().size(), 1);
    ASSERT_EQ(real.getClient().front().getID(), pckg.getPackagedElements().get(3).getID());
    ASSERT_EQ(real.getSupplier().size(), 1);
    ASSERT_EQ(real.getSupplier().front().getID(), pckg.getPackagedElements().get(4).getID());

    ASSERT_EQ(pckg.getPackagedElements().get(2).getElementType(), ElementType::USAGE);
    Usage& usage = pckg.getPackagedElements().get(2).as<Usage>();
    ASSERT_EQ(usage.getClient().size(), 1);
    ASSERT_EQ(usage.getClient().front().getID(), pckg.getPackagedElements().get(3).getID());
    ASSERT_EQ(usage.getSupplier().size(), 1);
    ASSERT_EQ(usage.getSupplier().front().getID(), pckg.getPackagedElements().get(4).getID());
}

TEST_F(DependencyParserTest, emitAllDependencySubClassesTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Abstraction& abstraction = m.create<Abstraction>();
    Realization& realization = m.create<Realization>();
    Usage& usage = m.create<Usage>();
    Package& client = m.create<Package>();
    Package& supplier = m.create<Package>();
    pckg.setID("oT59r8w9_ZlGzo2NFpN&vJgH_4YJ");
    abstraction.setID("tAps&UBn21dKnQ5z7qaAzKBZqR7S");
    realization.setID("V5lXdO3DLF2UCpqipGloE976L6QN");
    usage.setID("ouZEty1jCLeAk_tZzWBKblwwBdGm");
    client.setID("zMVDkDbSoENGrPr&JLyOGzYo&_D0");
    supplier.setID("uONNU0sKPVjLALJuw2pHcNqljgkg");
    pckg.getPackagedElements().add(abstraction);
    pckg.getPackagedElements().add(realization);
    pckg.getPackagedElements().add(usage);
    pckg.getPackagedElements().add(client);
    pckg.getPackagedElements().add(supplier);
    abstraction.setName("test");
    abstraction.getClient().add(client);
    abstraction.getSupplier().add(supplier);
    realization.setName("r");
    realization.getClient().add(client);
    realization.getSupplier().add(supplier);
    usage.setName("u");
    usage.getClient().add(client);
    usage.getSupplier().add(supplier);
    string expectedEmit = R""""(package:
  id: oT59r8w9_ZlGzo2NFpN&vJgH_4YJ
  packagedElements:
    - abstraction:
        id: tAps&UBn21dKnQ5z7qaAzKBZqR7S
        name: test
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
    - realization:
        id: V5lXdO3DLF2UCpqipGloE976L6QN
        name: r
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
    - usage:
        id: ouZEty1jCLeAk_tZzWBKblwwBdGm
        name: u
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
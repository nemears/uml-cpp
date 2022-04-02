#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "dependencyTests/basicDependency.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("dependency").getElementType(), ElementType::DEPENDENCY);
    Dependency& dep = pckg.getPackagedElements().get("dependency").as<Dependency>();
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front().getID(), pckg.getPackagedElements().get("client").getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front().getID(), pckg.getPackagedElements().get("supplier").getID());
}

TEST_F(DependencyParserTest, basicDependencyEmitTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Dependency& dependency = *m.create<Dependency>();
    Package& client = *m.create<Package>();
    Package& supplier = *m.create<Package>();
    pckg.setID("oT59r8w9_ZlGzo2NFpN&vJgH_4YJ");
    dependency.setID("tAps&UBn21dKnQ5z7qaAzKBZqR7S");
    client.setID("zMVDkDbSoENGrPr&JLyOGzYo&_D0");
    supplier.setID("uONNU0sKPVjLALJuw2pHcNqljgkg");
    pckg.getPackagedElements().add(dependency);
    pckg.getPackagedElements().add(client);
    pckg.getPackagedElements().add(supplier);
    dependency.setName("test");
    dependency.getClients().add(client);
    dependency.getSuppliers().add(supplier);
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
        id: uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: zMVDkDbSoENGrPr&JLyOGzYo&_D0
        clientDependencies:
          - tAps&UBn21dKnQ5z7qaAzKBZqR7S)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DependencyParserTest, parseAllTheSubclassesTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "dependencyTests/allSubClasses.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 5);
    ASSERT_EQ(pckg.getPackagedElements().get("abstraction").getElementType(), ElementType::ABSTRACTION);
    Abstraction& abs = pckg.getPackagedElements().get("abstraction").as<Abstraction>();
    ASSERT_EQ(abs.getClients().size(), 1);
    ASSERT_EQ(abs.getClients().front().getID(), pckg.getPackagedElements().get("client").getID());
    ASSERT_EQ(abs.getSuppliers().size(), 1);
    ASSERT_EQ(abs.getSuppliers().front().getID(), pckg.getPackagedElements().get("supplier").getID());

    ASSERT_EQ(pckg.getPackagedElements().get("realization").getElementType(), ElementType::REALIZATION);
    Realization& real = pckg.getPackagedElements().get("realization").as<Realization>();
    ASSERT_EQ(real.getClients().size(), 1);
    ASSERT_EQ(real.getClients().front().getID(), pckg.getPackagedElements().get("client").getID());
    ASSERT_EQ(real.getSuppliers().size(), 1);
    ASSERT_EQ(real.getSuppliers().front().getID(), pckg.getPackagedElements().get("supplier").getID());

    ASSERT_EQ(pckg.getPackagedElements().get("usage").getElementType(), ElementType::USAGE);
    Usage& usage = pckg.getPackagedElements().get("usage").as<Usage>();
    ASSERT_EQ(usage.getClients().size(), 1);
    ASSERT_EQ(usage.getClients().front().getID(), pckg.getPackagedElements().get("client").getID());
    ASSERT_EQ(usage.getSuppliers().size(), 1);
    ASSERT_EQ(usage.getSuppliers().front().getID(), pckg.getPackagedElements().get("supplier").getID());
}

TEST_F(DependencyParserTest, emitAllDependencySubClassesTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Abstraction& abstraction = *m.create<Abstraction>();
    Realization& realization = *m.create<Realization>();
    Usage& usage = *m.create<Usage>();
    Package& client = *m.create<Package>();
    Package& supplier = *m.create<Package>();
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
    abstraction.getClients().add(client);
    abstraction.getSuppliers().add(supplier);
    realization.setName("r");
    realization.getClients().add(client);
    realization.getSuppliers().add(supplier);
    usage.setName("u");
    usage.getClients().add(client);
    usage.getSuppliers().add(supplier);
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
    - usage:
        id: ouZEty1jCLeAk_tZzWBKblwwBdGm
        name: u
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: zMVDkDbSoENGrPr&JLyOGzYo&_D0
        clientDependencies:
          - tAps&UBn21dKnQ5z7qaAzKBZqR7S
          - ouZEty1jCLeAk_tZzWBKblwwBdGm
          - V5lXdO3DLF2UCpqipGloE976L6QN
    - realization:
        id: V5lXdO3DLF2UCpqipGloE976L6QN
        name: r
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg)"""";
    string expectedEmit2 = R""""(package:
  id: oT59r8w9_ZlGzo2NFpN&vJgH_4YJ
  packagedElements:
    - abstraction:
        id: tAps&UBn21dKnQ5z7qaAzKBZqR7S
        name: test
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
    - realization:
        id: V5lXdO3DLF2UCpqipGloE976L6QN
        name: r
        client:
          - zMVDkDbSoENGrPr&JLyOGzYo&_D0
        supplier:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: uONNU0sKPVjLALJuw2pHcNqljgkg
    - package:
        id: zMVDkDbSoENGrPr&JLyOGzYo&_D0
        clientDependencies:
          - tAps&UBn21dKnQ5z7qaAzKBZqR7S
          - ouZEty1jCLeAk_tZzWBKblwwBdGm
          - V5lXdO3DLF2UCpqipGloE976L6QN)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_TRUE(expectedEmit == generatedEmit || expectedEmit2 == generatedEmit);
}

void ASSERT_RESTORE_DEPENDENCY(Dependency& dependency, NamedElement& client, NamedElement& supplier) {
    ASSERT_EQ(dependency.getClients().size(), 1);
    ASSERT_EQ(dependency.getClients().front(), client);
    ASSERT_EQ(dependency.getSources().size(), 1);
    ASSERT_EQ(dependency.getSources().front(), client);
    ASSERT_TRUE(dependency.getRelatedElements().count(client.getID()));
    ASSERT_EQ(dependency.getSuppliers().size(), 1);
    ASSERT_EQ(dependency.getSuppliers().front(), supplier);
    ASSERT_EQ(dependency.getTargets().size(), 1);
    ASSERT_EQ(dependency.getTargets().front(), supplier);
    ASSERT_TRUE(dependency.getRelatedElements().count(supplier.getID()));
}

TEST_F(DependencyParserTest, mountAndEditDependencyTest) {
  UmlManager m;
  Package& root = *m.create<Package>();
  Package& supplier = *m.create<Package>();
  Package& client = *m.create<Package>();
  Dependency& dependency = *m.create<Dependency>();
  dependency.getClients().add(client);
  dependency.getSuppliers().add(supplier);
  root.getPackagedElements().add(dependency, client, supplier);
  m.setRoot(&root);
  m.mount(ymlPath + "dependencyTests");
  
  ID dependencyID = dependency.getID();
  m.release(dependency);
  ASSERT_FALSE(m.loaded(dependencyID));
  Dependency& dependency2 = m.aquire(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency2, client, supplier));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency2, root));

  ID clientID = client.getID();
  m.release(client, dependency2);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(clientID));
  Dependency& dependency3 = m.aquire(dependencyID)->as<Dependency>();
  ASSERT_FALSE(m.loaded(clientID));
  Package& client2 = m.aquire(clientID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency3, client2, supplier));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency3, root));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(client2, root));

  m.release(dependency3, client2);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(clientID));
  Package& client3= m.aquire(clientID)->as<Package>();
  ASSERT_FALSE(m.loaded(dependencyID));
  Dependency& dependency4 = m.aquire(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency4, client3, supplier));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency4, root));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(client3, root));

  ID supplierID = supplier.getID();
  m.release(supplier);
  ASSERT_FALSE(m.loaded(supplierID));
  Package& supplier2 = m.aquire(supplierID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency4, client3, supplier2));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier2, root));

  m.release(supplier2, dependency4);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(supplierID));
  Dependency& dependency5 = m.aquire(dependencyID)->as<Dependency>();
  ASSERT_FALSE(m.loaded(supplierID));
  Package& supplier3 = m.aquire(supplierID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency5, client3, supplier3));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier3, root));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency5, root));

  m.release(dependency5, supplier3);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(supplierID));
  Package& supplier4 = m.aquire(supplierID)->as<Package>();
  ASSERT_FALSE(m.loaded(dependencyID));
  Dependency& dependency6 = m.aquire(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency6, client3, supplier4));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier4, root));
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency6, root));
}
#include "uml/types/dependency.h"
#include "gtest/gtest.h"
#include "uml/types/abstraction.h"
#include "uml/types/realization.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(DependencySuppliers, Package, Dependency, getSuppliers)
UML_SET_INTEGRATION_TEST(DependencyClients, Package, Dependency, getClients)
UML_SET_INTEGRATION_TEST(NamedElementClientDependencies, Dependency, Package, getClientDependencies)

class DependencyTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = "../examples/";
    };
};

TEST_F(DependencyTest, addClientAndSupplierTest) {
    UmlManager m;
    auto& client = *m.create<Package>();
    auto& supplier = *m.create<Package>();
    auto& dep = *m.create<Dependency>();
    dep.getClients().add(client);
    dep.getSuppliers().add(supplier);
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front()->getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front()->getID(), supplier.getID());
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(*client.getClientDependencies().front(), dep);
}

TEST_F(DependencyTest, removeClientAndSupplierTest) {
    UmlManager m;
    auto& client = *m.create<Package>();
    auto& supplier = *m.create<Package>();
    auto& dep = *m.create<Dependency>();
    dep.getClients().add(client);
    dep.getSuppliers().add(supplier);
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front()->getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front()->getID(), supplier.getID());
    dep.getClients().remove(client);
    dep.getSuppliers().remove(supplier);
    ASSERT_EQ(dep.getClients().size(), 0);
    ASSERT_EQ(dep.getSources().size(), 0);
    ASSERT_EQ(dep.getSuppliers().size(), 0);
    ASSERT_EQ(dep.getTargets().size(), 0);
    ASSERT_EQ(client.getClientDependencies().size(), 0);
}

TEST_F(DependencyTest, setAndRemoveFromClientTest) {
    UmlManager m;
    auto& client = *m.create<Package>();
    auto& dependency = *m.create<Dependency>();
    client.getClientDependencies().add(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(*client.getClientDependencies().front(), dependency);
    ASSERT_EQ(dependency.getClients().size(), 1);
    ASSERT_EQ(dependency.getClients().front()->getID(), client.getID());
    ASSERT_EQ(dependency.getSources().size(), 1);
    ASSERT_EQ(dependency.getSources().front()->getID(), client.getID());
    ASSERT_EQ(dependency.getRelatedElements().size(), 1);
    ASSERT_EQ(*dependency.getRelatedElements().front(), client);
    client.getClientDependencies().remove(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 0);
    ASSERT_EQ(dependency.getClients().size(), 0);
    ASSERT_EQ(dependency.getSources().size(), 0);
    ASSERT_EQ(dependency.getRelatedElements().size(), 0);
}
TEST_F(DependencyTest, basicDependencyTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "dependencyTests/basicDependency.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == UmlManager::ElementType<Package>::result);
    auto& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("dependency")->getElementType(), UmlManager::ElementType<Dependency>::result);
    auto& dep = pckg.getPackagedElements().get("dependency")->as<Dependency>();
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front()->getID(), pckg.getPackagedElements().get("client")->getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front()->getID(), pckg.getPackagedElements().get("supplier")->getID());
}

TEST_F(DependencyTest, basicDependencyEmitTest) {
    UmlManager m;
    auto& pckg = *m.create<Package>();
    auto& dependency = *m.create<Dependency>();
    auto& client = *m.create<Package>();
    auto& supplier = *m.create<Package>();
    pckg.setID(EGM::ID::fromString("oT59r8w9_ZlGzo2NFpN&vJgH_4YJ"));
    dependency.setID(EGM::ID::fromString("tAps&UBn21dKnQ5z7qaAzKBZqR7S"));
    client.setID(EGM::ID::fromString("zMVDkDbSoENGrPr&JLyOGzYo&_D0"));
    supplier.setID(EGM::ID::fromString("uONNU0sKPVjLALJuw2pHcNqljgkg"));
    pckg.getPackagedElements().add(dependency);
    pckg.getPackagedElements().add(client);
    pckg.getPackagedElements().add(supplier);
    dependency.setName("test");
    dependency.getClients().add(client);
    dependency.getSuppliers().add(supplier);
    std::string expectedEmit = R""""(Package:
  id: "oT59r8w9_ZlGzo2NFpN&vJgH_4YJ"
  packagedElements:
    - Package:
        id: uONNU0sKPVjLALJuw2pHcNqljgkg
    - Package:
        id: "zMVDkDbSoENGrPr&JLyOGzYo&_D0"
        clientDependencies:
          - "tAps&UBn21dKnQ5z7qaAzKBZqR7S"
    - Dependency:
        id: "tAps&UBn21dKnQ5z7qaAzKBZqR7S"
        name: test
        suppliers:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
        clients:
          - "zMVDkDbSoENGrPr&JLyOGzYo&_D0")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(pckg));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DependencyTest, parseAllTheSubclassesTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "dependencyTests/allSubClasses.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == UmlManager::ElementType<Package>::result);
    auto& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 5);
    ASSERT_EQ(pckg.getPackagedElements().get("abstraction")->getElementType(), UmlManager::ElementType<Abstraction>::result);
    auto& abs = pckg.getPackagedElements().get("abstraction")->as<Abstraction>();
    ASSERT_EQ(abs.getClients().size(), 1);
    ASSERT_EQ(abs.getClients().front()->getID(), pckg.getPackagedElements().get("client")->getID());
    ASSERT_EQ(abs.getSuppliers().size(), 1);
    ASSERT_EQ(abs.getSuppliers().front()->getID(), pckg.getPackagedElements().get("supplier")->getID());

    ASSERT_EQ(pckg.getPackagedElements().get("realization")->getElementType(), UmlManager::ElementType<Realization>::result);
    auto& real = pckg.getPackagedElements().get("realization")->as<Realization>();
    ASSERT_EQ(real.getClients().size(), 1);
    ASSERT_EQ(real.getClients().front()->getID(), pckg.getPackagedElements().get("client")->getID());
    ASSERT_EQ(real.getSuppliers().size(), 1);
    ASSERT_EQ(real.getSuppliers().front()->getID(), pckg.getPackagedElements().get("supplier")->getID());

    ASSERT_EQ(pckg.getPackagedElements().get("usage")->getElementType(), UmlManager::ElementType<Usage>::result);
    auto& usage = pckg.getPackagedElements().get("usage")->as<Usage>();
    ASSERT_EQ(usage.getClients().size(), 1);
    ASSERT_EQ(usage.getClients().front()->getID(), pckg.getPackagedElements().get("client")->getID());
    ASSERT_EQ(usage.getSuppliers().size(), 1);
    ASSERT_EQ(usage.getSuppliers().front()->getID(), pckg.getPackagedElements().get("supplier")->getID());
}

TEST_F(DependencyTest, emitAllDependencySubClassesTest) {
    UmlManager m;
    auto& pckg = *m.create<Package>();
    auto& abstraction = *m.create<Abstraction>();
    auto& realization = *m.create<Realization>();
    auto& usage = *m.create<Usage>();
    auto& client = *m.create<Package>();
    auto& supplier = *m.create<Package>();
    pckg.setID(EGM::ID::fromString("oT59r8w9_ZlGzo2NFpN&vJgH_4YJ"));
    abstraction.setID(EGM::ID::fromString("tAps&UBn21dKnQ5z7qaAzKBZqR7S"));
    realization.setID(EGM::ID::fromString("V5lXdO3DLF2UCpqipGloE976L6QN"));
    usage.setID(EGM::ID::fromString("ouZEty1jCLeAk_tZzWBKblwwBdGm"));
    client.setID(EGM::ID::fromString("zMVDkDbSoENGrPr&JLyOGzYo&_D0"));
    supplier.setID(EGM::ID::fromString("uONNU0sKPVjLALJuw2pHcNqljgkg"));
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
    std::string expectedEmit = R""""(Package:
  id: "oT59r8w9_ZlGzo2NFpN&vJgH_4YJ"
  packagedElements:
    - Package:
        id: uONNU0sKPVjLALJuw2pHcNqljgkg
    - Package:
        id: "zMVDkDbSoENGrPr&JLyOGzYo&_D0"
        clientDependencies:
          - ouZEty1jCLeAk_tZzWBKblwwBdGm
          - V5lXdO3DLF2UCpqipGloE976L6QN
          - "tAps&UBn21dKnQ5z7qaAzKBZqR7S"
    - Usage:
        id: ouZEty1jCLeAk_tZzWBKblwwBdGm
        name: u
        suppliers:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
        clients:
          - "zMVDkDbSoENGrPr&JLyOGzYo&_D0"
    - Realization:
        id: V5lXdO3DLF2UCpqipGloE976L6QN
        name: r
        suppliers:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
        clients:
          - "zMVDkDbSoENGrPr&JLyOGzYo&_D0"
    - Abstraction:
        id: "tAps&UBn21dKnQ5z7qaAzKBZqR7S"
        name: test
        suppliers:
          - uONNU0sKPVjLALJuw2pHcNqljgkg
        clients:
          - "zMVDkDbSoENGrPr&JLyOGzYo&_D0")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(pckg));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

void ASSERT_RESTORE_DEPENDENCY(Dependency<UmlManager::GenBaseHierarchy<Dependency>>& dependency, NamedElement<UmlManager::GenBaseHierarchy<NamedElement>>& client, NamedElement<UmlManager::GenBaseHierarchy<NamedElement>>& supplier) {
    ASSERT_EQ(dependency.getClients().size(), 1);
    ASSERT_EQ(*dependency.getClients().front(), client);
    ASSERT_EQ(dependency.getSources().size(), 1);
    ASSERT_EQ(*dependency.getSources().front(), client);
    ASSERT_TRUE(dependency.getRelatedElements().contains(client.getID()));
    ASSERT_EQ(dependency.getSuppliers().size(), 1);
    ASSERT_EQ(*dependency.getSuppliers().front(), supplier);
    ASSERT_EQ(dependency.getTargets().size(), 1);
    ASSERT_EQ(*dependency.getTargets().front(), supplier);
    ASSERT_TRUE(dependency.getRelatedElements().contains(supplier.getID()));
}

TEST_F(DependencyTest, mountAndEditDependencyTest) {
  UmlManager m;
  auto& root = *m.create<Package>();
  auto& supplier = *m.create<Package>();
  auto& client = *m.create<Package>();
  auto& dependency = *m.create<Dependency>();
  dependency.getClients().add(client);
  dependency.getSuppliers().add(supplier);
  root.getPackagedElements().add(dependency, client, supplier);
  m.setRoot(&root);
  m.mount(ymlPath + "dependencyTests");
  
  EGM::ID dependencyID = dependency.getID();
  m.release(dependency);
  ASSERT_FALSE(m.loaded(dependencyID));
  auto& dependency2 = m.get(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency2, client, supplier));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency2, root));

  EGM::ID clientID = client.getID();
  m.release(client, dependency2);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(clientID));
  auto& dependency3 = m.get(dependencyID)->as<Dependency>();
  // ASSERT_FALSE(m.loaded(clientID));
  auto& client2 = m.get(clientID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency3, client2, supplier));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency3, root));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(client2, root));

  m.release(dependency3, client2);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(clientID));
  auto& client3= m.get(clientID)->as<Package>();
  // ASSERT_FALSE(m.loaded(dependencyID));
  auto& dependency4 = m.get(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency4, client3, supplier));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency4, root));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(client3, root));

  EGM::ID supplierID = supplier.getID();
  m.release(supplier);
  ASSERT_FALSE(m.loaded(supplierID));
  auto& supplier2 = m.get(supplierID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency4, client3, supplier2));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier2, root));

  m.release(supplier2, dependency4);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(supplierID));
  auto& dependency5 = m.get(dependencyID)->as<Dependency>();
  // ASSERT_FALSE(m.loaded(supplierID));
  auto& supplier3 = m.get(supplierID)->as<Package>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency5, client3, supplier3));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier3, root));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency5, root));

  m.release(dependency5, supplier3);
  ASSERT_FALSE(m.loaded(dependencyID));
  ASSERT_FALSE(m.loaded(supplierID));
  auto& supplier4 = m.get(supplierID)->as<Package>();
  // ASSERT_FALSE(m.loaded(dependencyID));
  auto& dependency6 = m.get(dependencyID)->as<Dependency>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_DEPENDENCY(dependency6, client3, supplier4));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(supplier4, root));
  // ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(dependency6, root));
}

#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(DependencySuppliers, Package, Dependency, &Dependency::getSuppliers);
UML_SET_INTEGRATION_TEST(DependencyClients, Package, Dependency, &Dependency::getClients);
UML_SET_INTEGRATION_TEST(NamedElementClientDependencies, Dependency, Package, &NamedElement::getClientDependencies);

class DependencyTest : public ::testing::Test {};

TEST_F(DependencyTest, addClientAndSupplierTest) {
    UmlManager m;
    Package& client = *m.create<Package>();
    Package& supplier = *m.create<Package>();
    Dependency& dep = *m.create<Dependency>();
    dep.getClients().add(client);
    dep.getSuppliers().add(supplier);
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front().getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front().getID(), client.getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front().getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front().getID(), supplier.getID());
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(client.getClientDependencies().front(), dep);
}

TEST_F(DependencyTest, removeClientAndSupplierTest) {
    UmlManager m;
    Package& client = *m.create<Package>();
    Package& supplier = *m.create<Package>();
    Dependency& dep = *m.create<Dependency>();
    dep.getClients().add(client);
    dep.getSuppliers().add(supplier);
    ASSERT_EQ(dep.getClients().size(), 1);
    ASSERT_EQ(dep.getClients().front().getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front().getID(), client.getID());
    ASSERT_EQ(dep.getSuppliers().size(), 1);
    ASSERT_EQ(dep.getSuppliers().front().getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front().getID(), supplier.getID());
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
    Package& client = *m.create<Package>();
    Dependency& dependency = *m.create<Dependency>();
    client.getClientDependencies().add(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(client.getClientDependencies().front(), dependency);
    ASSERT_EQ(dependency.getClients().size(), 1);
    ASSERT_EQ(dependency.getClients().front().getID(), client.getID());
    ASSERT_EQ(dependency.getSources().size(), 1);
    ASSERT_EQ(dependency.getSources().front().getID(), client.getID());
    ASSERT_EQ(dependency.getRelatedElements().size(), 1);
    ASSERT_EQ(dependency.getRelatedElements().front(), client);
    client.getClientDependencies().remove(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 0);
    ASSERT_EQ(dependency.getClients().size(), 0);
    ASSERT_EQ(dependency.getSources().size(), 0);
    ASSERT_EQ(dependency.getRelatedElements().size(), 0);
}
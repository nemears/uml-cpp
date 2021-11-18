#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class DependencyTest : public ::testing::Test {};

TEST_F(DependencyTest, addClientAndSupplierTest) {
    UmlManager m;
    Package& client = m.create<Package>();
    Package& supplier = m.create<Package>();
    Dependency& dep = m.create<Dependency>();
    dep.getClient().add(client);
    dep.getSupplier().add(supplier);
    ASSERT_EQ(dep.getClient().size(), 1);
    ASSERT_EQ(dep.getClient().front().getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front().getID(), client.getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front().getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front().getID(), supplier.getID());
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(client.getClientDependencies().front(), dep);
    // ASSERT_EQ(client.getDirectedRelationships().size(), 1);
    // ASSERT_EQ(client.getDirectedRelationships().front(), dep);
    // ASSERT_EQ(client.getRelationships().size(), 1);
    // ASSERT_EQ(client.getRelationships().front(), dep);
    // ASSERT_EQ(supplier.getDirectedRelationships().size(), 1);
    // ASSERT_EQ(supplier.getDirectedRelationships().front(), dep);
    // ASSERT_EQ(supplier.getRelationships().size(), 1);
    // ASSERT_EQ(supplier.getRelationships().front(), dep);
}

TEST_F(DependencyTest, removeClientAndSupplierTest) {
    UmlManager m;
    Package& client = m.create<Package>();
    Package& supplier = m.create<Package>();
    Dependency& dep = m.create<Dependency>();
    dep.getClient().add(client);
    dep.getSupplier().add(supplier);
    ASSERT_EQ(dep.getClient().size(), 1);
    ASSERT_EQ(dep.getClient().front().getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front().getID(), client.getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front().getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front().getID(), supplier.getID());
    dep.getClient().remove(client);
    dep.getSupplier().remove(supplier);
    ASSERT_EQ(dep.getClient().size(), 0);
    ASSERT_EQ(dep.getSources().size(), 0);
    ASSERT_EQ(dep.getSupplier().size(), 0);
    ASSERT_EQ(dep.getTargets().size(), 0);
    ASSERT_EQ(client.getClientDependencies().size(), 0);
    // ASSERT_EQ(client.getDirectedRelationships().size(), 0);
    // ASSERT_EQ(client.getRelationships().size(), 0);
    // ASSERT_EQ(supplier.getDirectedRelationships().size(), 0);
    // ASSERT_EQ(supplier.getRelationships().size(), 0);
}

TEST_F(DependencyTest, setAndRemoveFromClientTest) {
    UmlManager m;
    Package& client = m.create<Package>();
    Dependency& dependency = m.create<Dependency>();
    client.getClientDependencies().add(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 1);
    ASSERT_EQ(client.getClientDependencies().front(), dependency);
    // ASSERT_EQ(client.getDirectedRelationships().size(), 1);
    // ASSERT_EQ(client.getDirectedRelationships().front(), dependency);
    // ASSERT_EQ(client.getRelationships().size(), 1);
    // ASSERT_EQ(client.getRelationships().front(), dependency);
    ASSERT_EQ(dependency.getClient().size(), 1);
    ASSERT_EQ(dependency.getClient().front().getID(), client.getID());
    ASSERT_EQ(dependency.getSources().size(), 1);
    ASSERT_EQ(dependency.getSources().front().getID(), client.getID());
    ASSERT_EQ(dependency.getRelatedElements().size(), 1);
    ASSERT_EQ(dependency.getRelatedElements().front(), client);
    client.getClientDependencies().remove(dependency);
    ASSERT_EQ(client.getClientDependencies().size(), 0);
    // ASSERT_EQ(client.getDirectedRelationships().size(), 0);
    // ASSERT_EQ(client.getRelationships().size(), 0);
    ASSERT_EQ(dependency.getClient().size(), 0);
    ASSERT_EQ(dependency.getSources().size(), 0);
    ASSERT_EQ(dependency.getRelatedElements().size(), 0);
}
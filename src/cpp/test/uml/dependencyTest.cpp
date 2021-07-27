#include "gtest/gtest.h"
#include "uml/dependency.h"
#include "uml/package.h"

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
    ASSERT_EQ(dep.getClient().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front()->getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front()->getID(), supplier.getID());
}

TEST_F(DependencyTest, removeClientAndSupplierTest) {
    UmlManager m;
    Package& client = m.create<Package>();
    Package& supplier = m.create<Package>();
    Dependency& dep = m.create<Dependency>();
    dep.getClient().add(client);
    dep.getSupplier().add(supplier);
    ASSERT_EQ(dep.getClient().size(), 1);
    ASSERT_EQ(dep.getClient().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSources().size(), 1);
    ASSERT_EQ(dep.getSources().front()->getID(), client.getID());
    ASSERT_EQ(dep.getSupplier().size(), 1);
    ASSERT_EQ(dep.getSupplier().front()->getID(), supplier.getID());
    ASSERT_EQ(dep.getTargets().size(), 1);
    ASSERT_EQ(dep.getTargets().front()->getID(), supplier.getID());
    dep.getClient().remove(client);
    dep.getSupplier().remove(client);
    ASSERT_EQ(dep.getClient().size(), 0);
    ASSERT_EQ(dep.getSources().size(), 0);
    ASSERT_EQ(dep.getSupplier().size(), 0);
    ASSERT_EQ(dep.getTargets().size(), 0);
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(NamespaceOwnedRules, Constraint, Package, &Namespace::getOwnedRules);
UML_SINGLETON_INTEGRATION_TEST(ConstraintContext, Package, Constraint, &Constraint::getContext, &Constraint::setContext);
// UML_SET_INTEGRATION_TEST(ConstraintConstrainedElements, Slot, Constraint, &Constraint::getConstrainedElements);
// TODO fix constraint
UML_SET_INTEGRATION_TEST(NamespaceElementImports, ElementImport, Class, &Namespace::getElementImports);
UML_SINGLETON_INTEGRATION_TEST(ElementImportImportingNamespace, Package, ElementImport, &ElementImport::getImportingNamespace, &ElementImport::setImportingNamespace);
UML_SINGLETON_INTEGRATION_TEST(ElementImportImportedElement, DataType, ElementImport, &ElementImport::getImportedElement, &ElementImport::setImportedElement);
UML_SET_INTEGRATION_TEST(NamespacePackageImports, PackageImport, Class, &Namespace::getPackageImports);
UML_SINGLETON_INTEGRATION_TEST(PackageImportImportedPackage, Package, PackageImport, &PackageImport::getImportedPackage, &PackageImport::setImportedPackage);
UML_SINGLETON_INTEGRATION_TEST(PackageImportImportingNamespace, Activity, PackageImport, &PackageImport::getImportingNamespace, &PackageImport::setImportingNamespace);

class NamespaceTest : public ::testing::Test {};

// Really this test isn't necessary just did it for posterity and getting used to gtest
TEST_F(NamespaceTest, SetNameTest) {
    BasicManager m;
    Package& n = *m.create<Package>();
    n.setName("test");
    Package& owningPackage = *m.create<Package>();
    owningPackage.getPackagedElements().add(n);
    ASSERT_EQ(n.getName(), "test");
    ASSERT_EQ(owningPackage.getPackagedElements().get("test"), n);
}

TEST_F(NamespaceTest, reindexID_Test) {
    BasicManager m;
    Package& nmspc = *m.create<Package>();
    Package& n = *m.create<Package>();
    nmspc.setName("nmspc");
    n.setOwningPackage(&nmspc);
    n.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_EQ(nmspc.getOwnedElements().get(n.getID()), n);
    ASSERT_EQ(nmspc.getMembers().get(n.getID()), n);
    ASSERT_EQ(nmspc.getOwnedMembers().get(n.getID()), n);
}

TEST_F(NamespaceTest, AddOwnedMemeberFunctorTest) {
    BasicManager mm;
    Package& n = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    n.getPackagedElements().add(m);
    ASSERT_EQ(n.getOwnedMembers().size(), 1);
    ASSERT_EQ(n.getOwnedMembers().front(), m);
    ASSERT_TRUE(m.getNamespace());
    ASSERT_EQ(*m.getNamespace(), n);
    ASSERT_EQ(n.getMembers().size(), 1);
    ASSERT_EQ(n.getMembers().front(), m);
}

TEST_F(NamespaceTest, setNamespaceTest) {
    BasicManager mm;
    Package& n = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    m.setOwningPackage(&n);
    ASSERT_EQ(n.getOwnedMembers().size(), 1);
    ASSERT_EQ(n.getOwnedMembers().front(), m);
    ASSERT_EQ(n.getMembers().size(), 1);
    ASSERT_EQ(n.getMembers().front(), m);
}

TEST_F(NamespaceTest, removeMemeberFunctorTest) {
    BasicManager mm;
    Package& n = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    n.getPackagedElements().add(m);
    ASSERT_NO_THROW(n.getPackagedElements().remove(m));
    ASSERT_EQ(n.getOwnedMembers().size(), 0);
    ASSERT_EQ(n.getMembers().size(), 0);
    ASSERT_EQ(n.getOwnedElements().size(), 0);
    ASSERT_FALSE(m.getNamespace());
}

TEST_F(NamespaceTest, addElementImportWithImportedElement) {
    BasicManager m;
    PackagePtr p = m.create<Package>();
    PackagePtr e = m.create<Package>();
    ElementImportPtr i = m.create<ElementImport>();
    i->setImportedElement(e);
    p->getElementImports().add(*i);
    ASSERT_EQ(p->getImportedMembers().size(), 1);
    ASSERT_EQ(p->getImportedMembers().front(), *e);
    p->getElementImports().remove(*i);
    ASSERT_EQ(p->getImportedMembers().size(), 0);
    i->setImportedElement(0);
    p->getElementImports().add(*i);
    i->setImportedElement(e);
    ASSERT_EQ(p->getImportedMembers().size(), 1);
    ASSERT_EQ(p->getImportedMembers().front(), *e);
    i->setImportedElement(0);
    ASSERT_EQ(p->getImportedMembers().size(), 0);
}

TEST_F(NamespaceTest, addAndRemovePackageImportManyWays) {
    BasicManager m;
    PackagePtr importedPackage = m.create<Package>();
    DataTypePtr nmspc = m.create<DataType>();
    PackageImportPtr import = m.create<PackageImport>();
    import->setImportedPackage(importedPackage);
    nmspc->getPackageImports().add(*import);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 0);
    nmspc->getPackageImports().remove(*import);
    LiteralBoolPtr packagedEl = m.create<LiteralBool>();
    importedPackage->getPackagedElements().add(*packagedEl);
    nmspc->getPackageImports().add(*import);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 1);
    ASSERT_EQ(nmspc->getImportedMembers().front(), *packagedEl);
    nmspc->getPackageImports().remove(*import);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 0);
    import->setImportedPackage(0);
    nmspc->getPackageImports().add(*import);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 0);
    import->setImportedPackage(*importedPackage);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 1);
    ASSERT_EQ(nmspc->getImportedMembers().front(), *packagedEl);
    importedPackage->getPackagedElements().remove(*packagedEl);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 0);
    importedPackage->getPackagedElements().add(*packagedEl);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 1);
    ASSERT_EQ(nmspc->getImportedMembers().front(), *packagedEl);
    DataTypePtr nmspc2 = m.create<DataType>();
    PackageImportPtr import2 = m.create<PackageImport>();
    import2->setImportedPackage(importedPackage);
    nmspc2->getPackageImports().add(*import2);
    ASSERT_EQ(nmspc2->getImportedMembers().size(), 1);
    ASSERT_EQ(nmspc2->getImportedMembers().front(), *packagedEl);
    nmspc2->getPackageImports().remove(*import2);
    ASSERT_EQ(nmspc2->getImportedMembers().size(), 0);
    LiteralBoolPtr packagedEl2 = m.create<LiteralBool>();
    importedPackage->getPackagedElements().add(*packagedEl2);
    ASSERT_EQ(nmspc->getImportedMembers().size(), 2);
    ASSERT_TRUE(nmspc->getImportedMembers().contains(*packagedEl));
    ASSERT_EQ(nmspc2->getImportedMembers().size(), 0);
}
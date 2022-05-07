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

class NamespaceTest : public ::testing::Test {};

// Really this test isn't necessary just did it for posterity and getting used to gtest
TEST_F(NamespaceTest, SetNameTest) {
    UmlManager m;
    Package& n = *m.create<Package>();
    n.setName("test");
    Package& owningPackage = *m.create<Package>();
    owningPackage.getPackagedElements().add(n);
    ASSERT_EQ(n.getName(), "test");
    ASSERT_EQ(owningPackage.getPackagedElements().get("test"), n);
}

TEST_F(NamespaceTest, reindexID_Test) {
    UmlManager m;
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
    UmlManager mm;
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
    UmlManager mm;
    Package& n = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    m.setOwningPackage(&n);
    ASSERT_EQ(n.getOwnedMembers().size(), 1);
    ASSERT_EQ(n.getOwnedMembers().front(), m);
    ASSERT_EQ(n.getMembers().size(), 1);
    ASSERT_EQ(n.getMembers().front(), m);
}

TEST_F(NamespaceTest, removeMemeberFunctorTest) {
    UmlManager mm;
    Package& n = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    n.getPackagedElements().add(m);
    ASSERT_NO_THROW(n.getPackagedElements().remove(m));
    ASSERT_EQ(n.getOwnedMembers().size(), 0);
    ASSERT_EQ(n.getMembers().size(), 0);
    ASSERT_EQ(n.getOwnedElements().size(), 0);
    ASSERT_FALSE(m.getNamespace());
}
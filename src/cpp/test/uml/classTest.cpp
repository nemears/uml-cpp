#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(StructuredClassifierOwnedAttributes, Property, Class, &StructuredClassifier::getOwnedAttributes);
UML_SET_INTEGRATION_TEST(ClassOwnedAttributes, Property, Class, &Class::getOwnedAttributes);
UML_SET_INTEGRATION_TEST(StructuredClassifierOwnedConnectors, Connector, Class, &StructuredClassifier::getOwnedConnectors);
UML_SET_INTEGRATION_TEST(ClassNestingClassifiers, DataType, Class, &Class::getNestedClassifiers);
UML_SET_INTEGRATION_TEST(ClassOwnedOperations, Operation, Class, &Class::getOwnedOperations);
UML_SET_INTEGRATION_TEST(ClassOwnedReceptions, Reception, Class, &Class::getOwnedReceptions);

class ClassTest : public ::testing::Test {
   
};

TEST_F(ClassTest, BasicOperationTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    OpaqueBehavior& oB = *m.create<OpaqueBehavior>();
    LiteralString& val = *m.create<LiteralString>();
    Parameter& p = *m.create<Parameter>();
    ASSERT_NO_THROW(oB.getOwnedParameters().add(p));
    ASSERT_NO_THROW(val.setValue("return true"));
    ASSERT_NO_THROW(oB.getBodies().add(val));
    ASSERT_NO_THROW(o.getMethods().add(oB));
    ASSERT_NO_THROW(c.getOwnedOperations().add(o));
}

TEST_F(ClassTest, addOperationFunctorTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    c.getOwnedOperations().add(o);
    ASSERT_TRUE(c.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&c.getOwnedOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(&c.getOwnedElements().get(o.getID()) == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, setClassTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    o.setClass(c);
    ASSERT_TRUE(c.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&c.getOwnedOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(&c.getOwnedElements().get(o.getID()) == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, overwriteClassTest) {
  UmlManager m;
  Class& p1 = *m.create<Class>();
  Class& p2 = *m.create<Class>();
  Operation& c = *m.create<Operation>();
  p1.getOwnedOperations().add(c);
  c.setClass(&p2);
  ASSERT_TRUE(p2.getOwnedOperations().size() == 1);
  ASSERT_TRUE(&p2.getOwnedOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(&p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(&p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().get(c.getID()) == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, overwriteClassByOperationsAddTest) {
  UmlManager m;
  Class& p1 = *m.create<Class>();
  Class& p2 = *m.create<Class>();
  Operation& c = *m.create<Operation>();
  p1.getOwnedOperations().add(c);
  p2.getOwnedOperations().add(c);
  ASSERT_TRUE(p2.getOwnedOperations().size() == 1);
  ASSERT_TRUE(&p2.getOwnedOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(&p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(&p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().get(c.getID()) == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, removeOperationFunctorTest) {
  UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
  c.getOwnedOperations().add(o);
  ASSERT_NO_THROW(c.getOwnedOperations().remove(o));
  ASSERT_TRUE(c.getOwnedOperations().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!o.getClass());
  ASSERT_TRUE(!o.getFeaturingClassifier());
  ASSERT_TRUE(!o.getNamespace());
  ASSERT_TRUE(!o.getOwner());
}

TEST_F(ClassTest, addOwnedAttributeTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(c.getOwnedAttributes().add(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.getClass());
  ASSERT_EQ(*p.getClass(), c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, addOwnedAttributeAsStructuredClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.as<StructuredClassifier>().getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.getClass());
  ASSERT_EQ(*p.getClass(), c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, setStructuredClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(p.setClass(&c));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  // ASSERT_TRUE(c.getRole().size() == 1);
  // ASSERT_TRUE(&c.getRole().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(*p.getFeaturingClassifier() == c);
  ASSERT_TRUE(*p.getNamespace() == c);
  ASSERT_TRUE(*p.getOwner() == c);
}

TEST_F(ClassTest, removeOwnedAttributeFunctorTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(c.getOwnedAttributes().remove(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  // ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_FALSE(p.getFeaturingClassifier());
  ASSERT_FALSE(p.getNamespace());
  ASSERT_FALSE(p.getOwner());
}

TEST_F(ClassTest, setFeaturingClassifierNullTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setClass(0));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  // ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);

  ASSERT_FALSE(p.getFeaturingClassifier());
  ASSERT_FALSE(p.getNamespace());
  ASSERT_FALSE(p.getOwner());
}

TEST_F(ClassTest, addCompositePropertyTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  p.setAggregation(AggregationKind::COMPOSITE);
  c.getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getParts().size() == 1);
  ASSERT_TRUE(&c.getParts().front() == &p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, backwardsAddCompositePropertyTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(c.getOwnedAttributes().add(p));
  ASSERT_NO_THROW(p.setAggregation(AggregationKind::COMPOSITE));
  ASSERT_TRUE(c.getParts().size() == 1);
  ASSERT_TRUE(&c.getParts().front() == &p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, removePropertyFromParts) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  p.setAggregation(AggregationKind::COMPOSITE);
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setAggregation(AggregationKind::NONE));

  ASSERT_TRUE(c.getParts().size() == 0);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(!p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, addAndRemoveNestedClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  DataType& d = *m.create<DataType>();
  c.getNestedClassifiers().add(d);
  ASSERT_EQ(c.getNestedClassifiers().size(), 1);
  ASSERT_EQ(c.getNestedClassifiers().front().getID(), d.getID());
  ASSERT_EQ(c.getOwnedMembers().size(), 1);
  ASSERT_EQ(c.getOwnedMembers().front().getID(), d.getID());
  // ASSERT_EQ(c.getRedefinedElements().size(), 1);
  // ASSERT_EQ(c.getRedefinedElements().front().getID(), d.getID());
  c.getNestedClassifiers().remove(d);
  ASSERT_EQ(c.getNestedClassifiers().size(), 0);
  ASSERT_EQ(c.getOwnedMembers().size(), 0);
  ASSERT_EQ(c.getNestedClassifiers().size(), 0);
}
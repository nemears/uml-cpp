#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/parameter.h"
#include "uml/sequence.h"
#include "uml/operation.h"

using namespace UML;

class ClassTest : public ::testing::Test {
   
};

TEST_F(ClassTest, BasicOperationTest) {
    Class c;
    Operation o;
    OpaqueBehavior oB;
    LiteralString val;
    Parameter p;
    ASSERT_NO_THROW(oB.getParameters().add(p));
    ASSERT_NO_THROW(val.setValue("return true"));
    ASSERT_NO_THROW(oB.getBodies().add(val));
    ASSERT_NO_THROW(o.getMethods().add(oB));
    ASSERT_NO_THROW(c.getOperations().add(o));
}

TEST_F(ClassTest, addOperationFunctorTest) {
    Class c;
    Operation o;
    c.getOperations().add(o);
    ASSERT_TRUE(c.getOperations().size() == 1);
    ASSERT_TRUE(c.getOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(c.getOwnedElements().front() == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, setClassTest) {
    Class c;
    Operation o;
    o.setClass(&c);
    ASSERT_TRUE(c.getOperations().size() == 1);
    ASSERT_TRUE(c.getOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(c.getOwnedElements().front() == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, overwriteClassTest) {
  Class p1;
  Class p2;
  Operation c;
  p1.getOperations().add(c);
  c.setClass(&p2);
  ASSERT_TRUE(p2.getOperations().size() == 1);
  ASSERT_TRUE(p2.getOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, overwriteClassByOperationsAddTest) {
  Class p1;
  Class p2;
  Operation c;
  p1.getOperations().add(c);
  p2.getOperations().add(c);
  ASSERT_TRUE(p2.getOperations().size() == 1);
  ASSERT_TRUE(p2.getOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, removeOperationFunctorTest) {
  Class c;
  Operation o;
  c.getOperations().add(o);
  ASSERT_NO_THROW(c.getOperations().remove(o));
  ASSERT_TRUE(c.getOperations().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!o.getClass());
  ASSERT_TRUE(!o.getFeaturingClassifier());
  ASSERT_TRUE(!o.getNamespace());
  ASSERT_TRUE(!o.getOwner());
}

TEST_F(ClassTest, addOwnedOperationTest) {
  Class c;
  Property p;
  ASSERT_NO_THROW(c.getOwnedAttributes().add(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRole().size() == 1);
  ASSERT_TRUE(c.getRole().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(c.getOwnedElements().front() == &p);

  ASSERT_TRUE(p.getStructuredClassifier() == &c);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getMemberNamespace().size() == 1);
  ASSERT_TRUE(p.getMemberNamespace().front() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, setStructuredClassifierTest) {
  Class c;
  Property p;
  ASSERT_NO_THROW(p.setStructuredClassifier(&c));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRole().size() == 1);
  ASSERT_TRUE(c.getRole().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(c.getOwnedElements().front() == &p);

  ASSERT_TRUE(p.getStructuredClassifier() == &c);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getMemberNamespace().size() == 1);
  ASSERT_TRUE(p.getMemberNamespace().front() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, removeOwnedAttributeFunctorTest) {
  Class c;
  Property p;
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(c.getOwnedAttributes().remove(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);

  ASSERT_TRUE(p.getStructuredClassifier() == 0);
  ASSERT_TRUE(p.getClassifier() == 0);
  ASSERT_TRUE(p.getFeaturingClassifier() == 0);
  ASSERT_TRUE(p.getNamespace() == 0);
  ASSERT_TRUE(p.getMemberNamespace().size() == 0);
  ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(ClassTest, setFeaturingClassifierNullTest) {
  Class c;
  Property p;
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setStructuredClassifier(0));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);

  ASSERT_TRUE(p.getStructuredClassifier() == 0);
  ASSERT_TRUE(p.getClassifier() == 0);
  ASSERT_TRUE(p.getFeaturingClassifier() == 0);
  ASSERT_TRUE(p.getNamespace() == 0);
  ASSERT_TRUE(p.getMemberNamespace().size() == 0);
  ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(ClassTest, addRoleFunctorTest) {
  StructuredClassifier c;
  ConnectableElement r;
  ASSERT_NO_THROW(c.getRole().add(r));
  ASSERT_TRUE(c.getRole().size() == 1);
  ASSERT_TRUE(c.getRole().front() == &r);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(c.getMembers().front() == &r);

  ASSERT_TRUE(r.getMemberNamespace().size() == 1);
  ASSERT_TRUE(r.getMemberNamespace().front() == &c);
}

TEST_F(ClassTest, removeRoleFunctorTest) {
  StructuredClassifier c;
  ConnectableElement r;
  c.getRole().add(r);
  ASSERT_NO_THROW(c.getRole().remove(r));
  ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(r.getMemberNamespace().size() == 0);
}
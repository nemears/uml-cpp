#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/parameter.h"
#include "uml/sequence.h"

using namespace UML;

class ClassTest : public ::testing::Test {
   
};

TEST_F(ClassTest, BasicOperationTest) {
    Class c;
    Operation o;
    OpaqueBehavior oB;
    Parameter p;
    ASSERT_NO_THROW(oB.getParameters().add(p));
    ASSERT_NO_THROW(oB.bodies.push_back("return true"));
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
  ASSERT_TRUE(p1.getOperations().size() == 0);
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
  ASSERT_TRUE(p1.getOperations().size() == 0);
}

TEST_F(ClassTest, removeOperationFunctorTest) {
  Class c;
  Operation o;
  c.getOperations().add(o);
  ASSERT_NO_THROW(c.getOperations().remove(o));
  ASSERT_TRUE(c.getOperations().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!o.getClass());
  ASSERT_TRUE(!o.getOwner());
}
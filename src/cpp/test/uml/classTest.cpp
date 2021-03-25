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
}

TEST_F(ClassTest, setClassTest) {
    Class c;
    Operation o;
    o.setClass(&c);
    ASSERT_TRUE(c.getOperations().size() == 1);
    ASSERT_TRUE(c.getOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
}
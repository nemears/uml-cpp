#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/parameter.h"

using namespace UML;

class ClassTest : public ::testing::Test {
   
};

TEST_F(ClassTest, BasicOperationTest) {
    Class c;
    Operation o;
    OpaqueBehavior oB;
    Parameter p;
    ASSERT_NO_THROW(o.parameters.push_back(&p));
    ASSERT_NO_THROW(oB.bodies.push_back("return true"));
    ASSERT_NO_THROW(o.methods.push_back(&oB));
    ASSERT_NO_THROW(c.operations.push_back(&o));
}

TEST_F(ClassTest, AddOperationTest) {
    Class c;
    Operation o;
    ASSERT_NO_THROW(c.addOperation(o));
    ASSERT_TRUE(c.operations.front()->uuid == o.uuid);
}

TEST_F(ClassTest, RemoveOperationTest) {
    Class c;
    Operation o;
    Operation o2;
    c.addOperation(o);
    c.addOperation(o2);
    ASSERT_TRUE(c.operations.front()->uuid == o.uuid);
    ASSERT_NO_THROW(c.removeOperation(o));
    ASSERT_TRUE(c.operations.front()->uuid == o2.uuid);
}
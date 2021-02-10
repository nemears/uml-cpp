#include "gtest/gtest.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class OperationTest : public ::testing::Test {};

TEST_F(OperationTest, AddParameterTest) {
    Operation o;
    Parameter p;
    ASSERT_NO_THROW(o.addParameter(p));
    ASSERT_TRUE(o.parameters.front()->uuid == p.uuid);
}

TEST_F(OperationTest, RemoveParameterTest) {
    Operation o;
    Parameter p;
    Parameter p2;
    o.addParameter(p);
    o.addParameter(p2);
    ASSERT_TRUE(o.parameters.front()->uuid == p.uuid);
    ASSERT_NO_THROW(o.removeParameter(p));
    ASSERT_TRUE(o.parameters.front()->uuid == p2.uuid);
}

TEST_F(OperationTest, AddMethodTest) {
    Operation o;
    OpaqueBehavior b;
    ASSERT_NO_THROW(o.addMethod(b));
    ASSERT_TRUE(o.methods.front()->uuid == b.uuid);
}

TEST_F(OperationTest, RemoveMethodTest) {
    Operation o;
    OpaqueBehavior b;
    OpaqueBehavior b2;
    o.addMethod(b);
    o.addMethod(b2);
    ASSERT_TRUE(o.methods.front()->uuid == b.uuid);
    ASSERT_NO_THROW(o.removeMethod(b));
    ASSERT_TRUE(o.methods.front()->uuid == b2.uuid);
}
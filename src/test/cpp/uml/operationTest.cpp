#include "gtest/gtest.h"
#include "uml/operation.h"

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
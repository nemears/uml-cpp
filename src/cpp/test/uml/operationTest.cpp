#include "gtest/gtest.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class OperationTest : public ::testing::Test {};

TEST_F(OperationTest, SetAndGetTypeTest) {
    Class c;
    Operation o;
    ASSERT_NO_THROW(o.setType(&c));
    ASSERT_TRUE(o.getType()->getID() == c.getID());
}

TEST_F(OperationTest, reIndexID_Test) {
    Class c;
    Operation o;
    o.setClass(&c);
    c.getOperations().add(o);
    c.getOwnedElements().add(o);
    o.setOwner(&c);
    o.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    ASSERT_TRUE(c.getOperations().get(o.getID()) != NULL);
    ASSERT_TRUE(c.getOwnedElements().get(o.getID()) != NULL);
}

TEST_F(OperationTest, reIndexNameTest) {
    Class c;
    Operation o;
    o.setClass(&c);
    c.getOperations().add(o);
    c.getOwnedElements().add(o);
    o.setOwner(&c);
    o.setName("test");
    ASSERT_TRUE(c.getOperations().get("test") != NULL);
    ASSERT_TRUE(c.getOwnedElements().get("test") != NULL);
}
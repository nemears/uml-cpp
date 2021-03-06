#include "gtest/gtest.h"
#include "uml/objectNode.h"
#include "uml/literalBool.h"

using namespace UML;

class ObjectNodeTest : public ::testing::Test {
   
};

TEST_F(ObjectNodeTest, getNullUpperBoundTest) {
    ObjectNode o;
    ASSERT_TRUE(o.getUpperBound() == NULL);
}

TEST_F(ObjectNodeTest, setUpperBoundTest) {
    ObjectNode o;
    LiteralBool b;
    b.setValue(true);
    ASSERT_NO_THROW(o.setUpperBound(&b));
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(o.getUpperBound())->getValue());
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ObjectNodeTest : public ::testing::Test {
   
};

TEST_F(ObjectNodeTest, getNullUpperBoundTest) {
    Manager<> m;
    ObjectNode& o = *m.create<ObjectNode>();
    ASSERT_FALSE(o.getUpperBound());
}

TEST_F(ObjectNodeTest, setUpperBoundTest) {
    Manager<> m;
    ObjectNode& o = *m.create<ObjectNode>();
    LiteralBool& b = *m.create<LiteralBool>();
    b.setValue(true);
    ASSERT_NO_THROW(o.setUpperBound(&b));
    ASSERT_TRUE(o.getUpperBound()->as<LiteralBool>().getValue());
}
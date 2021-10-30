#include "gtest/gtest.h"
#include "uml/objectNode.h"
#include "uml/literalBool.h"
#include "uml/umlManager.h"

using namespace UML;

class ObjectNodeTest : public ::testing::Test {
   
};

TEST_F(ObjectNodeTest, getNullUpperBoundTest) {
    UmlManager m;
    ObjectNode& o = m.create<ObjectNode>();
    ASSERT_TRUE(o.getUpperBound() == NULL);
}

TEST_F(ObjectNodeTest, setUpperBoundTest) {
    UmlManager m;
    ObjectNode& o = m.create<ObjectNode>();
    LiteralBool& b = m.create<LiteralBool>();
    b.setValue(true);
    ASSERT_NO_THROW(o.setUpperBound(&b));
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(o.getUpperBound())->getValue());
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace std;
using namespace UML;

class ValueSpecificationTest : public ::testing::Test {};

TEST_F(ValueSpecificationTest, LiteralStringTest) {
    UmlManager m;
    string val = "test";
    Property& p = m.create<Property>();
    PrimitiveType& prim = m.create<PrimitiveType>();
    // prim.setPrimitiveType(PrimitiveType::Primitive::STRING);
    p.setType(&prim);
    LiteralString& ls = m.create<LiteralString>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), "test");
}

TEST_F(ValueSpecificationTest, LiteralIntTest) {
    UmlManager m;
    int val = -1;
    Property& p = m.create<Property>();
    PrimitiveType& prim = m.create<PrimitiveType>();
    // prim.setPrimitiveType(PrimitiveType::Primitive::INT);
    p.setType(&prim);
    LiteralInt& ls = m.create<LiteralInt>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -1);
}

TEST_F(ValueSpecificationTest, LiteralRealTest) {
    UmlManager m;
    double val = -3.14159;
    Property& p = m.create<Property>();
    PrimitiveType& prim = m.create<PrimitiveType>();
    // prim.setPrimitiveType(PrimitiveType::Primitive::REAL);
    p.setType(&prim);
    LiteralReal& ls = m.create<LiteralReal>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -3.14159);
}

TEST_F(ValueSpecificationTest, LiteralBoolTest) {
    UmlManager m;
    bool val = 0; // > 0 is true
    Property& p = m.create<Property>();
    PrimitiveType& prim = m.create<PrimitiveType>();
    // prim.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    p.setType(&prim);
    LiteralBool& ls = m.create<LiteralBool>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), false);
}

TEST_F(ValueSpecificationTest, reindexID_forSlotTest) {
    UmlManager m;
    LiteralBool& v = m.create<LiteralBool>();
    Slot& s = m.create<Slot>();
    s.getValues().add(v);
    v.setID("eb092018_0bef_4ad6_b80f_05fa");
    ASSERT_NO_THROW(s.getValues().get(v.getID()));
    ASSERT_NO_THROW(s.getOwnedElements().get(v.getID()));
}

// TEST_F(ValueSpecificationTest, reindexNameForSlotTest) {
//     LiteralInt v;
//     Slot s;
//     s.getValues().add(v);
//     v.setName("test");
//     ASSERT_TRUE(s.getValues().get("test"));
//     ASSERT_TRUE(s.getOwnedElements().get("test"));
// }

// TEST_F(ValueSpecificationTest, reindexID_ExpressionTest) {
//     UmlManager m;
//     Expression& e = m.create<Expression>();
//     LiteralBool& b = m.create<LiteralBool>();
//     e.getOperands().add(b);
//     e.setSymbol("==");
//     b.setID("eb092018_0bef_4ad6_b80f_05fa");
//     ASSERT_NO_THROW(e.getOperands().get(b.getID()));
//     ASSERT_NO_THROW(e.getOwnedElements().get(b.getID()));
// }

// TEST_F(ValueSpecificationTest, reindexNameExpressionTest) {
//     Expression e;
//     LiteralBool b;
//     e.getOperands().add(b);
//     e.setSymbol("==");
//     b.setName("test");
//     ASSERT_TRUE(e.getOperands().get("test"));
//     ASSERT_TRUE(e.getOwnedElements().get("test"));
// }

TEST_F(ValueSpecificationTest, LiteralUnlimitedNaturalTest) {
    UmlManager m;
    LiteralUnlimitedNatural& n = m.create<LiteralUnlimitedNatural>();
    ASSERT_EQ(n.getNumberValue(), 0);
    ASSERT_EQ(n.isInfinite(), false);
    n.setNumberValue(6);
    ASSERT_EQ(n.getNumberValue(), 6);
    ASSERT_EQ(n.isInfinite(), false);
    n.setInfinite();
    ASSERT_EQ(n.getNumberValue(), 0);
    ASSERT_EQ(n.isInfinite(), true);
}
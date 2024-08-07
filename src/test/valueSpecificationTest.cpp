#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace std;
using namespace UML;

UML_SET_INTEGRATION_TEST(ExpressionOperands, Expression, Expression, &Expression::getOperands)

class ValueSpecificationTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ValueSpecificationTest, LiteralStringTest) {
    Manager<> m;
    string val = "test";
    Property& p = *m.create<Property>();
    PrimitiveType& prim = *m.create<PrimitiveType>();
    p.setType(prim);
    LiteralString& ls = *m.create<LiteralString>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), "test");
}

TEST_F(ValueSpecificationTest, LiteralIntTest) {
    Manager<> m;
    int val = -1;
    Property& p = *m.create<Property>();
    PrimitiveType& prim = *m.create<PrimitiveType>();
    p.setType(prim);
    LiteralInt& ls = *m.create<LiteralInt>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -1);
}

TEST_F(ValueSpecificationTest, LiteralRealTest) {
    Manager<> m;
    double val = -3.14159;
    Property& p = *m.create<Property>();
    PrimitiveType& prim = *m.create<PrimitiveType>();
    p.setType(prim);
    LiteralReal& ls = *m.create<LiteralReal>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -3.14159);
}

TEST_F(ValueSpecificationTest, LiteralBoolTest) {
    Manager<> m;
    bool val = 0; // > 0 is true
    Property& p = *m.create<Property>();
    PrimitiveType& prim = *m.create<PrimitiveType>();
    p.setType(prim);
    LiteralBool& ls = *m.create<LiteralBool>();
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), false);
}

TEST_F(ValueSpecificationTest, reindexNameForSlotTest) {
    Manager<> m;
    LiteralInt& v = *m.create<LiteralInt>();
    Slot& s = *m.create<Slot>();
    s.getValues().add(v);
    v.setName("test");
    ASSERT_EQ(s.getValues().get("test"), v);
    ASSERT_EQ(s.getOwnedElements().get("test"), v);
}

TEST_F(ValueSpecificationTest, reindexNameExpressionTest) {
    Manager<> m;
    Expression& e = *m.create<Expression>();
    LiteralBool& b = *m.create<LiteralBool>();
    e.getOperands().add(b);
    e.setSymbol("==");
    b.setName("test");
    ASSERT_EQ(e.getOperands().get("test"), b);
    ASSERT_EQ(e.getOwnedElements().get("test"), b);
}

TEST_F(ValueSpecificationTest, LiteralUnlimitedNaturalTest) {
    Manager<> m;
    LiteralUnlimitedNatural& n = *m.create<LiteralUnlimitedNatural>();
    ASSERT_EQ(n.getNumberValue(), 0);
    ASSERT_EQ(n.isInfinite(), false);
    n.setNumberValue(6);
    ASSERT_EQ(n.getNumberValue(), 6);
    ASSERT_EQ(n.isInfinite(), false);
    n.setInfinite();
    ASSERT_EQ(n.getNumberValue(), 0);
    ASSERT_EQ(n.isInfinite(), true);
}

TEST_F(ValueSpecificationTest, expressionTest) {
    Manager<> m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "expressionTests/expression.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveTypePtr b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK")));
    PrimitiveTypePtr i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("int_r9nNbBukx47IomXrT2raqtc4")));
    PrimitiveTypePtr r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("real_aZG&w6yl61bXVWutgeyScN9")));
    PrimitiveTypePtr s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT")));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("hvfrdhQ6M6undbb5bWGKlMESCn_l")).getElementType() == ElementType::EXPRESSION);
    Expression* exp = dynamic_cast<Expression*>(&pckg->getPackagedElements().get(ID::fromString("hvfrdhQ6M6undbb5bWGKlMESCn_l")));
    ASSERT_TRUE(exp->getSymbol().compare("==") == 0);
    ASSERT_TRUE(exp->getType() == b);
    ASSERT_TRUE(exp->getOperands().size() == 2);
    ASSERT_TRUE(exp->getOperands().front().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* a = dynamic_cast<LiteralInt*>(&exp->getOperands().get("a"));
    ASSERT_TRUE(a->getValue() == 1);
    ASSERT_TRUE(exp->getOperands().back().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* ib = dynamic_cast<LiteralInt*>(&exp->getOperands().get("b"));
    ASSERT_TRUE(ib->getValue() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("Zx5giHhheJ6GSaP5oLEN6FxI99L8")).getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().get(ID::fromString("Zx5giHhheJ6GSaP5oLEN6FxI99L8")));
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDefaultValue());
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == ElementType::EXPRESSION);
    Expression* e2 = dynamic_cast<Expression*>(p->getDefaultValue().ptr());
    ASSERT_TRUE(e2->getSymbol().compare("++") == 0);
    ASSERT_TRUE(e2->getOperands().size() == 1);
    ASSERT_TRUE(e2->getOperands().front().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* pi = dynamic_cast<LiteralInt*>(&e2->getOperands().front());
    ASSERT_TRUE(pi->getValue() == 1);
}

TEST_F(ValueSpecificationTest, mountExpressionTest) {
    Manager<> m;
    Expression& expression = *m.create<Expression>();
    LiteralReal& first = *m.create<LiteralReal>();
    LiteralInt& last = *m.create<LiteralInt>();
    expression.setSymbol("+");
    expression.getOperands().add(first, last);
    m.setRoot(&expression);
    m.mount(ymlPath + "expressionTests");

    ID expressionID = expression.getID();
    ID firstID = first.getID();
    m.release(expression);
    ASSERT_FALSE(m.loaded(expressionID));
    Expression& expression2 = m.get(expressionID)->as<Expression>();
    ASSERT_EQ(expression2.getOperands().size(), 2);
    ASSERT_EQ(expression2.getOperands().front(), first);
    ASSERT_EQ(expression2.getOperands().back(), last);
    ASSERT_EQ(expression2.getOwnedElements().size(), 2);
    ASSERT_TRUE(expression2.getOwnedElements().contains(last));
    ASSERT_TRUE(expression2.getOwnedElements().contains(first));
    ASSERT_TRUE(first.getOwner());
    ASSERT_TRUE(last.getOwner());

    m.release(first);
    ASSERT_FALSE(m.loaded(firstID));
    ASSERT_EQ(*expression2.getOperands().ids().begin(), firstID);
    LiteralReal& first2 = m.get(firstID)->as<LiteralReal>();
    ASSERT_EQ(expression2.getOperands().size(), 2);
    ASSERT_EQ(expression2.getOperands().front(), first2);
    ASSERT_EQ(expression2.getOperands().back(), last);
    ASSERT_EQ(expression2.getOwnedElements().size(), 2);
    ASSERT_TRUE(expression2.getOwnedElements().contains(last));
    ASSERT_TRUE(expression2.getOwnedElements().contains(first2));
    ASSERT_TRUE(first2.getOwner());
    ASSERT_TRUE(last.getOwner());

    m.release(first2, expression2);
    ASSERT_FALSE(m.loaded(firstID));
    ASSERT_FALSE(m.loaded(expressionID));
    LiteralReal& first3 = m.get(firstID)->as<LiteralReal>();
    Expression& expression3 = m.get(expressionID)->as<Expression>();
    ASSERT_EQ(expression3.getOperands().size(), 2);
    ASSERT_EQ(expression3.getOperands().front(), first3);
    ASSERT_EQ(expression3.getOperands().back(), last);
    ASSERT_EQ(expression3.getOwnedElements().size(), 2);
    ASSERT_TRUE(expression3.getOwnedElements().contains(last));
    ASSERT_TRUE(expression3.getOwnedElements().contains(first3));
    ASSERT_TRUE(first3.getOwner());
    ASSERT_TRUE(last.getOwner());
}

TEST_F(ValueSpecificationTest, testParsingSomeLiterals) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "literalsTests/someLiterals.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_EQ(pckg->getPackagedElements().size(), 3);
    ASSERT_EQ(pckg->getPackagedElements().get("infinity").getElementType(), ElementType::LITERAL_UNLIMITED_NATURAL);
    LiteralUnlimitedNatural* n1 = dynamic_cast<LiteralUnlimitedNatural*>(&pckg->getPackagedElements().get("infinity"));
    ASSERT_EQ(n1->getName(), "infinity");
    ASSERT_EQ(n1->isInfinite(), true);
    ASSERT_EQ(pckg->getPackagedElements().get("number").getElementType(), ElementType::LITERAL_UNLIMITED_NATURAL);
    LiteralUnlimitedNatural* n2 = dynamic_cast<LiteralUnlimitedNatural*>(&pckg->getPackagedElements().get("number"));
    ASSERT_EQ(n2->getName(), "number");
    ASSERT_EQ(n2->isInfinite(), false);
    ASSERT_EQ(n2->getNumberValue(), 9);
    ASSERT_EQ(pckg->getPackagedElements().get("nullLiteral").getElementType(), ElementType::LITERAL_NULL);
    LiteralNull* n3 = dynamic_cast<LiteralNull*>(&pckg->getPackagedElements().get("nullLiteral"));
    ASSERT_EQ(n3->getName(), "nullLiteral");
}

TEST_F(ValueSpecificationTest, testEmitLiteralUnlimitedNatural) {
    Manager<> m;
    Package& p = *m.create<Package>();
    LiteralUnlimitedNatural& l1 = *m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l2 = *m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l3 = *m.create<LiteralUnlimitedNatural>();
    LiteralNull& ln = *m.create<LiteralNull>();
    p.setID("e_ob7tgbN16Plhj_sTAOVD5ijLrL");
    l1.setID("7bYUY3yFUBrfPmzKKrV2NJmXuECA");
    l2.setID("puJaUTZsLPdGJkJSJtdX51MIA2ch");
    l3.setID("8&K_0aLhvQDM12ZeYg9nPiSrexHo");
    ln.setID("4gA4RgL9vKTRYd61D99y1d_Yggj6");
    l2.setInfinite();
    l3.setNumberValue(9999);
    p.getPackagedElements().add(l1);
    p.getPackagedElements().add(l2);
    p.getPackagedElements().add(l3);
    p.getPackagedElements().add(ln);

    string expectedEmit = R""""(Package:
  id: e_ob7tgbN16Plhj_sTAOVD5ijLrL
  packagedElements:
    - LiteralUnlimitedNatural:
        id: "8&K_0aLhvQDM12ZeYg9nPiSrexHo"
        value: 9999
    - LiteralUnlimitedNatural:
        id: 7bYUY3yFUBrfPmzKKrV2NJmXuECA
        value: 0
    - LiteralUnlimitedNatural:
        id: puJaUTZsLPdGJkJSJtdX51MIA2ch
        value: "*"
    - LiteralNull:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6)"""";
    string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(p, data));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

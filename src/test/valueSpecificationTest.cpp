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
    UmlManager m;
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
    UmlManager m;
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
    UmlManager m;
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
    UmlManager m;
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
    UmlManager m;
    LiteralInt& v = *m.create<LiteralInt>();
    Slot& s = *m.create<Slot>();
    s.getValues().add(v);
    v.setName("test");
    ASSERT_EQ(s.getValues().get("test"), &v);
}

TEST_F(ValueSpecificationTest, reindexNameExpressionTest) {
    UmlManager m;
    Expression& e = *m.create<Expression>();
    LiteralBool& b = *m.create<LiteralBool>();
    e.getOperands().add(b);
    e.setSymbol("==");
    b.setName("test");
    ASSERT_EQ(e.getOperands().get("test"), &b);
}

TEST_F(ValueSpecificationTest, LiteralUnlimitedNaturalTest) {
    UmlManager m;
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
    UmlManager m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "expressionTests/expression.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == Package::Info::elementType);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveTypePtr b = pckg->getPackageMerge().front()->getMergedPackage()->getPackagedElements().get(ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK"));
    PrimitiveTypePtr i = pckg->getPackageMerge().front()->getMergedPackage()->getPackagedElements().get(ID::fromString("int_r9nNbBukx47IomXrT2raqtc4"));
    PrimitiveTypePtr r = pckg->getPackageMerge().front()->getMergedPackage()->getPackagedElements().get(ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
    PrimitiveTypePtr s = pckg->getPackageMerge().front()->getMergedPackage()->getPackagedElements().get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT"));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("hvfrdhQ6M6undbb5bWGKlMESCn_l"))->getElementType() == Expression::Info::elementType);
    ExpressionPtr exp = pckg->getPackagedElements().get(ID::fromString("hvfrdhQ6M6undbb5bWGKlMESCn_l"));
    ASSERT_TRUE(exp->getSymbol().compare("==") == 0);
    ASSERT_TRUE(exp->getType() == b);
    ASSERT_TRUE(exp->getOperands().size() == 2);
    ASSERT_TRUE(exp->getOperands().front()->getElementType() == LiteralInt::Info::elementType);
    LiteralIntPtr a = exp->getOperands().get("a");
    ASSERT_TRUE(a->getValue() == 1);
    ASSERT_TRUE(exp->getOperands().back()->getElementType() == LiteralInt::Info::elementType);
    LiteralIntPtr ib = exp->getOperands().get("b");
    ASSERT_TRUE(ib->getValue() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("Zx5giHhheJ6GSaP5oLEN6FxI99L8"))->getElementType() == Class::Info::elementType);
    ClassPtr c = pckg->getPackagedElements().get(ID::fromString("Zx5giHhheJ6GSaP5oLEN6FxI99L8"));
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    PropertyPtr p = c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDefaultValue());
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == Expression::Info::elementType);
    ExpressionPtr e2 = p->getDefaultValue();
    ASSERT_TRUE(e2->getSymbol().compare("++") == 0);
    ASSERT_TRUE(e2->getOperands().size() == 1);
    ASSERT_TRUE(e2->getOperands().front()->getElementType() == LiteralInt::Info::elementType);
    LiteralIntPtr pi = e2->getOperands().front();
    ASSERT_TRUE(pi->getValue() == 1);
}

TEST_F(ValueSpecificationTest, mountExpressionTest) {
    UmlManager m;
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
    ASSERT_EQ(expression2.getOperands().front(), &first);
    ASSERT_EQ(expression2.getOperands().back(), &last);
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
    ASSERT_EQ(expression2.getOperands().front(), &first2);
    ASSERT_EQ(expression2.getOperands().back(), &last);
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
    ASSERT_EQ(expression3.getOperands().front(), &first3);
    ASSERT_EQ(expression3.getOperands().back(), &last);
    ASSERT_EQ(expression3.getOwnedElements().size(), 2);
    ASSERT_TRUE(expression3.getOwnedElements().contains(last));
    ASSERT_TRUE(expression3.getOwnedElements().contains(first3));
    ASSERT_TRUE(first3.getOwner());
    ASSERT_TRUE(last.getOwner());
}

TEST_F(ValueSpecificationTest, testParsingSomeLiterals) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "literalsTests/someLiterals.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), Package::Info::elementType);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_EQ(pckg->getPackagedElements().size(), 3);
    ASSERT_EQ(pckg->getPackagedElements().get("infinity")->getElementType(), LiteralUnlimitedNatural::Info::elementType);
    LiteralUnlimitedNaturalPtr n1 = pckg->getPackagedElements().get("infinity");
    ASSERT_EQ(n1->getName(), "infinity");
    ASSERT_EQ(n1->isInfinite(), true);
    ASSERT_EQ(pckg->getPackagedElements().get("number")->getElementType(), LiteralUnlimitedNatural::Info::elementType);
    LiteralUnlimitedNaturalPtr n2 = pckg->getPackagedElements().get("number");
    ASSERT_EQ(n2->getName(), "number");
    ASSERT_EQ(n2->isInfinite(), false);
    ASSERT_EQ(n2->getNumberValue(), 9);
    ASSERT_EQ(pckg->getPackagedElements().get("nullLiteral")->getElementType(), LiteralNull::Info::elementType);
    LiteralNullPtr n3 = pckg->getPackagedElements().get("nullLiteral");
    ASSERT_EQ(n3->getName(), "nullLiteral");
}

TEST_F(ValueSpecificationTest, testEmitLiteralUnlimitedNatural) {
    UmlManager m;
    Package& p = *m.create<Package>();
    LiteralUnlimitedNatural& l1 = *m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l2 = *m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l3 = *m.create<LiteralUnlimitedNatural>();
    LiteralNull& ln = *m.create<LiteralNull>();
    p.setID(ID::fromString("e_ob7tgbN16Plhj_sTAOVD5ijLrL"));
    l1.setID(ID::fromString("7bYUY3yFUBrfPmzKKrV2NJmXuECA"));
    l2.setID(ID::fromString("puJaUTZsLPdGJkJSJtdX51MIA2ch"));
    l3.setID(ID::fromString("8&K_0aLhvQDM12ZeYg9nPiSrexHo"));
    ln.setID(ID::fromString("4gA4RgL9vKTRYd61D99y1d_Yggj6"));
    l2.setInfinite();
    l3.setNumberValue(9999);
    p.getPackagedElements().add(l1);
    p.getPackagedElements().add(l2);
    p.getPackagedElements().add(l3);
    p.getPackagedElements().add(ln);

    string expectedEmit = R""""(Package:
  id: e_ob7tgbN16Plhj_sTAOVD5ijLrL
  packagedElements:
    - LiteralNull:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6
    - LiteralUnlimitedNatural:
        id: "8&K_0aLhvQDM12ZeYg9nPiSrexHo"
        value: 9999
    - LiteralUnlimitedNatural:
        id: puJaUTZsLPdGJkJSJtdX51MIA2ch
        value: "*"
    - LiteralUnlimitedNatural:
        id: 7bYUY3yFUBrfPmzKKrV2NJmXuECA
        value: 0)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(p));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

using namespace std;
using namespace UML;

class ExpressionParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ExpressionParserTest, expressionTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "expressionTests/expression.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveTypePtr b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK")));
    PrimitiveTypePtr i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("int_r9nNbBukx47IomXrT2raqtc4")));
    PrimitiveTypePtr r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("real_aZG&w6yl61bXVWutgeyScN9")));
    PrimitiveTypePtr s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT")));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::EXPRESSION);
    Expression* exp = dynamic_cast<Expression*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(exp->getSymbol().compare("==") == 0);
    ASSERT_TRUE(exp->getType() == b);
    ASSERT_TRUE(exp->getOperands().size() == 2);
    ASSERT_TRUE(exp->getOperands().front().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* a = dynamic_cast<LiteralInt*>(&exp->getOperands().get("a"));
    ASSERT_TRUE(a->getValue() == 1);
    ASSERT_TRUE(exp->getOperands().back().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* ib = dynamic_cast<LiteralInt*>(&exp->getOperands().get("b"));
    ASSERT_TRUE(ib->getValue() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
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

TEST_F(ExpressionParserTest, mountExpressionTest) {
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
    ID lastID = last.getID();
    m.release(expression);
    ASSERT_FALSE(m.loaded(expressionID));
    Expression& expression2 = m.aquire(expressionID)->as<Expression>();
    ASSERT_EQ(expression2.getOperands().size(), 2);
    ASSERT_EQ(expression2.getOperands().front(), first);
    ASSERT_EQ(expression2.getOperands().back(), last);
    ASSERT_EQ(expression2.getOwnedElements().size(), 2);
    ASSERT_EQ(*expression2.getOwnedElements().begin(), first);
    ASSERT_EQ(*(expression2.getOwnedElements().begin()++), last);
    ASSERT_TRUE(first.getOwner());
    ASSERT_TRUE(last.getOwner());

    m.release(first);
    ASSERT_FALSE(m.loaded(firstID));
    ASSERT_EQ(*expression2.getOperands().ids().begin(), firstID);
    LiteralReal& first2 = m.aquire(firstID)->as<LiteralReal>();
    ASSERT_EQ(expression2.getOperands().size(), 2);
    ASSERT_EQ(expression2.getOperands().front(), first2);
    ASSERT_EQ(expression2.getOperands().back(), last);
    ASSERT_EQ(expression2.getOwnedElements().size(), 2);
    ASSERT_EQ(*expression2.getOwnedElements().begin(), first2);
    ASSERT_EQ(*(expression2.getOwnedElements().begin()++), last);
    ASSERT_TRUE(first2.getOwner());
    ASSERT_TRUE(last.getOwner());

    m.release(first2, expression2);
    ASSERT_FALSE(m.loaded(firstID));
    ASSERT_FALSE(m.loaded(expressionID));
    LiteralReal& first3 = m.aquire(firstID)->as<LiteralReal>();
    Expression& expression3 = m.aquire(expressionID)->as<Expression>();
    ASSERT_EQ(expression3.getOperands().size(), 2);
    ASSERT_EQ(expression3.getOperands().front(), first3);
    ASSERT_EQ(expression3.getOperands().back(), last);
    ASSERT_EQ(expression3.getOwnedElements().size(), 2);
    ASSERT_EQ(*expression3.getOwnedElements().begin(), first3);
    ASSERT_EQ(*(expression3.getOwnedElements().begin()++), last);
    ASSERT_TRUE(first3.getOwner());
    ASSERT_TRUE(last.getOwner());
}
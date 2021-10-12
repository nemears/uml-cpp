#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/expression.h"
#include "uml/package.h"
#include "uml/packageMerge.h"
#include "uml/primitiveType.h"
#include "uml/literalInt.h"
#include "uml/class.h"
#include "uml/property.h"
#include "uml/literalReal.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "expressionTests/expression.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().front());
    PrimitiveType* i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(1));
    PrimitiveType* r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(2));
    PrimitiveType* s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(3));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::EXPRESSION);
    Expression* exp = dynamic_cast<Expression*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(exp->getSymbol().compare("==") == 0);
    ASSERT_TRUE(exp->getType() == b);
    ASSERT_TRUE(exp->getOperands().size() == 2);
    ASSERT_TRUE(exp->getOperands().front().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* a = dynamic_cast<LiteralInt*>(&exp->getOperands().front());
    ASSERT_TRUE(a->getValue() == 1);
    ASSERT_TRUE(exp->getOperands().back().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* ib = dynamic_cast<LiteralInt*>(&exp->getOperands().back());
    ASSERT_TRUE(ib->getValue() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDefaultValue() != 0);
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == ElementType::EXPRESSION);
    Expression* e2 = dynamic_cast<Expression*>(p->getDefaultValue());
    ASSERT_TRUE(e2->getSymbol().compare("++") == 0);
    ASSERT_TRUE(e2->getOperands().size() == 1);
    ASSERT_TRUE(e2->getOperands().front().getElementType() == ElementType::LITERAL_INT);
    LiteralInt* pi = dynamic_cast<LiteralInt*>(&e2->getOperands().front());
    ASSERT_TRUE(pi->getValue() == 1);
}

TEST_F(ExpressionParserTest, mountExpressionTest) {
    UmlManager m;
    Expression& expression = m.create<Expression>();
    LiteralReal& first = m.create<LiteralReal>();
    LiteralInt& last = m.create<LiteralInt>();
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
    ASSERT_EQ(expression2.getOwnedElements().front(), first);
    ASSERT_EQ(expression2.getOwnedElements().back(), last);
    ASSERT_TRUE(first.hasOwner());
    ASSERT_TRUE(last.hasOwner());
}
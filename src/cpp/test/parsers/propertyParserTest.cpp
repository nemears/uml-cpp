#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/property.h"
#include "uml/class.h"
#include "uml/package.h"

using namespace std;
using namespace UML;

class PropertyParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(PropertyParserTest, forwardTypeTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "propertyTests/forwardType.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::CLASS);
    Class* clazz1 = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    Class* clazz2 = dynamic_cast<Class*>(pckg->getPackagedElements().back());
    ASSERT_TRUE(clazz2->getAttributes().size() == 1);
    Property* prop = clazz2->getAttributes().front();
    ASSERT_TRUE(prop->getType() == clazz1);
}

TEST_F(PropertyParserTest, backwardsTypeTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "propertyTests/backwardTypeTest.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::CLASS);
    Class* clazz1 = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    Class* clazz2 = dynamic_cast<Class*>(pckg->getPackagedElements().back());
    ASSERT_TRUE(clazz1->getAttributes().size() == 1);
    Property* prop = clazz1->getAttributes().front();
    ASSERT_TRUE(prop->getType() == clazz2);
}

TEST_F(PropertyParserTest, multiplicityTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "propertyTests/multiplicityTest.yml"));
    ASSERT_TRUE(el);
    ASSERT_TRUE(el->getElementType() == ElementType::PROPERTY);
    Property* prop = dynamic_cast<Property*>(el);
    ASSERT_TRUE(prop->multiplicitySpecified());
    ASSERT_TRUE(prop->getLowerValue());
    ASSERT_TRUE(prop->getLower() == 0);
    ASSERT_TRUE(prop->getLowerValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getLower() == dynamic_cast<LiteralInt*>(prop->getLowerValue())->getValue());
    ASSERT_TRUE(prop->getUpperValue());
    ASSERT_TRUE(prop->getUpper() == 1);
    ASSERT_TRUE(prop->getUpperValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getUpper() == dynamic_cast<LiteralInt*>(prop->getUpperValue())->getValue());
}

TEST_F(PropertyParserTest, improperTypeTest) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/improperType.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/improperType2.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/improperType3.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/propertyNotMap.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/attributesNotSequence.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/invalidLower.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "propertyTests/invalidUpper.yml"), Parsers::UmlParserException);
}

TEST_F(PropertyParserTest, literalBoolDefaultValueTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "propertyTests/literalBool.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(pckg->getPackageMerge().front()->getMergedPackage()->getPackagedElements().front());
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == b);
    ASSERT_TRUE(p->getDefaultValue() != 0);
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == ElementType::LITERAL_BOOL);
    LiteralBool* lb = dynamic_cast<LiteralBool*>(p->getDefaultValue());
    ASSERT_TRUE(lb->getValue());
}
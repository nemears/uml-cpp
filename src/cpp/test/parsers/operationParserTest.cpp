#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/operation.h"
#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/parameter.h"
#include "uml/primitiveType.h"

using namespace std;
using namespace UML;

class OperationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(OperationParserTest, properExceptions) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidBehavior.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidMethodList.yml"), Parsers::UmlParserException);
}

TEST_F(OperationParserTest, basicParamTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "operationTests/basicParameter.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().front());
    PrimitiveType* i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(1));
    PrimitiveType* r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(2));
    PrimitiveType* s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(3));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedOperations().size() == 1);
    Operation* o =&c->getOwnedOperations().front();
    ASSERT_TRUE(o->getOwnedParameters().size() == 1);
    Parameter* p = &o->getOwnedParameters().front();
    ASSERT_TRUE(p->getName().compare("testInt") == 0);
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDirection() == ParameterDirectionKind::IN);
}
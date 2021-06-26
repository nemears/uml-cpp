#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"

using namespace std;
using namespace UML;

class TemplateableElementParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(TemplateableElementParserTest, basicTemplateSignature) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/classW_emptySignature.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature() != 0);
    TemplateSignature* s = c->getOwnedTemplateSignature();
    ASSERT_EQ(s->getID(), ID::fromString("b4EasFCBjochdruOQfxBubQw3VlD"));
}

TEST_F(TemplateableElementParserTest, singleEmptyTemplateParameterTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/classW_SingleTemplateParameter.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class * c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature() != 0);
    TemplateSignature* s = c->getOwnedTemplateSignature();
    ASSERT_EQ(s->getOwnedParameter().size(), 1);
    TemplateParameter* p = s->getOwnedParameter().front();
    ASSERT_EQ(p->getID(), ID::fromString("t9FFOy3xNADeUDNvWJOc&7USIfsf"));
}
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

TEST_F(TemplateableElementParserTest, multipleParametersTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/clasW_multipleParameters.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature() != 0);
    TemplateSignature* s = c->getOwnedTemplateSignature();
    ASSERT_EQ(s->getOwnedParameter().size(), 3);
    TemplateParameter* p1 = s->getOwnedParameter().front();
    ASSERT_TRUE(p1->getOwnedParameteredElement() != 0);
    ASSERT_EQ(p1->getOwnedParameteredElement()->getElementType(), ElementType::PRIMITIVE_TYPE);
    PrimitiveType* p = dynamic_cast<PrimitiveType*>(p1->getOwnedParameteredElement());
    ASSERT_EQ(p->getName(), "long");
    TemplateParameter* p2 = s->getOwnedParameter().get(1);
    ASSERT_EQ(p2->getOwnedParameteredElement()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(p2->getOwnedParameteredElement());
    ASSERT_EQ(i->getName(), "test");
    TemplateParameter* p3 = s->getOwnedParameter().back();
    ASSERT_TRUE(p3->getOwnedParameteredElement() != 0);
    ASSERT_EQ(p3->getOwnedParameteredElement()->getElementType(), ElementType::LITERAL_INT);
    LiteralInt* li = dynamic_cast<LiteralInt*>(p3->getOwnedParameteredElement());
    ASSERT_EQ(li->getValue(), 1);
}

TEST_F(TemplateableElementParserTest, referencedParameterTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/operationW_referencedParameter.yml"));
    ASSERT_EQ(el->getElementType() , ElementType::CLASS);
    Class& c = dynamic_cast<Class&>(*el);
    ASSERT_EQ(c.getOperations().size(), 2);
    Operation& o1 = *c.getOperations().front();
    ASSERT_TRUE(o1.getOwnedTemplateSignature() != 0);
    TemplateSignature& s1 = *o1.getOwnedTemplateSignature();
    ASSERT_EQ(s1.getOwnedParameter().size(), 1);
    TemplateParameter& p = *s1.getOwnedParameter().front();
    ASSERT_EQ(p.getID(), ID::fromString("fGtHlUWITxbIKyNFeKCXI4d_3EAc"));
    Operation& o2 = *c.getOperations().back();
    ASSERT_TRUE(o2.getOwnedTemplateSignature() != 0);
    TemplateSignature& s2 = *o2.getOwnedTemplateSignature();
    ASSERT_EQ(s2.getParameter().size(), 1);
    ASSERT_EQ(s2.getParameter().front()->getID(), p.getID());
}

TEST_F(TemplateableElementParserTest, referenceParameteredElementTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/operationW_referenceParameteredElement.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class& c = dynamic_cast<Class&>(*el);
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& prop = *c.getOwnedAttributes().front();
    ASSERT_EQ(prop.getID(), ID::fromString("B2dyPF44MATTZ02XsQwgcbeBsq&d"));
    ASSERT_EQ(c.getOperations().size(), 1);
    Operation& op = *c.getOperations().front();
    ASSERT_TRUE(op.getOwnedTemplateSignature() != 0);
    TemplateSignature& s = *op.getOwnedTemplateSignature();
    ASSERT_EQ(s.getOwnedParameter().size(), 1);
    TemplateParameter& p = *s.getOwnedParameter().front();
    ASSERT_TRUE(p.getParameteredElement() != 0);
    ASSERT_EQ(p.getParameteredElement()->getID(), prop.getID());
}

TEST_F(TemplateableElementParserTest, basicTemplateBindingTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/templateBinding.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(*pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature() != 0);
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(*pckg.getPackagedElements().back());
    ASSERT_TRUE(c2.getTemplateBinding() != 0);
    TemplateBinding& b = *c2.getTemplateBinding();
    ASSERT_TRUE(b.getSignature() != 0);
    ASSERT_EQ(b.getSignature()->getID(), s.getID());
}

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_formalTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_Formal.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(*pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature() != 0);
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(s.getOwnedParameter().size(), 1);
    TemplateParameter& t = *s.getOwnedParameter().front();
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(*pckg.getPackagedElements().back());
    ASSERT_TRUE(c2.getTemplateBinding() != 0);
    TemplateBinding& b = *c2.getTemplateBinding();
    ASSERT_TRUE(b.getSignature() != 0);
    ASSERT_EQ(b.getSignature()->getID(), s.getID());
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    TemplateParameterSubstitution& ps = *b.getParameterSubstitution().front();
    ASSERT_TRUE(ps.getFormal() != 0);
    ASSERT_EQ(ps.getFormal()->getID(), t.getID());
}

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_OwnedActualTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_OwnedActual.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(*pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature() != 0);
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(*pckg.getPackagedElements().back());
    ASSERT_TRUE(c2.getTemplateBinding() != 0);
    TemplateBinding& b = *c2.getTemplateBinding();
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    TemplateParameterSubstitution& p = *b.getParameterSubstitution().front();
    ASSERT_TRUE(p.getOwnedActual() != 0);
    ASSERT_EQ(p.getOwnedActual()->getElementType(), ElementType::PRIMITIVE_TYPE);
    PrimitiveType& t = dynamic_cast<PrimitiveType&>(*p.getOwnedActual());
}
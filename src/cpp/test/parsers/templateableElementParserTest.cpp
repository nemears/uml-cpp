#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"
#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateSignature.h"
#include "uml/templateParameter.h"

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

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_Actual) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_backwardsActual.yml"));
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
    ASSERT_TRUE(p.getActual() != 0);
    ASSERT_EQ(p.getActual()->getElementType(), ElementType::PRIMITIVE_TYPE);
    ASSERT_EQ(p.getActual()->getID(), ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK"));
}

TEST_F(TemplateableElementParserTest, emitBigTemplateExampleTest) {
    srand (time(NULL));
    UmlManager m;
    Package& pckg = m.create<Package>();
    Class& c1 = m.create<Class>();
    TemplateSignature& sig = m.create<TemplateSignature>();
    TemplateParameter& p1 = m.create<TemplateParameter>();
    TemplateParameter& p2 = m.create<TemplateParameter>();
    PrimitiveType& d1 = m.create<PrimitiveType>();
    PrimitiveType& d2 = m.create<PrimitiveType>();
    Class& c2 = m.create<Class>();
    TemplateBinding& b = m.create<TemplateBinding>();
    TemplateParameterSubstitution& ps1 = m.create<TemplateParameterSubstitution>();
    TemplateParameterSubstitution& ps2 = m.create<TemplateParameterSubstitution>();
    PrimitiveType& st1 = m.create<PrimitiveType>();
    PrimitiveType& st2 = m.create<PrimitiveType>();
    pckg.setID("b4EasFCBjochdruOQfxBubQw3VlD");
    c1.setID("NYok8HRGpv_rOfAmfrRB94uwOZrb");
    c2.setID("fMWs7G1YTFU1VQEAgNcZqt4lp6dB");
    sig.setID("nOh5namt9s4oOvimAXQpR8nJHfTF");
    p1.setID("OLULeTlF1Rzf4U5IpNQVW1nYd29c");
    p2.setID("Km4WF5rf3ohUeLTr99POiW7VMb_4");
    d1.setID("GZaiGksTjm4GeM2GdJ5BXuajWnGU");
    d2.setID("a2arTP9Z2LteDWsjTS0ziALCWlXU");
    b.setID("e_ob7tgbN16Plhj_sTAOVD5ijLrL");
    ps1.setID("7bYUY3yFUBrfPmzKKrV2NJmXuECA");
    ps2.setID("puJaUTZsLPdGJkJSJtdX51MIA2ch");
    st1.setID("8&K_0aLhvQDM12ZeYg9nPiSrexHo");
    st2.setID("4gA4RgL9vKTRYd61D99y1d_Yggj6");
    c1.setOwnedTemplateSignature(&sig);
    sig.getOwnedParameter().add(p1);
    sig.getOwnedParameter().add(p2);
    p1.setOwnedDefault(&d1);
    p2.setDefault(&d2);
    c2.setTemplateBinding(&b);
    b.setSignature(&sig);
    b.getParameterSubstitution().add(ps1);
    b.getParameterSubstitution().add(ps2);
    ps1.setFormal(&p1);
    ps2.setFormal(&p2);
    ps1.setOwnedActual(&st1);
    ps2.setActual(&st2);
    pckg.getPackagedElements().add(d2);
    pckg.getPackagedElements().add(st2);
    pckg.getPackagedElements().add(c1);
    pckg.getPackagedElements().add(c2);

    string expectedEmit = R""""(package:
  id: b4EasFCBjochdruOQfxBubQw3VlD
  packagedElements:
    - primitiveType:
        id: a2arTP9Z2LteDWsjTS0ziALCWlXU
    - primitiveType:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6
    - class:
        id: NYok8HRGpv_rOfAmfrRB94uwOZrb
        templateSignature:
          id: nOh5namt9s4oOvimAXQpR8nJHfTF
          ownedParameters:
            - templateParameter:
                id: OLULeTlF1Rzf4U5IpNQVW1nYd29c
                ownedDefault:
                  primitiveType:
                    id: GZaiGksTjm4GeM2GdJ5BXuajWnGU
            - templateParameter:
                id: Km4WF5rf3ohUeLTr99POiW7VMb_4
                default: a2arTP9Z2LteDWsjTS0ziALCWlXU
    - class:
        id: fMWs7G1YTFU1VQEAgNcZqt4lp6dB
        templateBinding:
          id: e_ob7tgbN16Plhj_sTAOVD5ijLrL
          signature: nOh5namt9s4oOvimAXQpR8nJHfTF
          parameterSubstitution:
            - templateParameterSubstitution:
                id: 7bYUY3yFUBrfPmzKKrV2NJmXuECA
                formal: OLULeTlF1Rzf4U5IpNQVW1nYd29c
                ownedActual:
                  primitiveType:
                    id: 8&K_0aLhvQDM12ZeYg9nPiSrexHo
            - templateParameterSubstitution:
                id: puJaUTZsLPdGJkJSJtdX51MIA2ch
                formal: Km4WF5rf3ohUeLTr99POiW7VMb_4
                actual: 4gA4RgL9vKTRYd61D99y1d_Yggj6)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
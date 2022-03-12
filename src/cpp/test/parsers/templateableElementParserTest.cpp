#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/classW_emptySignature.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature());
    TemplateSignaturePtr s = c->getOwnedTemplateSignature();
    ASSERT_EQ(s->getID(), ID::fromString("b4EasFCBjochdruOQfxBubQw3VlD"));
}

TEST_F(TemplateableElementParserTest, singleEmptyTemplateParameterTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/classW_SingleTemplateParameter.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class * c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature());
    TemplateSignature* s = c->getOwnedTemplateSignature().ptr();
    ASSERT_EQ(s->getOwnedParameters().size(), 1);
    TemplateParameter* p = &s->getOwnedParameters().front();
    ASSERT_EQ(p->getID(), ID::fromString("t9FFOy3xNADeUDNvWJOc&7USIfsf"));
}

TEST_F(TemplateableElementParserTest, multipleParametersTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/clasW_multipleParameters.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(el);
    ASSERT_TRUE(c->getOwnedTemplateSignature());
    TemplateSignature* s = c->getOwnedTemplateSignature().ptr();
    ASSERT_EQ(s->getOwnedParameters().size(), 3);
    TemplateParameter* p1 = &s->getOwnedParameters().front();
    ASSERT_TRUE(p1->getOwnedParameteredElement());
    ASSERT_EQ(p1->getOwnedParameteredElement()->getElementType(), ElementType::PRIMITIVE_TYPE);
    PrimitiveType* p = dynamic_cast<PrimitiveType*>(p1->getOwnedParameteredElement().ptr());
    ASSERT_EQ(p->getName(), "long");
    TemplateParameter* p2 = &s->getOwnedParameters().get(1);
    ASSERT_EQ(p2->getOwnedParameteredElement()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(p2->getOwnedParameteredElement().ptr());
    ASSERT_EQ(i->getName(), "test");
    TemplateParameter* p3 = &s->getOwnedParameters().back();
    ASSERT_TRUE(p3->getOwnedParameteredElement());
    ASSERT_EQ(p3->getOwnedParameteredElement()->getElementType(), ElementType::LITERAL_INT);
    LiteralInt* li = dynamic_cast<LiteralInt*>(p3->getOwnedParameteredElement().ptr());
    ASSERT_EQ(li->getValue(), 1);
}

TEST_F(TemplateableElementParserTest, referencedParameterTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/operationW_referencedParameter.yml").ptr());
    ASSERT_EQ(el->getElementType() , ElementType::CLASS);
    Class& c = dynamic_cast<Class&>(*el);
    ASSERT_EQ(c.getOwnedOperations().size(), 2);
    Operation& o1 = c.getOwnedOperations().front();
    ASSERT_TRUE(o1.getOwnedTemplateSignature());
    TemplateSignature& s1 = *o1.getOwnedTemplateSignature();
    ASSERT_EQ(s1.getOwnedParameters().size(), 1);
    TemplateParameter& p = s1.getOwnedParameters().front();
    ASSERT_EQ(p.getID(), ID::fromString("fGtHlUWITxbIKyNFeKCXI4d_3EAc"));
    Operation& o2 = c.getOwnedOperations().back();
    ASSERT_TRUE(o2.getOwnedTemplateSignature());
    TemplateSignature& s2 = *o2.getOwnedTemplateSignature();
    ASSERT_EQ(s2.getParameters().size(), 1);
    ASSERT_EQ(s2.getParameters().front().getID(), p.getID());
}

TEST_F(TemplateableElementParserTest, referenceParameteredElementTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/operationW_referenceParameteredElement.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class& c = dynamic_cast<Class&>(*el);
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& prop = c.getOwnedAttributes().front();
    ASSERT_EQ(prop.getID(), ID::fromString("B2dyPF44MATTZ02XsQwgcbeBsq&d"));
    ASSERT_EQ(c.getOwnedOperations().size(), 1);
    Operation& op = c.getOwnedOperations().front();
    ASSERT_TRUE(op.getOwnedTemplateSignature());
    TemplateSignature& s = *op.getOwnedTemplateSignature();
    ASSERT_EQ(s.getOwnedParameters().size(), 1);
    TemplateParameter& p = s.getOwnedParameters().front();
    ASSERT_TRUE(p.getParameteredElement());
    ASSERT_EQ(p.getParameteredElement()->getID(), prop.getID());
}

TEST_F(TemplateableElementParserTest, basicTemplateBindingTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/templateBinding.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature());
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(pckg.getPackagedElements().back());
    ASSERT_EQ(c2.getTemplateBindings().size(), 1);
    TemplateBinding& b = c2.getTemplateBindings().front();
    ASSERT_TRUE(b.getSignature());
    ASSERT_EQ(b.getSignature()->getID(), s.getID());
}

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_formalTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_Formal.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature());
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(s.getOwnedParameters().size(), 1);
    TemplateParameter& t = s.getOwnedParameters().front();
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(pckg.getPackagedElements().back());
    ASSERT_EQ(c2.getTemplateBindings().size(), 1);
    TemplateBinding& b = c2.getTemplateBindings().front();
    ASSERT_TRUE(b.getSignature());
    ASSERT_EQ(b.getSignature()->getID(), s.getID());
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    TemplateParameterSubstitution& ps = b.getParameterSubstitution().front();
    ASSERT_TRUE(ps.getFormal());
    ASSERT_EQ(ps.getFormal()->getID(), t.getID());
}

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_OwnedActualTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_OwnedActual.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature());
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(pckg.getPackagedElements().back());
    ASSERT_EQ(c2.getTemplateBindings().size(), 1);
    TemplateBinding& b = c2.getTemplateBindings().front();
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    TemplateParameterSubstitution& p = b.getParameterSubstitution().front();
    ASSERT_TRUE(p.getOwnedActual());
    ASSERT_EQ(p.getOwnedActual()->getElementType(), ElementType::PRIMITIVE_TYPE);
    PrimitiveType& t = dynamic_cast<PrimitiveType&>(*p.getOwnedActual());
}

TEST_F(TemplateableElementParserTest, parameterSubstitutionW_Actual) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "templateableElementTests/parameterSubstitutionW_backwardsActual.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& c1 = dynamic_cast<Class&>(pckg.getPackagedElements().front());
    ASSERT_TRUE(c1.getOwnedTemplateSignature());
    TemplateSignature& s = *c1.getOwnedTemplateSignature();
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    Class& c2 = dynamic_cast<Class&>(pckg.getPackagedElements().back());
    ASSERT_EQ(c2.getTemplateBindings().size(), 1);
    TemplateBinding& b = c2.getTemplateBindings().front();
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    TemplateParameterSubstitution& p = b.getParameterSubstitution().front();
    ASSERT_TRUE(p.getActual());
    ASSERT_EQ(p.getActual()->getElementType(), ElementType::PRIMITIVE_TYPE);
    ASSERT_EQ(p.getActual()->getID(), ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK"));
}

TEST_F(TemplateableElementParserTest, emitBigTemplateExampleTest) {
    srand (time(NULL));
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Class& c1 = *m.create<Class>();
    TemplateSignature& sig = *m.create<TemplateSignature>();
    TemplateParameter& p1 = *m.create<TemplateParameter>();
    TemplateParameter& p2 = *m.create<TemplateParameter>();
    PrimitiveType& d1 = *m.create<PrimitiveType>();
    PrimitiveType& d2 = *m.create<PrimitiveType>();
    Class& c2 = *m.create<Class>();
    TemplateBinding& b = *m.create<TemplateBinding>();
    TemplateParameterSubstitution& ps1 = *m.create<TemplateParameterSubstitution>();
    TemplateParameterSubstitution& ps2 = *m.create<TemplateParameterSubstitution>();
    PrimitiveType& st1 = *m.create<PrimitiveType>();
    PrimitiveType& st2 = *m.create<PrimitiveType>();
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
    sig.getOwnedParameters().add(p1);
    sig.getOwnedParameters().add(p2);
    p1.setOwnedDefault(&d1);
    p2.setDefault(&d2);
    c2.getTemplateBindings().add(b);
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
        templateParameter: Km4WF5rf3ohUeLTr99POiW7VMb_4
    - primitiveType:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6
    - class:
        id: NYok8HRGpv_rOfAmfrRB94uwOZrb
        templateSignature:
          templateSignature:
            id: nOh5namt9s4oOvimAXQpR8nJHfTF
            ownedParameters:
              - templateParameter:
                  id: OLULeTlF1Rzf4U5IpNQVW1nYd29c
                  ownedDefault:
                    primitiveType:
                      id: GZaiGksTjm4GeM2GdJ5BXuajWnGU
                      templateParameter: OLULeTlF1Rzf4U5IpNQVW1nYd29c
              - templateParameter:
                  id: Km4WF5rf3ohUeLTr99POiW7VMb_4
                  default: a2arTP9Z2LteDWsjTS0ziALCWlXU
    - class:
        id: fMWs7G1YTFU1VQEAgNcZqt4lp6dB
        templateBindings:
          - templateBinding:
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
    string expectedEmit2 = R""""(package:
  id: b4EasFCBjochdruOQfxBubQw3VlD
  packagedElements:
    - primitiveType:
        id: a2arTP9Z2LteDWsjTS0ziALCWlXU
        templateParameter: Km4WF5rf3ohUeLTr99POiW7VMb_4
    - class:
        id: NYok8HRGpv_rOfAmfrRB94uwOZrb
        templateSignature:
          templateSignature:
            id: nOh5namt9s4oOvimAXQpR8nJHfTF
            ownedParameters:
              - templateParameter:
                  id: OLULeTlF1Rzf4U5IpNQVW1nYd29c
                  ownedDefault:
                    primitiveType:
                      id: GZaiGksTjm4GeM2GdJ5BXuajWnGU
                      templateParameter: OLULeTlF1Rzf4U5IpNQVW1nYd29c
              - templateParameter:
                  id: Km4WF5rf3ohUeLTr99POiW7VMb_4
                  default: a2arTP9Z2LteDWsjTS0ziALCWlXU
    - primitiveType:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6
    - class:
        id: fMWs7G1YTFU1VQEAgNcZqt4lp6dB
        templateBindings:
          - templateBinding:
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
    ASSERT_TRUE(expectedEmit == generatedEmit || expectedEmit2 == generatedEmit);
}

TEST_F(TemplateableElementParserTest, mountClassWithTemplateSignature) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Class& clazz = *m.create<Class>();
    Class& otherClazz = *m.create<Class>();
    TemplateSignature& signature = *m.create<TemplateSignature>();
    TemplateSignature& otherSignature = *m.create<TemplateSignature>();
    TemplateParameter& ownedParameter = *m.create<TemplateParameter>();
    PrimitiveType& ownedParameterableElement = *m.create<PrimitiveType>();
    TemplateParameter& otherParameter = *m.create<TemplateParameter>();
    Property& parameteredElement = *m.create<Property>();
    PrimitiveType& ownedDefault = *m.create<PrimitiveType>();
    Property& defaultParam = *m.create<Property>();
    Class& boundEl = *m.create<Class>();
    Class& otherBoundEl = *m.create<Class>();
    TemplateBinding& binding = *m.create<TemplateBinding>();
    TemplateBinding& otherBinding = *m.create<TemplateBinding>();
    PrimitiveType& actual = *m.create<PrimitiveType>();
    PrimitiveType& ownedActual = *m.create<PrimitiveType>();
    TemplateParameterSubstitution& subW_Actual = *m.create<TemplateParameterSubstitution>();
    TemplateParameterSubstitution& subW_OwnedActual = *m.create<TemplateParameterSubstitution>();
    ownedParameter.setOwnedParameteredElement(ownedParameterableElement);
    ownedParameter.setOwnedDefault(ownedDefault);
    otherParameter.setParameteredElement(parameteredElement);
    otherParameter.setDefault(defaultParam);
    signature.getOwnedParameters().add(ownedParameter);
    otherSignature.getOwnedParameters().add(otherParameter);
    clazz.setOwnedTemplateSignature(signature);
    otherClazz.setOwnedTemplateSignature(otherSignature);
    otherClazz.getOwnedAttributes().add(parameteredElement, defaultParam);
    binding.setSignature(signature);
    boundEl.getTemplateBindings().add(binding);
    otherBinding.setSignature(signature);
    otherBoundEl.getTemplateBindings().add(otherBinding);
    subW_Actual.setFormal(ownedParameter);
    subW_Actual.setActual(actual);
    subW_OwnedActual.setFormal(ownedParameter);
    subW_OwnedActual.setOwnedActual(ownedActual);
    binding.getParameterSubstitution().add(subW_OwnedActual);
    otherBinding.getParameterSubstitution().add(subW_Actual);
    root.getPackagedElements().add(clazz, otherClazz, boundEl, actual);

    // TODO more

    m.setRoot(root);
    m.mount(ymlPath + "templateableElementTests");

    ID clazzID = clazz.getID();
    m.release(clazz);
    ASSERT_FALSE(m.loaded(clazzID));
    Class& clazz2 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(clazz2.getOwnedTemplateSignature());
    ASSERT_EQ(*clazz2.getOwnedTemplateSignature(), signature);
    ASSERT_EQ(clazz2.getOwnedElements().size(), 1);
    ASSERT_EQ(*clazz2.getOwnedElements().begin(), signature);
    ASSERT_TRUE(signature.getTemplate());
    ASSERT_EQ(*signature.getTemplate(), clazz2);
    ASSERT_TRUE(signature.getOwner());
    ASSERT_EQ(*signature.getOwner(), clazz2);

    ID signatureID = signature.getID();
    m.release(signature, clazz2);
    ASSERT_FALSE(m.loaded(clazzID));
    ASSERT_FALSE(m.loaded(signatureID));
    TemplateSignature& signature2 = m.aquire(signatureID)->as<TemplateSignature>();
    ASSERT_TRUE(signature2.getTemplate());
    ASSERT_EQ(signature2.getTemplate().id(), clazzID);
    ASSERT_TRUE(signature2.getOwner());
    ASSERT_EQ(signature2.getOwner().id(), clazzID);
    Class& clazz3 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(clazz3.getOwnedTemplateSignature());
    ASSERT_EQ(*clazz3.getOwnedTemplateSignature(), signature2);
    ASSERT_EQ(clazz3.getOwnedElements().size(), 1);
    ASSERT_EQ(*clazz3.getOwnedElements().begin(), signature2);
    ASSERT_TRUE(signature2.getTemplate());
    ASSERT_EQ(*signature2.getTemplate(), clazz3);
    ASSERT_TRUE(signature2.getOwner());
    ASSERT_EQ(*signature2.getOwner(), clazz3);
    ASSERT_EQ(signature2.getOwnedParameters().size(), 1);
    ASSERT_EQ(signature2.getOwnedParameters().front(), ownedParameter);
    ASSERT_EQ(signature2.getOwnedElements().size(), 1);
    ASSERT_EQ(*signature2.getOwnedElements().begin(), ownedParameter);

    m.release(signature2, clazz3);
    ASSERT_FALSE(m.loaded(clazzID));
    ASSERT_FALSE(m.loaded(signatureID));
    Class& clazz4 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(clazz4.getOwnedTemplateSignature());
    ASSERT_EQ(clazz4.getOwnedTemplateSignature().id(), signatureID);
    ASSERT_TRUE(clazz4.getOwnedElements().count(signatureID));
    ASSERT_EQ(*clazz4.getOwnedElements().ids().begin(), signatureID);
    TemplateSignature& signature3 = m.aquire(signatureID)->as<TemplateSignature>();
    ASSERT_TRUE(clazz4.getOwnedTemplateSignature());
    ASSERT_EQ(*clazz4.getOwnedTemplateSignature(), signature3);
    ASSERT_EQ(clazz4.getOwnedElements().size(), 1);
    ASSERT_EQ(*clazz4.getOwnedElements().begin(), signature3);
    ASSERT_TRUE(signature3.getTemplate());
    ASSERT_EQ(*signature3.getTemplate(), clazz4);
    ASSERT_TRUE(signature3.getOwner());
    ASSERT_EQ(*signature3.getOwner(), clazz4);
    ASSERT_EQ(signature3.getOwnedParameters().size(), 1);
    ASSERT_EQ(signature3.getOwnedParameters().front(), ownedParameter);
    ASSERT_EQ(signature3.getOwnedElements().size(), 1);
    ASSERT_EQ(*signature3.getOwnedElements().begin(), ownedParameter);

    ID ownedParameterID = ownedParameter.getID();
    m.release(ownedParameter, signature3);
    ASSERT_FALSE(m.loaded(signatureID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    TemplateParameter& ownedParameter2 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter2.getSignature());
    ASSERT_EQ(ownedParameter2.getSignature().id(), signatureID);
    ASSERT_TRUE(ownedParameter2.getOwner());
    ASSERT_EQ(ownedParameter2.getOwner().id(), signatureID);
    TemplateSignature& signature4 = m.aquire(signatureID)->as<TemplateSignature>();
    ASSERT_EQ(*signature4.getOwner(), clazz4);
    ASSERT_EQ(signature4.getOwnedParameters().size(), 1);
    ASSERT_EQ(signature4.getOwnedParameters().front(), ownedParameter2);
    ASSERT_EQ(signature4.getOwnedElements().size(), 1);
    ASSERT_EQ(*signature4.getOwnedElements().begin(), ownedParameter2);
    ASSERT_EQ(*ownedParameter2.getOwner(), signature4);
    ASSERT_EQ(*ownedParameter2.getSignature(), signature4);
    ASSERT_TRUE(ownedParameter2.getOwnedParameteredElement());
    ASSERT_EQ(*ownedParameter2.getOwnedParameteredElement(), ownedParameterableElement);

    m.release(ownedParameter2, signature4);
    ASSERT_FALSE(m.loaded(signatureID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    TemplateSignature& signature5 = m.aquire(signatureID)->as<TemplateSignature>();
    ASSERT_EQ(signature5.getOwnedParameters().size(), 1);
    ASSERT_EQ(*signature5.getOwnedParameters().ids().begin(), ownedParameterID);
    ASSERT_EQ(signature5.getOwnedElements().size(), 1);
    ASSERT_EQ(*signature5.getOwnedElements().ids().begin(), ownedParameterID);
    TemplateParameter& ownedParameter3 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter3.getSignature());
    ASSERT_EQ(*ownedParameter3.getSignature(), signature5);
    ASSERT_TRUE(ownedParameter3.getOwner());
    ASSERT_EQ(*ownedParameter3.getOwner(), signature5);
    ASSERT_EQ(signature5.getOwnedParameters().front(), ownedParameter3);
    ASSERT_EQ(*signature5.getOwnedElements().begin(), ownedParameter3);
    ASSERT_TRUE(ownedParameter3.getOwnedParameteredElement());
    ASSERT_EQ(*ownedParameter3.getOwnedParameteredElement(), ownedParameterableElement);

    ID ownedParameterableElementID = ownedParameterableElement.getID();
    m.release(ownedParameterableElement, ownedParameter3);
    ASSERT_FALSE(m.loaded(ownedParameterableElementID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    PrimitiveType& ownedParameterableElement2 = m.aquire(ownedParameterableElementID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedParameterableElement2.getOwningTemplateParameter());
    ASSERT_EQ(ownedParameterableElement2.getOwningTemplateParameter().id(), ownedParameterID);
    ASSERT_TRUE(ownedParameterableElement2.getOwner());
    ASSERT_EQ(ownedParameterableElement2.getOwner().id(), ownedParameterID);
    TemplateParameter& ownedParameter4 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter4.getOwnedParameteredElement());
    ASSERT_EQ(*ownedParameter4.getOwnedParameteredElement(), ownedParameterableElement2);
    ASSERT_EQ(ownedParameter4.getOwnedElements().size(), 2);
    ASSERT_TRUE(ownedParameter4.getOwnedElements().contains(ownedParameterableElement2));
    ASSERT_TRUE(ownedParameter4.getParameteredElement());
    ASSERT_EQ(*ownedParameter4.getParameteredElement(), ownedParameterableElement2);
    ASSERT_EQ(*ownedParameterableElement2.getOwningTemplateParameter(), ownedParameter4);
    ASSERT_EQ(*ownedParameterableElement2.getOwner(), ownedParameter4);
    ASSERT_TRUE(ownedParameter4.getOwnedDefault());
    ASSERT_EQ(*ownedParameter4.getOwnedDefault(), ownedDefault);
    ASSERT_TRUE(ownedParameter4.getDefault());
    ASSERT_EQ(*ownedParameter4.getDefault(), ownedDefault);

    m.release(ownedParameterableElement2, ownedParameter4);
    ASSERT_FALSE(m.loaded(ownedParameterableElementID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    TemplateParameter& ownedParameter5 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter5.getOwnedParameteredElement());
    ASSERT_EQ(ownedParameter5.getOwnedParameteredElement().id(), ownedParameterableElementID);
    ASSERT_EQ(ownedParameter5.getOwnedElements().size(), 2);
    //ASSERT_EQ(*(ownedParameter5.getOwnedElements().ids().begin()++), ownedParameterableElementID);
    ASSERT_TRUE(ownedParameter5.getParameteredElement());
    ASSERT_EQ(ownedParameter5.getParameteredElement().id(), ownedParameterableElementID);
    ASSERT_TRUE(ownedParameter5.getOwnedDefault());
    ASSERT_EQ(*ownedParameter5.getOwnedDefault(), ownedDefault);
    ASSERT_TRUE(ownedParameter5.getDefault());
    ASSERT_EQ(*ownedParameter5.getDefault(), ownedDefault);
    PrimitiveType& ownedParameterableElement3 = m.aquire(ownedParameterableElementID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedParameter5.getOwnedParameteredElement());
    ASSERT_EQ(*ownedParameter5.getOwnedParameteredElement(), ownedParameterableElement3);
    ASSERT_EQ(ownedParameter5.getOwnedElements().size(), 2);
    ASSERT_TRUE(ownedParameter5.getOwnedElements().contains(ownedParameterableElement3));
    ASSERT_TRUE(ownedParameter5.getParameteredElement());
    ASSERT_EQ(*ownedParameter5.getParameteredElement(), ownedParameterableElement3);
    ASSERT_TRUE(ownedParameterableElement3.getOwningTemplateParameter());
    ASSERT_EQ(*ownedParameterableElement3.getOwningTemplateParameter(), ownedParameter5);
    ASSERT_TRUE(ownedParameterableElement3.getOwner());
    ASSERT_EQ(*ownedParameterableElement3.getOwner(), ownedParameter5);

    ID ownedDefaultID = ownedDefault.getID();
    m.release(ownedDefault, ownedParameter5);
    ASSERT_FALSE(m.loaded(ownedDefaultID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    TemplateParameter& ownedParameter6 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter6.getOwnedDefault());
    ASSERT_EQ(ownedParameter6.getOwnedDefault().id(), ownedDefaultID);
    ASSERT_TRUE(ownedParameter6.getDefault());
    ASSERT_EQ(ownedParameter6.getDefault().id(), ownedDefaultID);
    ASSERT_EQ(ownedParameter6.getOwnedElements().size(), 2);
    ASSERT_TRUE(ownedParameter6.getOwnedElements().contains(ownedDefaultID));
    PrimitiveType& ownedDefault2 = m.aquire(ownedDefaultID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedDefault2.getTemplateParameter());
    ASSERT_EQ(*ownedDefault2.getTemplateParameter(), ownedParameter6);
    ASSERT_TRUE(ownedDefault2.getOwner());
    ASSERT_EQ(*ownedDefault2.getOwner(), ownedParameter6);
    ASSERT_EQ(*ownedParameter6.getOwnedDefault(), ownedDefault2);
    ASSERT_EQ(*ownedParameter6.getDefault(), ownedDefault2);
    ASSERT_TRUE(ownedParameter6.getOwnedElements().contains(ownedDefault2));

    m.release(ownedDefault2, ownedParameter6);
    ASSERT_FALSE(m.loaded(ownedDefaultID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    PrimitiveType& ownedDefault3 = m.aquire(ownedDefaultID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedDefault3.getTemplateParameter());
    ASSERT_EQ(ownedDefault3.getTemplateParameter().id(), ownedParameterID);
    ASSERT_TRUE(ownedDefault3.getOwner());
    ASSERT_EQ(ownedDefault3.getOwner().id(), ownedParameterID);
    TemplateParameter& ownedParameter7 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(ownedParameter7.getOwnedDefault());
    ASSERT_EQ(*ownedParameter7.getOwnedDefault(), ownedDefault3);
    ASSERT_TRUE(ownedParameter7.getDefault());
    ASSERT_EQ(*ownedParameter7.getDefault(), ownedDefault3);
    ASSERT_EQ(ownedParameter7.getOwnedElements().size(), 2);
    ASSERT_TRUE(ownedParameter7.getOwnedElements().contains(ownedDefault3));
    ASSERT_EQ(*ownedDefault3.getTemplateParameter(), ownedParameter7);
    ASSERT_EQ(*ownedDefault3.getOwner(), ownedParameter7);

    ID otherParameterID = otherParameter.getID();
    ID parameteredElementID = parameteredElement.getID();
    m.release(otherParameter, parameteredElement);
    ASSERT_FALSE(m.loaded(otherParameterID));
    ASSERT_FALSE(m.loaded(parameteredElementID));
    TemplateParameter& otherParameter2 = m.aquire(otherParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(otherParameter2.getParameteredElement());
    ASSERT_EQ(otherParameter2.getParameteredElement().id(), parameteredElementID);
    ASSERT_TRUE(otherParameter2.getDefault());
    ASSERT_EQ(*otherParameter2.getDefault(), defaultParam);
    Property& parameteredElement2 = m.aquire(parameteredElementID)->as<Property>();
    ASSERT_TRUE(parameteredElement2.getTemplateParameter());
    ASSERT_EQ(*parameteredElement2.getTemplateParameter(), otherParameter2);
    ASSERT_EQ(*otherParameter2.getParameteredElement(), parameteredElement2);

    m.release(otherParameter2, parameteredElement2);
    ASSERT_FALSE(m.loaded(otherParameterID));
    ASSERT_FALSE(m.loaded(parameteredElementID));
    Property& parameteredElement3 = m.aquire(parameteredElementID)->as<Property>();
    ASSERT_TRUE(parameteredElement3.getTemplateParameter());
    ASSERT_EQ(parameteredElement3.getTemplateParameter().id(), otherParameterID);
    TemplateParameter& otherParameter3 = m.aquire(otherParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(otherParameter3.getParameteredElement());
    ASSERT_EQ(*otherParameter3.getParameteredElement(), parameteredElement3);
    ASSERT_EQ(*parameteredElement3.getTemplateParameter(), otherParameter3);
    ASSERT_TRUE(otherParameter3.getDefault());
    ASSERT_EQ(*otherParameter3.getDefault(), defaultParam);

    ID defaultParamID = defaultParam.getID();
    m.release(defaultParam, otherParameter3);
    ASSERT_FALSE(m.loaded(defaultParamID));
    ASSERT_FALSE(m.loaded(otherParameterID));
    Property& defaultParam2 = m.aquire(defaultParamID)->as<Property>();
    ASSERT_TRUE(defaultParam2.getTemplateParameter());
    ASSERT_EQ(defaultParam2.getTemplateParameter().id(), otherParameterID);
    TemplateParameter& otherParameter4 = m.aquire(otherParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(otherParameter4.getDefault());
    ASSERT_EQ(*otherParameter4.getDefault(), defaultParam2);
    ASSERT_EQ(*defaultParam2.getTemplateParameter(), otherParameter4);

    m.release(defaultParam2, otherParameter4);
    ASSERT_FALSE(m.loaded(defaultParamID));
    ASSERT_FALSE(m.loaded(otherParameterID));
    TemplateParameter& otherParameter5 = m.aquire(otherParameterID)->as<TemplateParameter>();
    ASSERT_TRUE(otherParameter5.getDefault());
    ASSERT_EQ(otherParameter5.getDefault().id(), defaultParamID);
    Property& defaultParam3 = m.aquire(defaultParamID)->as<Property>();
    ASSERT_TRUE(defaultParam3.getTemplateParameter());
    ASSERT_EQ(*defaultParam3.getTemplateParameter(), otherParameter5);
    ASSERT_EQ(*otherParameter5.getDefault(), defaultParam3);

    ID bindingID = binding.getID();
    m.release(binding);
    ASSERT_FALSE(m.loaded(bindingID));
    TemplateBinding& binding2 = m.aquire(bindingID)->as<TemplateBinding>();
    ASSERT_TRUE(binding2.getBoundElement());
    ASSERT_EQ(*binding2.getBoundElement(), boundEl);
    ASSERT_TRUE(binding2.getOwner());
    ASSERT_EQ(*binding2.getOwner(), boundEl);
    ASSERT_EQ(binding2.getSources().size(), 1);
    ASSERT_EQ(binding2.getSources().front(), boundEl);
    ASSERT_EQ(binding2.getRelatedElements().size(), 2);
    ASSERT_TRUE(binding2.getSignature());
    ASSERT_EQ(*binding2.getSignature(), signature5);
    ASSERT_EQ(binding2.getTargets().size(), 1);
    ASSERT_EQ(binding2.getTargets().front(), signature5);
    ASSERT_TRUE(binding2.getRelatedElements().contains(signature5));
    ASSERT_TRUE(binding2.getRelatedElements().contains(boundEl));


    ID boundElID = boundEl.getID();
    m.release(boundEl);
    ASSERT_FALSE(m.loaded(boundElID));
    Class& boundEl2 = m.aquire(boundElID)->as<Class>();
    ASSERT_EQ(boundEl2.getTemplateBindings().size(), 1);
    ASSERT_EQ(boundEl2.getTemplateBindings().front(), binding2);
    ASSERT_EQ(boundEl2.getOwnedElements().size(), 1);
    ASSERT_EQ(boundEl2.getOwnedElements().front(), binding2);

    m.release(boundEl2, binding2);
    ASSERT_FALSE(m.loaded(bindingID));
    ASSERT_FALSE(m.loaded(boundElID));
    TemplateBinding& binding3 = m.aquire(bindingID)->as<TemplateBinding>();
    ASSERT_TRUE(binding3.getBoundElement());
    ASSERT_EQ(binding3.getBoundElement().id(), boundElID);
    ASSERT_EQ(binding3.getSources().size(), 1);
    ASSERT_TRUE(binding3.getSources().contains(boundElID));
    ASSERT_EQ(binding3.getRelatedElements().size(), 2);
    ASSERT_TRUE(binding3.getRelatedElements().contains(boundElID));
    ASSERT_TRUE(binding3.getOwner());
    ASSERT_EQ(binding3.getOwner().id(), boundElID);
    Class& boundEl3 = m.aquire(boundElID)->as<Class>();
    ASSERT_EQ(boundEl3.getTemplateBindings().size(), 1);
    ASSERT_EQ(boundEl3.getTemplateBindings().front(), binding3);
    ASSERT_EQ(boundEl3.getOwnedElements().size(), 1);
    ASSERT_EQ(boundEl3.getOwnedElements().front(), binding3);
    ASSERT_EQ(*binding3.getBoundElement(), boundEl3);
    ASSERT_EQ(*binding3.getOwner(), boundEl3);
    ASSERT_EQ(binding3.getSources().front(), boundEl3);
    ASSERT_TRUE(binding3.getRelatedElements().contains(boundEl3));

    m.release(boundEl3, binding3);
    ASSERT_FALSE(m.loaded(bindingID));
    ASSERT_FALSE(m.loaded(boundElID));
    Class& boundEl4 = m.aquire(boundElID)->as<Class>();
    ASSERT_EQ(boundEl4.getTemplateBindings().size(), 1);
    ASSERT_TRUE(boundEl4.getTemplateBindings().contains(bindingID));
    ASSERT_EQ(boundEl4.getOwnedElements().size(), 1);
    ASSERT_TRUE(boundEl4.getOwnedElements().contains(bindingID));
    TemplateBinding& binding4 = m.aquire(bindingID)->as<TemplateBinding>();
    ASSERT_TRUE(binding4.getBoundElement());
    ASSERT_EQ(*binding4.getBoundElement(), boundEl4);
    ASSERT_TRUE(binding4.getOwner());
    ASSERT_EQ(*binding4.getOwner(), boundEl4);
    ASSERT_EQ(binding4.getSources().size(), 1);
    ASSERT_EQ(binding4.getSources().front(), boundEl4);
    ASSERT_EQ(binding4.getRelatedElements().size(), 2);
    ASSERT_TRUE(binding4.getRelatedElements().contains(boundEl4));

    ID subW_OwnedActualID = subW_OwnedActual.getID();
    m.release(subW_OwnedActual);
    ASSERT_FALSE(m.loaded(subW_OwnedActualID));
    TemplateParameterSubstitution& subW_OwnedActual2 = m.aquire(subW_OwnedActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_OwnedActual2.getTemplateBinding());
    ASSERT_EQ(*subW_OwnedActual2.getTemplateBinding(), binding4);
    ASSERT_TRUE(subW_OwnedActual2.getOwner());
    ASSERT_EQ(*subW_OwnedActual2.getOwner(), binding4);
    ASSERT_TRUE(subW_OwnedActual2.getFormal());
    ASSERT_EQ(*subW_OwnedActual2.getFormal(), ownedParameter7);
    ASSERT_TRUE(subW_OwnedActual2.getOwnedActual());
    ASSERT_EQ(*subW_OwnedActual2.getOwnedActual(), ownedActual);
    ASSERT_TRUE(subW_OwnedActual2.getActual());
    ASSERT_EQ(*subW_OwnedActual2.getActual(), ownedActual);
    ASSERT_EQ(subW_OwnedActual2.getOwnedElements().size(), 1);
    ASSERT_EQ(subW_OwnedActual2.getOwnedElements().front(), ownedActual);
    
    m.release(subW_OwnedActual2, binding4);
    ASSERT_FALSE(m.loaded(subW_OwnedActualID));
    ASSERT_FALSE(m.loaded(bindingID));
    TemplateBinding& binding5 = m.aquire(bindingID)->as<TemplateBinding>();
    ASSERT_EQ(binding5.getParameterSubstitution().size(), 1);
    ASSERT_TRUE(binding5.getParameterSubstitution().contains(subW_OwnedActualID));
    ASSERT_EQ(binding5.getOwnedElements().size(), 1);
    ASSERT_TRUE(binding5.getOwnedElements().contains(subW_OwnedActualID));
    TemplateParameterSubstitution& subW_OwnedActual3 = m.aquire(subW_OwnedActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_OwnedActual3.getTemplateBinding());
    ASSERT_EQ(*subW_OwnedActual3.getTemplateBinding(), binding5);
    ASSERT_TRUE(subW_OwnedActual3.getOwner());
    ASSERT_EQ(*subW_OwnedActual3.getOwner(), binding5);

    m.release(subW_OwnedActual3, ownedParameter7);
    ASSERT_FALSE(m.loaded(subW_OwnedActualID));
    ASSERT_FALSE(m.loaded(ownedParameterID));
    TemplateParameterSubstitution& subW_OwnedActual4 = m.aquire(subW_OwnedActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_OwnedActual4.getFormal());
    ASSERT_EQ(subW_OwnedActual4.getFormal().id(), ownedParameterID);
    TemplateParameter& ownedParameter8 = m.aquire(ownedParameterID)->as<TemplateParameter>();
    ASSERT_EQ(*subW_OwnedActual4.getFormal(), ownedParameter8);

    ID ownedActualID = ownedActual.getID();
    m.release(ownedActual, subW_OwnedActual4);
    ASSERT_FALSE(m.loaded(ownedActualID));
    ASSERT_FALSE(m.loaded(subW_OwnedActualID));
    PrimitiveType& ownedActual2 = m.aquire(ownedActualID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedActual2.getOwner());
    ASSERT_EQ(ownedActual2.getOwner().id(), subW_OwnedActualID);
    TemplateParameterSubstitution& subW_OwnedActual5 = m.aquire(subW_OwnedActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_OwnedActual5.getOwnedActual());
    ASSERT_EQ(*subW_OwnedActual5.getOwnedActual(), ownedActual2);
    ASSERT_TRUE(subW_OwnedActual5.getActual());
    ASSERT_EQ(*subW_OwnedActual5.getActual(), ownedActual2);
    ASSERT_EQ(subW_OwnedActual5.getOwnedElements().size(), 1);
    ASSERT_EQ(subW_OwnedActual5.getOwnedElements().front(), ownedActual2);
    ASSERT_EQ(*ownedActual2.getOwner(), subW_OwnedActual5);

    m.release(ownedActual2, subW_OwnedActual5);
    ASSERT_FALSE(m.loaded(ownedActualID));
    ASSERT_FALSE(m.loaded(subW_OwnedActualID));
    TemplateParameterSubstitution& subW_OwnedActual6 = m.aquire(subW_OwnedActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_OwnedActual6.getOwnedActual());
    ASSERT_EQ(subW_OwnedActual6.getOwnedActual().id(), ownedActualID);
    ASSERT_TRUE(subW_OwnedActual6.getActual());
    ASSERT_EQ(subW_OwnedActual6.getActual().id(), ownedActualID);
    ASSERT_EQ(subW_OwnedActual6.getOwnedElements().size(), 1);
    ASSERT_EQ(*subW_OwnedActual6.getOwnedElements().ids().begin(), ownedActualID);
    PrimitiveType& ownedActual3 = m.aquire(ownedActualID)->as<PrimitiveType>();
    ASSERT_TRUE(ownedActual3.getOwner());
    ASSERT_EQ(*ownedActual3.getOwner(), subW_OwnedActual6);
    ASSERT_EQ(*subW_OwnedActual6.getOwnedActual(), ownedActual3);
    ASSERT_EQ(*subW_OwnedActual6.getActual(), ownedActual3);
    ASSERT_EQ(subW_OwnedActual6.getOwnedElements().front(), ownedActual3);

    ID subW_ActualID = subW_Actual.getID();
    ID actualID = actual.getID();
    m.release(subW_Actual);
    ASSERT_FALSE(m.loaded(subW_ActualID));
    TemplateParameterSubstitution& subW_Actual2 = m.aquire(subW_ActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_Actual2.getActual());
    ASSERT_EQ(subW_Actual2.getActual().id(), actualID);
    ASSERT_EQ(*subW_Actual2.getActual(), actual);


    m.release(actual, subW_Actual2);
    ASSERT_FALSE(m.loaded(subW_ActualID));
    ASSERT_FALSE(m.loaded(actualID));
    TemplateParameterSubstitution& subW_Actual3 = m.aquire(subW_ActualID)->as<TemplateParameterSubstitution>();
    ASSERT_TRUE(subW_Actual3.getActual());
    ASSERT_EQ(subW_Actual3.getActual().id(), actualID);
    PrimitiveType& actual2 = m.aquire(actualID)->as<PrimitiveType>();
    ASSERT_EQ(*subW_Actual3.getActual(), actual2);

}
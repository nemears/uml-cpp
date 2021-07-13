#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/primitiveType.h"
#include "uml/feature.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/package.h"
#include "uml/generalization.h"

using namespace std;
using namespace UML;

class PrimitiveTypeParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(PrimitiveTypeParserTest, basicPrimitiveTypeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "primitiveTypeTests/basicPrimitiveType.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PRIMITIVE_TYPE);
    DataType d = *dynamic_cast<DataType*>(el);
    ASSERT_TRUE(d.getName().compare("int") == 0);
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    Property* p = d.getOwnedAttribute().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    Operation* o = d.getOwnedOperation().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(d.getFeatures().front() == p);
    ASSERT_TRUE(d.getFeatures().back() == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(d.getOwnedMembers().front() == p);
    ASSERT_TRUE(d.getOwnedMembers().back() == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(d.getMembers().front() == p);
    ASSERT_TRUE(d.getMembers().back() == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
    ASSERT_TRUE(d.getOwnedElements().front() == p);
    ASSERT_TRUE(d.getOwnedElements().back() == o);
}

TEST_F(PrimitiveTypeParserTest, emitPrimWGeneralAndAttribute) {
    Package pckg;
    PrimitiveType t;
    PrimitiveType g;
    Generalization gen;
    PrimitiveType s;
    Property p;
    pckg.setID("ScxedgF1Ej1JJubABfwVAK1X&_28");
    pckg.setName("owningPackage");
    t.setID("ufsWC1O42Rz36lNEixxF&gH6SKdj");
    t.setName("type");
    g.setID("bD3tGZ_XwovlDpB4NPqiaMl19DUV");
    g.setName("general");
    s.setID("3di2YkXSv7_2E8yUAdNElvDPCP4&");
    s.setName("specific");
    gen.setID("Fpn&_P3zERcsyDCu8kN837WxY_90");
    gen.setGeneral(&g);
    s.getGeneralizations().add(gen);
    p.setID("yShg7h43ZHm9I2Dqw2RoSXV&Y0PQ");
    p.setName("generalProp");
    p.setType(&t);
    g.getOwnedAttribute().add(p);
    pckg.getPackagedElements().add(t);
    pckg.getPackagedElements().add(g);
    pckg.getPackagedElements().add(s);
    string expectedEmit = R""""(package:
  id: ScxedgF1Ej1JJubABfwVAK1X&_28
  name: owningPackage
  packagedElements:
    - primitiveType:
        id: ufsWC1O42Rz36lNEixxF&gH6SKdj
        name: type
    - primitiveType:
        id: bD3tGZ_XwovlDpB4NPqiaMl19DUV
        name: general
        ownedAttribute:
          - property:
              id: yShg7h43ZHm9I2Dqw2RoSXV&Y0PQ
              name: generalProp
              type: ufsWC1O42Rz36lNEixxF&gH6SKdj
    - primitiveType:
        id: 3di2YkXSv7_2E8yUAdNElvDPCP4&
        name: specific
        generalizations:
          - generalization:
              id: Fpn&_P3zERcsyDCu8kN837WxY_90
              general: bD3tGZ_XwovlDpB4NPqiaMl19DUV)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
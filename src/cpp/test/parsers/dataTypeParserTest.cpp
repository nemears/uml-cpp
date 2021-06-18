#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/dataType.h"

using namespace std;
using namespace UML;

class DataTypeParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DataTypeParserTest, basicDataTypeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "dataTypeTests/basicDataType.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::DATA_TYPE);
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

TEST_F(DataTypeParserTest, emitDataTypeW_GeneralAndAttribute) {
    Package pckg;
    DataType t;
    DataType g;
    Generalization gen;
    DataType s;
    Property p;
    pckg.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    pckg.setName("owningPackage");
    t.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    t.setName("type");
    g.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    g.setName("general");
    s.setID("208d68d3-9fe3-4e70-ad48-a01b55e7dc04");
    s.setName("specific");
    gen.setID("ce8f718d-9de4-446d-9fa5-d6480396aced");
    gen.setGeneral(&g);
    s.getGeneralizations().add(gen);
    p.setID("7d51c6b6-8f53-4fcb-a85a-13f39b978e03");
    p.setName("generalProp");
    p.setType(&t);
    g.getOwnedAttribute().add(p);
    pckg.getPackagedElements().add(t);
    pckg.getPackagedElements().add(g);
    pckg.getPackagedElements().add(s);
    string expectedEmit = R""""(package:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: owningPackage
  packagedElements:
    - dataType:
        id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
        name: type
    - dataType:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        name: general
        ownedAttribute:
          - property:
              id: 7d51c6b6-8f53-4fcb-a85a-13f39b978e03
              name: generalProp
              type: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
    - dataType:
        id: 208d68d3-9fe3-4e70-ad48-a01b55e7dc04
        name: specific
        generalizations:
          - generalization:
              id: ce8f718d-9de4-446d-9fa5-d6480396aced
              general: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
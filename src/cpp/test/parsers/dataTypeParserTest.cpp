 #include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/dataType.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/package.h"
#include "uml/generalization.h"

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
    Property* p = &d.getOwnedAttribute().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    Operation* o = &d.getOwnedOperation().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(&d.getFeatures().front() == p);
    ASSERT_TRUE(&d.getFeatures().back() == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(&d.getOwnedMembers().front() == p);
    ASSERT_TRUE(&d.getOwnedMembers().back() == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(&d.getMembers().front() == p);
    ASSERT_TRUE(&d.getMembers().back() == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
    ASSERT_TRUE(&d.getOwnedElements().front() == p);
    ASSERT_TRUE(&d.getOwnedElements().back() == o);
}

TEST_F(DataTypeParserTest, emitDataTypeW_GeneralAndAttribute) {
    UmlManager m;
    Package pckg = m.create<Package>();
    DataType t = m.create<DataType>();
    DataType g = m.create<DataType>();
    Generalization gen = m.create<Generalization>();
    DataType s = m.create<DataType>();
    Property p = m.create<Property>();
    pckg.setID("zN&UM2AHrXX07rAiNxTmmMwLYI1O");
    pckg.setName("owningPackage");
    t.setID("mGbq9i_gGHuMFYg0y3tMzcmHx1B3");
    t.setName("type");
    g.setID("FqaulNq6bCe_8J5M0Ff2oCCaQD05");
    g.setName("general");
    s.setID("FZeUbleSO7P_Zqwn2&r8HKnEbSU5");
    s.setName("specific");
    gen.setID("k&CQ7BNYYbkhtw_g7NaNY8wUHXYs");
    gen.setGeneral(&g);
    s.getGeneralizations().add(gen);
    p.setID("m8K65o0wEqtIznmEPmuXaTph2JJu");
    p.setName("generalProp");
    p.setType(&t);
    g.getOwnedAttribute().add(p);
    pckg.getPackagedElements().add(t);
    pckg.getPackagedElements().add(g);
    pckg.getPackagedElements().add(s);
    string expectedEmit = R""""(package:
  id: zN&UM2AHrXX07rAiNxTmmMwLYI1O
  name: owningPackage
  packagedElements:
    - dataType:
        id: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        name: type
    - dataType:
        id: FqaulNq6bCe_8J5M0Ff2oCCaQD05
        name: general
        ownedAttribute:
          - property:
              id: m8K65o0wEqtIznmEPmuXaTph2JJu
              name: generalProp
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
    - dataType:
        id: FZeUbleSO7P_Zqwn2&r8HKnEbSU5
        name: specific
        generalizations:
          - generalization:
              id: k&CQ7BNYYbkhtw_g7NaNY8wUHXYs
              general: FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DataTypeParserTest, mountAndEditDataType) {
    UmlManager m;
    DataType& baseType = m.create<DataType>();
    DataType& specificType = m.create<DataType>();
    Generalization& generalization = m.create<Generalization>();
    Property& baseProp = m.create<Property>();
    Operation& baseOp = m.create<Operation>();
    Property& specProp = m.create<Property>();
    Package& root = m.create<Package>();
    generalization.setGeneral(baseType);
    specificType.getGeneralizations().add(generalization);
    baseType.getOwnedAttribute().add(baseProp);
    baseType.getOwnedOperation().add(baseOp);
    specificType.getOwnedAttribute().add(specProp);
    specProp.getRedefinedProperties().add(baseProp);
    root.getPackagedElements().add(baseType, specificType);
    m.setRoot(&root);
    m.mount(ymlPath + "packageParserTests");

    m.release(baseType);
    DataType& baseType2 = root.getPackagedElements().front().as<DataType>();
    ASSERT_TRUE(baseType2.hasOwningPackage());
    ASSERT_EQ(baseType2.getOwningPackageRef(), root);
    ASSERT_TRUE(baseType2.hasNamespace());
    ASSERT_EQ(baseType2.getNamespaceRef(), root);
    ASSERT_TRUE(baseType2.getMemberNamespace().count(root.getID()));
    ASSERT_TRUE(baseType2.hasNamespace());
    ASSERT_EQ(baseType2.getNamespaceRef(), root);
    
    ID baseTypeID = baseType2.getID();
    m.release(baseType2, root);
    DataType& baseType3 = m.aquire(baseTypeID)->as<DataType>();
    ASSERT_TRUE(baseType3.hasOwningPackage());
    Package& root2 = baseType3.getOwningPackageRef();
    ASSERT_TRUE(baseType3.hasNamespace());
    ASSERT_EQ(baseType3.getNamespaceRef(), root2);
    ASSERT_TRUE(baseType3.getMemberNamespace().count(baseTypeID));
    ASSERT_TRUE(baseType3.hasOwner());
    ASSERT_EQ(baseType3.getOwnerRef(), root2);
}
 #include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

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
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    Property* p = &d.getOwnedAttributes().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    Operation* o = &d.getOwnedOperations().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(&d.getFeatures().get("val") == p);
    ASSERT_TRUE(&d.getFeatures().get("+") == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(&d.getOwnedMembers().get("val") == p);
    ASSERT_TRUE(&d.getOwnedMembers().get("+") == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(&d.getMembers().get("val") == p);
    ASSERT_TRUE(&d.getMembers().get("+") == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
    ASSERT_TRUE(d.getOwnedElements().contains(*p));
    ASSERT_TRUE(d.getOwnedElements().contains(*o));
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
    g.getOwnedAttributes().add(p);
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
    // UmlManager m;
    // DataType& baseType = m.create<DataType>();
    // DataType& specificType = m.create<DataType>();
    // Generalization& generalization = m.create<Generalization>();
    // Property& baseProp = m.create<Property>();
    // Operation& baseOp = m.create<Operation>();
    // Property& specProp = m.create<Property>();
    // Package& root = m.create<Package>();
    // generalization.setGeneral(baseType);
    // specificType.getGeneralizations().add(generalization);
    // baseType.getOwnedAttributes().add(baseProp);
    // baseType.getOwnedOperations().add(baseOp);
    // specificType.getOwnedAttributes().add(specProp);
    // specProp.getRedefinedProperties().add(baseProp);
    // root.getPackagedElements().add(baseType, specificType);
    // m.setRoot(&root);
    // m.mount(ymlPath + "dataTypeTests");

    // m.release(baseType);
    // DataType& baseType2 = root.getPackagedElements().front().as<DataType>();
    // ASSERT_TRUE(baseType2.hasOwningPackage());
    // ASSERT_EQ(baseType2.getOwningPackageRef(), root);
    // ASSERT_TRUE(baseType2.hasNamespace());
    // ASSERT_EQ(baseType2.getNamespaceRef(), root);
    
    // ID baseTypeID = baseType2.getID();
    // m.release(baseType2, root);
    // DataType& baseType3 = m.aquire(baseTypeID)->as<DataType>();
    // ASSERT_TRUE(baseType3.hasOwningPackage());
    // Package& root2 = baseType3.getOwningPackageRef();
    // ASSERT_TRUE(baseType3.hasNamespace());
    // ASSERT_EQ(baseType3.getNamespaceRef(), root2);
    // ASSERT_TRUE(baseType3.hasOwner());
    // ASSERT_EQ(baseType3.getOwnerRef(), root2);

    // m.release(baseProp);
    // Property& baseProp2 = baseType3.getOwnedAttributes().front();
    // ASSERT_TRUE(baseProp2.hasDataType());
    // ASSERT_EQ(baseProp2.getDataTypeRef(), baseType3);
    // ASSERT_TRUE(baseProp2.hasFeaturingClassifier());
    // ASSERT_EQ(baseProp2.getFeaturingClassifierRef(), baseType3);
    // ASSERT_TRUE(baseProp2.hasNamespace());
    // ASSERT_EQ(baseProp2.getNamespaceRef(), baseType3);
    // ASSERT_TRUE(baseProp2.hasOwner());
    // ASSERT_EQ(baseProp2.getOwnerRef(), baseType3);
    // ASSERT_TRUE(baseType3.getOwnedAttributes().count(baseProp2.getID()));
    // ASSERT_TRUE(baseType3.getAttributes().count(baseProp2.getID()));
    // ASSERT_TRUE(baseType3.getFeatures().count(baseProp2.getID()));
    // ASSERT_TRUE(baseType3.getOwnedMembers().count(baseProp2.getID()));
    // ASSERT_TRUE(baseType3.getMembers().count(baseProp2.getID()));
    // ASSERT_TRUE(baseType3.getOwnedElements().count(baseProp2.getID()));
    
    // ID basePropID = baseProp2.getID();
    // m.release(baseProp2, baseType3);
    // Property& baseProp3 = m.aquire(basePropID)->as<Property>();
    // DataType& baseType4 = baseProp3.getDataTypeRef();
    // ASSERT_TRUE(baseProp3.hasFeaturingClassifier());
    // ASSERT_EQ(baseProp3.getFeaturingClassifierRef(), baseType4);
    // ASSERT_TRUE(baseProp3.hasNamespace());
    // ASSERT_EQ(baseProp3.getNamespaceRef(), baseType4);
    // ASSERT_TRUE(baseProp3.hasOwner());
    // ASSERT_EQ(baseProp3.getOwnerRef(), baseType4);
    // ASSERT_TRUE(baseType4.getOwnedAttributes().count(baseProp3.getID()));
    // ASSERT_TRUE(baseType4.getAttributes().count(baseProp3.getID()));
    // ASSERT_TRUE(baseType4.getFeatures().count(baseProp3.getID()));
    // ASSERT_TRUE(baseType4.getOwnedMembers().count(baseProp3.getID()));
    // ASSERT_TRUE(baseType4.getMembers().count(baseProp3.getID()));
    // ASSERT_TRUE(baseType4.getOwnedElements().count(baseProp3.getID()));

    // m.release(baseOp);
    // Operation& baseOp2 = baseType4.getOwnedOperations().front();
    // ASSERT_TRUE(baseOp2.hasDataType());
    // ASSERT_EQ(baseOp2.getDataTypeRef(), baseType4);
    // ASSERT_TRUE(baseOp2.hasFeaturingClassifier());
    // ASSERT_EQ(baseOp2.getFeaturingClassifierRef(), baseType4);
    // ASSERT_TRUE(baseOp2.hasNamespace());
    // ASSERT_EQ(baseOp2.getNamespaceRef(), baseType4);
    // ASSERT_TRUE(baseOp2.hasOwner());
    // ASSERT_EQ(baseOp2.getOwnerRef(), baseType4);
    // ASSERT_TRUE(baseType4.getFeatures().count(baseOp2.getID()));
    // ASSERT_TRUE(baseType4.getOwnedMembers().count(baseOp2.getID()));
    // ASSERT_TRUE(baseType4.getMembers().count(baseOp2.getID()));
    // ASSERT_TRUE(baseType4.getOwnedElements().count(baseOp2.getID()));

    // ID baseOpID = baseOp2.getID();
    // m.release(baseOp2, baseType4);
    // Operation& baseOp3 = m.aquire(baseOpID)->as<Operation>();
    // DataType& baseType5 = baseOp3.getDataTypeRef();
    // ASSERT_TRUE(baseOp3.hasDataType());
    // ASSERT_EQ(baseOp3.getDataTypeRef(), baseType5);
    // ASSERT_TRUE(baseOp3.hasFeaturingClassifier());
    // ASSERT_EQ(baseOp3.getFeaturingClassifierRef(), baseType5);
    // ASSERT_TRUE(baseOp3.hasNamespace());
    // ASSERT_EQ(baseOp3.getNamespaceRef(), baseType5);
    // ASSERT_TRUE(baseOp3.hasOwner());
    // ASSERT_EQ(baseOp3.getOwnerRef(), baseType5);
    // ASSERT_TRUE(baseType5.getOwnedOperations().count(baseOp3.getID()));
    // ASSERT_TRUE(baseType5.getFeatures().count(baseOp3.getID()));
    // ASSERT_TRUE(baseType5.getOwnedMembers().count(baseOp3.getID()));
    // ASSERT_TRUE(baseType5.getMembers().count(baseOp3.getID()));
    // ASSERT_TRUE(baseType5.getOwnedElements().count(baseOp3.getID()));

    std::cout << "!!!!!!!!!!\nTODO uncomment me por favor\n!!!!!!!!!!!!" << std::endl;
}
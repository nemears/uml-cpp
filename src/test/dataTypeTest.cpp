#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(DataTypeOwnedAttributes, Property, DataType, &DataType::getOwnedAttributes)
UML_SET_INTEGRATION_TEST(DataTypeOwnedOperations, Operation, PrimitiveType, &DataType::getOwnedOperations)
UML_SINGLETON_INTEGRATION_TEST(PropertyDataType, DataType, Property, &Property::getDataType, &Property::setDataType)
UML_SINGLETON_INTEGRATION_TEST(OperationDataType, DataType, Operation, &Operation::getDataType, &Operation::setDataType)

typedef UmlPtr<Property> PropertyPtr;

class DataTypeTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DataTypeTest, addOwnedAttributeTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Property& p = *m.create<Property>();
    d.getOwnedAttributes().add(p);
    ASSERT_EQ(d.getOwnedAttributes().size(), 1);
    ASSERT_EQ(d.getOwnedAttributes().front(), &p);
    ASSERT_EQ(d.getAttributes().size(),1);
    ASSERT_EQ(d.getAttributes().front(), &p);
    ASSERT_EQ(d.getMembers().size(), 1);
    ASSERT_EQ(d.getMembers().front(), &p);
    ASSERT_EQ(d.getOwnedMembers().size(), 1);
    ASSERT_EQ(d.getOwnedMembers().front(), &p);
    ASSERT_EQ(d.getFeatures().size(), 1);
    ASSERT_EQ(d.getFeatures().front(), &p);
    ASSERT_EQ(d.getOwnedElements().size(), 1);
    ASSERT_EQ(d.getOwnedElements().front(), &p);

    ASSERT_EQ(*p.getDataType(), d);
    ASSERT_EQ(*p.getFeaturingClassifier(), d);
    ASSERT_EQ(*p.getNamespace(), d);
    ASSERT_EQ(*p.getOwner(), d);
}

TEST_F(DataTypeTest, setDataTypeTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Property& p = *m.create<Property>();
    p.setDataType(d);
    ASSERT_EQ(d.getOwnedAttributes().size(), 1);
    ASSERT_EQ(d.getOwnedAttributes().front(), &p);
    ASSERT_EQ(d.getAttributes().size(), 1);
    ASSERT_EQ(d.getAttributes().front(), &p);
    ASSERT_EQ(d.getMembers().size(), 1);
    ASSERT_EQ(d.getMembers().front(), &p);
    ASSERT_EQ(d.getOwnedMembers().size(), 1);
    ASSERT_EQ(d.getOwnedMembers().front(), &p);
    ASSERT_EQ(d.getFeatures().size(), 1);
    ASSERT_EQ(d.getFeatures().front(), &p);
    ASSERT_EQ(d.getOwnedElements().size(), 1);
    ASSERT_EQ(d.getOwnedElements().front(), &p);

    ASSERT_EQ(*p.getDataType(), d);
    ASSERT_EQ(*p.getFeaturingClassifier(), d);
    ASSERT_EQ(*p.getNamespace(), d);
    ASSERT_EQ(*p.getOwner(), d);
}

TEST_F(DataTypeTest, removeOwnedAttributeFunctorTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Property& p = *m.create<Property>();
    Property& p2 = *m.create<Property>();
    d.getOwnedAttributes().add(p);
    
    ASSERT_EQ(d.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(d.getOwnedAttributes().contains(p));
    ASSERT_EQ(d.getAttributes().size(), 1);
    ASSERT_TRUE(d.getAttributes().contains(p));
    ASSERT_EQ(d.getFeatures().size(), 1);
    ASSERT_TRUE(d.getFeatures().contains(p));
    ASSERT_EQ(d.getOwnedMembers().size(), 1);
    ASSERT_TRUE(d.getOwnedMembers().contains(p));
    ASSERT_EQ(d.getMembers().size(), 1);
    ASSERT_TRUE(d.getMembers().contains(p));
    ASSERT_EQ(d.getOwnedElements().size(), 1);
    ASSERT_TRUE(d.getOwnedElements().contains(p));

    d.getOwnedAttributes().add(p2);

    ASSERT_EQ(d.getOwnedAttributes().size(), 2);
    ASSERT_TRUE(d.getOwnedAttributes().contains(p2));
    ASSERT_EQ(d.getAttributes().size(), 2);
    ASSERT_TRUE(d.getAttributes().contains(p2));
    ASSERT_EQ(d.getFeatures().size(), 2);
    ASSERT_TRUE(d.getFeatures().contains(p2));
    ASSERT_EQ(d.getOwnedMembers().size(), 2);
    ASSERT_TRUE(d.getOwnedMembers().contains(p2));
    ASSERT_EQ(d.getMembers().size(), 2);
    ASSERT_TRUE(d.getMembers().contains(p2));
    ASSERT_EQ(d.getOwnedElements().size(), 2);
    ASSERT_TRUE(d.getOwnedElements().contains(p2));

    d.getOwnedAttributes().remove(p2);
    
    ASSERT_EQ(d.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(d.getOwnedAttributes().front() == &p);
    ASSERT_EQ(d.getAttributes().size(), 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_EQ(d.getMembers().size(), 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_EQ(d.getOwnedMembers().size(), 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_FALSE(p2.getDataType());
    ASSERT_FALSE(p2.getFeaturingClassifier());
    ASSERT_FALSE(p2.getNamespace());
    ASSERT_FALSE(p2.getOwner());
}

TEST_F(DataTypeTest, OverridePropertyDataTypeW_NullTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Property& p = *m.create<Property>();
    Property& p2 = *m.create<Property>();
    d.getOwnedAttributes().add(p);
    d.getOwnedAttributes().add(p2);
    ASSERT_NO_THROW(p2.setDataType(0));
    
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_FALSE(p2.getDataType());
    ASSERT_FALSE(p2.getFeaturingClassifier());
    ASSERT_FALSE(p2.getNamespace());
    ASSERT_FALSE(p2.getOwner());
}

TEST_F(DataTypeTest, OverridePropertyDataTypeW_OtherTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Property& p = *m.create<Property>();
    Property& p2 = *m.create<Property>();
    DataType& d2 = *m.create<DataType>();
    d.getOwnedAttributes().add(p);
    d.getOwnedAttributes().add(p2);
    ASSERT_NO_THROW(p2.setDataType(&d2));
    
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(d2.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(d2.getOwnedAttributes().front() == &p2);
    ASSERT_TRUE(d2.getAttributes().size() == 1);
    ASSERT_TRUE(d2.getAttributes().front() == &p2);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(d2.getMembers().front() == &p2);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d2.getOwnedMembers().front() == &p2);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(d2.getFeatures().front() == &p2);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d2.getOwnedElements().begin() == p2);

    ASSERT_TRUE(p2.getDataType() == &d2);
    ASSERT_TRUE(p2.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(p2.getNamespace() == &d2);
    ASSERT_TRUE(p2.getOwner() == &d2);
}

TEST_F(DataTypeTest, addOwnedOperationTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Operation& o = *m.create<Operation>();
    ASSERT_NO_THROW(d.getOwnedOperations().add(o));
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, OperationSetDataTypeTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Operation& o = *m.create<Operation>();
    ASSERT_NO_THROW(o.setDataType(&d));
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, RemoveOwnedOperationFunctorTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Operation& o = *m.create<Operation>();
    Operation& o2 = *m.create<Operation>();
    d.getOwnedOperations().add(o);
    d.getOwnedOperations().add(o2);
    ASSERT_NO_THROW(d.getOwnedOperations().remove(o2));

    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);

    ASSERT_FALSE(o2.getDataType());
    ASSERT_FALSE(o2.getFeaturingClassifier());
    ASSERT_FALSE(o2.getNamespace());
    ASSERT_FALSE(o2.getOwner());
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_NullTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Operation& o = *m.create<Operation>();
    d.getOwnedOperations().add(o);
    ASSERT_NO_THROW(o.setDataType(0));
    ASSERT_TRUE(d.getOwnedOperations().size() == 0);
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_FALSE(o.getDataType());
    ASSERT_FALSE(o.getFeaturingClassifier());
    ASSERT_FALSE(o.getNamespace());
    ASSERT_FALSE(o.getOwner());
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_OtherOperationTest) {
    Manager<> m;
    DataType& d = *m.create<DataType>();
    Operation& o = *m.create<Operation>();
    DataType& d2 = *m.create<DataType>();
    d.getOwnedOperations().add(o);
    ASSERT_NO_THROW(o.setDataType(d2));
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_TRUE(d2.getOwnedOperations().size() == 1);
    ASSERT_TRUE(d2.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(d2.getFeatures().front() == &o);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d2.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(d2.getMembers().front() == &o);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d2.getOwnedElements().begin() == o);

    ASSERT_TRUE(o.getDataType() == &d2);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(o.getNamespace() == &d2);
    ASSERT_TRUE(o.getOwner() == &d2);
}

TEST_F(DataTypeTest, reindexOwnedAttributeNameTest) {
    Manager<> m;
    DataTypePtr d = m.create<DataType>();
    PropertyPtr p = m.create<Property>();
    p->setName("1");
    d->getOwnedAttributes().add(*p);
    p->setName("2");
    ASSERT_EQ(d->getOwnedAttributes().get("2"), p);
    ASSERT_EQ(d->getAttributes().get("2"), p);
    ASSERT_EQ(d->getFeatures().get("2"), p);
    ASSERT_EQ(d->getMembers().get("2"), p);
    ASSERT_EQ(d->getOwnedMembers().get("2"), p);
}

TEST_F(DataTypeTest, reindexOwnedOperationNameTest) {
    Manager<> m;
    DataTypePtr d = m.create<DataType>();
    OperationPtr o = m.create<Operation>();
    o->setName("1");
    d->getOwnedOperations().add(*o);
    o->setName("2");
    ASSERT_EQ(d->getOwnedOperations().get("2"), o);
    ASSERT_EQ(d->getFeatures().get("2"), o);
    ASSERT_EQ(d->getMembers().get("2"), o);
    ASSERT_EQ(d->getOwnedMembers().get("2"), o);
}

TEST_F(DataTypeTest, basicDataTypeTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "dataTypeTests/basicDataType.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::DATA_TYPE);
    DataType& d = m.getRoot()->as<DataType>();
    ASSERT_TRUE(d.getName().compare("int") == 0);
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    PropertyPtr p = d.getOwnedAttributes().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    OperationPtr o = d.getOwnedOperations().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(d.getFeatures().get("val") == p);
    ASSERT_TRUE(d.getFeatures().get("+") == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(d.getOwnedMembers().get("val") == p);
    ASSERT_TRUE(d.getOwnedMembers().get("+") == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(d.getMembers().get("val") == p);
    ASSERT_TRUE(d.getMembers().get("+") == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
    ASSERT_TRUE(d.getOwnedElements().contains(*p));
    ASSERT_TRUE(d.getOwnedElements().contains(*o));
}

TEST_F(DataTypeTest, emitDataTypeW_GeneralAndAttribute) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    DataType& t = *m.create<DataType>();
    DataType& g = *m.create<DataType>();
    Generalization& gen = *m.create<Generalization>();
    DataType& s = *m.create<DataType>();
    Property& p = *m.create<Property>();
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
    std::string expectedEmit = R""""(Package:
  id: "zN&UM2AHrXX07rAiNxTmmMwLYI1O"
  name: owningPackage
  packagedElements:
    - DataType:
        id: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        name: type
    - DataType:
        id: FqaulNq6bCe_8J5M0Ff2oCCaQD05
        name: general
        ownedAttributes:
          - Property:
              id: m8K65o0wEqtIznmEPmuXaTph2JJu
              name: generalProp
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
    - DataType:
        id: "FZeUbleSO7P_Zqwn2&r8HKnEbSU5"
        name: specific
        generalizations:
          - Generalization:
              id: "k&CQ7BNYYbkhtw_g7NaNY8wUHXYs"
              general: FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    std::string expectedEmit2 = R""""(Package:
  id: "zN&UM2AHrXX07rAiNxTmmMwLYI1O"
  name: owningPackage
  packagedElements:
    - DataType:
        id: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        name: type
    - DataType:
        id: "FZeUbleSO7P_Zqwn2&r8HKnEbSU5"
        name: specific
        generalizations:
          - Generalization:
              id: "k&CQ7BNYYbkhtw_g7NaNY8wUHXYs"
              general: FqaulNq6bCe_8J5M0Ff2oCCaQD05
    - DataType:
        id: FqaulNq6bCe_8J5M0Ff2oCCaQD05
        name: general
        ownedAttributes:
          - Property:
              id: m8K65o0wEqtIznmEPmuXaTph2JJu
              name: generalProp
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_TRUE(expectedEmit == generatedEmit || expectedEmit2 == generatedEmit);
}

TEST_F(DataTypeTest, mountAndEditDataType) {
    Manager<> m;
    DataType& baseType = *m.create<DataType>();
    DataType& specificType = *m.create<DataType>();
    Generalization& generalization = *m.create<Generalization>();
    Property& baseProp = *m.create<Property>();
    Operation& baseOp = *m.create<Operation>();
    Property& specProp = *m.create<Property>();
    Package& root = *m.create<Package>();
    generalization.setGeneral(baseType);
    specificType.getGeneralizations().add(generalization);
    baseType.getOwnedAttributes().add(baseProp);
    baseType.getOwnedOperations().add(baseOp);
    specificType.getOwnedAttributes().add(specProp);
    specProp.getRedefinedProperties().add(baseProp);
    root.getPackagedElements().add(baseType, specificType);
    m.setRoot(&root);
    m.mount(ymlPath + "dataTypeTests");

    ID baseTypeID = baseType.getID();
    m.release(baseType);
    DataType& baseType2 = root.getPackagedElements().get(baseTypeID)->as<DataType>();
    ASSERT_TRUE(baseType2.getOwningPackage());
    ASSERT_EQ(*baseType2.getOwningPackage(), root);
    ASSERT_TRUE(baseType2.getNamespace());
    ASSERT_EQ(*baseType2.getNamespace(), root);
    
    m.release(baseType2, root);
    DataType& baseType3 = m.get(baseTypeID)->as<DataType>();
    ASSERT_TRUE(baseType3.getOwningPackage());
    Package& root2 = *baseType3.getOwningPackage(); // This line removes root from its node->m_ptrs list
    ASSERT_TRUE(baseType3.getNamespace());
    ASSERT_EQ(*baseType3.getNamespace(), root2);
    ASSERT_TRUE(baseType3.getOwner());
    ASSERT_EQ(*baseType3.getOwner(), root2);

    ID basePropID = baseProp.getID();
    m.release(baseProp);
    Property& baseProp2 = *baseType3.getOwnedAttributes().get(basePropID);
    ASSERT_TRUE(baseProp2.getDataType());
    ASSERT_EQ(*baseProp2.getDataType(), baseType3);
    ASSERT_TRUE(baseProp2.getFeaturingClassifier());
    ASSERT_EQ(*baseProp2.getFeaturingClassifier(), baseType3);
    ASSERT_TRUE(baseProp2.getNamespace());
    ASSERT_EQ(*baseProp2.getNamespace(), baseType3);
    ASSERT_TRUE(baseProp2.getOwner());
    ASSERT_EQ(*baseProp2.getOwner(), baseType3);
    ASSERT_TRUE(baseType3.getOwnedAttributes().contains(baseProp2.getID()));
    ASSERT_TRUE(baseType3.getAttributes().contains(baseProp2.getID()));
    ASSERT_TRUE(baseType3.getFeatures().contains(baseProp2.getID()));
    ASSERT_TRUE(baseType3.getOwnedMembers().contains(baseProp2.getID()));
    ASSERT_TRUE(baseType3.getMembers().contains(baseProp2.getID()));
    ASSERT_TRUE(baseType3.getOwnedElements().contains(baseProp2.getID()));

    m.release(baseProp2, baseType3);
    Property& baseProp3 = m.get(basePropID)->as<Property>();
    DataType& baseType4 = *baseProp3.getDataType();
    ASSERT_TRUE(baseProp3.getFeaturingClassifier());
    ASSERT_EQ(*baseProp3.getFeaturingClassifier(), baseType4);
    ASSERT_TRUE(baseProp3.getNamespace());
    ASSERT_EQ(*baseProp3.getNamespace(), baseType4);
    ASSERT_TRUE(baseProp3.getOwner());
    ASSERT_EQ(*baseProp3.getOwner(), baseType4);
    ASSERT_TRUE(baseType4.getOwnedAttributes().contains(baseProp3.getID()));
    ASSERT_TRUE(baseType4.getAttributes().contains(baseProp3.getID()));
    ASSERT_TRUE(baseType4.getFeatures().contains(baseProp3.getID()));
    ASSERT_TRUE(baseType4.getOwnedMembers().contains(baseProp3.getID()));
    ASSERT_TRUE(baseType4.getMembers().contains(baseProp3.getID()));
    ASSERT_TRUE(baseType4.getOwnedElements().contains(baseProp3.getID()));

    m.release(baseOp);
    Operation& baseOp2 = *baseType4.getOwnedOperations().front();
    ASSERT_TRUE(baseOp2.getDataType());
    ASSERT_EQ(*baseOp2.getDataType(), baseType4);
    ASSERT_TRUE(baseOp2.getFeaturingClassifier());
    ASSERT_EQ(*baseOp2.getFeaturingClassifier(), baseType4);
    ASSERT_TRUE(baseOp2.getNamespace());
    ASSERT_EQ(*baseOp2.getNamespace(), baseType4);
    ASSERT_TRUE(baseOp2.getOwner());
    ASSERT_EQ(*baseOp2.getOwner(), baseType4);
    ASSERT_TRUE(baseType4.getFeatures().contains(baseOp2.getID()));
    ASSERT_TRUE(baseType4.getOwnedMembers().contains(baseOp2.getID()));
    ASSERT_TRUE(baseType4.getMembers().contains(baseOp2.getID()));
    ASSERT_TRUE(baseType4.getOwnedElements().contains(baseOp2.getID()));

    ID baseOpID = baseOp2.getID();
    m.release(baseOp2, baseType4);
    Operation& baseOp3 = m.get(baseOpID)->as<Operation>();
    DataType& baseType5 = *baseOp3.getDataType();
    ASSERT_TRUE(baseOp3.getDataType());
    ASSERT_EQ(*baseOp3.getDataType(), baseType5);
    ASSERT_TRUE(baseOp3.getFeaturingClassifier());
    ASSERT_EQ(*baseOp3.getFeaturingClassifier(), baseType5);
    ASSERT_TRUE(baseOp3.getNamespace());
    ASSERT_EQ(*baseOp3.getNamespace(), baseType5);
    ASSERT_TRUE(baseOp3.getOwner());
    ASSERT_EQ(*baseOp3.getOwner(), baseType5);
    ASSERT_TRUE(baseType5.getOwnedOperations().contains(baseOp3.getID()));
    ASSERT_TRUE(baseType5.getFeatures().contains(baseOp3.getID()));
    ASSERT_TRUE(baseType5.getOwnedMembers().contains(baseOp3.getID()));
    ASSERT_TRUE(baseType5.getMembers().contains(baseOp3.getID()));
    ASSERT_TRUE(baseType5.getOwnedElements().contains(baseOp3.getID()));
}

TEST_F(DataTypeTest, basicPrimitiveTypeTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "primitiveTypeTests/basicPrimitiveType.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PRIMITIVE_TYPE);
    DataType& d = m.getRoot()->as<DataType>();
    ASSERT_TRUE(d.getName().compare("int") == 0);
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    PropertyPtr p = d.getOwnedAttributes().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    OperationPtr o = d.getOwnedOperations().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(d.getFeatures().get("val") == p);
    ASSERT_TRUE(d.getFeatures().get("+") == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(d.getOwnedMembers().get("val") == p);
    ASSERT_TRUE(d.getOwnedMembers().get("+") == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(d.getMembers().get("val") == p);
    ASSERT_TRUE(d.getMembers().get("+") == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
}

TEST_F(DataTypeTest, emitPrimWGeneralAndAttribute) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    PrimitiveType& t = *m.create<PrimitiveType>();
    PrimitiveType& g = *m.create<PrimitiveType>();;
    Generalization& gen = *m.create<Generalization>();
    PrimitiveType& s = *m.create<PrimitiveType>();
    Property& p = *m.create<Property>();
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
    g.getOwnedAttributes().add(p);
    pckg.getPackagedElements().add(t);
    pckg.getPackagedElements().add(g);
    pckg.getPackagedElements().add(s);
    std::string expectedEmit = R""""(Package:
  id: "ScxedgF1Ej1JJubABfwVAK1X&_28"
  name: owningPackage
  packagedElements:
    - PrimitiveType:
        id: "3di2YkXSv7_2E8yUAdNElvDPCP4&"
        name: specific
        generalizations:
          - Generalization:
              id: "Fpn&_P3zERcsyDCu8kN837WxY_90"
              general: bD3tGZ_XwovlDpB4NPqiaMl19DUV
    - PrimitiveType:
        id: bD3tGZ_XwovlDpB4NPqiaMl19DUV
        name: general
        ownedAttributes:
          - Property:
              id: "yShg7h43ZHm9I2Dqw2RoSXV&Y0PQ"
              name: generalProp
              type: "ufsWC1O42Rz36lNEixxF&gH6SKdj"
    - PrimitiveType:
        id: "ufsWC1O42Rz36lNEixxF&gH6SKdj"
        name: type)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

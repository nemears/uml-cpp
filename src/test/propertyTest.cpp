#include "gtest/gtest.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(PropertyDefaultValue, Expression, Property, &Property::getDefaultValue, &Property::setDefaultValue)
UML_SET_INTEGRATION_TEST(PropertyRedefinedProperties, Property, Property, &Property::getRedefinedProperties)
UML_SET_INTEGRATION_TEST(PropertySubsettedProperties, Property, Property, &Property::getSubsettedProperties)

class PropertyTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
    Manager<> m;
    Property& p = *m.create<Property>();
    PrimitiveType& stringP = *m.create<PrimitiveType>();
    p.setType(&stringP);
    LiteralString& ls = *m.create<LiteralString>();
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
    ASSERT_TRUE(p.getDefaultValue() == &ls);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(&p.getOwnedElements().get(ls.getID()) == &ls);
}

TEST_F(PropertyTest, reindexNameForClassifierTest) {
    Manager<> m;
    Class& c = *m.create<Class>();
    Property& p = *m.create<Property>();
    p.setAggregation(AggregationKind::COMPOSITE);
    c.getOwnedAttributes().add(p);
    ASSERT_NO_THROW(p.setName("test"));
    ASSERT_EQ(c.getOwnedElements().get("test"), p);
    ASSERT_EQ(c.getMembers().get("test"), p);
    ASSERT_EQ(c.getOwnedMembers().get("test"), p);
    ASSERT_EQ(c.getFeatures().get("test"), p);
    ASSERT_EQ(c.getAttributes().get("test"), p);
    ASSERT_EQ(c.getOwnedAttributes().get("test"), p);
    ASSERT_EQ(c.getRoles().get("test"), p);
    ASSERT_EQ(c.getParts().get("test"), p);

    Association& a = *m.create<Association>();
    Property& p2 = *m.create<Property>();;
    a.getNavigableOwnedEnds().add(p2);
    ASSERT_NO_THROW(p2.setName("test"));
    ASSERT_EQ(a.getNavigableOwnedEnds().get("test"), p2);
    ASSERT_EQ(a.getOwnedEnds().get("test"), p2);
    ASSERT_EQ(a.getMemberEnds().get("test"), p2);
    ASSERT_EQ(a.getFeatures().get("test"), p2);
    ASSERT_EQ(a.getOwnedMembers().get("test"), p2);
    ASSERT_EQ(a.getMembers().get("test"), p2);
    ASSERT_EQ(a.getOwnedElements().get("test"), p2);
}

TEST_F(PropertyTest, overwriteClassifierTest) {
    Manager<> m;
    Class& p1 = *m.create<Class>();
    Class& p2 = *m.create<Class>();
    Property& c = *m.create<Property>();
    p1.getOwnedAttributes().add(c);
    c.setClass(&p2);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(*c.getFeaturingClassifier(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, overwriteClassifierByAttributesAddTest) {
    Manager<> m;
    Class& p1 = *m.create<Class>();
    Class& p2 = *m.create<Class>();
    Property& c = *m.create<Property>();
    p1.getOwnedAttributes().add(c);
    p2.getOwnedAttributes().add(c);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(*c.getFeaturingClassifier(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, redefinePropertyTest) {
    Manager<> m;
    Property& prop = *m.create<Property>();
    prop.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    Property& redefined = *m.create<Property>();
    prop.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
    Class& b = *m.create<Class>();
    Class& s = *m.create<Class>();
    Generalization& gen = *m.create<Generalization>();
    s.getGeneralizations().add(gen);
    gen.setGeneral(b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    ASSERT_EQ(prop.getRedefinedProperties().size(), 1);
    ASSERT_EQ(prop.getRedefinedProperties().front().getID(), redefined.getID());
    ASSERT_EQ(prop.getRedefinitionContext().size(), 1);
    ASSERT_EQ(prop.getRedefinitionContext().front().getID(), s.getID());
    ASSERT_EQ(prop.getRedefinedElements().size(), 1);
    ASSERT_EQ(prop.getRedefinedElements().front().getID(), redefined.getID());

    ASSERT_EQ(b.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(b.getOwnedAttributes().contains(redefined));
    ASSERT_EQ(b.getAttributes().size(), 1);
    ASSERT_TRUE(b.getAttributes().contains(redefined));
    ASSERT_EQ(b.getFeatures().size(), 1);
    ASSERT_TRUE(b.getFeatures().contains(redefined));
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_TRUE(b.getOwnedMembers().contains(redefined));
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_TRUE(b.getMembers().contains(redefined));
    ASSERT_EQ(b.getOwnedElements().size(), 1);
    ASSERT_TRUE(b.getOwnedElements().contains(redefined));

    ASSERT_EQ(s.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(s.getOwnedAttributes().contains(prop));
    ASSERT_EQ(s.getAttributes().size(), 1);
    ASSERT_TRUE(s.getAttributes().contains(prop));
    ASSERT_EQ(s.getFeatures().size(), 1);
    ASSERT_TRUE(s.getFeatures().contains(prop));
    ASSERT_EQ(s.getOwnedMembers().size(), 1);
    ASSERT_TRUE(s.getOwnedMembers().contains(prop));
    ASSERT_EQ(s.getMembers().size(), 2);
    ASSERT_TRUE(s.getMembers().contains(prop));
    ASSERT_EQ(s.getOwnedElements().size(), 2);
    ASSERT_TRUE(s.getOwnedElements().contains(prop));

  // TODO : restore below
//   Property& notRelated = m.create<Property>();
//   ASSERT_THROW(prop.getRedefinedProperties().add(notRelated), ImproperRedefinitionException);
}

TEST_F(PropertyTest, forwardTypeTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/forwardType.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class& clazz1 = pckg->getPackagedElements().get(ID::fromString("5pyZRmwKoVZBKmqRI6qp93kvQYUR")).as<Class>();
    Class& clazz2 = pckg->getPackagedElements().get(ID::fromString("J5Y0janY19dgKxqwQ1YYfFgMgXmD")).as<Class>();
    ASSERT_TRUE(clazz2.getAttributes().size() == 1);
    Property* prop = &clazz2.getAttributes().front();
    ASSERT_TRUE(prop->getType()->as<Class>() == clazz1);
}

TEST_F(PropertyTest, backwardsTypeTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/backwardTypeTest.yml").ptr());
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class& clazz1 = pckg->getPackagedElements().get(ID::fromString("UKqPzBXPjVPhV89kTmNr7xRc7Z_&")).as<Class>();
    Class& clazz2 = pckg->getPackagedElements().get(ID::fromString("J5Y0janY19dgKxqwQ1YYfFgMgXmD")).as<Class>();
    ASSERT_TRUE(clazz2.getAttributes().size() == 1);
    Property* prop = &clazz2.getAttributes().front();
    ASSERT_TRUE(prop->getType()->as<Class>() == clazz1);
}

TEST_F(PropertyTest, multiplicityTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/multiplicityTest.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PROPERTY);
    Property* prop = &m.getRoot()->as<Property>();
    ASSERT_TRUE(prop->multiplicitySpecified());
    ASSERT_TRUE(prop->getLowerValue());
    ASSERT_TRUE(prop->getLower() == 0);
    ASSERT_TRUE(prop->getLowerValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getLower() == dynamic_cast<LiteralInt*>(prop->getLowerValue().ptr())->getValue());
    ASSERT_TRUE(prop->getUpperValue());
    ASSERT_TRUE(prop->getUpper() == 1);
    ASSERT_TRUE(prop->getUpperValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getUpper() == dynamic_cast<LiteralInt*>(prop->getUpperValue().ptr())->getValue());
}

TEST_F(PropertyTest, improperTypeTest) {
    Manager<> m;
    ASSERT_THROW(m.open(ymlPath + "propertyTests/improperType.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/improperType2.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/improperType3.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/propertyNotMap.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/attributesNotSequence.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/invalidLower.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "propertyTests/invalidUpper.yml"), SerializationError);
}

TEST_F(PropertyTest, literalBoolDefaultValueTest) {
    Manager<> m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/literalBool.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get("bool"));
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == b);
    ASSERT_TRUE(p->getDefaultValue());
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == ElementType::LITERAL_BOOL);
    LiteralBool* lb = dynamic_cast<LiteralBool*>(p->getDefaultValue().ptr());
    ASSERT_TRUE(lb->getValue());
}

TEST_F(PropertyTest, literalsTest) {
    Manager<> m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/defaultValue.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get("bool"));
    PrimitiveType* i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get("int"));
    PrimitiveType* r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get("real"));
    PrimitiveType* s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get("string"));
    
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 4);
    Property* stringProp = &c->getOwnedAttributes().get(0);
    ASSERT_TRUE(stringProp->getType());
    ASSERT_TRUE(stringProp->getType() == s);
    ASSERT_TRUE(stringProp->getDefaultValue());
    ASSERT_TRUE(stringProp->getDefaultValue()->getElementType() == ElementType::LITERAL_STRING);
    LiteralString* ls = dynamic_cast<LiteralString*>(stringProp->getDefaultValue().ptr());
    ASSERT_TRUE(ls->getValue().compare("testValue") == 0);
    Property* intProp = &c->getOwnedAttributes().get(1);
    ASSERT_TRUE(intProp->getType());
    ASSERT_TRUE(intProp->getType() == i);
    ASSERT_TRUE(intProp->getDefaultValue());
    ASSERT_TRUE(intProp->getDefaultValue()->getElementType() == ElementType::LITERAL_INT);
    LiteralInt* li =  dynamic_cast<LiteralInt*>(intProp->getDefaultValue().ptr());
    ASSERT_TRUE(li->getValue() == -444);
    Property* realProp = &c->getOwnedAttributes().get(2);
    ASSERT_TRUE(realProp->getType());
    ASSERT_TRUE(realProp->getType() == r);
    ASSERT_TRUE(realProp->getDefaultValue());
    ASSERT_TRUE(realProp->getDefaultValue()->getElementType() == ElementType::LITERAL_REAL);
    LiteralReal* lr = dynamic_cast<LiteralReal*>(realProp->getDefaultValue().ptr());
    ASSERT_TRUE(lr->getValue() == 555.888);
    Property* boolProp = &c->getOwnedAttributes().get(3);
    ASSERT_TRUE(boolProp->getType());
    ASSERT_TRUE(boolProp->getType() == b);
    ASSERT_TRUE(boolProp->getDefaultValue());
    ASSERT_TRUE(boolProp->getDefaultValue()->getElementType() == ElementType::LITERAL_BOOL);
    LiteralBool* lb = dynamic_cast<LiteralBool*>(boolProp->getDefaultValue().ptr());
    ASSERT_TRUE(lb->getValue() == false);
}

TEST_F(PropertyTest, parseRedefinedPropertyTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "propertyTests/redefinedProperty.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().get(ID::fromString("2luT6NreEKbmUKCHLqO4tfqxx5pX")).getElementType(), ElementType::CLASS);
    Class& base = pckg.getPackagedElements().get(ID::fromString("2luT6NreEKbmUKCHLqO4tfqxx5pX")).as<Class>();
    ASSERT_EQ(base.getOwnedAttributes().size(), 1);
    Property& redefined = base.getOwnedAttributes().front();
    ASSERT_EQ(pckg.getPackagedElements().get(ID::fromString("VI1AWv&JVnbq8P8mvFztq1W4v&0y")).getElementType(), ElementType::CLASS);
    Class& spec = pckg.getPackagedElements().get(ID::fromString("VI1AWv&JVnbq8P8mvFztq1W4v&0y")).as<Class>();
    ASSERT_EQ(spec.getGenerals().size(), 1);
    ASSERT_EQ(spec.getGenerals().front().getID(), base.getID());
    ASSERT_EQ(spec.getOwnedAttributes().size(), 1);
    Property& prop = spec.getOwnedAttributes().front();
    ASSERT_EQ(prop.getRedefinedProperties().size(), 1);
    ASSERT_EQ(prop.getRedefinedProperties().front().getID(), redefined.getID());
}

TEST_F(PropertyTest, emitRedefinedPropertyTest) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    Property& prop = *m.create<Property>();
    Property& redefined = *m.create<Property>();
    Class& b = *m.create<Class>();
    Class& s = *m.create<Class>();
    Generalization& gen = *m.create<Generalization>();
    pckg.setID("RC5KnOAfUJQY6BnxohDHLqrMadYI");
    prop.setID("sVhU3UWy392YuTfewtNoyaWLhAQw");
    redefined.setID("9m50Dir0MgpEaLu8ghn7cSlZ5Yzh");
    b.setID("yzUVzVw8sod2KyBH5LxX_OLI7HrV");
    s.setID("Y8UtleiQO3UuN4GEqSzai0G8&GqC");
    gen.setID("RL5_MDmj_CskU1njfiL74QSxP7Bw");
    s.getGeneralizations().add(gen);
    gen.setGeneral(b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    pckg.getPackagedElements().add(b);
    pckg.getPackagedElements().add(s);
    std::string expectedEmit = R""""(Package:
  id: RC5KnOAfUJQY6BnxohDHLqrMadYI
  packagedElements:
    - Class:
        id: "Y8UtleiQO3UuN4GEqSzai0G8&GqC"
        generalizations:
          - Generalization:
              id: RL5_MDmj_CskU1njfiL74QSxP7Bw
              general: yzUVzVw8sod2KyBH5LxX_OLI7HrV
        ownedAttributes:
          - Property:
              id: sVhU3UWy392YuTfewtNoyaWLhAQw
              redefinedProperties:
                - 9m50Dir0MgpEaLu8ghn7cSlZ5Yzh
    - Class:
        id: yzUVzVw8sod2KyBH5LxX_OLI7HrV
        ownedAttributes:
          - Property:
              id: 9m50Dir0MgpEaLu8ghn7cSlZ5Yzh)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PropertyTest, mountPropertyTest) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    m.setRoot(&pckg);
    m.open(ymlPath + "uml/primitiveTypes.yml");
    PackageMerge& merge = *m.create<PackageMerge>();
    merge.setMergedPackage(m.get(ID::fromString("Primitive_Types_WZcyDDLemQ97"))->as<Package>());
    pckg.getPackageMerge().add(merge);
    Property& prop = *m.create<Property>();
    Property& redefined = *m.create<Property>();
    Class& b = *m.create<Class>();
    Class& s = *m.create<Class>();
    Generalization& gen = *m.create<Generalization>();
    LiteralString& defaultValue = *m.create<LiteralString>();
    s.getGeneralizations().add(gen);
    gen.setGeneral(&b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    redefined.setType(m.get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT"))->as<Type>());
    prop.setDefaultValue(&defaultValue);
    pckg.getPackagedElements().add(b);
    pckg.getPackagedElements().add(s);
    ASSERT_NO_THROW(m.mount(ymlPath + "propertyTests"));
    // TODO explore tree (probably don't need to)
    ASSERT_NO_THROW(m.release(prop));
    Property& prop2 = s.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.getClass());
    ASSERT_EQ(prop2.getClass(), &s);
    ASSERT_TRUE(prop2.getNamespace());
    ASSERT_EQ(prop2.getNamespace(), &s);
    ASSERT_TRUE(prop2.getOwner());
    ASSERT_EQ(prop2.getOwner(), &s);
    ASSERT_TRUE(prop2.getDefaultValue());
    ASSERT_EQ(defaultValue, *prop2.getDefaultValue());
    ASSERT_EQ(prop2.getOwnedElements().size(), 1);
    ASSERT_EQ(*prop2.getOwnedElements().begin(), defaultValue);
    ASSERT_EQ(prop2.getRedefinedProperties().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedProperties().front(), &redefined);
    ASSERT_EQ(prop2.getRedefinedElements().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedElements().front(), &redefined);
    ASSERT_EQ(prop2.getRedefinitionContext().size(), 1);
    ASSERT_EQ(&prop2.getRedefinitionContext().front(), &s);

    ASSERT_EQ(s.getAttributes().size(), 1);
    ASSERT_EQ(&s.getAttributes().front(), &prop2);
    ASSERT_EQ(s.getFeatures().size(), 1);
    ASSERT_EQ(&s.getFeatures().front(), &prop2);
    ASSERT_EQ(s.getOwnedMembers().size(), 1);
    ASSERT_EQ(&s.getOwnedMembers().front(), &prop2);
    ASSERT_EQ(s.getMembers().size(), 2);
    ASSERT_TRUE(s.getMembers().contains(redefined));
    ASSERT_TRUE(s.getMembers().contains(prop2));
    ASSERT_EQ(s.getOwnedElements().size(), 2);
    ASSERT_TRUE(s.getOwnedElements().contains(gen));
    ASSERT_TRUE(s.getOwnedElements().contains(prop2));

    // Release the redefined prop
    ASSERT_NO_THROW(m.release(redefined));
    Property& redefined2 = b.getOwnedAttributes().front();
    ASSERT_EQ(prop2.getRedefinedProperties().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedProperties().front(), &redefined2);
    ASSERT_EQ(prop2.getRedefinedElements().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedElements().front(), &redefined2);
    ASSERT_TRUE(redefined2.getType());
    ASSERT_EQ(redefined2.getType(), &m.get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT"))->as<Type>());
    ASSERT_TRUE(redefined2.getClass());
    ASSERT_EQ(redefined2.getClass()->getID(), b.getID());
    ASSERT_TRUE(redefined2.getFeaturingClassifier());
    ASSERT_EQ(redefined2.getFeaturingClassifier()->getID(), b.getID());

    ID redefinedID = redefined2.getID();
    Type& stringType = *redefined2.getType();
    m.release(redefined2, stringType);
    Property& redefined3 = m.get(redefinedID)->as<Property>();
    ASSERT_EQ(prop2.getRedefinedProperties().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedProperties().front(), &redefined3);
    ASSERT_EQ(prop2.getRedefinedElements().size(), 1);
    ASSERT_EQ(&prop2.getRedefinedElements().front(), &redefined3);
    ASSERT_TRUE(redefined3.getType());
    ASSERT_EQ(redefined3.getType(), &m.get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT"))->as<Type>());
    ASSERT_TRUE(redefined3.getClass());
    ASSERT_EQ(redefined3.getClass()->getID(), b.getID());
    ASSERT_TRUE(redefined3.getFeaturingClassifier());
    ASSERT_EQ(redefined3.getFeaturingClassifier()->getID(), b.getID());

    ID propID = prop2.getID();

    m.release(prop2, defaultValue);
    Property& prop3 = m.get(propID)->as<Property>();
    ASSERT_TRUE(prop3.getDefaultValue());
    ASSERT_TRUE(prop3.getDefaultValue()->isSubClassOf(ElementType::LITERAL_STRING));
    ASSERT_EQ(prop3.getOwnedElements().size(), 1);
    ASSERT_EQ(prop3.getOwnedElements().front(), *prop3.getDefaultValue());
    LiteralString& defaultValue2 = prop3.getDefaultValue()->as<LiteralString>();
    ASSERT_TRUE(defaultValue2.getOwner());
    ASSERT_EQ(*defaultValue2.getOwner(), prop3);
}

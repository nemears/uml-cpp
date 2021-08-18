#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/property.h"
#include "uml/class.h"
#include "uml/package.h"
#include "uml/packageMerge.h"
#include "uml/primitiveType.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalString.h"
#include "uml/literalReal.h"
#include "uml/generalization.h"

using namespace std;
using namespace UML;

class PropertyParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(PropertyParserTest, forwardTypeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/forwardType.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* clazz1 = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    Class* clazz2 = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(clazz2->getAttributes().size() == 1);
    Property* prop = &clazz2->getAttributes().front();
    ASSERT_TRUE(prop->getType() == clazz1);
}

TEST_F(PropertyParserTest, backwardsTypeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/backwardTypeTest.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* clazz1 = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    Class* clazz2 = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(clazz1->getAttributes().size() == 1);
    Property* prop = &clazz1->getAttributes().front();
    ASSERT_TRUE(prop->getType() == clazz2);
}

TEST_F(PropertyParserTest, multiplicityTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/multiplicityTest.yml"));
    ASSERT_TRUE(el);
    ASSERT_TRUE(el->getElementType() == ElementType::PROPERTY);
    Property* prop = dynamic_cast<Property*>(el);
    ASSERT_TRUE(prop->multiplicitySpecified());
    ASSERT_TRUE(prop->getLowerValue());
    ASSERT_TRUE(prop->getLower() == 0);
    ASSERT_TRUE(prop->getLowerValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getLower() == dynamic_cast<LiteralInt*>(prop->getLowerValue())->getValue());
    ASSERT_TRUE(prop->getUpperValue());
    ASSERT_TRUE(prop->getUpper() == 1);
    ASSERT_TRUE(prop->getUpperValue()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(prop->getUpper() == dynamic_cast<LiteralInt*>(prop->getUpperValue())->getValue());
}

TEST_F(PropertyParserTest, improperTypeTest) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/improperType.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/improperType2.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/improperType3.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/propertyNotMap.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/attributesNotSequence.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/invalidLower.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "propertyTests/invalidUpper.yml"), Parsers::UmlParserException);
}

TEST_F(PropertyParserTest, literalBoolDefaultValueTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/literalBool.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().front());
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == b);
    ASSERT_TRUE(p->getDefaultValue() != 0);
    ASSERT_TRUE(p->getDefaultValue()->getElementType() == ElementType::LITERAL_BOOL);
    LiteralBool* lb = dynamic_cast<LiteralBool*>(p->getDefaultValue());
    ASSERT_TRUE(lb->getValue());
}

TEST_F(PropertyParserTest, literalsTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/defaultValue.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().front());
    PrimitiveType* i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(1));
    PrimitiveType* r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(2));
    PrimitiveType* s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(3));
    
    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedAttributes().size() == 4);
    Property* stringProp = &c->getOwnedAttributes().get(0);
    ASSERT_TRUE(stringProp->getType() != 0);
    ASSERT_TRUE(stringProp->getType() == s);
    ASSERT_TRUE(stringProp->getDefaultValue() != 0);
    ASSERT_TRUE(stringProp->getDefaultValue()->getElementType() == ElementType::LITERAL_STRING);
    LiteralString* ls = dynamic_cast<LiteralString*>(stringProp->getDefaultValue());
    ASSERT_TRUE(ls->getValue().compare("testValue") == 0);
    Property* intProp = &c->getOwnedAttributes().get(1);
    ASSERT_TRUE(intProp->getType() != 0);
    ASSERT_TRUE(intProp->getType() == i);
    ASSERT_TRUE(intProp->getDefaultValue() != 0);
    ASSERT_TRUE(intProp->getDefaultValue()->getElementType() == ElementType::LITERAL_INT);
    LiteralInt* li =  dynamic_cast<LiteralInt*>(intProp->getDefaultValue());
    ASSERT_TRUE(li->getValue() == -444);
    Property* realProp = &c->getOwnedAttributes().get(2);
    ASSERT_TRUE(realProp->getType() != 0);
    ASSERT_TRUE(realProp->getType() == r);
    ASSERT_TRUE(realProp->getDefaultValue() != 0);
    ASSERT_TRUE(realProp->getDefaultValue()->getElementType() == ElementType::LITERAL_REAL);
    LiteralReal* lr = dynamic_cast<LiteralReal*>(realProp->getDefaultValue());
    ASSERT_TRUE(lr->getValue() == 555.888);
    Property* boolProp = &c->getOwnedAttributes().get(3);
    ASSERT_TRUE(boolProp->getType() != 0);
    ASSERT_TRUE(boolProp->getType() == b);
    ASSERT_TRUE(boolProp->getDefaultValue() != 0);
    ASSERT_TRUE(boolProp->getDefaultValue()->getElementType() == ElementType::LITERAL_BOOL);
    LiteralBool* lb = dynamic_cast<LiteralBool*>(boolProp->getDefaultValue());
    ASSERT_TRUE(lb->getValue() == false);
}

TEST_F(PropertyParserTest, parseRedefinedPropertyTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "propertyTests/redefinedProperty.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& base = pckg.getPackagedElements().front().as<Class>();
    ASSERT_EQ(base.getOwnedAttributes().size(), 1);
    Property& redefined = base.getOwnedAttributes().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::CLASS);
    Class& spec = pckg.getPackagedElements().get(1).as<Class>();
    ASSERT_EQ(spec.getGenerals().size(), 1);
    ASSERT_EQ(spec.getGenerals().front().getID(), base.getID());
    ASSERT_EQ(spec.getOwnedAttributes().size(), 1);
    Property& prop = spec.getOwnedAttributes().front();
    ASSERT_EQ(prop.getRedefinedProperties().size(), 1);
    ASSERT_EQ(prop.getRedefinedProperties().front().getID(), redefined.getID());
}

TEST_F(PropertyParserTest, emitRedefinedPropertyTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Property& prop = m.create<Property>();
    Property& redefined = m.create<Property>();
    Class& b = m.create<Class>();
    Class& s = m.create<Class>();
    Generalization& gen = m.create<Generalization>();
    pckg.setID("RC5KnOAfUJQY6BnxohDHLqrMadYI");
    prop.setID("sVhU3UWy392YuTfewtNoyaWLhAQw");
    redefined.setID("9m50Dir0MgpEaLu8ghn7cSlZ5Yzh");
    b.setID("yzUVzVw8sod2KyBH5LxX_OLI7HrV");
    s.setID("Y8UtleiQO3UuN4GEqSzai0G8&GqC");
    gen.setID("RL5_MDmj_CskU1njfiL74QSxP7Bw");
    s.getGeneralizations().add(gen);
    gen.setGeneral(&b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    pckg.getPackagedElements().add(b);
    pckg.getPackagedElements().add(s);
    string expectedEmit = R""""(package:
  id: RC5KnOAfUJQY6BnxohDHLqrMadYI
  packagedElements:
    - class:
        id: yzUVzVw8sod2KyBH5LxX_OLI7HrV
        ownedAttributes:
          - property:
              id: 9m50Dir0MgpEaLu8ghn7cSlZ5Yzh
    - class:
        id: Y8UtleiQO3UuN4GEqSzai0G8&GqC
        generalizations:
          - generalization:
              id: RL5_MDmj_CskU1njfiL74QSxP7Bw
              general: yzUVzVw8sod2KyBH5LxX_OLI7HrV
        ownedAttributes:
          - property:
              id: sVhU3UWy392YuTfewtNoyaWLhAQw
              redefinedProperties:
                - 9m50Dir0MgpEaLu8ghn7cSlZ5Yzh)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PropertyParserTest, mountPropertyTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    m.setRoot(&pckg);
    m.parse(ymlPath + "uml/primitiveTypes.yml");
    PackageMerge& merge = m.create<PackageMerge>();
    merge.setMergedPackage(&m.get<Package>(ID::fromString("Primitive_Types_WZcyDDLemQ97")));
    pckg.getPackageMerge().add(merge);
    Property& prop = m.create<Property>();
    Property& redefined = m.create<Property>();
    Class& b = m.create<Class>();
    Class& s = m.create<Class>();
    Generalization& gen = m.create<Generalization>();
    LiteralString& defaultValue = m.create<LiteralString>();
    s.getGeneralizations().add(gen);
    gen.setGeneral(&b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    redefined.setType(&m.get<Type>(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT")));
    prop.setDefaultValue(&defaultValue);
    pckg.getPackagedElements().add(b);
    pckg.getPackagedElements().add(s);
    ASSERT_NO_THROW(m.mount(ymlPath + "/propertyTests"));
    // TODO explore tree (probably don't need to)
    ASSERT_NO_THROW(m.release(prop.getID()));
    Property& prop2 = s.getOwnedAttributes().front();
}
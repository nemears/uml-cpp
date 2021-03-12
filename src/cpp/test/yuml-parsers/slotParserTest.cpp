#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/slot.h"
#include "uml/class.h"
#include "uml/primitiveType.h"
#include "uml/instanceSpecification.h"
#include "uml/literalString.h"
#include "uml/instanceValue.h"

using namespace UML;

class SlotParserTest : public ::testing::Test {
    public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(SlotParserTest, ParseValueWithoutFeatureTest) {
    // Setup
    ModelParser valWithoutFeatureParser = ModelParser::createNewParser();
    YAML::Node valWithoutFeatureNode = YAML::LoadFile(ymlPath + "slotTests/valWithoutFeature.yml");

    // Test
    ASSERT_THROW(valWithoutFeatureParser.parse(valWithoutFeatureNode), Slot::NullDefiningFeatureException);
}

TEST_F(SlotParserTest, ParseNullTypeTest) {
    // Setup
    ModelParser valWithNullTypeParser = ModelParser::createNewParser();
    YAML::Node valWithNullTypeNode = YAML::LoadFile(ymlPath + "slotTests/nullType.yml");

    // Test
    ASSERT_THROW(valWithNullTypeParser.parse(valWithNullTypeNode), StructuralFeature::InvalidValueException);
}

TEST_F(SlotParserTest, EmitAbstractValue) {
    // Setup
    Model m;
    Class c;
    Property p;
    Class c2;
    InstanceSpecification i;
    Slot s;
    ValueSpecification vs;
    p.setType(&c2);
    c.ownedAttributes.push_back(&p);
    i.setClassifier(&c);
    s.setDefiningFeature(&p);
    s.values.push_back(&vs);
    i.slots.push_back(&s);
    m.ownedElements.push_back(&c2);
    m.ownedElements.push_back(&c);
    m.ownedElements.push_back(&i);
    ModelParser abstractValueParser = ModelParser::createNewParser();

    // Test
    ASSERT_THROW(abstractValueParser.emitDocument(&m), ElementParser::AbstractTypeEmitException);
}

TEST_F(SlotParserTest, EmitValueW_NullTypeTest) {
    // Setup
    Model m;
    Class c;
    Property p;
    InstanceSpecification i;
    Slot s;
    LiteralString ls;
    c.ownedAttributes.push_back(&p);
    ls.setValue("test");
    s.setDefiningFeature(&p);
    s.values.push_back(&ls);
    i.setClassifier(&c);
    i.slots.push_back(&s);
    m.ownedElements.push_back(&c);
    m.ownedElements.push_back(&i);
    ModelParser valueW_NullTypeParser = ModelParser::createNewParser();

    // Test
    ASSERT_THROW(valueW_NullTypeParser.emitDocument(&m), StructuralFeature::InvalidValueException);
}

TEST_F(SlotParserTest, BackwardsDefiningFeatureTest) {
    // Setup
    ModelParser backwardsDefiningFeatureParser = ModelParser::createNewParser();
    YAML::Node backwardsDefiningFeatureNode = YAML::LoadFile(ymlPath + "slotTests/backwardsDefiningFeature.yml");

    // Test
    ASSERT_NO_THROW(backwardsDefiningFeatureParser.parse(backwardsDefiningFeatureNode));
    ASSERT_TRUE(backwardsDefiningFeatureParser.theEl->ownedElements.size() == 2);
    ASSERT_TRUE(backwardsDefiningFeatureParser.theEl->ownedElements.back()->getID() == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
    ASSERT_TRUE(backwardsDefiningFeatureParser.theEl->ownedElements.front()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsDefiningFeatureParser.theEl->ownedElements.front())->slots.size() == 1);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsDefiningFeatureParser.theEl->ownedElements.front())->slots.front()->getDefiningFeature() != NULL);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsDefiningFeatureParser.theEl->ownedElements.front())->slots.front()->getDefiningFeature()->getID() == dynamic_cast<Classifier*>(backwardsDefiningFeatureParser.theEl->ownedElements.back())->ownedAttributes.front()->getID());
}

TEST_F(SlotParserTest, BackwardsValueTest) {
    // Setup
    ModelParser backwardsValueParser = ModelParser::createNewParser();
    YAML::Node backwardsValueNode = YAML::LoadFile(ymlPath + "slotTests/backwardsValue.yml");

    // Test
    ASSERT_NO_THROW(backwardsValueParser.parse(backwardsValueNode));
    ASSERT_TRUE(backwardsValueParser.theEl->ownedElements.size() == 4);
    ASSERT_TRUE(backwardsValueParser.theEl->ownedElements.front()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.size() == 1);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.front()->getDefiningFeature() !=  NULL);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.front()->getDefiningFeature()->getID() == boost::lexical_cast<boost::uuids::uuid>("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a"));
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.front()->values.size() == 1);
    ASSERT_TRUE(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.front()->values.front()->getElementType() == ElementType::INSTANCE_VALUE);
    ASSERT_TRUE(dynamic_cast<InstanceValue*>(dynamic_cast<InstanceSpecification*>(backwardsValueParser.theEl->ownedElements.front())->slots.front()->values.front())->getInstance()->getID() == boost::lexical_cast<boost::uuids::uuid>("9cdae5be-6b75-4284-b1e3-445fcb3dd071"));
}
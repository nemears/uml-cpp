#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/slot.h"
#include "uml/class.h"
#include "uml/primitiveType.h"
#include "uml/instanceSpecification.h"
#include "uml/literalString.h"

using namespace UML;

class SlotParserTest : public ::testing::Test {

};

TEST_F(SlotParserTest, ParseValueWithoutFeatureTest) {
    // Setup
    ModelParser valWithoutFeatureParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node valWithoutFeatureNode = YAML::LoadFile("../../../../../src/test/yml/slotTests/valWithoutFeature.yml");

    // Test
    ASSERT_THROW(valWithoutFeatureParser.parse(valWithoutFeatureNode), Slot::NullDefiningFeatureException);
}

TEST_F(SlotParserTest, ParseNullTypeTest) {
    // Setup
    ModelParser valWithNullTypeParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node valWithNullTypeNode = YAML::LoadFile("../../../../../src/test/yml/slotTests/nullType.yml");

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
    ModelParser abstractValueParser(new map<boost::uuids::uuid, Element*>);

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
    ModelParser valueW_NullTypeParser(new map<boost::uuids::uuid, Element*>);

    // Test
    ASSERT_THROW(valueW_NullTypeParser.emitDocument(&m), StructuralFeature::InvalidValueException);
}
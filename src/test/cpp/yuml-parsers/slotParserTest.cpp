#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/slot.h"

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
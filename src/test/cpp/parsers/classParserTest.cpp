#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/uml/headers/class.h"

using namespace UML;

class ClassParserTest : public ::testing::Test {

};

TEST_F(ClassParserTest, testBasicAttributesAndOperations) {
    ModelParser pp(new map<boost::uuids::uuid, Element*>);
    YAML::Node node = YAML::LoadFile("../../../../../src/test/yml/classTests/operation.yml");
    ASSERT_NO_THROW(pp.parse(node));
}
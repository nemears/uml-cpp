#include "gtest/gtest.h"
#include "uml/model.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/primitiveType.h"
#include "uml/opaqueBehavior.h"
#include "yuml-parsers/modelParser.h"

using namespace UML;

class OperationParserTest : public ::testing::Test {

};

TEST_F(OperationParserTest, ParseInvalidParameterTest) {
  // Setup
  ModelParser invalidParameterParser(new map<boost::uuids::uuid, Element*>);
  YAML::Node invalidParameterNode = YAML::LoadFile("../../../../../src/test/yml/operationTests/invalidParameterList.yml");

  // Test
  EXPECT_THROW(invalidParameterParser.parse(invalidParameterNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(OperationParserTest, ParseInvalidMethodTest) {
  ModelParser invalidMethodParser(new map<boost::uuids::uuid, Element*>);
  YAML::Node invalidMethodNode = YAML::LoadFile("../../../../../src/test/yml/operationTests/invalidMethodList.yml");

  EXPECT_THROW(invalidMethodParser.parse(invalidMethodNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(OperationParserTest, ParseInvalidTypeTest) {
  // Setup
  ModelParser invalidTypeParser(new map<boost::uuids::uuid, Element*>);
  YAML::Node invalidTypeNode = YAML::LoadFile("../../../../../src/test/yml/operationTests/invalidType.yml");

  // Test
  EXPECT_THROW(invalidTypeParser.parse(invalidTypeNode), ElementParser::InvalidIdentifierException);
}

TEST_F(OperationParserTest, EmitOperationWithLiteralParameter) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Parameter p;
  p.setID("563f4740-e107-4d08-8618-2489f0fe1865");
  p.setName("test");
  PrimitiveType pt;
  pt.setPrimitiveType(PrimitiveType::Primitive::STRING);
  p.setType(&pt);
  o.parameters.push_back(&p);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithParameterTestParser(new map<boost::uuids::uuid, Element*>);
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              parameters:
                - parameter:
                    type: STRING
                    name: test
                    id: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithParameterTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(OperationParserTest, EmitOperationWithInstanceParameter) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c2;
  c2.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Parameter p;
  p.setID("563f4740-e107-4d08-8618-2489f0fe1865");
  p.setName("test");
  p.setType(&c2);
  o.parameters.push_back(&p);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c2);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithInstanceParameterTestParser(new map<boost::uuids::uuid, Element*>);
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              parameters:
                - parameter:
                    type: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
                    name: test
                    id: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithInstanceParameterTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(OperationParserTest, EmitOperationWithBlankOpaqueBehaviorTest) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  OpaqueBehavior ob;
  ob.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
  ob.setName("foo");
  o.methods.push_back(&ob);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithBlankOpaqueBehaviorTestParser(new map<boost::uuids::uuid, Element*>);
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              methods:
                - opaqueBehavior:
                    name: foo
                    id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithBlankOpaqueBehaviorTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}
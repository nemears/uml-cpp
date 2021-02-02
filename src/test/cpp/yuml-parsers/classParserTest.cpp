#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/primitiveType.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class ClassParserTest : public ::testing::Test {

};

TEST_F(ClassParserTest, testBasicOperations) {
    ModelParser pp(new map<boost::uuids::uuid, Element*>);
    YAML::Node node = YAML::LoadFile("../../../../../src/test/yml/classTests/operation.yml");
    ASSERT_NO_THROW(pp.parse(node));
    ASSERT_TRUE((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")]);
    ASSERT_TRUE(((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.empty() == false);
    ASSERT_TRUE(((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->parameters.empty() == false);
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->parameters.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    ASSERT_TRUE(((Parameter*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->parameters.front())->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Parameter*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->parameters.front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(((Parameter*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->parameters.front())->getName().compare("debug") == 0);
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->methods.empty() == false);
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->methods.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->methods.front()->getName().compare("isValid") == 0);
    ASSERT_TRUE(((OpaqueBehavior*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->methods.front())->bodies.empty() == false);
    ASSERT_TRUE(((OpaqueBehavior*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->methods.front())->bodies.front().compare("return true") == 0);
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->getType() != NULL);
    ASSERT_TRUE(((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Operation*)((Class*)(*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->operations.front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
}

TEST_F(ClassParserTest, EmitOperationTest) {
    Model m;
    m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    Class c;
    c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    Operation o;
    o.setID("563f4740-e107-4d08-8618-2489f0fe1865");
    c.operations.push_back(&o);
    m.ownedElements.push_back(&c);

    ModelParser emitBasicOperationParser(new map<boost::uuids::uuid, Element*>);
    string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitBasicOperationParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}
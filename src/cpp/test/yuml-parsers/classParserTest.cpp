#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/primitiveType.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class ClassParserTest : public ::testing::Test {
  public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ClassParserTest, testBasicOperations) {
    ModelParser pp = ModelParser::createNewParser();
    YAML::Node node = YAML::LoadFile(ymlPath + "classTests/operation.yml");
    ASSERT_NO_THROW(pp.parse(node));
    ASSERT_TRUE((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")]);
    ASSERT_TRUE(dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().empty() == false);
    ASSERT_TRUE(dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front()->getID() == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().empty() == false);
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getID() == boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getName().compare("isValid") == 0);
    ASSERT_TRUE(((OpaqueBehavior*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front())->bodies.empty() == false);
    ASSERT_TRUE(((OpaqueBehavior*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front())->bodies.front().compare("return true") == 0);
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getParameters().empty() == false);
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getParameters().front()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    ASSERT_TRUE(((Parameter*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getParameters().front())->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Parameter*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getParameters().front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(((Parameter*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getMethods().front()->getParameters().front())->getName().compare("debug") == 0);
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getType() != NULL);
    ASSERT_TRUE(((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Operation*)dynamic_cast<Class*>((*pp.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getOperations().front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
}

TEST_F(ClassParserTest, NonSequenceOperationsExceptionTest) {
  // Setup
  ModelParser nonSequenceOperationParser = ModelParser::createNewParser();
  YAML::Node nonSequenceOperationNode = YAML::LoadFile(ymlPath + "classTests/invalidOperation.yml");

  // Test
  ASSERT_THROW(nonSequenceOperationParser.parse(nonSequenceOperationNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(ClassParserTest, EmitOperationTest) {
    Model m;
    m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    Class c;
    c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    Operation o;
    o.setID("563f4740-e107-4d08-8618-2489f0fe1865");
    c.getOperations().add(o);
    m.getOwnedElements().add(c);

    ModelParser emitBasicOperationParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        children:
          - 563f4740-e107-4d08-8618-2489f0fe1865
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
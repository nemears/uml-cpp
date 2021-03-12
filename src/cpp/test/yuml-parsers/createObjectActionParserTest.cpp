#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/createObjectAction.h"
#include "uml/primitiveType.h"
#include "uml/class.h"

using namespace UML;

class CreateObjectParserTest : public ::testing::Test {
  public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(CreateObjectParserTest, ParseLiteralClassifierTest) {
    // Setup
    ModelParser literalClassifierParser = ModelParser::createNewParser();
    YAML::Node literalClassifierNode = YAML::LoadFile(ymlPath + "createObjectActionTests/primitiveClassifier.yml");

    // Test
    ASSERT_NO_THROW(literalClassifierParser.parse(literalClassifierNode));
    ASSERT_TRUE(literalClassifierParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(literalClassifierParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    
    // Activity
    Activity* act = dynamic_cast<Activity*>(literalClassifierParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(act->nodes.size() == 2);
    ASSERT_TRUE(act->nodes.front()->getElementType() == ElementType::CREATE_OBJECT_ACTION);

    // CreateObjectAction
    CreateObjectAction* coa = dynamic_cast<CreateObjectAction*>(act->nodes.front());
    ASSERT_TRUE(coa->getClassifier() != NULL);
    ASSERT_TRUE(coa->getClassifier()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(coa->getClassifier())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(coa->outputs.size() == 1);
    ASSERT_TRUE(coa->outputs.front()->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(coa->outputs.front()->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);

    // Tear down
    ModelParser::deleteParser(&literalClassifierParser);
}

TEST_F(CreateObjectParserTest, ParseClassClassifierTest) {
    // Setup
    ModelParser classClassifierParser = ModelParser::createNewParser();
    YAML::Node classClassifierNode = YAML::LoadFile(ymlPath +  "createObjectActionTests/classClassifier.yml");

    // Test
    ASSERT_NO_THROW(classClassifierParser.parse(classClassifierNode));
    ASSERT_TRUE(classClassifierParser.theEl->getOwnedElements().size() == 2);
    ASSERT_TRUE(classClassifierParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(classClassifierParser.theEl->getOwnedElements().back()->getElementType() == ElementType::CLASS);
    
    // Activity
    Activity* act = dynamic_cast<Activity*>(classClassifierParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(act->nodes.size() == 2);
    ASSERT_TRUE(act->nodes.front()->getElementType() == ElementType::CREATE_OBJECT_ACTION);

    // CreateObjectAction
    CreateObjectAction* coa = dynamic_cast<CreateObjectAction*>(act->nodes.front());
    ASSERT_TRUE(coa->getClassifier() != NULL);
    ASSERT_TRUE(coa->getClassifier()->getElementType() == ElementType::CLASS);

    // Test corespondence to class
    Class* clazz = dynamic_cast<Class*>(classClassifierParser.theEl->getOwnedElements().back());
    ASSERT_TRUE(coa->getClassifier()->getID() == clazz->getID());
    ASSERT_TRUE(coa->getClassifier() == clazz);

    // OutputPin
    ASSERT_TRUE(coa->outputs.size() == 1);
    ASSERT_TRUE(coa->outputs.front()->getType()->getElementType() == ElementType::CLASS);
    ASSERT_TRUE(coa->outputs.front()->getType()->getID() == clazz->getID());
    ASSERT_TRUE(coa->outputs.front()->getType() == clazz);

    // Tear down
    ModelParser::deleteParser(&classClassifierParser);
}

TEST_F(CreateObjectParserTest, EmitCreateObjectActionTest) {
    Model m;
    m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    Activity a;
    a.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    CreateObjectAction coa1;
    coa1.setID("c6681a3f-ea10-4069-a4e8-bb9e1cc4bb0b");
    CreateObjectAction coa2;
    coa2.setID("9a8b4d56-45fe-4041-bac5-6ed8319064a9");
    OutputPin p1;
    p1.setID("2ebcd640-e405-4b15-bf4b-805ae7f69457");
    OutputPin p2;
    p2.setID("dea1b287-79d1-43f0-ad70-2c19275aa9c8");
    PrimitiveType boolType;
    Class c;
    c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    boolType.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    coa1.setClassifier(&boolType);
    p1.setType(&boolType);
    coa1.outputs.push_back(&p1);
    coa2.setClassifier(&c);
    p2.setType(&c);
    coa2.outputs.push_back(&p2);
    coa1.setActivity(&a);
    coa2.setActivity(&a);
    a.nodes.push_back(&coa1);
    a.nodes.push_back(&p1);
    a.nodes.push_back(&coa2);
    a.nodes.push_back(&p2);
    m.getOwnedElements().add(a);
    a.setOwner(&m);
    m.getOwnedElements().add(c);
    c.setOwner(&m);

    ModelParser emitCreateObjectActionParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - activity:
        id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        nodes:
          - createObjectAction:
              id: c6681a3f-ea10-4069-a4e8-bb9e1cc4bb0b
              outputs:
                - 2ebcd640-e405-4b15-bf4b-805ae7f69457
              classifier: BOOL
          - outputPin:
              id: 2ebcd640-e405-4b15-bf4b-805ae7f69457
              type: BOOL
          - createObjectAction:
              id: 9a8b4d56-45fe-4041-bac5-6ed8319064a9
              outputs:
                - dea1b287-79d1-43f0-ad70-2c19275aa9c8
              classifier: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
          - outputPin:
              id: dea1b287-79d1-43f0-ad70-2c19275aa9c8
              type: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitCreateObjectActionParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}
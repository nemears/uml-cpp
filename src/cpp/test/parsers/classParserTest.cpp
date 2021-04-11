#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"

using namespace UML;

class ClassParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ClassParserTest, parseID_andName) {
    Element* el = Parsers::parse(YAML::LoadFile(ymlPath + "classTests/class_w_id_and_name.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getID() == boost::lexical_cast<boost::uuids::uuid>("54e8f139-9581-48a4-8021-32ff00606c93"));
    ASSERT_TRUE(clazz->getName().compare("test") == 0);
}

TEST_F(ClassParserTest, parseBasicProperty) {
    Element* el = Parsers::parse(YAML::LoadFile(ymlPath + "classTests/classWithAttributes.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getAttributes().size() == 2);
    Property* prop1 = dynamic_cast<Property*>(clazz->getAttributes().front());
    Property* prop2 = dynamic_cast<Property*>(clazz->getAttributes().back());
    ASSERT_TRUE(prop1->getClassifier() == clazz);
    ASSERT_TRUE(prop1->getNamespace() == clazz);
    ASSERT_TRUE(prop1->getOwner() == clazz);
    ASSERT_TRUE(clazz->getMembers().front() == prop1);
    ASSERT_TRUE(clazz->getOwnedElements().front() == prop1);
    ASSERT_TRUE(prop2->getClassifier() == clazz);
    ASSERT_TRUE(prop2->getNamespace() == clazz);
    ASSERT_TRUE(prop2->getOwner() == clazz);
    ASSERT_TRUE(clazz->getMembers().back() == prop2);
    ASSERT_TRUE(clazz->getOwnedElements().back() == prop2);
}
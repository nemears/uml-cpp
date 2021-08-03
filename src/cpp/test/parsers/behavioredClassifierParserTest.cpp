#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"
#include "uml/opaqueBehavior.h"

using namespace std;
using namespace UML;

class BehavioredClassifierParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(BehavioredClassifierParserTest, simpleClassTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "behavioredClassifierTests/simpleClass.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class& clazz = el->as<Class>();
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getElementType(), ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior& bhv = clazz.getOwnedBehaviors().front()->as<OpaqueBehavior>();
    ASSERT_TRUE(clazz.getClassifierBehavior() != 0);
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
}

TEST_F(BehavioredClassifierParserTest, simpleClassEmitTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    clazz.setID("E0q8HmS9yU_Qk9ct2XLjnuJOXxwc");
    bhv.setID("C1cleV_7fGQEgHdOfOYZ319RoCNu");
    clazz.setClassifierBehavior(&bhv);
    string expectedEmit = R""""(class:
  id: E0q8HmS9yU_Qk9ct2XLjnuJOXxwc
  ownedBehaviors:
    - opaqueBehavior:
        id: C1cleV_7fGQEgHdOfOYZ319RoCNu
  classifierBehavior: C1cleV_7fGQEgHdOfOYZ319RoCNu)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(clazz));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
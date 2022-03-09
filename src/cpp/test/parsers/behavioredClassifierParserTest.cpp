#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "test/umlTestUtil.h"
#include "uml/uml-stable.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "behavioredClassifierTests/simpleClass.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class& clazz = el->as<Class>();
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getElementType(), ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior& bhv = clazz.getOwnedBehaviors().front().as<OpaqueBehavior>();
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
}

TEST_F(BehavioredClassifierParserTest, simpleClassEmitTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.setID("E0q8HmS9yU_Qk9ct2XLjnuJOXxwc");
    bhv.setID("C1cleV_7fGQEgHdOfOYZ319RoCNu");
    clazz.setClassifierBehavior(bhv);
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

TEST_F(BehavioredClassifierParserTest, mountFullBehavioredClassifierTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& classifierBehavior = *m.create<OpaqueBehavior>();
    OpaqueBehavior& ownedBehavior = *m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(classifierBehavior);
    clazz.getOwnedBehaviors().add(ownedBehavior);
    m.setRoot(&clazz);
    m.mount(ymlPath + "behavioredClassifierTests");

    ID clazzID = clazz.getID();
    ID classifierBehaviorID = classifierBehavior.getID();
    ID ownedBehaviorID = ownedBehavior.getID();
    m.release(clazz);
    ASSERT_FALSE(m.loaded(clazzID));
    Class& clazz2 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(clazz2.getClassifierBehavior());
    ASSERT_EQ(*clazz2.getClassifierBehavior(), classifierBehavior);
    ASSERT_EQ(clazz2.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz2.getOwnedBehaviors().contains(classifierBehavior));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(classifierBehavior, clazz2));
    ASSERT_TRUE(clazz2.getOwnedBehaviors().contains(ownedBehavior));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(ownedBehavior, clazz2));

    m.release(classifierBehavior, clazz2);
    ASSERT_FALSE(m.loaded(classifierBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    Class& clazz3 = m.aquire(clazzID)->as<Class>();
    ASSERT_EQ(clazz3.getOwnedBehaviors().size(), 2);
    ASSERT_EQ(clazz3.getOwnedMembers().size(), 2);
    ASSERT_EQ(clazz3.getMembers().size(), 2);
    ASSERT_EQ(clazz3.getOwnedElements().size(), 2);
    OpaqueBehavior& classifierBehavior2 = m.aquire(classifierBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(clazz3.getClassifierBehavior());
    ASSERT_EQ(*clazz3.getClassifierBehavior(), classifierBehavior2);
    ASSERT_EQ(clazz3.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz3.getOwnedBehaviors().contains(classifierBehavior2));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(classifierBehavior2, clazz3));

    m.release(clazz3, classifierBehavior2);
    ASSERT_FALSE(m.loaded(classifierBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    OpaqueBehavior& classifierBehavior3 = m.aquire(classifierBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(classifierBehavior3.getNamespace());
    ASSERT_TRUE(classifierBehavior3.getOwner());
    Class& clazz4 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(clazz4.getClassifierBehavior());
    ASSERT_EQ(*clazz4.getClassifierBehavior(), classifierBehavior3);
    ASSERT_EQ(clazz4.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz4.getOwnedBehaviors().contains(classifierBehavior3));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(classifierBehavior3, clazz4));

    m.release(ownedBehavior, clazz4);
    ASSERT_FALSE(m.loaded(ownedBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    OpaqueBehavior& ownedBehavior2 = m.aquire(ownedBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(ownedBehavior2.getNamespace());
    ASSERT_TRUE(ownedBehavior2.getOwner());
    Class& clazz5 = m.aquire(clazzID)->as<Class>();
    ASSERT_EQ(clazz5.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz5.getOwnedBehaviors().contains(ownedBehavior2));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(ownedBehavior2, clazz5));

    m.release(ownedBehavior2, clazz5);
    ASSERT_FALSE(m.loaded(ownedBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    Class& clazz6 = m.aquire(clazzID)->as<Class>();
    ASSERT_EQ(clazz6.getOwnedBehaviors().size(), 2);
    ASSERT_EQ(clazz6.getOwnedMembers().size(), 2);
    ASSERT_EQ(clazz6.getMembers().size(), 2);
    ASSERT_EQ(clazz6.getOwnedElements().size(), 2);
    OpaqueBehavior& ownedBehavior3 = m.aquire(ownedBehaviorID)->as<OpaqueBehavior>();
    ASSERT_EQ(clazz6.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz6.getOwnedBehaviors().contains(ownedBehavior3));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(ownedBehavior3, clazz6));
}
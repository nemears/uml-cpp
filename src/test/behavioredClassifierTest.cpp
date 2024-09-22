#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(BehavioredClassifierOwnedBehavior, OpaqueBehavior, Class, &BehavioredClassifier::getOwnedBehaviors)
UML_SINGLETON_INTEGRATION_TEST(BehavioredClassifierClassifierBehavior, OpaqueBehavior, Class, &BehavioredClassifier::getClassifierBehavior, &BehavioredClassifier::setClassifierBehavior)

class BehavioredClassifierTest : public ::testing::Test {
   public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(BehavioredClassifierTest, addAndRemoveOwnedBehaviorTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.getOwnedBehaviors().add(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front()->getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace());
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    clazz.getOwnedBehaviors().remove(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
}

TEST_F(BehavioredClassifierTest, setAndRemoveClassifierBehaviorTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(bhv);
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front()->getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace());
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    OpaqueBehavior& b2 = *m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(&b2);
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front()->getID(), b2.getID());
    ASSERT_TRUE(b2.getNamespace());
    ASSERT_EQ(b2.getNamespace()->getID(), clazz.getID());
    ASSERT_FALSE(bhv.getNamespace());
    clazz.setClassifierBehavior(0);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
    ASSERT_FALSE(b2.getNamespace());
}

TEST_F(BehavioredClassifierTest, setClassifierBehaviorTest) {
    UmlManager m;
    Class& a = *m.create<Class>();
    OpaqueBehavior& ab = *m.create<OpaqueBehavior>();
    a.setClassifierBehavior(ab);

    ASSERT_TRUE(a.getClassifierBehavior());
    ASSERT_EQ(a.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(a.getOwnedMembers().size(), 1);
    ASSERT_EQ(a.getMembers().size(), 1);
    ASSERT_EQ(a.getOwnedElements().size(), 1);

    ASSERT_EQ(*a.getClassifierBehavior(), ab);
    ASSERT_TRUE(a.getOwnedBehaviors().contains(ab));
    ASSERT_TRUE(a.getOwnedMembers().contains(ab));
    ASSERT_TRUE(a.getMembers().contains(ab));
    ASSERT_TRUE(a.getOwnedElements().contains(ab));

    ASSERT_TRUE(ab.getNamespace());
    ASSERT_TRUE(ab.getOwner());
    ASSERT_EQ(*ab.getNamespace(), a);
    ASSERT_EQ(*ab.getOwner(), a);


    Class& b = *m.create<Class>();
    OpaqueBehavior& bb = *m.create<OpaqueBehavior>();
    b.getOwnedBehaviors().add(bb);

    ASSERT_EQ(b.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_EQ(b.getOwnedElements().size(), 1);

    ASSERT_TRUE(b.getOwnedBehaviors().contains(bb));
    ASSERT_TRUE(b.getOwnedMembers().contains(bb));
    ASSERT_TRUE(b.getMembers().contains(bb));
    ASSERT_TRUE(b.getOwnedElements().contains(bb));

    ASSERT_TRUE(bb.getNamespace());
    ASSERT_TRUE(bb.getOwner());
    ASSERT_EQ(*bb.getNamespace(), b);
    ASSERT_EQ(*bb.getOwner(), b);

    b.setClassifierBehavior(bb);

    ASSERT_TRUE(b.getClassifierBehavior());
    ASSERT_EQ(b.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_EQ(b.getOwnedElements().size(), 1);

    ASSERT_EQ(*b.getClassifierBehavior(), bb);
    ASSERT_TRUE(b.getOwnedBehaviors().contains(bb));
    ASSERT_TRUE(b.getOwnedMembers().contains(bb));
    ASSERT_TRUE(b.getMembers().contains(bb));
    ASSERT_TRUE(b.getOwnedElements().contains(bb));

    ASSERT_TRUE(bb.getNamespace());
    ASSERT_TRUE(bb.getOwner());
    ASSERT_EQ(*bb.getNamespace(), b);
    ASSERT_EQ(*bb.getOwner(), b);
}

TEST_F(BehavioredClassifierTest, simpleClassTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "behavioredClassifierTests/simpleClass.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), Class::Info::elementType);
    Class& clazz = m.getRoot()->as<Class>();
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getElementType(), OpaqueBehavior::Info::elementType);
    OpaqueBehavior& bhv = clazz.getOwnedBehaviors().front()->as<OpaqueBehavior>();
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
}

TEST_F(BehavioredClassifierTest, simpleClassEmitTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.setID(ID::fromString("E0q8HmS9yU_Qk9ct2XLjnuJOXxwc"));
    bhv.setID(ID::fromString("C1cleV_7fGQEgHdOfOYZ319RoCNu"));
    clazz.setClassifierBehavior(bhv);
    std::string expectedEmit = R""""(Class:
  id: E0q8HmS9yU_Qk9ct2XLjnuJOXxwc
  classifierBehavior:
    OpaqueBehavior:
      id: C1cleV_7fGQEgHdOfOYZ319RoCNu)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(clazz));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(BehavioredClassifierTest, mountFullBehavioredClassifierTest) {
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
    Class& clazz2 = m.get(clazzID)->as<Class>();
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
    Class& clazz3 = m.get(clazzID)->as<Class>();
    ASSERT_EQ(clazz3.getOwnedBehaviors().size(), 2);
    ASSERT_EQ(clazz3.getOwnedMembers().size(), 2);
    ASSERT_EQ(clazz3.getMembers().size(), 2);
    ASSERT_EQ(clazz3.getOwnedElements().size(), 2);
    OpaqueBehavior& classifierBehavior2 = m.get(classifierBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(clazz3.getClassifierBehavior());
    ASSERT_EQ(*clazz3.getClassifierBehavior(), classifierBehavior2);
    ASSERT_EQ(clazz3.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz3.getOwnedBehaviors().contains(classifierBehavior2));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(classifierBehavior2, clazz3));

    m.release(clazz3, classifierBehavior2);
    ASSERT_FALSE(m.loaded(classifierBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    OpaqueBehavior& classifierBehavior3 = m.get(classifierBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(classifierBehavior3.getNamespace());
    ASSERT_TRUE(classifierBehavior3.getOwner());
    Class& clazz4 = m.get(clazzID)->as<Class>();
    ASSERT_TRUE(clazz4.getClassifierBehavior());
    ASSERT_EQ(*clazz4.getClassifierBehavior(), classifierBehavior3);
    ASSERT_EQ(clazz4.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz4.getOwnedBehaviors().contains(classifierBehavior3));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(classifierBehavior3, clazz4));

    m.release(ownedBehavior, clazz4);
    ASSERT_FALSE(m.loaded(ownedBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    OpaqueBehavior& ownedBehavior2 = m.get(ownedBehaviorID)->as<OpaqueBehavior>();
    ASSERT_TRUE(ownedBehavior2.getNamespace());
    ASSERT_TRUE(ownedBehavior2.getOwner());
    Class& clazz5 = m.get(clazzID)->as<Class>();
    ASSERT_EQ(clazz5.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz5.getOwnedBehaviors().contains(ownedBehavior2));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(ownedBehavior2, clazz5));

    m.release(ownedBehavior2, clazz5);
    ASSERT_FALSE(m.loaded(ownedBehaviorID));
    ASSERT_FALSE(m.loaded(clazzID));
    Class& clazz6 = m.get(clazzID)->as<Class>();
    ASSERT_EQ(clazz6.getOwnedBehaviors().size(), 2);
    ASSERT_EQ(clazz6.getOwnedMembers().size(), 2);
    ASSERT_EQ(clazz6.getMembers().size(), 2);
    ASSERT_EQ(clazz6.getOwnedElements().size(), 2);
    OpaqueBehavior& ownedBehavior3 = m.get(ownedBehaviorID)->as<OpaqueBehavior>();
    ASSERT_EQ(clazz6.getOwnedBehaviors().size(), 2);
    ASSERT_TRUE(clazz6.getOwnedBehaviors().contains(ownedBehavior3));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(ownedBehavior3, clazz6));
}

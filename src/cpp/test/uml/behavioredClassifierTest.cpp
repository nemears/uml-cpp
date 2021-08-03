#include "gtest/gtest.h"
#include "uml/opaqueBehavior.h"
#include "uml/class.h"

using namespace UML;

class BehavioredClassifierTest : public ::testing::Test {
   
};

TEST_F(BehavioredClassifierTest, addAndRemoveOwnedBehaviorTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    clazz.getOwnedBehaviors().add(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front()->getID(), bhv.getID());
    ASSERT_TRUE(bhv.getBehavioredClassifier() != 0);
    ASSERT_EQ(bhv.getBehavioredClassifier()->getID(), clazz.getID());
    ASSERT_TRUE(bhv.getNamespace() != 0);
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    clazz.getOwnedBehaviors().remove(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
    ASSERT_TRUE(bhv.getBehavioredClassifier() == 0);
}

TEST_F(BehavioredClassifierTest, setandOverrideBehavioredClassifierTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    bhv.setBehavioredClassifier(&clazz);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front()->getID(), bhv.getID());
    ASSERT_TRUE(bhv.getBehavioredClassifier() != 0);
    ASSERT_EQ(bhv.getBehavioredClassifier()->getID(), clazz.getID());
    ASSERT_TRUE(bhv.getNamespace() != 0);
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    Class& c2 = m.create<Class>();
    bhv.setBehavioredClassifier(&c2);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
    ASSERT_EQ(c2.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(c2.getOwnedBehaviors().front()->getID(), bhv.getID());
    ASSERT_EQ(c2.getOwnedMembers().size(), 1);
    ASSERT_EQ(c2.getOwnedMembers().front()->getID(), bhv.getID());
    ASSERT_TRUE(bhv.getBehavioredClassifier() != 0);
    ASSERT_EQ(bhv.getBehavioredClassifier()->getID(), c2.getID());
    ASSERT_TRUE(bhv.getNamespace() != 0);
    ASSERT_EQ(bhv.getNamespace()->getID(), c2.getID());
}
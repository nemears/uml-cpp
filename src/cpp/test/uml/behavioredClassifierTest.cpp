#include "gtest/gtest.h"
#include "uml/opaqueBehavior.h"
#include "uml/class.h"
#include "uml/uml-stable.h"

using namespace UML;

class BehavioredClassifierTest : public ::testing::Test {
   
};

TEST_F(BehavioredClassifierTest, addAndRemoveOwnedBehaviorTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.getOwnedBehaviors().add(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace());
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    clazz.getOwnedBehaviors().remove(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
}

// TEST_F(BehavioredClassifierTest, setandOverrideBehavioredClassifierTest) {
//     UmlManager m;
//     Class& clazz = m.create<Class>();
//     OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
//     bhv.setBehavioredClassifier(&clazz);
//     ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
//     ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), bhv.getID());
//     ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
//     ASSERT_EQ(clazz.getOwnedMembers().front().getID(), bhv.getID());
//     ASSERT_TRUE(bhv.getBehavioredClassifier() != 0);
//     ASSERT_EQ(bhv.getBehavioredClassifier()->getID(), clazz.getID());
//     ASSERT_TRUE(bhv.getNamespace() != 0);
//     ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
//     Class& c2 = m.create<Class>();
//     bhv.setBehavioredClassifier(&c2);
//     ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
//     ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
//     ASSERT_EQ(c2.getOwnedBehaviors().size(), 1);
//     ASSERT_EQ(c2.getOwnedBehaviors().front().getID(), bhv.getID());
//     ASSERT_EQ(c2.getOwnedMembers().size(), 1);
//     ASSERT_EQ(c2.getOwnedMembers().front().getID(), bhv.getID());
//     ASSERT_TRUE(bhv.getBehavioredClassifier() != 0);
//     ASSERT_EQ(bhv.getBehavioredClassifier()->getID(), c2.getID());
//     ASSERT_TRUE(bhv.getNamespace() != 0);
//     ASSERT_EQ(bhv.getNamespace()->getID(), c2.getID());
// }

TEST_F(BehavioredClassifierTest, setAndRemoveClassifierBehaviorTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(bhv);
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace());
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    OpaqueBehavior& b2 = *m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(&b2);
    ASSERT_TRUE(clazz.getClassifierBehavior());
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), b2.getID());
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
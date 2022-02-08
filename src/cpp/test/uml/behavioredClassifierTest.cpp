#include "gtest/gtest.h"
#include "uml/opaqueBehavior.h"
#include "uml/class.h"
#include "uml/uml-stable.h"

using namespace UML;

class BehavioredClassifierTest : public ::testing::Test {
   
};

TEST_F(BehavioredClassifierTest, addAndRemoveOwnedBehaviorTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    clazz.getOwnedBehaviors().add(bhv);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace() != 0);
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
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(&bhv);
    ASSERT_TRUE(clazz.getClassifierBehavior() != 0);
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), bhv.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), bhv.getID());
    ASSERT_TRUE(bhv.getNamespace() != 0);
    ASSERT_EQ(bhv.getNamespace()->getID(), clazz.getID());
    OpaqueBehavior& b2 = m.create<OpaqueBehavior>();
    clazz.setClassifierBehavior(&b2);
    ASSERT_TRUE(clazz.getClassifierBehavior() != 0);
    ASSERT_EQ(clazz.getClassifierBehavior()->getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(clazz.getOwnedBehaviors().front().getID(), b2.getID());
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().front().getID(), b2.getID());
    ASSERT_TRUE(b2.getNamespace() != 0);
    ASSERT_EQ(b2.getNamespace()->getID(), clazz.getID());
    ASSERT_TRUE(bhv.getNamespace() == 0);
    clazz.setClassifierBehavior(0);
    ASSERT_EQ(clazz.getOwnedBehaviors().size(), 0);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 0);
    ASSERT_TRUE(b2.getNamespace() == 0);
}

TEST_F(BehavioredClassifierTest, setClassifierBehaviorTest) {
    UmlManager m;
    Class& a = m.create<Class>();
    OpaqueBehavior& ab = m.create<OpaqueBehavior>();
    a.setClassifierBehavior(ab);

    ASSERT_TRUE(a.hasClassifierBehavior());
    ASSERT_EQ(a.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(a.getOwnedMembers().size(), 1);
    ASSERT_EQ(a.getMembers().size(), 1);
    ASSERT_EQ(a.getOwnedElements().size(), 1);

    ASSERT_EQ(a.getClassifierBehaviorRef(), ab);
    ASSERT_TRUE(a.getOwnedBehaviors().contains(ab));
    ASSERT_TRUE(a.getOwnedMembers().contains(ab));
    ASSERT_TRUE(a.getMembers().contains(ab));
    ASSERT_TRUE(a.getOwnedElements().contains(ab));

    ASSERT_TRUE(ab.hasNamespace());
    ASSERT_TRUE(ab.hasOwner());
    ASSERT_EQ(ab.getNamespaceRef(), a);
    ASSERT_EQ(ab.getOwnerRef(), a);


    Class& b = m.create<Class>();
    OpaqueBehavior& bb = m.create<OpaqueBehavior>();
    b.getOwnedBehaviors().add(bb);

    ASSERT_EQ(b.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_EQ(b.getOwnedElements().size(), 1);

    ASSERT_TRUE(b.getOwnedBehaviors().contains(bb));
    ASSERT_TRUE(b.getOwnedMembers().contains(bb));
    ASSERT_TRUE(b.getMembers().contains(bb));
    ASSERT_TRUE(b.getOwnedElements().contains(bb));

    ASSERT_TRUE(bb.hasNamespace());
    ASSERT_TRUE(bb.hasOwner());
    ASSERT_EQ(bb.getNamespaceRef(), b);
    ASSERT_EQ(bb.getOwnerRef(), b);

    b.setClassifierBehavior(bb);

    ASSERT_TRUE(b.hasClassifierBehavior());
    ASSERT_EQ(b.getOwnedBehaviors().size(), 1);
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_EQ(b.getOwnedElements().size(), 1);

    ASSERT_EQ(b.getClassifierBehaviorRef(), bb);
    ASSERT_TRUE(b.getOwnedBehaviors().contains(bb));
    ASSERT_TRUE(b.getOwnedMembers().contains(bb));
    ASSERT_TRUE(b.getMembers().contains(bb));
    ASSERT_TRUE(b.getOwnedElements().contains(bb));

    ASSERT_TRUE(bb.hasNamespace());
    ASSERT_TRUE(bb.hasOwner());
    ASSERT_EQ(bb.getNamespaceRef(), b);
    ASSERT_EQ(bb.getOwnerRef(), b);
}
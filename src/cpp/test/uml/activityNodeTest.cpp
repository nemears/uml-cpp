#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ActivityNodeTest : public ::testing::Test {
   
};

TEST_F(ActivityNodeTest, reindexID_test) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityNode& n = *m.create<ActivityNode>();
    a.getNodes().add(n);
    n.setActivity(&a);
    n.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(a.getNodes().get(n.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(n.getID()));
}

TEST_F(ActivityNodeTest, reindexNameTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityNode& n = *m.create<ActivityNode>();
    a.getNodes().add(n);
    n.setActivity(&a);
    n.setName("test");
    ASSERT_NO_THROW(a.getNodes().get("test"));
}

TEST_F(ActivityNodeTest, addIncomingFunctorTest) {
    UmlManager m;
    ActivityNode& n = *m.create<ActivityNode>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    n.getIncoming().add(e);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(*e.getTarget(), n);
}

TEST_F(ActivityNodeTest, setTargetTest) {
    UmlManager m;
    ActivityNode& n = *m.create<ActivityNode>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    e.setTarget(&n);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(*e.getTarget(), n);
}

TEST_F(ActivityNodeTest, addOutgoingFunctorTest) {
    UmlManager m;
    ActivityNode& n = *m.create<ActivityNode>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    n.getOutgoing().add(e);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(*e.getSource(), n);
}

TEST_F(ActivityNodeTest, setSourceTest) {
    UmlManager m;
    ActivityNode& n = *m.create<ActivityNode>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    e.setSource(&n);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(*e.getSource(), n);
}

// TEST_F(ActivityNodeTest, DuplicateEdgeExceptionIncomingTest) {
//     UmlManager m;
//     ActivityNode& n = *m.create<ActivityNode>();
//     ActivityEdge& e = *m.create<ActivityEdge>();
//     n.getIncoming().add(e);
//     ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
//     n.getIncoming().remove(e);
//     e.setTarget(&n);
//     ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
// }

// TEST_F(ActivityNodeTest, DuplicateEdgeExceptionOutgoingTest) {
//     UmlManager m;
//     ActivityNode n = m.create<ActivityNode>();
//     ActivityEdge e = m.create<ActivityEdge>();
//     n.getOutgoing().add(e);
//     ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
//     n.getOutgoing().remove(e);
//     e.setSource(&n);
//     ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
// }
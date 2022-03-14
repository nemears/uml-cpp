#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ActivityEdgeTest : public ::testing::Test {
   
};

TEST_F(ActivityEdgeTest, GetNullTargetTest) {
    UmlManager m;
    ActivityEdge& a = *m.create<ActivityEdge>();
    ASSERT_TRUE(!a.getTarget());
}

TEST_F(ActivityEdgeTest, GetNullSourceTest) {
    UmlManager m;
    ActivityEdge& a = *m.create<ActivityEdge>();
    ASSERT_TRUE(!a.getSource());
}

TEST_F(ActivityEdgeTest, SetRegularTargetTest) {
    UmlManager m;
    ActivityEdge& e = *m.create<ActivityEdge>();
    ActivityNode& n = *m.create<ActivityNode>();
    e.setTarget(n);
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(*e.getTarget(), n);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
}

TEST_F(ActivityEdgeTest, SetRegularSourceTest) {
    UmlManager m;
    ActivityEdge& e = *m.create<ActivityEdge>();
    ActivityNode& n = *m.create<ActivityNode>();
    e.setSource(n);
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(*e.getSource(), n);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
}

TEST_F(ActivityEdgeTest, reindexID_Test) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    ActivityNode& n = *m.create<ActivityNode>();
    e.setSource(n);
    e.setTarget(n);
    a.getNodes().add(n);
    a.getEdges().add(e);
    e.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(n.getIncoming().get(e.getID()));
    ASSERT_NO_THROW(n.getOutgoing().get(e.getID()));
    ASSERT_NO_THROW(a.getEdges().get(e.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(e.getID()));
}

TEST_F(ActivityEdgeTest, reindexNameTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    ActivityNode& n = *m.create<ActivityNode>();
    e.setSource(n);
    e.setTarget(n);
    a.getNodes().add(n);
    a.getEdges().add(e);
    e.setName("test");
    ASSERT_EQ(n.getIncoming().get("test"), e);
    ASSERT_EQ(n.getOutgoing().get("test"), e);
    ASSERT_EQ(a.getEdges().get("test"), e);
    ASSERT_EQ(a.getOwnedElements().get("test"), e);
}

TEST_F(ActivityEdgeTest, overwriteIncomingTest) {
    UmlManager m;
    ActivityNode& p1 = *m.create<ActivityNode>();
    ActivityNode& p2 = *m.create<ActivityNode>();
    ActivityEdge& c = *m.create<ActivityEdge>();
    p1.getIncoming().add(c);
    c.setTarget(p2);
    ASSERT_EQ(p2.getIncoming().size(), 1);
    ASSERT_EQ(p2.getIncoming().front(), c);
    ASSERT_TRUE(c.getTarget());
    ASSERT_EQ(*c.getTarget(), p2);
    ASSERT_EQ(p1.getIncoming().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteIncomingByIncomingAddTest) {
    UmlManager m;
    ActivityNode& p1 = *m.create<ActivityNode>();
    ActivityNode& p2 = *m.create<ActivityNode>();
    ActivityEdge& c = *m.create<ActivityEdge>();
    p1.getIncoming().add(c);
    p2.getIncoming().add(c);
    ASSERT_EQ(p2.getIncoming().size(), 1);
    ASSERT_EQ(p2.getIncoming().front(), c);
    ASSERT_TRUE(c.getTarget());
    ASSERT_EQ(*c.getTarget(), p2);
    ASSERT_EQ(p1.getIncoming().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingTest) {
    UmlManager m;
    ActivityNode& p1 = *m.create<ActivityNode>();
    ActivityNode& p2 = *m.create<ActivityNode>();
    ActivityEdge& c = *m.create<ActivityEdge>();
    p1.getOutgoing().add(c);
    c.setSource(p2);
    ASSERT_EQ(p2.getOutgoing().size(), 1);
    ASSERT_EQ(p2.getOutgoing().front(), c);
    ASSERT_TRUE(c.getSource());
    ASSERT_EQ(*c.getSource(), p2);
    ASSERT_EQ(p1.getOutgoing().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingByOutgoingAddTest) {
    UmlManager m;
    ActivityNode& p1 = *m.create<ActivityNode>();
    ActivityNode& p2 = *m.create<ActivityNode>();
    ActivityEdge& c = *m.create<ActivityEdge>();
    p1.getOutgoing().add(c);
    p2.getOutgoing().add(c);
    ASSERT_EQ(p2.getOutgoing().size(), 1);
    ASSERT_EQ(p2.getOutgoing().front(), c);
    ASSERT_TRUE(c.getSource());
    ASSERT_EQ(*c.getSource(), p2);
    ASSERT_EQ(p1.getOutgoing().size(), 0);
}
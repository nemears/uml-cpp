#include "gtest/gtest.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

class ActivityEdgeTest : public ::testing::Test {
   
};

TEST_F(ActivityEdgeTest, GetNullTargetTest) {
    UmlManager m;
    ActivityEdge a = m.create<ActivityEdge>();
    ASSERT_TRUE(a.getTarget() == NULL);
}

TEST_F(ActivityEdgeTest, GetNullSourceTest) {
    UmlManager m;
    ActivityEdge a = m.create<ActivityEdge>();
    ASSERT_TRUE(a.getSource() == NULL);
}

TEST_F(ActivityEdgeTest, SetRegularTargetTest) {
    UmlManager m;
    ActivityEdge e = m.create<ActivityEdge>();
    ActivityNode n = m.create<ActivityNode>();
    e.setTarget(n);
    ASSERT_TRUE(e.hasTarget());
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(e.getTargetRef(), n);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
}

TEST_F(ActivityEdgeTest, SetRegularSourceTest) {
    UmlManager m;
    ActivityEdge e = m.create<ActivityEdge>();
    ActivityNode n = m.create<ActivityNode>();
    e.setSource(&n);
    ASSERT_TRUE(e.hasSource());
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(e.getSourceRef(), n);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
}

TEST_F(ActivityEdgeTest, reindexID_Test) {
    UmlManager m;
    Activity a = m.create<Activity>();
    ActivityEdge e = m.create<ActivityEdge>();
    ActivityNode n = m.create<ActivityNode>();
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

// TEST_F(ActivityEdgeTest, reindexNameTest) {
//     Activity a;
//     ActivityEdge e;
//     ActivityNode n;
//     e.setSource(&n);
//     e.setTarget(&n);
//     a.getNodes().add(n);
//     a.getEdges().add(e);
//     e.setName("test");
//     ASSERT_TRUE(n.getIncoming().get("test"));
//     ASSERT_TRUE(n.getOutgoing().get("test"));
//     ASSERT_TRUE(a.getEdges().get("test"));
//     //ASSERT_TRUE(a.getMembers().get("test"));
//     ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

TEST_F(ActivityEdgeTest, overwriteIncomingTest) {
    UmlManager m;
    ActivityNode p1 = m.create<ActivityNode>();
    ActivityNode p2 = m.create<ActivityNode>();
    ActivityEdge c = m.create<ActivityEdge>();
    p1.getIncoming().add(c);
    c.setTarget(p2);
    ASSERT_EQ(p2.getIncoming().size(), 1);
    ASSERT_EQ(p2.getIncoming().front(), c);
    ASSERT_TRUE(c.hasTarget());
    ASSERT_TRUE(c.getTarget());
    ASSERT_EQ(c.getTargetRef(), p2);
    ASSERT_EQ(p1.getIncoming().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteIncomingByIncomingAddTest) {
    UmlManager m;
    ActivityNode p1 = m.create<ActivityNode>();
    ActivityNode p2 = m.create<ActivityNode>();
    ActivityEdge c = m.create<ActivityEdge>();
    p1.getIncoming().add(c);
    p2.getIncoming().add(c);
    ASSERT_EQ(p2.getIncoming().size(), 1);
    ASSERT_EQ(p2.getIncoming().front(), c);
    ASSERT_TRUE(c.hasTarget());
    ASSERT_TRUE(c.getTarget());
    ASSERT_EQ(c.getTargetRef(), p2);
    ASSERT_EQ(p1.getIncoming().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingTest) {
    UmlManager m;
    ActivityNode p1 = m.create<ActivityNode>();
    ActivityNode p2 = m.create<ActivityNode>();
    ActivityEdge c = m.create<ActivityEdge>();
    p1.getOutgoing().add(c);
    c.setSource(p2);
    ASSERT_EQ(p2.getOutgoing().size(), 1);
    ASSERT_EQ(p2.getOutgoing().front(), c);
    ASSERT_TRUE(c.hasSource());
    ASSERT_TRUE(c.getSource());
    ASSERT_EQ(c.getSourceRef(), p2);
    ASSERT_EQ(p1.getOutgoing().size(), 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingByOutgoingAddTest) {
    UmlManager m;
    ActivityNode p1 = m.create<ActivityNode>();
    ActivityNode p2 = m.create<ActivityNode>();
    ActivityEdge c = m.create<ActivityEdge>();
    p1.getOutgoing().add(c);
    p2.getOutgoing().add(c);
    ASSERT_EQ(p2.getOutgoing().size(), 1);
    ASSERT_EQ(p2.getOutgoing().front(), c);
    ASSERT_TRUE(c.hasSource());
    ASSERT_TRUE(c.getSource());
    ASSERT_EQ(c.getSourceRef(), p2);
    ASSERT_EQ(p1.getOutgoing().size(), 0);
}
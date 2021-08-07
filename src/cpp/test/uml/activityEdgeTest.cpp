#include "gtest/gtest.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

class ActivityEdgeTest : public ::testing::Test {
   
};

TEST_F(ActivityEdgeTest, GetNullTargetTest) {
    ActivityEdge a;
    ASSERT_TRUE(a.getTarget() == NULL);
}

TEST_F(ActivityEdgeTest, GetNullSourceTest) {
    ActivityEdge a;
    ASSERT_TRUE(a.getSource() == NULL);
}

TEST_F(ActivityEdgeTest, SetRegularTargetTest) {
    ActivityEdge e;
    ActivityNode n;
    e.setTarget(&n);
    ASSERT_TRUE(e.getTarget()->getID() == n.getID());
}

TEST_F(ActivityEdgeTest, SetRegularSourceTest) {
    ActivityEdge e;
    ActivityNode n;
    e.setSource(&n);
    ASSERT_TRUE(e.getSource()->getID() == n.getID());
}

TEST_F(ActivityEdgeTest, reindexID_Test) {
    Activity a;
    ActivityEdge e;
    ActivityNode n;
    e.setSource(&n);
    e.setTarget(&n);
    a.getNodes().add(n);
    a.getEdges().add(e);
    e.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(n.getIncoming().get(e.getID()));
    ASSERT_NO_THROW(n.getOutgoing().get(e.getID()));
    ASSERT_NO_THROW(a.getEdges().get(e.getID()));
    //ASSERT_TRUE(a.getMembers().get(e.getID()));
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
  ActivityNode p1;
  ActivityNode p2;
  ActivityEdge c;
  p1.getIncoming().add(c);
  c.setTarget(&p2);
  ASSERT_TRUE(p2.getIncoming().size() == 1);
  ASSERT_TRUE(&p2.getIncoming().front() == &c);
  ASSERT_TRUE(c.getTarget() == &p2);
  ASSERT_TRUE(p1.getIncoming().size() == 0);
}

TEST_F(ActivityEdgeTest, overwriteIncomingByIncomingAddTest) {
  ActivityNode p1;
  ActivityNode p2;
  ActivityEdge c;
  p1.getIncoming().add(c);
  p2.getIncoming().add(c);
  ASSERT_TRUE(p2.getIncoming().size() == 1);
  ASSERT_TRUE(&p2.getIncoming().front() == &c);
  ASSERT_TRUE(c.getTarget() == &p2);
  ASSERT_TRUE(p1.getIncoming().size() == 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingTest) {
  ActivityNode p1;
  ActivityNode p2;
  ActivityEdge c;
  p1.getOutgoing().add(c);
  c.setSource(&p2);
  ASSERT_TRUE(p2.getOutgoing().size() == 1);
  ASSERT_TRUE(&p2.getOutgoing().front() == &c);
  ASSERT_TRUE(c.getSource() == &p2);
  ASSERT_TRUE(p1.getOutgoing().size() == 0);
}

TEST_F(ActivityEdgeTest, overwriteOutgoingByOutgoingAddTest) {
  ActivityNode p1;
  ActivityNode p2;
  ActivityEdge c;
  p1.getOutgoing().add(c);
  p2.getOutgoing().add(c);
  ASSERT_TRUE(p2.getOutgoing().size() == 1);
  ASSERT_TRUE(&p2.getOutgoing().front() == &c);
  ASSERT_TRUE(c.getSource() == &p2);
  ASSERT_TRUE(p1.getOutgoing().size() == 0);
}
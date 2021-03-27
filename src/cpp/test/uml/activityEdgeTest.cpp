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
    n.setActivity(&a);
    e.setActivity(&a);
    n.setOwner(&a);
    e.setOwner(&a);
    n.setNamespace(&a);
    e.setNamespace(&a);
    a.getMembers().add(n);
    a.getMembers().add(e);
    a.getOwnedElements().add(n);
    a.getOwnedElements().add(e);
    e.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
    ASSERT_TRUE(n.getIncoming().get(e.getID()));
    ASSERT_TRUE(n.getOutgoing().get(e.getID()));
    ASSERT_TRUE(a.getEdges().get(e.getID()));
    ASSERT_TRUE(a.getMembers().get(e.getID()));
    ASSERT_TRUE(a.getOwnedElements().get(e.getID()));
}

TEST_F(ActivityEdgeTest, reindexNameTest) {
    Activity a;
    ActivityEdge e;
    ActivityNode n;
    e.setSource(&n);
    e.setTarget(&n);
    a.getNodes().add(n);
    a.getEdges().add(e);
    n.setActivity(&a);
    e.setActivity(&a);
    n.setOwner(&a);
    e.setOwner(&a);
    n.setNamespace(&a);
    e.setNamespace(&a);
    a.getMembers().add(n);
    a.getMembers().add(e);
    a.getOwnedElements().add(n);
    a.getOwnedElements().add(e);
    e.setName("test");
    ASSERT_TRUE(n.getIncoming().get("test"));
    ASSERT_TRUE(n.getOutgoing().get("test"));
    ASSERT_TRUE(a.getEdges().get("test"));
    ASSERT_TRUE(a.getMembers().get("test"));
    ASSERT_TRUE(a.getOwnedElements().get("test"));
}
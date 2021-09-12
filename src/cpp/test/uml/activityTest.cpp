#include "gtest/gtest.h"
#include "uml/activity.h"
#include "uml/activityNode.h"
#include "uml/activityEdge.h"

using namespace UML;

class ActivityTest : public ::testing::Test {
   
};

TEST_F(ActivityTest, addNodeFunctorTest) {
    UmlManager m;
    Activity a = m.create<Activity>();
    ActivityNode n = m.create<ActivityNode>();
    a.getNodes().add(n);
    ASSERT_EQ(a.getNodes().size(), 1);
    ASSERT_EQ(a.getNodes().front(), n);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(a.getOwnedElements().front(), n);
    ASSERT_TRUE(n.hasActivity());
    ASSERT_TRUE(n.getActivity());
    ASSERT_EQ(n.getActivityRef(), a);
    ASSERT_EQ(n.getOwner(), &a);
}

TEST_F(ActivityTest, setNodeActivityTest) {
    UmlManager m;
    Activity a = m.create<Activity>();
    ActivityNode n = m.create<ActivityNode>();
    n.setActivity(a);
    ASSERT_EQ(a.getNodes().size(), 1);
    ASSERT_EQ(a.getNodes().front(), n);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(a.getOwnedElements().front(), n);
    ASSERT_TRUE(n.hasActivity());
    ASSERT_TRUE(n.getActivity());
    ASSERT_EQ(n.getActivityRef(), a);
    ASSERT_EQ(n.getOwner(), &a);
}

TEST_F(ActivityTest, addEdgeFunctorTest) {
    UmlManager m;
    Activity a = m.create<Activity>();
    ActivityEdge e = m.create<ActivityEdge>();
    a.getEdges().add(e);
    ASSERT_EQ(a.getEdges().size(), 1);
    ASSERT_EQ(a.getEdges().front(), e);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(a.getOwnedElements().front(), e);
    ASSERT_TRUE(e.hasActivity());
    ASSERT_TRUE(e.getActivity());
    ASSERT_EQ(e.getActivityRef(), a);
    ASSERT_EQ(e.getOwner(), &a);
}

TEST_F(ActivityTest, setEdgeActivityTest) {
    UmlManager m;
    Activity a = m.create<Activity>();
    ActivityEdge e = m.create<ActivityEdge>();
    e.setActivity(&a);
    ASSERT_EQ(a.getEdges().size(), 1);
    ASSERT_EQ(a.getEdges().front(), e);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(a.getOwnedElements().front(), e);
    ASSERT_TRUE(e.hasActivity());
    ASSERT_TRUE(e.getActivity());
    ASSERT_EQ(e.getActivityRef(), a);
    ASSERT_EQ(e.getOwner(), &a);
}
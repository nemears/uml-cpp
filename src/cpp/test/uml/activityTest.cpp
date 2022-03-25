#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(ActivityNodes, OpaqueAction, Activity, &Activity::getNodes);
UML_SET_INTEGRATION_TEST(ActivityEdges, ControlFlow, Activity, &Activity::getEdges);
UML_SET_INTEGRATION_TEST(ActivityPartitions, ActivityPartition, Activity, &Activity::getPartitions);

class ActivityTest : public ::testing::Test {
   
};

TEST_F(ActivityTest, addNodeFunctorTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityNode& n = *m.create<ActivityNode>();
    a.getNodes().add(n);
    ASSERT_EQ(a.getNodes().size(), 1);
    ASSERT_EQ(a.getNodes().front(), n);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(*a.getOwnedElements().begin(), n);
    ASSERT_TRUE(n.getActivity());
    ASSERT_EQ(*n.getActivity(), a);
    ASSERT_EQ(*n.getOwner(), a);
}

TEST_F(ActivityTest, setNodeActivityTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityNode& n = *m.create<ActivityNode>();
    n.setActivity(a);
    ASSERT_EQ(a.getNodes().size(), 1);
    ASSERT_EQ(a.getNodes().front(), n);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(*a.getOwnedElements().begin(), n);
    ASSERT_TRUE(n.getActivity());
    ASSERT_EQ(*n.getActivity(), a);
    ASSERT_EQ(*n.getOwner(), a);
}

TEST_F(ActivityTest, addEdgeFunctorTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    a.getEdges().add(e);
    ASSERT_EQ(a.getEdges().size(), 1);
    ASSERT_EQ(a.getEdges().front(), e);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(*a.getOwnedElements().begin(), e);
    ASSERT_TRUE(e.getActivity());
    ASSERT_EQ(*e.getActivity(), a);
    ASSERT_EQ(*e.getOwner(), a);
}

TEST_F(ActivityTest, setEdgeActivityTest) {
    UmlManager m;
    Activity& a = *m.create<Activity>();
    ActivityEdge& e = *m.create<ActivityEdge>();
    e.setActivity(&a);
    ASSERT_EQ(a.getEdges().size(), 1);
    ASSERT_EQ(a.getEdges().front(), e);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(*a.getOwnedElements().begin(), e);
    ASSERT_TRUE(e.getActivity());
    ASSERT_EQ(*e.getActivity(), a);
    ASSERT_EQ(*e.getOwner(), a);
}
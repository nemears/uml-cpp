#include "gtest/gtest.h"
#include "uml/activity.h"
#include "uml/activityNode.h"
#include "uml/activityEdge.h"

using namespace UML;

class ActivityTest : public ::testing::Test {
   
};

TEST_F(ActivityTest, addNodeFunctorTest) {
    Activity a;
    ActivityNode n;
    a.getNodes().add(n);
    ASSERT_TRUE(a.getNodes().size() == 1);
    ASSERT_TRUE(&a.getNodes().front() == &n);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().front() == &n);
    ASSERT_TRUE(n.getActivity() == &a);
    ASSERT_TRUE(n.getOwner() == &a);
}

TEST_F(ActivityTest, setNodeActivityTest) {
    Activity a;
    ActivityNode n;
    n.setActivity(&a);
    ASSERT_TRUE(a.getNodes().size() == 1);
    ASSERT_TRUE(&a.getNodes().front() == &n);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().front() == &n);
    ASSERT_TRUE(n.getActivity() == &a);
    ASSERT_TRUE(n.getOwner() == &a);
}

TEST_F(ActivityTest, addEdgeFunctorTest) {
    Activity a;
    ActivityEdge e;
    a.getEdges().add(e);
    ASSERT_TRUE(a.getEdges().size() == 1);
    ASSERT_TRUE(&a.getEdges().front() == &e);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().front() == &e);
    ASSERT_TRUE(e.getActivity() == &a);
    ASSERT_TRUE(e.getOwner() == &a);
}

TEST_F(ActivityTest, setEdgeActivityTest) {
    Activity a;
    ActivityEdge e;
    e.setActivity(&a);
    ASSERT_TRUE(a.getEdges().size() == 1);
    ASSERT_TRUE(&a.getEdges().front() == &e);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().front() == &e);
    ASSERT_TRUE(e.getActivity() == &a);
    ASSERT_TRUE(e.getOwner() == &a);
}
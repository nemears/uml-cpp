#include "gtest/gtest.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"

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
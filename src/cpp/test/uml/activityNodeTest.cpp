#include "gtest/gtest.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"

using namespace UML;

class ActivityNodeTest : public ::testing::Test {
   
};

TEST_F(ActivityNodeTest, reindexID_test) {
    Activity a;
    ActivityNode n;
    a.getNodes().add(n);
    n.setActivity(&a);
    n.setID("05f66d05-4b6a-46a1-874e-90649c8e4049");
    ASSERT_TRUE(a.getNodes().get(n.getID()));
}

TEST_F(ActivityNodeTest, reindexNameTest) {
    Activity a;
    ActivityNode n;
    a.getNodes().add(n);
    n.setActivity(&a);
    n.setName("test");
    ASSERT_TRUE(a.getNodes().get("test"));
}
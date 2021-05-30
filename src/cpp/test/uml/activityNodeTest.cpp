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

// TEST_F(ActivityNodeTest, reindexNameTest) {
//     Activity a;
//     ActivityNode n;
//     a.getNodes().add(n);
//     n.setActivity(&a);
//     n.setName("test");
//     ASSERT_TRUE(a.getNodes().get("test"));
// }

TEST_F(ActivityNodeTest, addIncomingFunctorTest) {
    ActivityNode n;
    ActivityEdge e;
    n.getIncoming().add(e);
    ASSERT_TRUE(n.getIncoming().size() == 1);
    ASSERT_TRUE(n.getIncoming().front() == &e);
    ASSERT_TRUE(e.getTarget() == &n);
}

TEST_F(ActivityNodeTest, setTargetTest) {
    ActivityNode n;
    ActivityEdge e;
    e.setTarget(&n);
    ASSERT_TRUE(n.getIncoming().size() == 1);
    ASSERT_TRUE(n.getIncoming().front() == &e);
    ASSERT_TRUE(e.getTarget() == &n);
}

TEST_F(ActivityNodeTest, addOutgoingFunctorTest) {
    ActivityNode n;
    ActivityEdge e;
    n.getOutgoing().add(e);
    ASSERT_TRUE(n.getOutgoing().size() == 1);
    ASSERT_TRUE(n.getOutgoing().front() == &e);
    ASSERT_TRUE(e.getSource() == &n);
}

TEST_F(ActivityNodeTest, setSourceTest) {
    ActivityNode n;
    ActivityEdge e;
    e.setSource(&n);
    ASSERT_TRUE(n.getOutgoing().size() == 1);
    ASSERT_TRUE(n.getOutgoing().front() == &e);
    ASSERT_TRUE(e.getSource() == &n);
}

TEST_F(ActivityNodeTest, DuplicateEdgeExceptionIncomingTest) {
    ActivityNode n;
    ActivityEdge e;
    n.getIncoming().add(e);
    ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
    n.getIncoming().remove(e);
    e.setTarget(&n);
    ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
}

TEST_F(ActivityNodeTest, DuplicateEdgeExceptionOutgoingTest) {
    ActivityNode n;
    ActivityEdge e;
    n.getOutgoing().add(e);
    ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
    n.getOutgoing().remove(e);
    e.setSource(&n);
    ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
}
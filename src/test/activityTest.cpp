#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(ActivityNodes, OpaqueAction, Activity, &Activity::getNodes);
UML_SET_INTEGRATION_TEST(ActivityEdges, ControlFlow, Activity, &Activity::getEdges);
UML_SET_INTEGRATION_TEST(ActivityPartitions, ActivityPartition, Activity, &Activity::getPartitions);
UML_SINGLETON_INTEGRATION_TEST(ActivityGroupInActivity, Activity, ActivityPartition, &ActivityGroup::getInActivity, &ActivityGroup::setInActivity);
UML_SET_INTEGRATION_TEST(ActivityPartitionSubPartitions, ActivityPartition, ActivityPartition, &ActivityPartition::getSubPartitions);
UML_SINGLETON_INTEGRATION_TEST(ActivityPartitionSuperPartition, ActivityPartition, ActivityPartition, &ActivityPartition::getSuperPartition, &ActivityPartition::setSuperPartition);
UML_SET_INTEGRATION_TEST(ActivityPartitionNodes, OpaqueAction, ActivityPartition, &ActivityPartition::getNodes);
UML_SET_INTEGRATION_TEST(ActivityNodeInPartition, ActivityPartition, OpaqueAction, &ActivityNode::getInPartitions);
UML_SET_INTEGRATION_TEST(ActivityPartitionEdges, ControlFlow, ActivityPartition, &ActivityPartition::getEdges);
UML_SET_INTEGRATION_TEST(ActivityEdgeInPartion, ActivityPartition, ControlFlow, &ActivityEdge::getInPartitions);
UML_SINGLETON_INTEGRATION_TEST(ActivityPartitionRepresents, Property, ActivityPartition, &ActivityPartition::getRepresents, &ActivityPartition::setRepresents);
// TODO Interruptible regions

class ActivityTest : public ::testing::Test {
   public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ActivityTest, addNodeFunctorTest) {
    Manager<> m;
    Activity& a = *m.create<Activity>();
    OpaqueAction& n = *m.create<OpaqueAction>();
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
    Manager<> m;
    Activity& a = *m.create<Activity>();
    OpaqueAction& n = *m.create<OpaqueAction>();
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
    Manager<> m;
    Activity& a = *m.create<Activity>();
    ControlFlow& e = *m.create<ControlFlow>();
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
    Manager<> m;
    Activity& a = *m.create<Activity>();
    ControlFlow& e = *m.create<ControlFlow>();
    e.setActivity(&a);
    ASSERT_EQ(a.getEdges().size(), 1);
    ASSERT_EQ(a.getEdges().front(), e);
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ(*a.getOwnedElements().begin(), e);
    ASSERT_TRUE(e.getActivity());
    ASSERT_EQ(*e.getActivity(), a);
    ASSERT_EQ(*e.getOwner(), a);
}

TEST_F(ActivityTest, parseControlNodes) {
    Manager<> m;
    m.open(ymlPath + "activityTests/controlNodes.yml");
    ElementPtr parsed = m.getRoot();
    ASSERT_EQ(parsed->getElementType(), ElementType::ACTIVITY);
    Activity& activity = parsed->as<Activity>();
    ASSERT_EQ(activity.getNodes().size(), 5);
    ASSERT_EQ(activity.getEdges().size(), 5);
    ASSERT_EQ(activity.getPartitions().size(), 1);
    InitialNode& initialNode = activity.getNodes().get("initialNode").as<InitialNode>();
    ForkNode& forkNode = activity.getNodes().get("forkNode").as<ForkNode>();
    FlowFinalNode& flowFinalNode = activity.getNodes().get("flowFinalNode").as<FlowFinalNode>();
    JoinNode& joinNode = activity.getNodes().get("joinNode").as<JoinNode>();
    ActivityFinalNode& activityFinalNode = activity.getNodes().get("activityFinalNode").as<ActivityFinalNode>();
    ActivityPartition& partition = activity.getPartitions().front();
    ASSERT_EQ(*initialNode.getOutgoing().front().getTarget(), forkNode);
    ASSERT_EQ(*forkNode.getIncoming().front().getSource(), initialNode);
    ASSERT_TRUE(forkNode.getOutgoing().contains(joinNode.getIncoming().front()));
    ASSERT_TRUE(forkNode.getOutgoing().contains(joinNode.getIncoming().back()));
    ASSERT_TRUE(forkNode.getOutgoing().contains(flowFinalNode.getIncoming().front()));
    ASSERT_EQ(*activityFinalNode.getIncoming().front().getSource(), joinNode);
    ASSERT_EQ(*joinNode.getOutgoing().front().getTarget(), activityFinalNode);
    ASSERT_TRUE(partition.getNodes().contains(joinNode));
    ASSERT_TRUE(partition.getNodes().contains(flowFinalNode));
    ASSERT_TRUE(partition.getEdges().contains(flowFinalNode.getIncoming().front()));
    ASSERT_EQ(joinNode.getInPartitions().front(), partition);
    ASSERT_EQ(flowFinalNode.getInPartitions().front(), partition);
}

TEST_F(ActivityTest, objectNodeTest) {
    Manager<> m;
    m.open(ymlPath + "activityTests/objectNode.yml");
    ElementPtr parsed = m.getRoot();
    ASSERT_EQ(parsed->getElementType(), ElementType::ACTIVITY);
    Activity& activity = parsed->as<Activity>();
    ASSERT_EQ(activity.getNodes().size(), 5);
    ASSERT_EQ(activity.getEdges().size(), 2);
    // InitialNode& initialNode = activity.getNodes().get("initialNode").as<InitialNode>();
    DataStoreNode& objectNode = activity.getNodes().get("objectNode").as<DataStoreNode>();
    CentralBufferNode& objectNode2 = activity.getNodes().get("objectNode2").as<CentralBufferNode>();
    CentralBufferNode& objectNode3 = activity.getNodes().get("three").as<CentralBufferNode>();
    ObjectNode& objectNode4 = activity.getNodes().get("4").as<ObjectNode>();
    ASSERT_FALSE(objectNode.isControlType());
    ASSERT_EQ(objectNode.getOrdering(), ObjectNodeOrderingKind::FIFO);
    ASSERT_TRUE(objectNode2.isControlType());
    ASSERT_EQ(objectNode2.getOrdering(), ObjectNodeOrderingKind::UNORDERED);
    ASSERT_FALSE(objectNode3.isControlType());
    ASSERT_EQ(objectNode3.getOrdering(), ObjectNodeOrderingKind::ORDERED);
    ASSERT_TRUE(objectNode4.isControlType());
    ASSERT_EQ(objectNode4.getOrdering(), ObjectNodeOrderingKind::LIFO);
}

TEST_F(ActivityTest, emitActivityTest) {
    Manager<> m;
    Activity& activity = *m.create<Activity>();
    CentralBufferNode& objectNode = *m.create<CentralBufferNode>();
    objectNode.setOrdering(ObjectNodeOrderingKind::UNORDERED);
    activity.getNodes().add(objectNode);
    objectNode.setID("objectNode__________________");
    activity.setID("activity____________________");
    std::string expectedEmit = R""""(activity:
  id: activity____________________
  nodes:
    - centralBufferNode:
        id: objectNode__________________
        isControlType: false
        ordering: unordered)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(activity, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ActivityTest, mountActivityTest) {
    Manager<> m;
    ActivityPtr activity = m.create<Activity>();
    m.setRoot(activity.ptr());
    OpaqueActionPtr action = m.create<OpaqueAction>();
    ExceptionHandlerPtr exceptionHandler = m.create<ExceptionHandler>();
    activity->getNodes().add(*action);
    action->getHandlers().add(*exceptionHandler);
    m.mount(".");
    action.release();
    ASSERT_FALSE(action.loaded());
    ASSERT_EQ(action.id(), exceptionHandler->getOwner().id());
    ASSERT_FALSE(action.loaded());
    ASSERT_EQ(action->getActivity(), activity);
    ASSERT_TRUE(action.loaded());
}
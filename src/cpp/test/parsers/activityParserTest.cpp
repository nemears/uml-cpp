#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

class ActivityParserTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ActivityParserTest, parseControlNodes) {
    UmlManager m;
    ElementPtr parsed = m.parse(ymlPath + "activityTests/controlNodes.yml");
    ASSERT_EQ(parsed->getElementType(), ElementType::ACTIVITY);
    Activity& activity = parsed->as<Activity>();
    ASSERT_EQ(activity.getNodes().size(), 5);
    ASSERT_EQ(activity.getEdges().size(), 5);
    InitialNode& initialNode = activity.getNodes().get("initialNode").as<InitialNode>();
    ForkNode& forkNode = activity.getNodes().get("forkNode").as<ForkNode>();
    FlowFinalNode& flowFinalNode = activity.getNodes().get("flowFinalNode").as<FlowFinalNode>();
    JoinNode& joinNode = activity.getNodes().get("joinNode").as<JoinNode>();
    ActivityFinalNode& activityFinalNode = activity.getNodes().get("activityFinalNode").as<ActivityFinalNode>();
    ASSERT_EQ(*initialNode.getOutgoing().front().getTarget(), forkNode);
    ASSERT_EQ(*forkNode.getIncoming().front().getSource(), initialNode);
    ASSERT_TRUE(forkNode.getOutgoing().contains(joinNode.getIncoming().front()));
    ASSERT_TRUE(forkNode.getOutgoing().contains(joinNode.getIncoming().back()));
    ASSERT_TRUE(forkNode.getOutgoing().contains(flowFinalNode.getIncoming().front()));
    ASSERT_EQ(*activityFinalNode.getIncoming().front().getSource(), joinNode);
    ASSERT_EQ(*joinNode.getOutgoing().front().getTarget(), activityFinalNode);
}

TEST_F(ActivityParserTest, objectNodeTest) {
    UmlManager m;
    ElementPtr parsed = m.parse(ymlPath + "activityTests/objectNode.yml");
    ASSERT_EQ(parsed->getElementType(), ElementType::ACTIVITY);
    Activity& activity = parsed->as<Activity>();
    ASSERT_EQ(activity.getNodes().size(), 5);
    ASSERT_EQ(activity.getEdges().size(), 2);
    InitialNode& initialNode = activity.getNodes().get("initialNode").as<InitialNode>();
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

TEST_F(ActivityParserTest, emitActivityTest) {
    UmlManager m;
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
        controlType: false
        ordering: unordered)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(activity));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
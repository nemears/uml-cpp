#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(ActivityNodeActivity, Activity, OpaqueAction, &ActivityNode::getActivity, &ActivityNode::setActivity);
UML_SET_INTEGRATION_TEST(ActivityNodeIncoming, ControlFlow, OpaqueAction, &ActivityNode::getIncoming);
UML_SET_INTEGRATION_TEST(ActivityNodeOutgoing, ControlFlow, OpaqueAction, &ActivityNode::getOutgoing);
UML_SINGLETON_INTEGRATION_TEST(DecisionNodeDescisionInput, Activity, DecisionNode, &DecisionNode::getDecisionInput, &DecisionNode::setDecisionInput);
UML_SINGLETON_INTEGRATION_TEST(DecisionNodeDecisionInputFlow, ObjectFlow, DecisionNode, &DecisionNode::getDecisionInputFlow, &DecisionNode::setDecisionInputFlow);
UML_SINGLETON_INTEGRATION_TEST(JoinNodeJoinSpec, LiteralInt, JoinNode, &JoinNode::getJoinSpec, &JoinNode::setJoinSpec);
UML_SINGLETON_INTEGRATION_TEST(ObjectNodeSelection, Activity, CentralBufferNode, &ObjectNode::getSelection, &ObjectNode::setSelection);
UML_SINGLETON_INTEGRATION_TEST(ObjectNodeUpperBound, LiteralInt, DataStoreNode, &ObjectNode::getUpperBound, &ObjectNode::setUpperBound);
UML_SINGLETON_INTEGRATION_TEST(ActivityParameterNodeParameter, Parameter, ActivityParameterNode, &ActivityParameterNode::getParameter, &ActivityParameterNode::setParameter);
UML_SET_INTEGRATION_TEST(ExecutableNodeHandlers, ExceptionHandler, OpaqueAction, &ExecutableNode::getHandlers);
UML_SINGLETON_INTEGRATION_TEST(ExceptionHandlerProtectedNode, OpaqueAction, ExceptionHandler, &ExceptionHandler::getProtectedNode, &ExceptionHandler::setProtectedNode);
UML_SINGLETON_INTEGRATION_TEST(ExceptionHandlerHandlerBody, OpaqueAction, ExceptionHandler, &ExceptionHandler::getHandlerBody, &ExceptionHandler::setHandlerBody);
UML_SINGLETON_INTEGRATION_TEST(ExceptionHandlerExceptionInput, DataStoreNode, ExceptionHandler, &ExceptionHandler::getExceptionInput, &ExceptionHandler::setExceptionInput);
UML_SET_INTEGRATION_TEST(ExceptionHandlerExceptionTypes, Activity, ExceptionHandler, &ExceptionHandler::getExceptionTypes);
UML_SET_INTEGRATION_TEST(ActionLocalPreConditions, Constraint, OpaqueAction, &Action::getLocalPreconditions);
UML_SET_INTEGRATION_TEST(ActionLocalPostConditions, Constraint, OpaqueAction, &Action::getLocalPostconditions);
UML_SET_INTEGRATION_TEST(OpaqueActionInputValues, InputPin, OpaqueAction, &OpaqueAction::getInputValues);
UML_SET_INTEGRATION_TEST(OpaqueActionOutputValues, OutputPin, OpaqueAction, &OpaqueAction::getOutputValues);
UML_SET_INTEGRATION_TEST(OpaqueActionBodies, LiteralString, OpaqueAction, &OpaqueAction::getBodies);
UML_SINGLETON_INTEGRATION_TEST(ActionInputPinFromAction, OpaqueAction, ActionInputPin, &ActionInputPin::getFromAction, &ActionInputPin::setFromAction);
UML_SINGLETON_INTEGRATION_TEST(ValuePinValue, LiteralBool, ValuePin, &ValuePin::getValue, &ValuePin::setValue);
UML_SET_INTEGRATION_TEST(InvocationActionArguments, InputPin, CallBehaviorAction, &InvocationAction::getArguments);
UML_SET_INTEGRATION_TEST(CallActionResults, OutputPin, CallBehaviorAction, &CallAction::getResults);
UML_SINGLETON_INTEGRATION_TEST(CallBehaviorActionBehavior, Activity, CallBehaviorAction, &CallBehaviorAction::getBehavior, &CallBehaviorAction::setBehavior);

class ActivityNodeTest : public ::testing::Test {
   
};

TEST_F(ActivityNodeTest, reindexID_test) {
    Manager<> m;
    Activity& a = *m.create<Activity>();
    OpaqueAction& n = *m.create<OpaqueAction>();
    a.getNodes().add(n);
    n.setActivity(&a); // Maybe this should be allowed
    n.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(a.getNodes().get(n.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(n.getID()));
}

TEST_F(ActivityNodeTest, reindexNameTest) {
    Manager<> m;
    Activity& a = *m.create<Activity>();
    OpaqueAction& n = *m.create<OpaqueAction>();
    a.getNodes().add(n);
    n.setActivity(&a);
    n.setName("test");
    ASSERT_NO_THROW(a.getNodes().get("test"));
}

TEST_F(ActivityNodeTest, addIncomingFunctorTest) {
    Manager<> m;
    OpaqueAction& n = *m.create<OpaqueAction>();
    ControlFlow& e = *m.create<ControlFlow>();
    n.getIncoming().add(e);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(*e.getTarget(), n);
}

TEST_F(ActivityNodeTest, setTargetTest) {
    Manager<> m;
    OpaqueAction& n = *m.create<OpaqueAction>();
    ControlFlow& e = *m.create<ControlFlow>();
    e.setTarget(&n);
    ASSERT_EQ(n.getIncoming().size(), 1);
    ASSERT_EQ(n.getIncoming().front(), e);
    ASSERT_TRUE(e.getTarget());
    ASSERT_EQ(*e.getTarget(), n);
}

TEST_F(ActivityNodeTest, addOutgoingFunctorTest) {
    Manager<> m;
    OpaqueAction& n = *m.create<OpaqueAction>();
    ControlFlow& e = *m.create<ControlFlow>();
    n.getOutgoing().add(e);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(*e.getSource(), n);
}

TEST_F(ActivityNodeTest, setSourceTest) {
    Manager<> m;
    OpaqueAction& n = *m.create<OpaqueAction>();
    ControlFlow& e = *m.create<ControlFlow>();
    e.setSource(&n);
    ASSERT_EQ(n.getOutgoing().size(), 1);
    ASSERT_EQ(n.getOutgoing().front(), e);
    ASSERT_TRUE(e.getSource());
    ASSERT_EQ(*e.getSource(), n);
}

// TEST_F(ActivityNodeTest, DuplicateEdgeExceptionIncomingTest) {
//     UmlManager<> m;
//     ActivityNode& n = *m.create<ActivityNode>();
//     ActivityEdge& e = *m.create<ActivityEdge>();
//     n.getIncoming().add(e);
//     ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
//     n.getIncoming().remove(e);
//     e.setTarget(&n);
//     ASSERT_THROW(n.getIncoming().add(e), DuplicateEdgeException);
// }

// TEST_F(ActivityNodeTest, DuplicateEdgeExceptionOutgoingTest) {
//     UmlManager<> m;
//     ActivityNode n = m.create<ActivityNode>();
//     ActivityEdge e = m.create<ActivityEdge>();
//     n.getOutgoing().add(e);
//     ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
//     n.getOutgoing().remove(e);
//     e.setSource(&n);
//     ASSERT_THROW(n.getOutgoing().add(e), DuplicateEdgeException);
// }
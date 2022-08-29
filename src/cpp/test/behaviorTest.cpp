#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(BehaviorSpecification, Operation, Activity, &Behavior::getSpecification, &Behavior::setSpecification);
UML_SET_INTEGRATION_TEST(BehavioralFeatureOwnedParameters, Parameter, Operation, &BehavioralFeature::getOwnedParameters);
UML_SET_INTEGRATION_TEST(BehavioralFeatureMethods, Activity, Operation, &BehavioralFeature::getMethods);
UML_SET_INTEGRATION_TEST(BehavioralFeatureRaisedExceptions, Activity, Operation, &BehavioralFeature::getRaisedExceptions);
UML_SET_INTEGRATION_TEST(BehaviorParameters, Parameter, OpaqueBehavior, &Behavior::getOwnedParameters);

class BehaviorTest : public ::testing::Test {
   public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(BehaviorTest, removeParameterFunctorTest) {
    BasicManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    Operation& o = *m.create<Operation>();
    b.setSpecification(o);
    b.getOwnedParameters().add(p);
    b.getOwnedParameters().remove(p);
    ASSERT_TRUE(b.getOwnedParameters().size() == 0);
    ASSERT_TRUE(b.getOwnedElements().size() == 0);
    ASSERT_TRUE(!p.getOperation());
    ASSERT_TRUE(!p.getOwner());
}

TEST_F(BehaviorTest, reindexBehaviorID_Test) {
    BasicManager m;
    OpaqueBehavior& behavior = *m.create<OpaqueBehavior>();
    Parameter& param = *m.create<Parameter>();
    Class& owner = *m.create<Class>();
    Operation& specification = *m.create<Operation>();
    behavior.getOwnedParameters().add(param);
    owner.getOwnedBehaviors().add(behavior);
    owner.getOwnedOperations().add(specification);
    specification.getMethods().add(behavior);
    ID id = ID::fromString("paTTKz60cn0lguUMWAMLUNO0EoXS");
    behavior.setID(id);
    ASSERT_EQ(*param.getNamespace(), behavior);
    ASSERT_EQ(*param.getOwner(), behavior);
    ASSERT_EQ(owner.getOwnedBehaviors().get(id), behavior);
    ASSERT_EQ(owner.getOwnedMembers().get(id), behavior);
    ASSERT_EQ(owner.getMembers().get(id), behavior);
    ASSERT_EQ(owner.getOwnedElements().get(id), behavior);
    ASSERT_EQ(specification.getMethods().get(id), behavior);
}

TEST_F(BehaviorTest, parseMultipleSimpleBodies) {
    BasicManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = &m.getRoot()->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv->getName().compare("test") == 0);
    ASSERT_TRUE(bhv->getID() == ID::fromString("i0wopIpBjBHdekQ57DbWeHfWmQp3"));
    ASSERT_TRUE(bhv->getBodies().size() == 3);
    ASSERT_TRUE(bhv->getBodies().get(0).getValue().compare("one") == 0);
    ASSERT_TRUE(bhv->getBodies().get(1).getValue().compare("1") == 0);
    ASSERT_TRUE(bhv->getBodies().get(2).getValue().compare("true") == 0);
}

TEST_F(BehaviorTest, parseParameter) {
    BasicManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "opaqueBehaviorTests/param.yml").ptr());
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = &m.getRoot()->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv->getOwnedParameters().size() == 1);
    Parameter* param = &bhv->getOwnedParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(BehaviorTest, properParameters) {
    BasicManager m;
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/improperParameters.yml"), Parsers::UmlParserException);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodyNotLiteralString.yml"), Parsers::UmlParserException);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodiesEntryIsSequence.yml"), Parsers::UmlParserException);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodiesNotSequence.yml"), Parsers::UmlParserException);
}

TEST_F(BehaviorTest, emitBasicOpaqueBehavior) {
    BasicManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Property& p = *m.create<Property>();
    Operation& o = *m.create<Operation>();
    b.setID("0FTAvf5nrXsknnc60ziElK2TDb7D");
    b.setName("Opaque");
    p.setID("YVV4HY0dc4OS0zPOc1HPLfyLmx39");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("P73WqZXNrYSV0fEtHqKAJTv3RDTD");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.getOwnedAttributes().add(p);
    b.getOwnedOperations().add(o);
    std::string expectedEmit = R""""(opaqueBehavior:
  id: 0FTAvf5nrXsknnc60ziElK2TDb7D
  name: Opaque
  ownedAttributes:
    - property:
        id: YVV4HY0dc4OS0zPOc1HPLfyLmx39
        name: prop
        visibility: PRIVATE
  ownedOperations:
    - operation:
        id: P73WqZXNrYSV0fEtHqKAJTv3RDTD
        name: op
        visibility: PROTECTED)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
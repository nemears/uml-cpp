#include "gtest/gtest.h"
#include "uml/types/opaqueBehavior.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(BehaviorSpecification, Operation, OpaqueBehavior, &Behavior::getSpecification, &Behavior::setSpecification)
UML_SET_INTEGRATION_TEST(BehavioralFeatureOwnedParameters, Parameter, Operation, &BehavioralFeature::getOwnedParameters)
UML_SET_INTEGRATION_TEST(BehavioralFeatureMethods, OpaqueBehavior, Operation, &BehavioralFeature::getMethods)
UML_SET_INTEGRATION_TEST(BehavioralFeatureRaisedExceptions, OpaqueBehavior, Operation, &BehavioralFeature::getRaisedExceptions)
UML_SET_INTEGRATION_TEST(BehaviorParameters, Parameter, OpaqueBehavior, &Behavior::getOwnedParameters)

class BehaviorTest : public ::testing::Test {
   public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(BehaviorTest, removeParameterFunctorTest) {
    UmlManager m;
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

TEST_F(BehaviorTest, parseMultipleSimpleBodies) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == OpaqueBehavior::Info::elementType);
    OpaqueBehavior* bhv = &m.getRoot()->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv->getName().compare("test") == 0);
    ASSERT_TRUE(bhv->getID() == ID::fromString("i0wopIpBjBHdekQ57DbWeHfWmQp3"));
    ASSERT_TRUE(bhv->getBodies().size() == 3);
    ASSERT_TRUE(bhv->getBodies().get(0)->getValue().compare("one") == 0);
    ASSERT_TRUE(bhv->getBodies().get(1)->getValue().compare("1") == 0);
    ASSERT_TRUE(bhv->getBodies().get(2)->getValue().compare("true") == 0);
}

TEST_F(BehaviorTest, parseParameter) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "opaqueBehaviorTests/param.yml").ptr());
    ASSERT_TRUE(m.getRoot()->getElementType() == OpaqueBehavior::Info::elementType);
    OpaqueBehavior* bhv = &m.getRoot()->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv->getOwnedParameters().size() == 1);
    ParameterPtr param = bhv->getOwnedParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(BehaviorTest, properParameters) {
    UmlManager m;
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/improperParameters.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodyNotLiteralString.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodiesEntryIsSequence.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "opaqueBehaviorTests/bodiesNotSequence.yml"), SerializationError);
}

TEST_F(BehaviorTest, emitBasicOpaqueBehavior) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Property& p = *m.create<Property>();
    Operation& o = *m.create<Operation>();
    b.setID(ID::fromString("0FTAvf5nrXsknnc60ziElK2TDb7D"));
    b.setName("Opaque");
    p.setID(ID::fromString("YVV4HY0dc4OS0zPOc1HPLfyLmx39"));
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID(ID::fromString("P73WqZXNrYSV0fEtHqKAJTv3RDTD"));
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.getOwnedAttributes().add(p);
    b.getOwnedOperations().add(o);
    std::string expectedEmit = R""""(OpaqueBehavior:
  id: 0FTAvf5nrXsknnc60ziElK2TDb7D
  name: Opaque
  ownedAttributes:
    - Property:
        id: YVV4HY0dc4OS0zPOc1HPLfyLmx39
        name: prop
        visibility: private
  ownedOperations:
    - Operation:
        id: P73WqZXNrYSV0fEtHqKAJTv3RDTD
        name: op
        visibility: protected)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(b));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

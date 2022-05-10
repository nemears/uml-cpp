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

TEST_F(BehaviorTest, addParameterFunctorTest) {

    /** TODO: I think this implementation is wrong, double check with uml spec**/

    // Behavior b;
    // Parameter p;
    // Operation o;
    // b.setSpecification(&o);
    // b.getOwnedParameters().add(p);
    // ASSERT_TRUE(b.getOwnedParameters().size() == 1);
    // ASSERT_TRUE(&b.getOwnedParameters().front() == &p);
    // ASSERT_TRUE(b.getOwnedElements().size() == 1);
    // ASSERT_TRUE(&b.getOwnedElements().front() == &p);
    // ASSERT_TRUE(b.getSpecification() == &o);
    // ASSERT_TRUE(p.getOperation() == &o);
}

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

// TEST_F(BehaviorTest, removeParameterFunctorMultiMethodOpTest) {
//     Behavior b1;
//     Behavior b2;
//     // so this may be false in uml? TODO check
//     // specifically an operation having behaviors with different parameters
//     // i think it is valid, so allow it
//     Parameter p;
//     Operation o;
//     o.getMethods().add(b1);
//     o.getMethods().add(b2);
//     b1.getOwnedParameters().add(p);
//     ASSERT_TRUE(p.getOwner() == &b1);
//     ASSERT_TRUE(b1.getOwnedElements().size() == 1);
//     ASSERT_TRUE(b1.getOwnedElements().front() == &p);
//     ASSERT_TRUE(p.getOperation() == &o);
//     b2.getOwnedParameters().add(p);
//     ASSERT_NO_THROW(b1.getOwnedParameters().remove(p));
//     ASSERT_TRUE(b1.getOwnedParameters().size() == 0);
//     ASSERT_TRUE(b1.getOwnedElements().size() == 0);
//     ASSERT_TRUE(b1.getSpecification() == &o);
//     ASSERT_TRUE(b2.getOwnedParameters().size() == 1);
//     ASSERT_TRUE(b2.getOwnedParameters().front() == &p);
//     ASSERT_TRUE(b2.getOwnedElements().size() == 1);
//     ASSERT_TRUE(b2.getSpecification() == &o);
//     ASSERT_TRUE(o.getMethods().size() == 2);
//     ASSERT_TRUE(o.getMethods().front() == &b1);
//     ASSERT_TRUE(o.getMethods().back() == &b2);
//     ASSERT_TRUE(p.getOperation() == &o);
// }

TEST_F(BehaviorTest, reindexBehaviorID_Test) {
    UmlManager m;
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
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getName().compare("test") == 0);
    ASSERT_TRUE(bhv->getID() == ID::fromString("i0wopIpBjBHdekQ57DbWeHfWmQp3"));
    ASSERT_TRUE(bhv->getBodies().size() == 3);
    ASSERT_TRUE(bhv->getBodies().get(0).getValue().compare("one") == 0);
    ASSERT_TRUE(bhv->getBodies().get(1).getValue().compare("1") == 0);
    ASSERT_TRUE(bhv->getBodies().get(2).getValue().compare("true") == 0);
}

TEST_F(BehaviorTest, parseParameter) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/param.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getOwnedParameters().size() == 1);
    Parameter* param = &bhv->getOwnedParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(BehaviorTest, properParameters) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/improperParameters.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodyNotLiteralString.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodiesEntryIsSequence.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodiesNotSequence.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(BehaviorTest, emitBasicOpaqueBehavior) {
    UmlManager m;
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
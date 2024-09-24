#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(ParameterDefaultValue, LiteralString, Parameter, &Parameter::getDefaultValue, &Parameter::setDefaultValue)
UML_SET_INTEGRATION_TEST(ParameterParameterSets, ParameterSet, Parameter, &Parameter::getParameterSets)
UML_SET_INTEGRATION_TEST(ParmeterSetParameters, Parameter, ParameterSet, &ParameterSet::getParameters)
UML_SET_INTEGRATION_TEST(ParameterSetConditions, Constraint, ParameterSet, &ParameterSet::getConditions)
UML_SET_INTEGRATION_TEST(BehavioralFeatureOwnedParameterSets, ParameterSet, Operation, &BehavioralFeature::getOwnedParameterSets)

class ParameterTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(ParameterTest, TestSetDirection) {
    UmlManager m;
    Parameter& p = *m.create<Parameter>();
    p.setDirection(ParameterDirectionKind::IN_UML);
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(ParameterTest, TestGetNotSetDirection) {
    UmlManager m;
    Parameter& p = *m.create<Parameter>();
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::NONE);
}

TEST_F(ParameterTest, properExceptions) {
    UmlManager m;
    ASSERT_THROW(m.open(ymlPath + "parameterTests/invalidDirection.yml"), SerializationError);
    ASSERT_THROW(m.open(ymlPath + "parameterTests/invalidDirection2.yml"), SerializationError);
}

TEST_F(ParameterTest, emitParameterWMultiplicityTest) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    b.setID(ID::fromString("SeJ_0hSPaIa4EYap3sXgRQm4LuSn"));
    p.setID(ID::fromString("QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT"));
    LiteralInt& low = *m.create<LiteralInt>();
    LiteralInt& high = *m.create<LiteralInt>();
    low.setID(ID::fromString("K2_2cpL966uyziy9e2pGb60cCR5X"));
    low.setValue(0);
    high.setID(ID::fromString("sGGXJFNinAvKyKVSgc&JAlMWsbw5"));
    high.setValue(5);
    p.setLowerValue(&low);
    p.setUpperValue(&high);
    b.getOwnedParameters().add(p);
    std::string expectedEmit = R""""(OpaqueBehavior:
  id: SeJ_0hSPaIa4EYap3sXgRQm4LuSn
  ownedParameters:
    - Parameter:
        id: QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT
        lowerValue:
          LiteralInt:
            id: K2_2cpL966uyziy9e2pGb60cCR5X
            value: 0
        upperValue:
          LiteralInt:
            id: "sGGXJFNinAvKyKVSgc&JAlMWsbw5"
            value: 5)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(b));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

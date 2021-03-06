#include "gtest/gtest.h"
#include "uml/parameter.h"

using namespace UML;

class ParameterTest : public ::testing::Test {

    public:
        Parameter* pSetDirection;
        Parameter* pGetSetDirection;
        Parameter* pGetNullDirection;
    
    protected:
        ParameterTest() {
            pSetDirection = new Parameter;
            pGetSetDirection = new Parameter;
            pGetNullDirection= new Parameter;
            pGetSetDirection->setDirection(ParameterDirectionKind::IN);
        }


};

TEST_F(ParameterTest, TestSetDirection) {
    pSetDirection->setDirection(ParameterDirectionKind::IN);
    ASSERT_TRUE(pSetDirection->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterTest, TestGetSetDirection) {
    ASSERT_TRUE(pGetSetDirection->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterTest, TestGetNotSetDirection) {
    ASSERT_TRUE(pGetNullDirection->getDirection() == ParameterDirectionKind::NONE);
}
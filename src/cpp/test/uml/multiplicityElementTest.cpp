#include "gtest/gtest.h"
#include "uml/property.h"
#include "uml/literalInt.h"

using namespace UML;

class MultiplicityElementTest : public ::testing::Test {
   
};

TEST_F(MultiplicityElementTest, SetAndGetProperLowerAndUpperTest) {
    Property p;
    p.setLower(0);
    p.setUpper(1);
    ASSERT_NO_THROW(p.getLower());
    ASSERT_EQ(p.getLower(), 0);
    ASSERT_NO_THROW(p.getUpper());
    ASSERT_EQ(p.getUpper(), 1);
}

TEST_F(MultiplicityElementTest, GetLowerBeforeSettingTest) {
    Property p;
    ASSERT_THROW(p.getLower(), MultiplicityElement::MultiplicityNotSpecifiedException);
}

TEST_F(MultiplicityElementTest, GetUpperBeforeSettingTest) {
    Property p;
    ASSERT_THROW(p.getUpper(), MultiplicityElement::MultiplicityNotSpecifiedException);
}

TEST_F(MultiplicityElementTest, MultiplictySpecifiedTest) {
    Property p;
    ASSERT_FALSE(p.multiplicitySpecified());
    p.setLower(0);
    ASSERT_FALSE(p.multiplicitySpecified());
    p.setUpper(1);
    ASSERT_TRUE(p.multiplicitySpecified());
}

TEST_F(MultiplicityElementTest, MultiplicityValueTest) {
    Property p;
    LiteralInt low;
    low.setValue(0);
    LiteralInt up;
    up.setValue(10);
    p.setLowerValue(&low);
    p.setUpperValue(&up);
    ASSERT_TRUE(p.getLowerValue() == &low);
    ASSERT_TRUE(p.getLower() == 0);
    ASSERT_TRUE(p.getUpperValue() == &up);
    ASSERT_TRUE(p.getUpper() == 10);
    ASSERT_TRUE(p.multiplicitySpecified());
}
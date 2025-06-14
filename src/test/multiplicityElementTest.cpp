#include "gtest/gtest.h"
#include "uml/types/literalInteger.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(MultiplicityElementLowerValue, LiteralReal, Property, getLowerValueSingleton)
UML_SINGLETON_INTEGRATION_TEST(MultiplicityElementUpperValue, LiteralNull, Property, getUpperValueSingleton)

class MultiplicityElementTest : public ::testing::Test {
   
};

TEST_F(MultiplicityElementTest, SetAndGetProperLowerAndUpperTest) {
    UmlManager m;
    auto& p = *m.create<Property>();
    p.setLower(0);
    p.setUpper(1);
    ASSERT_NO_THROW(p.getLower());
    ASSERT_EQ(p.getLower(), 0);
    ASSERT_NO_THROW(p.getUpper());
    ASSERT_EQ(p.getUpper(), 1);
}

// TEST_F(MultiplicityElementTest, GetLowerBeforeSettingTest) {
//     UmlManager m;
//     Property& p = *m.create<Property>();
//     ASSERT_THROW(p.getLower(), MultiplicityElement::MultiplicityNotSpecifiedException);
// }

// TEST_F(MultiplicityElementTest, GetUpperBeforeSettingTest) {
//     UmlManager m;
//     Property& p = *m.create<Property>();
//     ASSERT_THROW(p.getUpper(), MultiplicityElement::MultiplicityNotSpecifiedException);
// }

TEST_F(MultiplicityElementTest, MultiplictySpecifiedTest) {
    UmlManager m;
    auto& p = *m.create<Property>();
    ASSERT_FALSE(p.multiplicitySpecified());
    p.setLower(0);
    ASSERT_FALSE(p.multiplicitySpecified());
    p.setUpper(1);
    ASSERT_TRUE(p.multiplicitySpecified());
}

TEST_F(MultiplicityElementTest, MultiplicityValueTest) {
    UmlManager m;
    auto& p = *m.create<Property>();
    auto& low = *m.create<LiteralInt>();
    low.setValue(0);
    auto& up = *m.create<LiteralInt>();
    up.setValue(10);
    p.setLowerValue(&low);
    p.setUpperValue(&up);
    ASSERT_TRUE(p.getLowerValue() == &low);
    ASSERT_TRUE(p.getLower() == 0);
    ASSERT_TRUE(p.getUpperValue() == &up);
    ASSERT_TRUE(p.getUpper() == 10);
    ASSERT_TRUE(p.multiplicitySpecified());
}



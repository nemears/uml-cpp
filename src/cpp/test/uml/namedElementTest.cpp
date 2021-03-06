#include "gtest/gtest.h"
#include "uml/namedElement.h"

using namespace UML;

class NamedElementTest : public ::testing::Test {
    public:

        NamedElement namedEl;
        NamedElement* namedElPtr;

    protected:

        NamedElementTest() {
            namedElPtr = new NamedElement;
        }

        ~NamedElementTest() override {
            delete namedElPtr;
        }

        void SetUp() override {
            namedEl.setName("test");
            namedElPtr->setName("ptrTest");
        }
};

TEST_F(NamedElementTest, SetNameTest) {
    EXPECT_EQ(namedEl.getName(), "test");
    EXPECT_EQ(namedElPtr->getName(), "ptrTest");
}

TEST_F(NamedElementTest, GetNullNameTest) {
    NamedElement ne;
    ASSERT_NO_THROW(ne.getName());
    EXPECT_TRUE(ne.getName().compare("") == 0);
}
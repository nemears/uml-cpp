#include "gtest/gtest.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"

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

TEST_F(NamedElementTest, reIndexNameTest) {
  Element e1;
  NamedElement e2;
  e1.getOwnedElements().add(e2);
  e2.setOwner(&e1);
  e2.setName("test");
  ASSERT_TRUE(e1.getOwnedElements().get("test") != NULL);
}
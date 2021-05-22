#include "gtest/gtest.h"
#include "uml/inputPin.h"

using namespace UML;

class InputPinTest : public ::testing::Test {
   
};

TEST_F(InputPinTest, reindexIDTest) {
    Action a;
    InputPin i;
    a.getInputs().add(i);
    i.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
    ASSERT_TRUE(a.getInputs().get(i.getID()));
    ASSERT_TRUE(a.getOwnedElements().get(i.getID()));
}

TEST_F(InputPinTest, reindexNameTest) {
    Action a;
    InputPin i;
    a.getInputs().add(i);
    i.setName("test");
    ASSERT_TRUE(a.getInputs().get("test"));
    ASSERT_TRUE(a.getOwnedElements().get("test"));
}

TEST_F(InputPinTest, AddPinFunctorTest) {
    Action a;
    InputPin i;
    a.getInputs().add(i);
    ASSERT_TRUE(a.getInputs().size() == 1);
    ASSERT_TRUE(a.getInputs().front() == &i);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(a.getOwnedElements().front() == &i);
    ASSERT_TRUE(i.getOwner() == &a);
}
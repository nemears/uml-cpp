#include "gtest/gtest.h"
#include "uml/outputPin.h"

using namespace UML;

class OutputPinTest : public ::testing::Test {
   
};

TEST_F(OutputPinTest, reindexIDTest) {
    Action a;
    OutputPin i;
    i.setOwner(&a);
    a.getOutputs().add(i);
    a.getOwnedElements().add(i);
    i.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
    ASSERT_TRUE(a.getOutputs().get(i.getID()));
    ASSERT_TRUE(a.getOwnedElements().get(i.getID()));
}

TEST_F(OutputPinTest, reindexNameTest) {
    Action a;
    OutputPin i;
    i.setOwner(&a);
    a.getOutputs().add(i);
    a.getOwnedElements().add(i);
    i.setName("test");
    ASSERT_TRUE(a.getOutputs().get("test"));
    ASSERT_TRUE(a.getOwnedElements().get("test"));
}
#include "gtest/gtest.h"
#include "uml/outputPin.h"

using namespace UML;

class OutputPinTest : public ::testing::Test {
   
};

TEST_F(OutputPinTest, reindexIDTest) {
    Action a;
    OutputPin i;
    a.getOutputs().add(i);
    i.setID("eb0920180bef4ad6b80f05fadsac");
    ASSERT_NO_THROW(a.getOutputs().get(i.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(i.getID()));
}

// TEST_F(OutputPinTest, reindexNameTest) {
//     Action a;
//     OutputPin i;
//     a.getOutputs().add(i);
//     i.setName("test");
//     ASSERT_TRUE(a.getOutputs().get("test"));
//     ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

TEST_F(OutputPinTest, AddPinFunctorTest) {
    Action a;
    OutputPin i;
    a.getOutputs().add(i);
    ASSERT_TRUE(a.getOutputs().size() == 1);
    ASSERT_TRUE(&a.getOutputs().front() == &i);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().front() == &i);
    ASSERT_TRUE(i.getOwner() == &a);
}
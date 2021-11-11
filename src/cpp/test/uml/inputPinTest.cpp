#include "gtest/gtest.h"
#include "uml/inputPin.h"

using namespace UML;

class InputPinTest : public ::testing::Test {
   
};

TEST_F(InputPinTest, reindexIDTest) {
    UmlManager m;
    Action& a = m.create<Action>();
    InputPin& i = m.create<InputPin>();
    a.getInputs().add(i);
    i.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(a.getInputs().get(i.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(i.getID()));
}

// TEST_F(InputPinTest, reindexNameTest) {
//     Action a;
//     InputPin i;
//     a.getInputs().add(i);
//     i.setName("test");
//     ASSERT_TRUE(a.getInputs().get("test"));
//     ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

TEST_F(InputPinTest, AddPinFunctorTest) {
    UmlManager m;
    Action& a = m.create<Action>();
    InputPin& i = m.create<InputPin>();
    a.getInputs().add(i);
    ASSERT_TRUE(a.getInputs().size() == 1);
    ASSERT_TRUE(&a.getInputs().front() == &i);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a.getOwnedElements().begin() == i);
    ASSERT_TRUE(i.getOwner() == &a);
}
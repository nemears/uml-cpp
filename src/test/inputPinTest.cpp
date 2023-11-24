#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class InputPinTest : public ::testing::Test {
   
};

TEST_F(InputPinTest, reindexNameTest) {
    Manager<> m;
    OpaqueAction& a = *m.create<OpaqueAction>();
    InputPin& i = *m.create<InputPin>();
    a.getInputValues().add(i);
    i.setName("test");
    ASSERT_NO_THROW(a.getInputs().get("test"));
    ASSERT_NO_THROW(a.getOwnedElements().get("test"));
}

TEST_F(InputPinTest, AddPinFunctorTest) {
    Manager<> m;
    OpaqueAction& a = *m.create<OpaqueAction>();
    InputPin& i = *m.create<InputPin>();
    a.getInputValues().add(i);
    ASSERT_TRUE(a.getInputs().size() == 1);
    ASSERT_TRUE(&a.getInputs().front() == &i);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a.getOwnedElements().begin() == i);
    ASSERT_TRUE(i.getOwner() == &a);
}
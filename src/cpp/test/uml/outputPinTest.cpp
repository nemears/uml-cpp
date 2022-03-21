#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class OutputPinTest : public ::testing::Test {
   
};

TEST_F(OutputPinTest, reindexIDTest) {
    UmlManager m;
    OpaqueAction& a = *m.create<OpaqueAction>();
    OutputPin& i = *m.create<OutputPin>();
    a.getOutputValues().add(i);
    i.setID("eb0920180bef4ad6b80f05fadsac");
    ASSERT_NO_THROW(a.getOutputs().get(i.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(i.getID()));
}

TEST_F(OutputPinTest, reindexNameTest) {
    UmlManager m;
    OpaqueAction& a = *m.create<OpaqueAction>();
    OutputPin& i = *m.create<OutputPin>();
    a.getOutputValues().add(i);
    i.setName("test");
    ASSERT_NO_THROW(a.getOutputs().get("test"));
    ASSERT_NO_THROW(a.getOwnedElements().get("test"));
}

TEST_F(OutputPinTest, AddPinFunctorTest) {
    UmlManager m;
    OpaqueAction& a = *m.create<OpaqueAction>();
    OutputPin& i = *m.create<OutputPin>();
    a.getOutputValues().add(i);
    ASSERT_TRUE(a.getOutputs().size() == 1);
    ASSERT_TRUE(&a.getOutputs().front() == &i);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(&a.getOwnedElements().get(i.getID()) == &i);
    ASSERT_TRUE(i.getOwner() == &a);
}
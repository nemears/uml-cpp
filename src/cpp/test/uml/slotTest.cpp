#include "gtest/gtest.h"
#include "uml/slot.h"
#include "uml/literalString.h"

using namespace UML;

class SlotTest : public ::testing::Test {
   
};

TEST_F(SlotTest, addValueFunctorTest) {
    Slot s;
    LiteralString l;
    s.getValues().add(l);
    ASSERT_TRUE(s.getValues().size() == 1);
    ASSERT_TRUE(s.getValues().front() == &l);
    ASSERT_TRUE(s.getOwnedElements().size() == 1);
    ASSERT_TRUE(s.getOwnedElements().front() == &l);
    ASSERT_TRUE(l.getOwner() == &s);
}
#include "gtest/gtest.h"
#include "uml/slot.h"
#include "uml/literalString.h"

using namespace UML;

class SlotTest : public ::testing::Test {
   
};

TEST_F(SlotTest, addValueFunctorTest) {
    UmlManager m;
    Slot& s = m.create<Slot>();
    LiteralString& l = m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_TRUE(s.getValues().size() == 1);
    ASSERT_TRUE(&s.getValues().front() == &l);
    ASSERT_TRUE(s.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s.getOwnedElements().front() == &l);
    ASSERT_TRUE(l.getOwningSlot() == &s);
    ASSERT_TRUE(l.getOwner() == &s);
}

TEST_F(SlotTest, addValueFunctorTestW_Manager) {
    UmlManager m;
    Slot& s = m.create<Slot>();
    LiteralString& l = m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_TRUE(s.getValues().size() == 1);
    ASSERT_TRUE(&s.getValues().front() == &l);
    ASSERT_TRUE(s.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s.getOwnedElements().front() == &l);
    ASSERT_TRUE(l.getOwningSlot() == &s);
    ASSERT_TRUE(l.getOwner() == &s);
}

TEST_F(SlotTest, removeValueFunctorTest) {
    UmlManager m;
    Slot& s = m.create<Slot>();
    LiteralString& l = m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_NO_THROW(s.getValues().remove(l));
    ASSERT_TRUE(s.getValues().size() == 0);
    ASSERT_TRUE(s.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getOwningSlot() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(SlotTest, overrideOwningSlotW_NullTest) {
    UmlManager m;
    Slot& s = m.create<Slot>();
    LiteralString& l = m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_NO_THROW(l.setOwningSlot(0));
    ASSERT_TRUE(s.getValues().size() == 0);
    ASSERT_TRUE(s.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getOwningSlot() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(SlotTest, overrideOwningSlotW_OtherSlotTest) {
    UmlManager m;
    Slot& s = m.create<Slot>();
    LiteralString& l = m.create<LiteralString>();
    Slot& s2 = m.create<Slot>();
    s.getValues().add(l);
    ASSERT_NO_THROW(s2.getValues().add(l));
    ASSERT_TRUE(s.getValues().size() == 0);
    ASSERT_TRUE(s.getOwnedElements().size() == 0);

    ASSERT_TRUE(s2.getValues().size() == 1);
    ASSERT_TRUE(&s2.getValues().front() == &l);
    ASSERT_TRUE(s2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s2.getOwnedElements().front() == &l);
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(SlotDefiningFeature, Property, Slot, &Slot::getDefiningFeature, &Slot::setDefiningFeature);
UML_SET_INTEGRATION_TEST(SlotValues, LiteralBool, Slot, &Slot::getValues);
UML_SINGLETON_INTEGRATION_TEST(SlotOwningInstance, InstanceSpecification, Slot, &Slot::getOwningInstance, &Slot::setOwningInstance);

class SlotTest : public ::testing::Test {
   
};

TEST_F(SlotTest, addValueFunctorTest) {
    UmlManager m;
    Slot& s = *m.create<Slot>();
    LiteralString& l = *m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_TRUE(s.getValues().size() == 1);
    ASSERT_TRUE(&s.getValues().front() == &l);
    ASSERT_TRUE(s.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s.getOwnedElements().get(l.getID()) == &l);
    // ASSERT_TRUE(l.getOwningSlot() == &s);
    ASSERT_TRUE(l.getOwner() == &s);
}

TEST_F(SlotTest, addValueFunctorTestW_Manager) {
    UmlManager m;
    Slot& s = *m.create<Slot>();
    LiteralString& l = *m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_TRUE(s.getValues().size() == 1);
    ASSERT_TRUE(&s.getValues().front() == &l);
    ASSERT_TRUE(s.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s.getOwnedElements().get(l.getID()) == &l);
    // ASSERT_TRUE(l.getOwningSlot() == &s);
    ASSERT_TRUE(l.getOwner() == &s);
}

TEST_F(SlotTest, removeValueFunctorTest) {
    UmlManager m;
    Slot& s = *m.create<Slot>();
    LiteralString& l = *m.create<LiteralString>();
    s.getValues().add(l);
    ASSERT_NO_THROW(s.getValues().remove(l));
    ASSERT_TRUE(s.getValues().size() == 0);
    ASSERT_TRUE(s.getOwnedElements().size() == 0);
    ASSERT_FALSE(l.getOwner());
}

TEST_F(SlotTest, overrideOwningSlotW_OtherSlotTest) {
    UmlManager m;
    Slot& s = *m.create<Slot>();
    LiteralString& l = *m.create<LiteralString>();
    Slot& s2 = *m.create<Slot>();
    s.getValues().add(l);
    ASSERT_NO_THROW(s2.getValues().add(l));
    ASSERT_TRUE(s.getValues().size() == 0);
    ASSERT_TRUE(s.getOwnedElements().size() == 0);

    ASSERT_TRUE(s2.getValues().size() == 1);
    ASSERT_TRUE(&s2.getValues().front() == &l);
    ASSERT_TRUE(s2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&s2.getOwnedElements().get(l.getID()) == &l);
}
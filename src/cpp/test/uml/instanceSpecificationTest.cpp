#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class InstanceSpecificationTest : public ::testing::Test {};

TEST_F(InstanceSpecificationTest, setClassifierAsClass) {
    UmlManager m; // This won't work without manager
    Class& c = *m.create<Class>();
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    ASSERT_NO_THROW(i.getClassifiers().add(c));
    EXPECT_TRUE(i.getClassifiers().front().getID() == c.getID());
}

TEST_F(InstanceSpecificationTest, setStringValueSlots) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Property& stringP = *m.create<Property>();
    PrimitiveType& stringPrim = *m.create<PrimitiveType>();
    stringP.setType(stringPrim);
    c.getOwnedAttributes().add(stringP);
    LiteralString& ls = *m.create<LiteralString>();
    ls.setValue("test");
    Slot& stringSlot = *m.create<Slot>();
    stringSlot.setDefiningFeature(&stringP);
    stringSlot.getValues().add(ls);
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    i.getSlots().add(stringSlot);
    ASSERT_TRUE(i.getSlots().front().getID() == stringSlot.getID());
    ASSERT_TRUE(i.getSlots().front().getDefiningFeature()->getID() == stringP.getID());
    ASSERT_TRUE(i.getSlots().front().getValues().front().getID() == ls.getID());
}

// TEST_F(InstanceSpecificationTest, setSlotAsInstanceValue) {
//     UmlManager m;
//     Class& c = *m.create<Class>();
//     c.setName("typeA");
//     Class& b = *m.create<Class>();
//     b.setName("typeB");
//     Property& bProp = *m.create<Property>();
//     bProp.setName("b");
//     bProp.setType(b);
//     c.getOwnedAttributes().add(bProp);
//     InstanceSpecification& bInst = *m.create<InstanceSpecification>();
//     bInst.getClassifiers().add(b);
//     InstanceSpecification& aInst = *m.create<InstanceSpecification>();
//     aInst.getClassifiers().add(c);
//     InstanceValue& bVal = *m.create<InstanceValue>();
//     bVal.setInstance(&bInst);
//     Slot& aSlot = m.create<Slot>();
//     aSlot.setDefiningFeature(&bProp);
//     aSlot.getValues().add(bVal);
//     aInst.getSlots().add(aSlot);
//     ASSERT_TRUE(aInst.getSlots().front().getDefiningFeature()->getID() == bProp.getID());
// }

// // TODO add throw for pushing slots that don't correspond structural feature


TEST_F(InstanceSpecificationTest, reindexSlotID_Test) {
    UmlManager m;
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    Slot& s = *m.create<Slot>();
    i.getSlots().add(s);
    s.setOwningInstance(&i);
    s.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_NO_THROW(i.getSlots().get(s.getID()));
    ASSERT_NO_THROW(i.getOwnedElements().get(s.getID()));
}

TEST_F(InstanceSpecificationTest, addSlotFunctorTest) {
    UmlManager m;
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    Slot& s = *m.create<Slot>();
    i.getSlots().add(s);
    ASSERT_EQ(i.getSlots().get(s.getID()), s);
    ASSERT_EQ(i.getSlots().size(), 1);
    ASSERT_EQ(i.getOwnedElements().get(s.getID()), s);
    ASSERT_EQ(i.getOwnedElements().size(), 1);
    ASSERT_TRUE(s.getOwningInstance());
    ASSERT_EQ(*s.getOwningInstance(), i);
}

TEST_F(InstanceSpecificationTest, SetOwningInstanceFunctionalityTest) {
    UmlManager m;
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    Slot& s = *m.create<Slot>();
    s.setOwningInstance(i);
    ASSERT_EQ(i.getSlots().get(s.getID()), s);
    ASSERT_EQ(i.getSlots().size(), 1);
    ASSERT_EQ(i.getOwnedElements().get(s.getID()), s);
    ASSERT_EQ(i.getOwnedElements().size(), 1);
    ASSERT_TRUE(s.getOwningInstance());
    ASSERT_EQ(*s.getOwningInstance(), i);
}

TEST_F(InstanceSpecificationTest, removeSlotFunctorTest) {
    UmlManager m;
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    Slot& s = *m.create<Slot>();
    s.setOwningInstance(&i);
    ASSERT_NO_THROW(i.getSlots().remove(s));
    ASSERT_TRUE(i.getSlots().size() == 0);
    ASSERT_TRUE(i.getOwnedElements().size() == 0);
    ASSERT_TRUE(!s.getOwningInstance());
    ASSERT_TRUE(!s.getOwner());
}

TEST_F(InstanceSpecificationTest, setAndRemoveSpecificationTest) {
    UmlManager m;
    InstanceSpecification& inst = *m.create<InstanceSpecification>();
    LiteralString& str = *m.create<LiteralString>();
    inst.setSpecification(&str);
    ASSERT_TRUE(inst.getSpecification());
    ASSERT_EQ(inst.getSpecification()->getID(), str.getID());
    ASSERT_EQ(inst.getOwnedElements().size(), 1);
    ASSERT_EQ(inst.getOwnedElements().get(str.getID()).getID(), str.getID()); //lol
    ASSERT_TRUE(str.getOwner());
    ASSERT_EQ(str.getOwner()->getID(), inst.getID());
    inst.setSpecification(0);
    ASSERT_FALSE(inst.getSpecification());
    ASSERT_EQ(inst.getOwnedElements().size(), 0);
    ASSERT_FALSE(str.getOwner());    
}

// TEST_F(InstanceSpecificationTest, reindexClassifierTest) {
//     UmlManager m;
//     Class& c = *m.create<Class>();
//     InstanceSpecification& i = *m.create<InstanceSpecification>();
//     Package& root = *m.create<Package>();
//     m.setRoot(&root);
//     root.getPackagedElements().add(c, i);
//     i.getClassifiers().add(c);
//     m.mount(ymlPath + "instanceSpecificationTests");
//     ID id = ID::fromString("9nU_h2_riMLlgcg2FzNiGyvtbew3");
//     c.setID(id);
//     ASSERT_FALSE(i.getClassifiers().empty());
//     ASSERT_EQ(i.getClassifiers().size(), 1);
//     ASSERT_EQ(c.getID(), i.getClassifiers().front().getID());
//     // m.release(c);
//     // ASSERT_TRUE(i.hasClassifier());
//     // ASSERT_TRUE(i.getClassifier());
//     // ASSERT_EQ(i.getClassifierRef().getID(), id);
// }
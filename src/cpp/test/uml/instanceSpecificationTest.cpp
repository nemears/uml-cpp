#include "gtest/gtest.h"
#include "uml/instanceSpecification.h"
#include "uml/class.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/instanceValue.h"

using namespace UML;

class InstanceSpecificationTest : public ::testing::Test {
    
};

TEST_F(InstanceSpecificationTest, setClassifierAsClass) {
    Class c;
    InstanceSpecification i;
    ASSERT_NO_THROW(i.setClassifier(&c));
    EXPECT_TRUE(i.getClassifier()->getID() == c.getID());
}

TEST_F(InstanceSpecificationTest, setStringValueSlots) {
    Class c;
    Property stringP;
    PrimitiveType stringPrim;
    stringPrim.setPrimitiveType(PrimitiveType::Primitive::STRING);
    stringP.setType(&stringPrim);
    c.getAttributes().add(stringP);
    LiteralString ls;
    ls.setValue("test");
    Slot stringSlot;
    stringSlot.setDefiningFeature(&stringP);
    stringSlot.getValues().add(ls);
    InstanceSpecification i;
    i.getSlots().add(stringSlot);
    ASSERT_TRUE(i.getSlots().front()->getID() == stringSlot.getID());
    ASSERT_TRUE(i.getSlots().front()->getDefiningFeature()->getID() == stringP.getID());
    ASSERT_TRUE(i.getSlots().front()->getValues().front()->getID() == ls.getID());
}

TEST_F(InstanceSpecificationTest, setSlotAsInstanceValue) {
    Class c;
    c.setName("typeA");
    Class b;
    b.setName("typeB");
    Property bProp;
    bProp.setName("b");
    bProp.setType(&b);
    c.getAttributes().add(bProp);
    InstanceSpecification bInst;
    bInst.setClassifier(&b);
    InstanceSpecification aInst;
    aInst.setClassifier(&c);
    InstanceValue bVal;
    bVal.setInstance(&bInst);
    Slot aSlot;
    aSlot.setDefiningFeature(&bProp);
    aSlot.getValues().add(bVal);
    aInst.getSlots().add(aSlot);
    ASSERT_TRUE(aInst.getSlots().front()->getDefiningFeature()->getID() == bProp.getID());
}

// TODO add throw for pushing slots that don't correspond structural feature


// TEST_F(InstanceSpecificationTest, AddSlotTest) {
//     InstanceSpecification i;
//     Slot s;
//     ASSERT_NO_THROW(i.addSlot(s));
//     ASSERT_TRUE(i.slots.front()->uuid == s.getID());
// }

// TEST_F(InstanceSpecificationTest, AddAndRemoveSlotTest) {
//     InstanceSpecification i;
//     Slot s;
//     Slot s2;
//     i.addSlot(s);
//     i.addSlot(s2);
//     ASSERT_TRUE(i.slots.front()->uuid ==s.getID());
//     ASSERT_NO_THROW(i.removeSlot(s));
//     ASSERT_TRUE(i.slots.front()->uuid == s2.getID());
// }
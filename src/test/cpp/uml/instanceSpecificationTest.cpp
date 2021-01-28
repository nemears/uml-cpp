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
    EXPECT_TRUE(i.getClassifier()->uuid == c.uuid);
}

TEST_F(InstanceSpecificationTest, setStringValueSlots) {
    Class c;
    Property stringP;
    PrimitiveType stringPrim;
    stringPrim.setPrimitiveType(PrimitiveType::Primitive::STRING);
    stringP.setType(&stringPrim);
    c.ownedAttributes.push_back(&stringP);
    LiteralString ls;
    ls.setValue("test");
    Slot stringSlot;
    stringSlot.setDefiningFeature(&stringP);
    stringSlot.values.push_back(&ls);
    InstanceSpecification i;
    i.slots.push_back(&stringSlot);
    ASSERT_TRUE(i.slots.front()->uuid == stringSlot.uuid);
    ASSERT_TRUE(i.slots.front()->getDefiningFeature()->uuid == stringP.uuid);
    ASSERT_TRUE(i.slots.front()->values.front()->uuid == ls.uuid);
}

TEST_F(InstanceSpecificationTest, setSlotAsInstanceValue) {
    Class c;
    c.setName("typeA");
    Class b;
    b.setName("typeB");
    Property bProp;
    bProp.setName("b");
    bProp.setType(&b);
    c.ownedAttributes.push_back(&bProp);
    InstanceSpecification bInst;
    bInst.setClassifier(&b);
    InstanceSpecification aInst;
    aInst.setClassifier(&c);
    InstanceValue bVal;
    bVal.setInstance(&bInst);
    Slot aSlot;
    aSlot.setDefiningFeature(&bProp);
    aSlot.values.push_back(&bVal);
    aInst.slots.push_back(&aSlot);
    ASSERT_TRUE(aInst.slots.front()->getDefiningFeature()->uuid == bProp.uuid);
}

// TODO add throw for pushing slots that don't correspond structural feature
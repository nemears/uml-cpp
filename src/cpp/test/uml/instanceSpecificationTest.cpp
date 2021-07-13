#include "gtest/gtest.h"
#include "uml/instanceSpecification.h"
#include "uml/class.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/instanceValue.h"
#include "uml/property.h"

using namespace UML;

class InstanceSpecificationTest : public ::testing::Test {};

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


TEST_F(InstanceSpecificationTest, reindexSlotID_Test) {
    InstanceSpecification i;
    Slot s;
    i.getSlots().add(s);
    s.setOwningInstance(&i);
    s.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_TRUE(i.getSlots().get(s.getID()) != NULL);
    ASSERT_TRUE(i.getOwnedElements().get(s.getID()) != NULL);
}

TEST_F(InstanceSpecificationTest, addSlotFunctorTest) {
    InstanceSpecification i;
    Slot s;
    i.getSlots().add(s);
    ASSERT_TRUE(i.getSlots().get(s.getID()) == &s);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(i.getOwnedElements().get(s.getID()) == &s);
    ASSERT_TRUE(i.getOwnedElements().size() == 1);
    ASSERT_TRUE(s.getOwningInstance() == &i);
}

TEST_F(InstanceSpecificationTest, SetOwningInstanceFunctionalityTest) {
    InstanceSpecification i;
    Slot s;
    s.setOwningInstance(&i);
    ASSERT_TRUE(i.getSlots().get(s.getID()) == &s);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(i.getOwnedElements().get(s.getID()) == &s);
    ASSERT_TRUE(i.getOwnedElements().size() == 1);
    ASSERT_TRUE(s.getOwningInstance() == &i);
}

TEST_F(InstanceSpecificationTest, SetOwningInstanceFunctionalityTestW_Manager) {
    UmlManager m;
    InstanceSpecification& i = m.create<InstanceSpecification>();
    Slot& s = m.create<Slot>();
    s.setOwningInstance(&i);
    ASSERT_TRUE(i.getSlots().get(s.getID()) == &s);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(i.getOwnedElements().get(s.getID()) == &s);
    ASSERT_TRUE(i.getOwnedElements().size() == 1);
    ASSERT_TRUE(s.getOwningInstance() == &i);
}

TEST_F(InstanceSpecificationTest, removeSlotFunctorTest) {
    InstanceSpecification i;
    Slot s;
    s.setOwningInstance(&i);
    ASSERT_NO_THROW(i.getSlots().remove(s));
    ASSERT_TRUE(i.getSlots().size() == 0);
    ASSERT_TRUE(i.getOwnedElements().size() == 0);
    ASSERT_TRUE(!s.getOwningInstance());
    ASSERT_TRUE(!s.getOwner());
}

TEST_F(InstanceSpecificationTest, removeSlotFunctorTestW_Manager) {
    UmlManager m;
    InstanceSpecification& i = m.create<InstanceSpecification>();
    Slot& s = m.create<Slot>();
    s.setOwningInstance(&i);
    ASSERT_NO_THROW(i.getSlots().remove(s));
    ASSERT_TRUE(i.getSlots().size() == 0);
    ASSERT_TRUE(i.getOwnedElements().size() == 0);
    ASSERT_TRUE(!s.getOwningInstance());
    ASSERT_TRUE(!s.getOwner());
}
#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/instanceSpecification.h"
#include "../../../cpp/uml/headers/class.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"

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
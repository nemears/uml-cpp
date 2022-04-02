#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(InterfaceOwnedAttribute, Property, Interface, &Interface::getOwnedAttributes);
UML_SINGLETON_INTEGRATION_TEST(PropertyInterface, Interface, Property, &Property::getInterface, &Property::setInterface);
UML_SET_INTEGRATION_TEST(InterfaceOwnedOperation, Operation, Interface, &Interface::getOwnedOperations);
UML_SINGLETON_INTEGRATION_TEST(OperationInterface, Interface, Operation, &Operation::getInterface, &Operation::setInterface);
// UML_SET_INTEGRATION_TEST(InterfaceReception, Reception, Interface, &Interface::getOwnedReceptions);
UML_SET_INTEGRATION_TEST(InterfaceNestedClassifiers, Class, Interface, &Interface::getNestedClassifiers);
UML_SINGLETON_INTEGRATION_TEST(InterfaceRealizationContract, Interface, InterfaceRealization, &InterfaceRealization::getContract, &InterfaceRealization::setContract);
UML_SINGLETON_INTEGRATION_TEST(InterfaceRealizationImplementingClassifier, Class, InterfaceRealization, &InterfaceRealization::getImplementingClassifier, &InterfaceRealization::setImplementingClassifier);
UML_SET_INTEGRATION_TEST(BehavioredClassifierInterfaceRealizations, InterfaceRealization, Class, &BehavioredClassifier::getInterfaceRealizations);

class InterfaceTest : public ::testing::Test {};

TEST_F(InterfaceTest, basicInterfaceTest) {
    UmlManager m;
    Interface& interface_uml = *m.create<Interface>();
    Property& property1 = *m.create<Property>();
    Property& property2 = *m.create<Property>();
    Operation& operation1 = *m.create<Operation>();
    Operation& operation2 = *m.create<Operation>();
    DataType& nested1 = *m.create<DataType>();
    Class& nested2 = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Class& implementingClassifier = *m.create<Class>();

    
    interface_uml.getOwnedAttributes().add(property1);
    interface_uml.getOwnedOperations().add(operation1);
    interface_uml.getNestedClassifiers().add(nested1);
    interface_uml.getOwnedAttributes().add(property2);
    interface_uml.getOwnedOperations().add(operation2);
    interface_uml.getNestedClassifiers().add(nested2);
    realization.setContract(interface_uml);
    implementingClassifier.getInterfaceRealizations().add(realization);

    ASSERT_EQ(interface_uml.getOwnedAttributes().size(), 2);
    ASSERT_EQ(interface_uml.getOwnedOperations().size(), 2);
    ASSERT_EQ(interface_uml.getNestedClassifiers().size(), 2);
    ASSERT_EQ(interface_uml.getAttributes().size(), 2);
    ASSERT_EQ(interface_uml.getFeatures().size(), 4);
    ASSERT_EQ(interface_uml.getOwnedMembers().size(), 6);
    ASSERT_EQ(interface_uml.getMembers().size(), 6);
    ASSERT_EQ(interface_uml.getOwnedElements().size(), 6);

    ASSERT_TRUE(interface_uml.getOwnedAttributes().contains(property1));
    ASSERT_FALSE(interface_uml.getOwnedOperations().contains(property1.getID()));
    ASSERT_TRUE(interface_uml.getAttributes().contains(property1));
    ASSERT_TRUE(interface_uml.getFeatures().contains(property1));
    ASSERT_FALSE(interface_uml.getNestedClassifiers().contains(property1.getID()));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(property1));
    ASSERT_TRUE(interface_uml.getMembers().contains(property1));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(property1));

    ASSERT_TRUE(interface_uml.getOwnedAttributes().contains(property2));
    ASSERT_FALSE(interface_uml.getOwnedOperations().contains(property2.getID()));
    ASSERT_TRUE(interface_uml.getAttributes().contains(property2));
    ASSERT_TRUE(interface_uml.getFeatures().contains(property2));
    ASSERT_FALSE(interface_uml.getNestedClassifiers().contains(property2.getID()));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(property2));
    ASSERT_TRUE(interface_uml.getMembers().contains(property2));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(property2));

    ASSERT_FALSE(interface_uml.getOwnedAttributes().contains(operation1.getID()));
    ASSERT_TRUE(interface_uml.getOwnedOperations().contains(operation1));
    ASSERT_FALSE(interface_uml.getAttributes().contains(operation1.getID()));
    ASSERT_TRUE(interface_uml.getFeatures().contains(operation1));
    ASSERT_FALSE(interface_uml.getNestedClassifiers().contains(operation1.getID()));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(operation1));
    ASSERT_TRUE(interface_uml.getMembers().contains(operation1));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(operation1));

    ASSERT_FALSE(interface_uml.getOwnedAttributes().contains(operation2.getID()));
    ASSERT_TRUE(interface_uml.getOwnedOperations().contains(operation2));
    ASSERT_FALSE(interface_uml.getAttributes().contains(operation2.getID()));
    ASSERT_TRUE(interface_uml.getFeatures().contains(operation2));
    ASSERT_FALSE(interface_uml.getNestedClassifiers().contains(operation2.getID()));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(operation2));
    ASSERT_TRUE(interface_uml.getMembers().contains(operation2));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(operation2));

    ASSERT_FALSE(interface_uml.getOwnedAttributes().contains(nested1.getID()));
    ASSERT_FALSE(interface_uml.getOwnedOperations().contains(nested1.getID()));
    ASSERT_FALSE(interface_uml.getAttributes().contains(nested1.getID()));
    ASSERT_FALSE(interface_uml.getFeatures().contains(nested1.getID()));
    ASSERT_TRUE(interface_uml.getNestedClassifiers().contains(nested1));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(nested1));
    ASSERT_TRUE(interface_uml.getMembers().contains(nested1));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(nested1));

    ASSERT_FALSE(interface_uml.getOwnedAttributes().contains(nested2.getID()));
    ASSERT_FALSE(interface_uml.getOwnedOperations().contains(nested2.getID()));
    ASSERT_FALSE(interface_uml.getAttributes().contains(nested2.getID()));
    ASSERT_FALSE(interface_uml.getFeatures().contains(nested2.getID()));
    ASSERT_TRUE(interface_uml.getNestedClassifiers().contains(nested2));
    ASSERT_TRUE(interface_uml.getOwnedMembers().contains(nested2));
    ASSERT_TRUE(interface_uml.getMembers().contains(nested2));
    ASSERT_TRUE(interface_uml.getOwnedElements().contains(nested2));

    ASSERT_EQ(implementingClassifier.getInterfaceRealizations().size(), 1);
    ASSERT_EQ(implementingClassifier.getOwnedElements().size(), 1);

    ASSERT_TRUE(implementingClassifier.getInterfaceRealizations().contains(realization));
    ASSERT_TRUE(implementingClassifier.getOwnedElements().contains(realization));

    ASSERT_TRUE(realization.getContract());
    ASSERT_TRUE(realization.getOwner());
    ASSERT_TRUE(realization.getImplementingClassifier());

    ASSERT_EQ(*realization.getContract(), interface_uml);
    ASSERT_EQ(*realization.getImplementingClassifier(), implementingClassifier);
    ASSERT_EQ(*realization.getOwner(), implementingClassifier);
}
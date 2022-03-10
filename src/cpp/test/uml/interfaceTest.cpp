#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class InterfaceTest : public ::testing::Test {};

TEST_F(InterfaceTest, basicInterfaceTest) {
    UmlManager m;
    Interface& interface = *m.create<Interface>();
    Property& property1 = *m.create<Property>();
    Property& property2 = *m.create<Property>();
    Operation& operation1 = *m.create<Operation>();
    Operation& operation2 = *m.create<Operation>();
    DataType& nested1 = *m.create<DataType>();
    Class& nested2 = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Class& implementingClassifier = *m.create<Class>();

    
    interface.getOwnedAttributes().add(property1);
    interface.getOwnedOperations().add(operation1);
    interface.getNestedClassifiers().add(nested1);
    interface.getOwnedAttributes().add(property2);
    interface.getOwnedOperations().add(operation2);
    interface.getNestedClassifiers().add(nested2);
    realization.setContract(interface);
    implementingClassifier.getInterfaceRealizations().add(realization);

    ASSERT_EQ(interface.getOwnedAttributes().size(), 2);
    ASSERT_EQ(interface.getOwnedOperations().size(), 2);
    ASSERT_EQ(interface.getNestedClassifiers().size(), 2);
    ASSERT_EQ(interface.getAttributes().size(), 2);
    ASSERT_EQ(interface.getFeatures().size(), 4);
    ASSERT_EQ(interface.getOwnedMembers().size(), 6);
    ASSERT_EQ(interface.getMembers().size(), 6);
    ASSERT_EQ(interface.getOwnedElements().size(), 6);

    ASSERT_TRUE(interface.getOwnedAttributes().contains(property1));
    ASSERT_FALSE(interface.getOwnedOperations().contains(property1.getID()));
    ASSERT_TRUE(interface.getAttributes().contains(property1));
    ASSERT_TRUE(interface.getFeatures().contains(property1));
    ASSERT_FALSE(interface.getNestedClassifiers().contains(property1.getID()));
    ASSERT_TRUE(interface.getOwnedMembers().contains(property1));
    ASSERT_TRUE(interface.getMembers().contains(property1));
    ASSERT_TRUE(interface.getOwnedElements().contains(property1));

    ASSERT_TRUE(interface.getOwnedAttributes().contains(property2));
    ASSERT_FALSE(interface.getOwnedOperations().contains(property2.getID()));
    ASSERT_TRUE(interface.getAttributes().contains(property2));
    ASSERT_TRUE(interface.getFeatures().contains(property2));
    ASSERT_FALSE(interface.getNestedClassifiers().contains(property2.getID()));
    ASSERT_TRUE(interface.getOwnedMembers().contains(property2));
    ASSERT_TRUE(interface.getMembers().contains(property2));
    ASSERT_TRUE(interface.getOwnedElements().contains(property2));

    ASSERT_FALSE(interface.getOwnedAttributes().contains(operation1.getID()));
    ASSERT_TRUE(interface.getOwnedOperations().contains(operation1));
    ASSERT_FALSE(interface.getAttributes().contains(operation1.getID()));
    ASSERT_TRUE(interface.getFeatures().contains(operation1));
    ASSERT_FALSE(interface.getNestedClassifiers().contains(operation1.getID()));
    ASSERT_TRUE(interface.getOwnedMembers().contains(operation1));
    ASSERT_TRUE(interface.getMembers().contains(operation1));
    ASSERT_TRUE(interface.getOwnedElements().contains(operation1));

    ASSERT_FALSE(interface.getOwnedAttributes().contains(operation2.getID()));
    ASSERT_TRUE(interface.getOwnedOperations().contains(operation2));
    ASSERT_FALSE(interface.getAttributes().contains(operation2.getID()));
    ASSERT_TRUE(interface.getFeatures().contains(operation2));
    ASSERT_FALSE(interface.getNestedClassifiers().contains(operation2.getID()));
    ASSERT_TRUE(interface.getOwnedMembers().contains(operation2));
    ASSERT_TRUE(interface.getMembers().contains(operation2));
    ASSERT_TRUE(interface.getOwnedElements().contains(operation2));

    ASSERT_FALSE(interface.getOwnedAttributes().contains(nested1.getID()));
    ASSERT_FALSE(interface.getOwnedOperations().contains(nested1.getID()));
    ASSERT_FALSE(interface.getAttributes().contains(nested1.getID()));
    ASSERT_FALSE(interface.getFeatures().contains(nested1.getID()));
    ASSERT_TRUE(interface.getNestedClassifiers().contains(nested1));
    ASSERT_TRUE(interface.getOwnedMembers().contains(nested1));
    ASSERT_TRUE(interface.getMembers().contains(nested1));
    ASSERT_TRUE(interface.getOwnedElements().contains(nested1));

    ASSERT_FALSE(interface.getOwnedAttributes().contains(nested2.getID()));
    ASSERT_FALSE(interface.getOwnedOperations().contains(nested2.getID()));
    ASSERT_FALSE(interface.getAttributes().contains(nested2.getID()));
    ASSERT_FALSE(interface.getFeatures().contains(nested2.getID()));
    ASSERT_TRUE(interface.getNestedClassifiers().contains(nested2));
    ASSERT_TRUE(interface.getOwnedMembers().contains(nested2));
    ASSERT_TRUE(interface.getMembers().contains(nested2));
    ASSERT_TRUE(interface.getOwnedElements().contains(nested2));

    ASSERT_EQ(implementingClassifier.getInterfaceRealizations().size(), 1);
    ASSERT_EQ(implementingClassifier.getOwnedElements().size(), 1);

    ASSERT_TRUE(implementingClassifier.getInterfaceRealizations().contains(realization));
    ASSERT_TRUE(implementingClassifier.getOwnedElements().contains(realization));

    ASSERT_TRUE(realization.getContract());
    ASSERT_TRUE(realization.getOwner());
    ASSERT_TRUE(realization.getImplementingClassifier());

    ASSERT_EQ(*realization.getContract(), interface);
    ASSERT_EQ(*realization.getImplementingClassifier(), implementingClassifier);
    ASSERT_EQ(*realization.getOwner(), implementingClassifier);
}
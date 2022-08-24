#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class PortTest : public ::testing::Test {};

TEST_F(PortTest, portWInterfaceType) {
    BasicManager m;
    Interface& type = *m.create<Interface>();
    Class& encapsulatedClassifier = *m.create<Class>();
    Port& port = *m.create<Port>();
    port.setType(type);
    encapsulatedClassifier.getOwnedAttributes().add(port);

    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_TRUE(port.getClass());

    ASSERT_EQ(port.getProvided().front(), type);
    ASSERT_EQ(*port.getClass(), encapsulatedClassifier);
    
    ASSERT_EQ(encapsulatedClassifier.getOwnedPorts().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getOwnedAttributes().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getAttributes().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getFeatures().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getOwnedMembers().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getMembers().size(), 1);
    ASSERT_EQ(encapsulatedClassifier.getOwnedElements().size(), 1);

    port.setIsConjugated(true);

    ASSERT_EQ(port.getProvided().size(), 0);
    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getRequired().front(), type);
}

TEST_F(PortTest, portW_TypeBehavioredClassifierW_Generals) {
    BasicManager m;
    Interface& realizedInterface = *m.create<Interface>();
    Interface& usedInterface = *m.create<Interface>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Usage& usage = *m.create<Usage>();
    Class& specific = *m.create<Class>();
    Class& general = *m.create<Class>();
    Generalization& generalization = *m.create<Generalization>();
    Port& port = *m.create<Port>();
    Class& encapsulatedClassifier = *m.create<Class>();
    realization.setContract(realizedInterface);
    specific.getInterfaceRealizations().add(realization);
    usage.getSuppliers().add(usedInterface);
    usage.getClients().add(general);
    generalization.setGeneral(general);
    specific.getGeneralizations().add(generalization);
    encapsulatedClassifier.getOwnedAttributes().add(port);
    port.setType(specific);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);

    ASSERT_EQ(port.getRequired().front(), usedInterface);
    ASSERT_EQ(port.getProvided().front(), realizedInterface);

    port.setIsConjugated(true);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);

    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    port.setType(0);

    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_EQ(port.getProvided().size(), 0);

    // TODO test getting rid of realization and usage, those will definitely not work
    port.setType(specific);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    specific.getInterfaceRealizations().remove(realization);

    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_EQ(port.getProvided().size(), 1);

    specific.getInterfaceRealizations().add(realization);
    
    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    realization.setContract(0);

    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_EQ(port.getProvided().size(), 1);

    realization.setContract(realizedInterface);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    usage.getClients().remove(general);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 0);

    usage.getClients().add(general);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    specific.getInterfaceRealizations().remove(realization);
    usage.getClients().remove(general);

    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_EQ(port.getProvided().size(), 0);

    general.getInterfaceRealizations().add(realization);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 0);

    usage.getClients().add(specific);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);

    general.getInterfaceRealizations().remove(realization);

    ASSERT_EQ(port.getRequired().size(), 0);
    ASSERT_EQ(port.getProvided().size(), 1);

    general.getInterfaceRealizations().add(realization);

    usage.getClients().remove(specific);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 0);

    usage.getClients().add(specific);

    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getProvided().size(), 1);
    ASSERT_EQ(port.getRequired().front(), realizedInterface);
    ASSERT_EQ(port.getProvided().front(), usedInterface);
}
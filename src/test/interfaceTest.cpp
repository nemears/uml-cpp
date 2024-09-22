#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(InterfaceOwnedAttribute, Property, Interface, &Interface::getOwnedAttributes)
UML_SINGLETON_INTEGRATION_TEST(PropertyInterface, Interface, Property, &Property::getInterface, &Property::setInterface)
UML_SET_INTEGRATION_TEST(InterfaceOwnedOperation, Operation, Interface, &Interface::getOwnedOperations)
UML_SINGLETON_INTEGRATION_TEST(OperationInterface, Interface, Operation, &Operation::getInterface, &Operation::setInterface)
// UML_SET_INTEGRATION_TEST(InterfaceReception, Reception, Interface, &Interface::getOwnedReceptions);
UML_SET_INTEGRATION_TEST(InterfaceNestedClassifiers, Class, Interface, &Interface::getNestedClassifiers)
UML_SINGLETON_INTEGRATION_TEST(InterfaceRealizationContract, Interface, InterfaceRealization, &InterfaceRealization::getContract, &InterfaceRealization::setContract)
UML_SINGLETON_INTEGRATION_TEST(InterfaceRealizationImplementingClassifier, Class, InterfaceRealization, &InterfaceRealization::getImplementingClassifier, &InterfaceRealization::setImplementingClassifier)
UML_SET_INTEGRATION_TEST(BehavioredClassifierInterfaceRealizations, InterfaceRealization, Class, &BehavioredClassifier::getInterfaceRealizations)

class InterfaceTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

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

TEST_F(InterfaceTest, parseBasicInterfaceTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "interfaceTests/basicInterface.yml"));
    ElementPtr el = m.getRoot();
    ASSERT_TRUE(el->getElementType() == Package::Info::elementType);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("interface")->getElementType(), Interface::Info::elementType);
    Interface& interface_uml = pckg.getPackagedElements().get("interface")->as<Interface>();
    ASSERT_EQ(interface_uml.getOwnedAttributes().size(), 1);
    ASSERT_EQ(interface_uml.getOwnedOperations().size(), 1);
    ASSERT_EQ(interface_uml.getNestedClassifiers().size(), 1);
    Property& prop = *interface_uml.getOwnedAttributes().front();
    Operation& op = *interface_uml.getOwnedOperations().front();
    ASSERT_EQ(interface_uml.getNestedClassifiers().front()->getElementType(), DataType::Info::elementType);
    DataType& nested = interface_uml.getNestedClassifiers().front()->as<DataType>();
    ASSERT_EQ(prop.getName(), "prop");
    ASSERT_EQ(op.getName(), "op");
    ASSERT_EQ(nested.getName(), "nest");
    ASSERT_EQ(interface_uml.getGeneralizations().size(), 1);
    Generalization& generalization = *interface_uml.getGeneralizations().front();
    ASSERT_TRUE(generalization.getGeneral());
    ASSERT_EQ(generalization.getGeneral()->getElementType(), Interface::Info::elementType);
    Interface& general = generalization.getGeneral()->as<Interface>();
    Class& clazz = pckg.getPackagedElements().get("implementingClazz")->as<Class>();
    ASSERT_EQ(clazz.getInterfaceRealizations().size(), 1);
    InterfaceRealization& realization = *clazz.getInterfaceRealizations().front();
    ASSERT_TRUE(realization.getContract());
    ASSERT_EQ(*realization.getContract(), interface_uml);
}

TEST_F(InterfaceTest, emitInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Interface& interface_uml = *m.create<Interface>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    DataType& nest = *m.create<DataType>();
    Interface& general = *m.create<Interface>();
    Generalization& generalization = *m.create<Generalization>();
    Class& implementing = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    root.setID(ID::fromString("efOYpQ48NuwY3f2xX0u9WkxcjfY6"));
    interface_uml.setID(ID::fromString("fqag25FXykqQlo_bQWmS&cAB6338"));
    prop.setID(ID::fromString("Jp2IhMjC2qNN7cIYPXiFZU4vDdun"));
    op.setID(ID::fromString("kbreSzh_ys_8SepvJR6Q58tzWdFI"));
    nest.setID(ID::fromString("TeIMyndF4nm_NOTbFZ&vZDLXxvtC"));
    general.setID(ID::fromString("msHdxlZxjCCYwm2GLlDyaIq3KX4o"));
    generalization.setID(ID::fromString("9mSwZjJaig2cKZA98jZku3nU74eH"));
    implementing.setID(ID::fromString("GqrX5Ta8KQDdFfaHrau08OS7Et3n"));
    realization.setID(ID::fromString("IFDK1OePanvL7GwUxLspBo4p2JjA"));
    interface_uml.getOwnedAttributes().add(prop);
    interface_uml.getOwnedOperations().add(op);
    interface_uml.getNestedClassifiers().add(nest);
    generalization.setGeneral(general);
    interface_uml.getGeneralizations().add(generalization);
    realization.setContract(interface_uml);
    implementing.getInterfaceRealizations().add(realization);
    root.getPackagedElements().add(interface_uml, general, implementing);
    std::string expectedEmit = R""""(Package:
  id: efOYpQ48NuwY3f2xX0u9WkxcjfY6
  packagedElements:
    - Class:
        id: GqrX5Ta8KQDdFfaHrau08OS7Et3n
        interfaceRealizations:
          - InterfaceRealization:
              id: IFDK1OePanvL7GwUxLspBo4p2JjA
              contract: "fqag25FXykqQlo_bQWmS&cAB6338"
    - Interface:
        id: msHdxlZxjCCYwm2GLlDyaIq3KX4o
    - Interface:
        id: "fqag25FXykqQlo_bQWmS&cAB6338"
        generalizations:
          - Generalization:
              id: 9mSwZjJaig2cKZA98jZku3nU74eH
              general: msHdxlZxjCCYwm2GLlDyaIq3KX4o
        ownedAttributes:
          - Property:
              id: Jp2IhMjC2qNN7cIYPXiFZU4vDdun
        ownedOperations:
          - Operation:
              id: kbreSzh_ys_8SepvJR6Q58tzWdFI
        nestedClassifiers:
          - DataType:
              id: "TeIMyndF4nm_NOTbFZ&vZDLXxvtC")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InterfaceTest, parsePortW_InterfaceTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "interfaceTests/portW_Interface.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == Package::Info::elementType);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    Class& implementing = pckg.getPackagedElements().get("implementing")->as<Class>();
    Interface& interface_uml = pckg.getPackagedElements().get("interface")->as<Interface>();
    Class& encapsulated = pckg.getPackagedElements().get("encapsulated")->as<Class>();
    ASSERT_EQ(implementing.getInterfaceRealizations().size(), 1);
    InterfaceRealization& realization = *implementing.getInterfaceRealizations().front();
    ASSERT_TRUE(realization.getContract());
    ASSERT_EQ(*realization.getContract(), interface_uml);
    ASSERT_EQ(encapsulated.getOwnedPorts().size(), 1);
    Port& port = *encapsulated.getOwnedPorts().front();
    ASSERT_TRUE(port.isConjugated());
    ASSERT_EQ(port.getRequired().size(), 1); // TODO fix type reference when opening
    ASSERT_EQ(port.getRequired().front(), &interface_uml);
}

TEST_F(InterfaceTest, emitPortWInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Class& implementing = *m.create<Class>();
    Interface& interface_uml = *m.create<Interface>();
    Class& encapsulated = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Port& port = *m.create<Port>();
    root.setID(ID::fromString("epLFcWN0KeMt8t5mAuF4TUCa75ns"));
    implementing.setID(ID::fromString("508FPtzv15GguudyAK6odJA7Rxoa"));
    interface_uml.setID(ID::fromString("Ehn7ZlJH&ULe75R26WWVcYlMKXeY"));
    encapsulated.setID(ID::fromString("R3dx7zjpK3&3NGLh0DVLt9Yolka8"));
    realization.setID(ID::fromString("65&HAREuzThGM38K2m82T1NWR28N"));
    port.setID(ID::fromString("loA63PcT8hpUsfQkDvU1p0YT4vRj"));
    realization.setContract(interface_uml);
    implementing.getInterfaceRealizations().add(realization);
    encapsulated.getOwnedAttributes().add(port);
    port.setType(implementing);
    port.setIsBehavior(true);
    port.setIsConjugated(true);
    port.setIsService(false);
    root.getPackagedElements().add(implementing, interface_uml, encapsulated);
    std::string expectedEmit = R""""(Package:
  id: epLFcWN0KeMt8t5mAuF4TUCa75ns
  packagedElements:
    - Class:
        id: "R3dx7zjpK3&3NGLh0DVLt9Yolka8"
        ownedAttributes:
          - Port:
              id: loA63PcT8hpUsfQkDvU1p0YT4vRj
              isBehavior: true
              isConjugated: true
              isService: false
              type: 508FPtzv15GguudyAK6odJA7Rxoa
    - Interface:
        id: "Ehn7ZlJH&ULe75R26WWVcYlMKXeY"
    - Class:
        id: 508FPtzv15GguudyAK6odJA7Rxoa
        interfaceRealizations:
          - InterfaceRealization:
              id: "65&HAREuzThGM38K2m82T1NWR28N"
              contract: "Ehn7ZlJH&ULe75R26WWVcYlMKXeY")"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InterfaceTest, mountInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Interface& interface_uml = *m.create<Interface>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    DataType& nest = *m.create<DataType>();
    Class& implementing = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Class& engine = *m.create<Class>();
    Class& propeller = *m.create<Class>();
    Port& pPort = *m.create<Port>();
    Port& ePort = *m.create<Port>();
    Property& pProp = *m.create<Property>();
    Property& eProp = *m.create<Property>();
    Class& boat = *m.create<Class>();
    Connector& connector = *m.create<Connector>();
    ConnectorEnd& pEnd = *m.create<ConnectorEnd>();
    ConnectorEnd& eEnd = *m.create<ConnectorEnd>();
    interface_uml.getOwnedAttributes().add(prop);
    interface_uml.getOwnedOperations().add(op);
    interface_uml.getNestedClassifiers().add(nest);
    realization.setContract(interface_uml);
    implementing.getInterfaceRealizations().add(realization);
    pPort.setType(implementing);
    ePort.setType(implementing);
    engine.getOwnedAttributes().add(ePort);
    propeller.getOwnedAttributes().add(pPort);
    pProp.setType(propeller);
    eProp.setType(engine);
    boat.getOwnedAttributes().add(eProp, pProp);
    eEnd.setRole(pPort);
    pEnd.setRole(ePort);
    // TODO part with port
    connector.getEnds().add(pEnd, eEnd);
    boat.getOwnedConnectors().add(connector);
    root.getPackagedElements().add(interface_uml, implementing, engine, propeller, boat);
    m.setRoot(&root);
    m.mount(ymlPath + "interfaceTests");

    ID interfaceID = interface_uml.getID();
    m.release(interface_uml);
    ASSERT_FALSE(m.loaded(interfaceID));
    Interface& interface2 = m.get(interfaceID)->as<Interface>();
    ASSERT_EQ(interface2.getOwningPackage().id(), root.getID());
    ASSERT_EQ(interface2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(interface2.getOwnedOperations().size(), 1);
    ASSERT_EQ(interface2.getNestedClassifiers().size(), 1);
    ASSERT_EQ(interface2.getAttributes().size(), 1);
    ASSERT_EQ(interface2.getFeatures().size(), 2);
    ASSERT_EQ(interface2.getOwnedMembers().size(), 3);
    ASSERT_EQ(interface2.getMembers().size(), 3);
    ASSERT_EQ(interface2.getOwnedElements().size(), 3);
    ASSERT_EQ(interface2.getOwnedAttributes().front(), &prop);
    ASSERT_EQ(interface2.getOwnedOperations().front(), &op);
    ASSERT_EQ(interface2.getNestedClassifiers().front(), &nest);

    ASSERT_EQ(pPort.getProvided().front(), &interface2);
    ASSERT_EQ(ePort.getProvided().front(), &interface2);

    ID propertyID = prop.getID();
    m.release(prop);
    ASSERT_FALSE(m.loaded(propertyID));
    Property& prop2 = m.get(propertyID)->as<Property>();
    ASSERT_TRUE(prop2.getInterface());
    ASSERT_EQ(*prop2.getInterface(), interface2);

    ID operationID = op.getID();
    m.release(op);
    ASSERT_FALSE(m.loaded(operationID));
    Operation& op2 = m.get(operationID)->as<Operation>();
    ASSERT_TRUE(op2.getInterface());
    ASSERT_EQ(*op2.getInterface(), interface2);

    ID nestID = nest.getID();
    m.release(nest);
    ASSERT_FALSE(m.loaded(nestID));
    DataType& nest2 = m.get(nestID)->as<DataType>();
    ASSERT_TRUE(nest2.getNamespace());
    ASSERT_EQ(*nest2.getNamespace(), interface2);

    ID realizationID = realization.getID();
    m.release(realization);
    ASSERT_FALSE(m.loaded(realizationID));
    InterfaceRealization& realization2 = m.get(realizationID)->as<InterfaceRealization>();
    ASSERT_TRUE(realization2.getImplementingClassifier());
    ASSERT_EQ(*realization2.getImplementingClassifier(), implementing);

    ID pPortID = pPort.getID();
    m.release(pPort);
    ASSERT_FALSE(m.loaded(pPortID));
    Port& pPort2 = m.get(pPortID)->as<Port>();
    ASSERT_TRUE(pPort2.getClass());
    ASSERT_EQ(*pPort2.getClass(), propeller);

    ID connectorID = connector.getID();
    m.release(connector);
    ASSERT_FALSE(m.loaded(connectorID));
    Connector& connector2 = m.get(connectorID)->as<Connector>();
    ASSERT_TRUE(connector2.getFeaturingClassifier());
    ASSERT_EQ(*connector2.getFeaturingClassifier(), boat);
}

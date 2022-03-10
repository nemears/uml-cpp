#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/yumlParsersTest.h"
#include "uml/parsers/parser.h"
#include "test/umlTestUtil.h"

using namespace UML;

class InterfaceParserTest : public ::testing::Test {
   public:
    std::string ymlPath = YML_FILES_PATH;
};

TEST_F(InterfaceParserTest, parseBasicInterfaceTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "interfaceTests/basicInterface.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("interface").getElementType(), ElementType::INTERFACE);
    Interface& interface = pckg.getPackagedElements().get("interface").as<Interface>();
    ASSERT_EQ(interface.getOwnedAttributes().size(), 1);
    ASSERT_EQ(interface.getOwnedOperations().size(), 1);
    ASSERT_EQ(interface.getNestedClassifiers().size(), 1);
    Property& prop = interface.getOwnedAttributes().front();
    Operation& op = interface.getOwnedOperations().front();
    ASSERT_EQ(interface.getNestedClassifiers().front().getElementType(), ElementType::DATA_TYPE);
    DataType& nested = interface.getNestedClassifiers().front().as<DataType>();
    ASSERT_EQ(prop.getName(), "prop");
    ASSERT_EQ(op.getName(), "op");
    ASSERT_EQ(nested.getName(), "nest");
    ASSERT_EQ(interface.getGeneralizations().size(), 1);
    Generalization& generalization = interface.getGeneralizations().front();
    ASSERT_TRUE(generalization.getGeneral());
    ASSERT_EQ(generalization.getGeneral()->getElementType(), ElementType::INTERFACE);
    Interface& general = generalization.getGeneral()->as<Interface>();
    Class& clazz = pckg.getPackagedElements().get("implementingClazz").as<Class>();
    ASSERT_EQ(clazz.getInterfaceRealizations().size(), 1);
    InterfaceRealization& realization = clazz.getInterfaceRealizations().front();
    ASSERT_TRUE(realization.getContract());
    ASSERT_EQ(*realization.getContract(), interface);
}

TEST_F(InterfaceParserTest, emitInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Interface& interface = *m.create<Interface>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    DataType& nest = *m.create<DataType>();
    Interface& general = *m.create<Interface>();
    Generalization& generalization = *m.create<Generalization>();
    Class& implementing = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    root.setID("efOYpQ48NuwY3f2xX0u9WkxcjfY6");
    interface.setID("fqag25FXykqQlo_bQWmS&cAB6338");
    prop.setID("Jp2IhMjC2qNN7cIYPXiFZU4vDdun");
    op.setID("kbreSzh_ys_8SepvJR6Q58tzWdFI");
    nest.setID("TeIMyndF4nm_NOTbFZ&vZDLXxvtC");
    general.setID("msHdxlZxjCCYwm2GLlDyaIq3KX4o");
    generalization.setID("9mSwZjJaig2cKZA98jZku3nU74eH");
    implementing.setID("GqrX5Ta8KQDdFfaHrau08OS7Et3n");
    realization.setID("IFDK1OePanvL7GwUxLspBo4p2JjA");
    interface.getOwnedAttributes().add(prop);
    interface.getOwnedOperations().add(op);
    interface.getNestedClassifiers().add(nest);
    generalization.setGeneral(general);
    interface.getGeneralizations().add(generalization);
    realization.setContract(interface);
    implementing.getInterfaceRealizations().add(realization);
    root.getPackagedElements().add(interface, general, implementing);
    std::string expectedEmit = R""""(package:
  id: efOYpQ48NuwY3f2xX0u9WkxcjfY6
  packagedElements:
    - interface:
        id: fqag25FXykqQlo_bQWmS&cAB6338
        generalizations:
          - generalization:
              id: 9mSwZjJaig2cKZA98jZku3nU74eH
              general: msHdxlZxjCCYwm2GLlDyaIq3KX4o
        ownedAttributes:
          - property:
              id: Jp2IhMjC2qNN7cIYPXiFZU4vDdun
        ownedOperations:
          - operation:
              id: kbreSzh_ys_8SepvJR6Q58tzWdFI
        nestedClassifiers:
          - dataType:
              id: TeIMyndF4nm_NOTbFZ&vZDLXxvtC
    - class:
        id: GqrX5Ta8KQDdFfaHrau08OS7Et3n
        interfaceRealizations:
          - interfaceRealization:
              id: IFDK1OePanvL7GwUxLspBo4p2JjA
              contract: fqag25FXykqQlo_bQWmS&cAB6338
    - interface:
        id: msHdxlZxjCCYwm2GLlDyaIq3KX4o)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InterfaceParserTest, parsePortW_InterfaceTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "interfaceTests/portW_Interface.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    Class& implementing = pckg.getPackagedElements().get("implementing").as<Class>();
    Interface& interface = pckg.getPackagedElements().get("interface").as<Interface>();
    Class& encapsulated = pckg.getPackagedElements().get("encapsulated").as<Class>();
    ASSERT_EQ(implementing.getInterfaceRealizations().size(), 1);
    InterfaceRealization& realization = implementing.getInterfaceRealizations().front();
    ASSERT_TRUE(realization.getContract());
    ASSERT_EQ(*realization.getContract(), interface);
    ASSERT_EQ(encapsulated.getOwnedPorts().size(), 1);
    Port& port = encapsulated.getOwnedPorts().front();
    ASSERT_TRUE(port.isConjugated());
    ASSERT_EQ(port.getRequired().size(), 1);
    ASSERT_EQ(port.getRequired().front(), interface);
}

TEST_F(InterfaceParserTest, emitPortWInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Class& implementing = *m.create<Class>();
    Interface& interface = *m.create<Interface>();
    Class& encapsulated = *m.create<Class>();
    InterfaceRealization& realization = *m.create<InterfaceRealization>();
    Port& port = *m.create<Port>();
    root.setID("epLFcWN0KeMt8t5mAuF4TUCa75ns");
    implementing.setID("508FPtzv15GguudyAK6odJA7Rxoa");
    interface.setID("Ehn7ZlJH&ULe75R26WWVcYlMKXeY");
    encapsulated.setID("R3dx7zjpK3&3NGLh0DVLt9Yolka8");
    realization.setID("65&HAREuzThGM38K2m82T1NWR28N");
    port.setID("loA63PcT8hpUsfQkDvU1p0YT4vRj");
    realization.setContract(interface);
    implementing.getInterfaceRealizations().add(realization);
    encapsulated.getOwnedAttributes().add(port);
    port.setType(implementing);
    port.setIsBehavior(true);
    port.setIsConjugated(true);
    port.setIsService(false);
    root.getPackagedElements().add(implementing, interface, encapsulated);
    std::string expectedEmit = R""""(package:
  id: epLFcWN0KeMt8t5mAuF4TUCa75ns
  packagedElements:
    - class:
        id: 508FPtzv15GguudyAK6odJA7Rxoa
        interfaceRealizations:
          - interfaceRealization:
              id: 65&HAREuzThGM38K2m82T1NWR28N
              contract: Ehn7ZlJH&ULe75R26WWVcYlMKXeY
    - interface:
        id: Ehn7ZlJH&ULe75R26WWVcYlMKXeY
    - class:
        id: R3dx7zjpK3&3NGLh0DVLt9Yolka8
        ownedAttributes:
          - port:
              id: loA63PcT8hpUsfQkDvU1p0YT4vRj
              type: 508FPtzv15GguudyAK6odJA7Rxoa
              isBehavior: true
              isConjugated: true
              isService: false)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InterfaceParserTest, mountInterfaceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Interface& interface = *m.create<Interface>();
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
    interface.getOwnedAttributes().add(prop);
    interface.getOwnedOperations().add(op);
    interface.getNestedClassifiers().add(nest);
    realization.setContract(interface);
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
    root.getPackagedElements().add(interface, implementing, engine, propeller, boat);
    m.setRoot(root);
    m.mount(ymlPath + "interfaceTests");

    ID interfaceID = interface.getID();
    m.release(interface);
    ASSERT_FALSE(m.loaded(interfaceID));
    Interface& interface2 = m.aquire(interfaceID)->as<Interface>();
    ASSERT_EQ(interface2.getOwningPackage().id(), root.getID());
    ASSERT_EQ(interface2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(interface2.getOwnedOperations().size(), 1);
    ASSERT_EQ(interface2.getNestedClassifiers().size(), 1);
    ASSERT_EQ(interface2.getAttributes().size(), 1);
    ASSERT_EQ(interface2.getFeatures().size(), 2);
    ASSERT_EQ(interface2.getOwnedMembers().size(), 3);
    ASSERT_EQ(interface2.getMembers().size(), 3);
    ASSERT_EQ(interface2.getOwnedElements().size(), 3);
    ASSERT_EQ(interface2.getOwnedAttributes().front(), prop);
    ASSERT_EQ(interface2.getOwnedOperations().front(), op);
    ASSERT_EQ(interface2.getNestedClassifiers().front(), nest);

    ASSERT_EQ(pPort.getProvided().front(), interface2);
    ASSERT_EQ(ePort.getProvided().front(), interface2);

    ID propertyID = prop.getID();
    m.release(prop);
    ASSERT_FALSE(m.loaded(propertyID));
    Property& prop2 = m.aquire(propertyID)->as<Property>();
    ASSERT_TRUE(prop2.getInterface());
    ASSERT_EQ(*prop2.getInterface(), interface2);

    ID operationID = op.getID();
    m.release(op);
    ASSERT_FALSE(m.loaded(operationID));
    Operation& op2 = m.aquire(operationID)->as<Operation>();
    ASSERT_TRUE(op2.getInterface());
    ASSERT_EQ(*op2.getInterface(), interface2);

    ID nestID = nest.getID();
    m.release(nest);
    ASSERT_FALSE(m.loaded(nestID));
    DataType& nest2 = m.aquire(nestID)->as<DataType>();
    ASSERT_TRUE(nest2.getNamespace());
    ASSERT_EQ(*nest2.getNamespace(), interface2);

    ID realizationID = realization.getID();
    m.release(realization);
    ASSERT_FALSE(m.loaded(realizationID));
    InterfaceRealization& realization2 = m.aquire(realizationID)->as<InterfaceRealization>();
    ASSERT_TRUE(realization2.getImplementingClassifier());
    ASSERT_EQ(*realization2.getImplementingClassifier(), implementing);

    ID pPortID = pPort.getID();
    m.release(pPort);
    ASSERT_FALSE(m.loaded(pPortID));
    Port& pPort2 = m.aquire(pPortID)->as<Port>();
    ASSERT_TRUE(pPort2.getClass());
    ASSERT_EQ(*pPort2.getClass(), propeller);

    ID connectorID = connector.getID();
    m.release(connector);
    ASSERT_FALSE(m.loaded(connectorID));
    Connector& connector2 = m.aquire(connectorID)->as<Connector>();
    ASSERT_TRUE(connector2.getFeaturingClassifier());
    ASSERT_EQ(*connector2.getFeaturingClassifier(), boat);
}
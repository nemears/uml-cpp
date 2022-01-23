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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "interfaceTests/basicInterface.yml"));
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
    ASSERT_TRUE(generalization.hasGeneral());
    ASSERT_EQ(generalization.getGeneralRef().getElementType(), ElementType::INTERFACE);
    Interface& general = generalization.getGeneralRef().as<Interface>();
    Class& clazz = pckg.getPackagedElements().get("implementingClazz").as<Class>();
    ASSERT_EQ(clazz.getInterfaceRealizations().size(), 1);
    InterfaceRealization& realization = clazz.getInterfaceRealizations().front();
    ASSERT_TRUE(realization.hasContract());
    ASSERT_EQ(realization.getContractRef(), interface);
}

TEST_F(InterfaceParserTest, emitInterfaceTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Interface& interface = m.create<Interface>();
    Property& prop = m.create<Property>();
    Operation& op = m.create<Operation>();
    DataType& nest = m.create<DataType>();
    Interface& general = m.create<Interface>();
    Generalization& generalization = m.create<Generalization>();
    Class& implementing = m.create<Class>();
    InterfaceRealization& realization = m.create<InterfaceRealization>();
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
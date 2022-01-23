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
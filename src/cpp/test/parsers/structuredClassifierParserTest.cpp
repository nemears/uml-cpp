#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

using namespace std;
using namespace UML;

class StructuredClassifierParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(StructuredClassifierParserTest, parseOwnedAttributeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/ownedAttributeTest.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRoles().size() == 1);
    ASSERT_TRUE(&c.getRoles().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getName().compare("test") == 0);
}

TEST_F(StructuredClassifierParserTest, partTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/partTest.yml").ptr());
    Class& c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getParts().size() == 1);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(&c.getParts().front() == p);
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRoles().size() == 1);
    ASSERT_TRUE(&c.getRoles().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getAggregation() == AggregationKind::COMPOSITE);
}

TEST_F(StructuredClassifierParserTest, parseConnectorTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/connector.yml").ptr());

    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& root = el->as<Package>();
    OpaqueBehavior& contract = root.getPackagedElements().get("contract").as<OpaqueBehavior>();
    Association& association = root.getPackagedElements().get("association").as<Association>();
    Property& assocEnd1 = association.getOwnedEnds().get("end1");
    Property& assocEnd2 = association.getOwnedEnds().get("end2");
    DataType& type1 = root.getPackagedElements().get("type1").as<DataType>();
    DataType& type2 = root.getPackagedElements().get("type2").as<DataType>();
    Class& clazz = root.getPackagedElements().get("clazz").as<Class>();
    ASSERT_EQ(association.getMemberEnds().size(), 2);
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 2);
    Property& prop1 = clazz.getOwnedAttributes().get("prop1");
    Property& prop2 = clazz.getOwnedAttributes().get("prop2");
    Connector& connector = clazz.getOwnedConnectors().front();
    ASSERT_TRUE(connector.getType());
    ASSERT_EQ(*connector.getType(), association);
    ASSERT_EQ(connector.getContracts().size(), 1);
    ASSERT_TRUE(connector.getContracts().contains(contract));
    ASSERT_EQ(connector.getEnds().size(), 2);
    ConnectorEnd& end1 = connector.getEnds().get(ID::fromString("iluwBJEOrucpPYWRjAf2Wl0Y7xJb"));
    ConnectorEnd& end2 = connector.getEnds().get(ID::fromString("Xa_ufGeUWxU5cUPY3f8VfRdmsH1V"));
    ASSERT_TRUE(end1.getRole());
    ASSERT_EQ(*end1.getRole(), prop1);
    ASSERT_TRUE(end1.getDefiningEnd());
    ASSERT_EQ(*end1.getDefiningEnd(), assocEnd1);
    ASSERT_TRUE(end2.getRole());
    ASSERT_EQ(*end2.getRole(), prop2);
    ASSERT_TRUE(end2.getDefiningEnd());
    ASSERT_EQ(*end2.getDefiningEnd(), assocEnd2);
}
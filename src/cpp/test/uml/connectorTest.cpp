#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ConnectorTest : public ::testing::Test {};

TEST_F(ConnectorTest, basicPropertyConnectorTest) {
    UmlManager m;
    Class& structuredClassifier = m.create<Class>();
    Connector& connector = m.create<Connector>();
    Property& prop1 = m.create<Property>();
    Property& prop2 = m.create<Property>();
    ConnectorEnd& end1 = m.create<ConnectorEnd>();
    ConnectorEnd& end2 = m.create<ConnectorEnd>();

    structuredClassifier.getOwnedAttributes().add(prop1, prop2);
    end1.setRole(prop1);
    end2.setRole(prop2);
    connector.getEnds().add(end1, end2);
    structuredClassifier.getOwnedConnectors().add(connector);

    ASSERT_EQ(structuredClassifier.getOwnedAttributes().size(), 2);
    ASSERT_EQ(structuredClassifier.getAttributes().size(), 2);
    ASSERT_EQ(structuredClassifier.getOwnedConnectors().size(), 1);
    ASSERT_EQ(structuredClassifier.getFeatures().size(), 3);
    ASSERT_EQ(structuredClassifier.getOwnedMembers().size(), 3);
    ASSERT_EQ(structuredClassifier.getMembers().size(), 3);
    ASSERT_EQ(structuredClassifier.getOwnedElements().size(), 3);

    ASSERT_TRUE(structuredClassifier.getOwnedAttributes().contains(prop1));
    ASSERT_TRUE(structuredClassifier.getAttributes().contains(prop1));
    ASSERT_FALSE(structuredClassifier.getOwnedConnectors().contains(prop1.getID()));
    ASSERT_TRUE(structuredClassifier.getFeatures().contains(prop1));
    ASSERT_TRUE(structuredClassifier.getOwnedMembers().contains(prop1));
    ASSERT_TRUE(structuredClassifier.getMembers().contains(prop1));
    ASSERT_TRUE(structuredClassifier.getOwnedElements().contains(prop1));

    ASSERT_TRUE(structuredClassifier.getOwnedAttributes().contains(prop2));
    ASSERT_TRUE(structuredClassifier.getAttributes().contains(prop2));
    ASSERT_FALSE(structuredClassifier.getOwnedConnectors().contains(prop2.getID()));
    ASSERT_TRUE(structuredClassifier.getFeatures().contains(prop2));
    ASSERT_TRUE(structuredClassifier.getOwnedMembers().contains(prop2));
    ASSERT_TRUE(structuredClassifier.getMembers().contains(prop2));
    ASSERT_TRUE(structuredClassifier.getOwnedElements().contains(prop2));

    ASSERT_FALSE(structuredClassifier.getOwnedAttributes().contains(connector.getID()));
    ASSERT_FALSE(structuredClassifier.getAttributes().contains(connector.getID()));
    ASSERT_TRUE(structuredClassifier.getOwnedConnectors().contains(connector));
    ASSERT_TRUE(structuredClassifier.getFeatures().contains(connector));
    ASSERT_TRUE(structuredClassifier.getOwnedMembers().contains(connector));
    ASSERT_TRUE(structuredClassifier.getMembers().contains(connector));
    ASSERT_TRUE(structuredClassifier.getOwnedElements().contains(connector));

    ASSERT_TRUE(prop1.hasFeaturingClassifier());
    ASSERT_TRUE(prop1.hasClass());
    ASSERT_TRUE(prop1.hasNamespace());
    ASSERT_TRUE(prop1.hasOwner());

    ASSERT_TRUE(prop2.hasFeaturingClassifier());
    ASSERT_TRUE(prop2.hasClass());
    ASSERT_TRUE(prop2.hasNamespace());
    ASSERT_TRUE(prop2.hasOwner());

    ASSERT_TRUE(connector.hasFeaturingClassifier());
    ASSERT_TRUE(connector.hasNamespace());
    ASSERT_TRUE(connector.hasOwner());

    ASSERT_EQ(prop1.getEnds().size(), 1);
    ASSERT_TRUE(prop1.getEnds().contains(end1));
    ASSERT_TRUE(end1.hasRole());
    ASSERT_EQ(end1.getRoleRef(), prop1);

    ASSERT_EQ(prop2.getEnds().size(), 1);
    ASSERT_TRUE(prop2.getEnds().contains(end2));
    ASSERT_TRUE(end2.hasRole());
    ASSERT_EQ(end2.getRoleRef(), prop2);

    ASSERT_EQ(connector.getEnds().size(), 2);
    ASSERT_TRUE(connector.getEnds().contains(end1));
    ASSERT_TRUE(connector.getEnds().contains(end2));
    ASSERT_TRUE(end1.hasOwner());
    ASSERT_EQ(end1.getOwnerRef(), connector);
    ASSERT_TRUE(end2.hasOwner());
    ASSERT_EQ(end2.getOwnerRef(), connector);
}
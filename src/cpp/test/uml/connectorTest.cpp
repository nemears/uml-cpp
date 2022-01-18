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

/**
 * This test is based on the uml2.5 spec figure 11.4 (page 185)
 **/
TEST_F(ConnectorTest, typedConnectorTest) {
    UmlManager m;
    Class& car = m.create<Class>();
    Class& wheel = m.create<Class>();
    Class& engine = m.create<Class>();
    Property& rear = m.create<Property>();
    Property& e = m.create<Property>();
    Association& wheelAssociation = m.create<Association>();
    Property& wheelAssociationEnd = m.create<Property>();
    Association& engineAssociation = m.create<Association>();
    Property& engineCarProp = m.create<Property>();
    Association& axle = m.create<Association>(); // connector type
    Property& axleWheelEnd = m.create<Property>();
    Property& axleEngineEnd = m.create<Property>();
    Connector& a = m.create<Connector>();
    ConnectorEnd& wheelEnd = m.create<ConnectorEnd>();
    ConnectorEnd& engineEnd = m.create<ConnectorEnd>();

    car.setName("Car");
    wheel.setName("Wheel");
    engine.setName("Engine");
    rear.setName("rear");
    rear.setType(wheel);
    rear.setLower(0);
    rear.setUpper(2);
    rear.setAggregation(AggregationKind::COMPOSITE);
    e.setName("e");
    e.setType(engine);
    car.getOwnedAttributes().add(rear, e);
    wheelAssociationEnd.setType(car);
    wheelAssociation.getMemberEnds().add(rear);
    wheelAssociation.getOwnedEnds().add(wheelAssociationEnd);
    engineCarProp.setType(car);
    engine.getOwnedAttributes().add(engineCarProp);
    engineAssociation.getMemberEnds().add(e, engineCarProp);
    axleWheelEnd.setType(wheel);
    axleEngineEnd.setType(engine);
    axle.getOwnedEnds().add(axleWheelEnd, axleEngineEnd);
    a.setName("a");
    a.setType(axle);
    wheelEnd.setRole(rear);
    engineEnd.setRole(e);
    a.getEnds().add(wheelEnd, engineEnd);
    car.getOwnedConnectors().add(a);

    // Car
    ASSERT_EQ(car.getParts().size(), 1);
    ASSERT_EQ(car.getRoles().size(), 2);
    ASSERT_EQ(car.getOwnedAttributes().size(), 2);
    ASSERT_EQ(car.getOwnedConnectors().size(), 1);
    ASSERT_EQ(car.getFeatures().size(), 3);
    ASSERT_EQ(car.getOwnedMembers().size(), 3);
    ASSERT_EQ(car.getMembers().size(), 3);
    ASSERT_EQ(car.getOwnedElements().size(), 3);

    ASSERT_TRUE(car.getOwnedAttributes().contains(rear));
    ASSERT_TRUE(car.getAttributes().contains(rear));
    ASSERT_FALSE(car.getOwnedConnectors().contains(rear.getID()));
    ASSERT_TRUE(car.getFeatures().contains(rear));
    ASSERT_TRUE(car.getOwnedMembers().contains(rear));
    ASSERT_TRUE(car.getMembers().contains(rear));
    ASSERT_TRUE(car.getOwnedElements().contains(rear));

    ASSERT_TRUE(car.getOwnedAttributes().contains(e));
    ASSERT_TRUE(car.getAttributes().contains(e));
    ASSERT_FALSE(car.getOwnedConnectors().contains(e.getID()));
    ASSERT_TRUE(car.getFeatures().contains(e));
    ASSERT_TRUE(car.getOwnedMembers().contains(e));
    ASSERT_TRUE(car.getMembers().contains(e));
    ASSERT_TRUE(car.getOwnedElements().contains(e));

    ASSERT_FALSE(car.getOwnedAttributes().contains(a.getID()));
    ASSERT_FALSE(car.getAttributes().contains(a.getID()));
    ASSERT_TRUE(car.getOwnedConnectors().contains(a));
    ASSERT_TRUE(car.getFeatures().contains(a));
    ASSERT_TRUE(car.getOwnedMembers().contains(a));
    ASSERT_TRUE(car.getMembers().contains(a));
    ASSERT_TRUE(car.getOwnedElements().contains(a));

    // rear association
    ASSERT_EQ(wheelAssociation.getNavigableOwnedEnds().size(), 0);
    ASSERT_EQ(wheelAssociation.getOwnedEnds().size(), 1);
    ASSERT_EQ(wheelAssociation.getMemberEnds().size(), 2);
    ASSERT_EQ(wheelAssociation.getMembers().size(), 2);
    ASSERT_EQ(wheelAssociation.getEndType().size(), 2);
    ASSERT_EQ(wheelAssociation.getOwnedMembers().size(), 1);
    ASSERT_EQ(wheelAssociation.getOwnedElements().size(), 1);

    ASSERT_TRUE(wheelAssociation.getOwnedEnds().contains(wheelAssociationEnd));
    ASSERT_TRUE(wheelAssociation.getMemberEnds().contains(wheelAssociationEnd));
    ASSERT_TRUE(wheelAssociation.getMembers().contains(wheelAssociationEnd));
    ASSERT_TRUE(wheelAssociation.getOwnedMembers().contains(wheelAssociationEnd));
    ASSERT_TRUE(wheelAssociation.getOwnedElements().contains(wheelAssociationEnd));
    ASSERT_TRUE(wheelAssociation.getEndType().contains(car));

    ASSERT_FALSE(wheelAssociation.getOwnedEnds().contains(rear));
    ASSERT_TRUE(wheelAssociation.getMemberEnds().contains(rear));
    ASSERT_TRUE(wheelAssociation.getMembers().contains(rear));
    ASSERT_FALSE(wheelAssociation.getOwnedMembers().contains(rear));
    ASSERT_FALSE(wheelAssociation.getOwnedElements().contains(rear));
    ASSERT_TRUE(wheelAssociation.getEndType().contains(wheel));

    // e association
    ASSERT_EQ(engineAssociation.getNavigableOwnedEnds().size(), 0);
    ASSERT_EQ(engineAssociation.getOwnedEnds().size(), 0);
    ASSERT_EQ(engineAssociation.getMemberEnds().size(), 2);
    ASSERT_EQ(engineAssociation.getMembers().size(), 2);
    ASSERT_EQ(engineAssociation.getEndType().size(), 2);
    ASSERT_EQ(engineAssociation.getOwnedMembers().size(), 0);
    ASSERT_EQ(engineAssociation.getOwnedElements().size(), 0);

    ASSERT_FALSE(engineAssociation.getOwnedEnds().contains(e));
    ASSERT_TRUE(engineAssociation.getMemberEnds().contains(e));
    ASSERT_TRUE(engineAssociation.getMembers().contains(e));
    ASSERT_FALSE(engineAssociation.getOwnedMembers().contains(e));
    ASSERT_FALSE(engineAssociation.getOwnedElements().contains(e));
    ASSERT_TRUE(engineAssociation.getEndType().contains(engine));

    ASSERT_FALSE(engineAssociation.getOwnedEnds().contains(engineCarProp));
    ASSERT_TRUE(engineAssociation.getMemberEnds().contains(engineCarProp));
    ASSERT_TRUE(engineAssociation.getMembers().contains(engineCarProp));
    ASSERT_FALSE(engineAssociation.getOwnedMembers().contains(engineCarProp));
    ASSERT_FALSE(engineAssociation.getOwnedElements().contains(engineCarProp));
    ASSERT_TRUE(engineAssociation.getEndType().contains(car));
}
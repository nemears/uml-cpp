#include "gtest/gtest.h"
#include "uml/deployment.h"
#include "uml/deploymentTarget.h"
#include "uml/deployedArtifact.h"
#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/package.h"
#include "uml/uml-stable.h"

using namespace UML;

class DeploymentTest : public ::testing::Test {};

TEST_F(DeploymentTest, basicDeploymentTest) {
    UmlManager m;
    Deployment& deployment = *m.create<Deployment>();
    DeploymentTarget& location = *m.create<DeploymentTarget>();
    DeployedArtifact& artifact = *m.create<DeployedArtifact>();
    deployment.setLocation(&location);
    deployment.getDeployedArtifacts().add(artifact);
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), location.getID());
    ASSERT_EQ(deployment.getClients().size(), 1);
    ASSERT_EQ(deployment.getClients().front().getID(), location.getID());
    ASSERT_TRUE(deployment.getOwner());
    ASSERT_EQ(deployment.getOwner()->getID(), location.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front().getID(), artifact.getID());
    ASSERT_EQ(deployment.getSuppliers().size(), 1);
    ASSERT_EQ(deployment.getSuppliers().front().getID(), artifact.getID());
    deployment.setLocation(0);
    deployment.getDeployedArtifacts().remove(artifact);
    ASSERT_FALSE(deployment.getLocation());
    ASSERT_EQ(deployment.getClients().size(), 0);
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 0);
    ASSERT_EQ(deployment.getSuppliers().size(), 0);
    ASSERT_FALSE(deployment.getOwner());
}

TEST_F(DeploymentTest, artifactOperationAndAttributeTest) {
    UmlManager m;
    Artifact& artifact = *m.create<Artifact>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    artifact.getOwnedAttributes().add(prop);
    artifact.getOwnedOperations().add(op);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_EQ(prop.getFeaturingClassifier()->getID(), artifact.getID());
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    artifact.getOwnedAttributes().remove(prop);
    artifact.getOwnedOperations().remove(op);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 0);
    ASSERT_EQ(artifact.getAttributes().size(), 0);
    ASSERT_EQ(artifact.getOwnedOperations().size(), 0);
}

TEST_F(DeploymentTest, artifactWithAttributeOperationAndNestedArtifact) {
    UmlManager m;
    Artifact& artifact = *m.create<Artifact>();
    Property& property = *m.create<Property>();
    Operation& operation = *m.create<Operation>();
    Artifact& nestedArtifact = *m.create<Artifact>();

    operation.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    property.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");

    artifact.getOwnedAttributes().add(property);
    artifact.getOwnedOperations().add(operation);
    artifact.getNestedArtifacts().add(nestedArtifact);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(artifact.getOwnedAttributes().contains(property));
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_TRUE(artifact.getOwnedOperations().contains(operation));
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    ASSERT_TRUE(artifact.getNestedArtifacts().contains(nestedArtifact));
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_TRUE(artifact.getAttributes().contains(property));
    ASSERT_EQ(artifact.getFeatures().size(), 2);
    ASSERT_TRUE(artifact.getFeatures().contains(property));
    ASSERT_TRUE(artifact.getFeatures().contains(operation));
    ASSERT_EQ(artifact.getOwnedMembers().size(), 3);
    ASSERT_TRUE(artifact.getOwnedMembers().contains(property));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(operation));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(nestedArtifact));
    ASSERT_EQ(artifact.getMembers().size(), 3);
    ASSERT_TRUE(artifact.getMembers().contains(property));
    ASSERT_TRUE(artifact.getMembers().contains(operation));
    ASSERT_TRUE(artifact.getMembers().contains(nestedArtifact));
    ASSERT_EQ(artifact.getOwnedElements().size(), 3);
    ASSERT_TRUE(artifact.getOwnedElements().contains(property));
    ASSERT_TRUE(artifact.getOwnedElements().contains(operation));
    ASSERT_TRUE(artifact.getOwnedElements().contains(nestedArtifact));
}
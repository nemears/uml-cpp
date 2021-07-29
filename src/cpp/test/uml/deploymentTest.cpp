#include "gtest/gtest.h"
#include "uml/deployment.h"
#include "uml/deploymentTarget.h"
#include "uml/deployedArtifact.h"

using namespace UML;

class DeploymentTest : public ::testing::Test {};

TEST_F(DeploymentTest, basicDeploymentTest) {
    UmlManager m;
    Deployment& deployment = m.create<Deployment>();
    DeploymentTarget& location = m.create<DeploymentTarget>();
    DeployedArtifact& artifact = m.create<DeployedArtifact>();
    deployment.setLocation(&location);
    deployment.getDeployedArtifact().add(artifact);
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), location.getID());
    ASSERT_EQ(deployment.getClient().size(), 1);
    ASSERT_EQ(deployment.getClient().front()->getID(), location.getID());
    ASSERT_TRUE(deployment.getOwner() != 0);
    ASSERT_EQ(deployment.getOwner()->getID(), location.getID());
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifact().front()->getID(), artifact.getID());
    ASSERT_EQ(deployment.getSupplier().size(), 1);
    ASSERT_EQ(deployment.getSupplier().front()->getID(), artifact.getID());
    deployment.setLocation(0);
    deployment.getDeployedArtifact().remove(artifact);
    ASSERT_TRUE(deployment.getLocation() == 0);
    ASSERT_EQ(deployment.getClient().size(), 0);
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 0);
    ASSERT_EQ(deployment.getSupplier().size(), 0);
    ASSERT_TRUE(deployment.getOwner() == 0);
}
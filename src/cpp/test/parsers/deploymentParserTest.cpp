#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/deployment.h"
#include "uml/package.h"
#include "uml/instanceSpecification.h"
#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"

using namespace std;
using namespace UML;

class DeploymentParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DeploymentParserTest, basicDeploymentTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/deployment.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = *inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1)->getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1)->as<Artifact>();
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifact().front()->getID(), artifact.getID());
}

TEST_F(DeploymentParserTest, basicArtifactTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/artifact.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = *inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1)->getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1)->as<Artifact>();
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifact().front()->getID(), artifact.getID());
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    Property& prop = *artifact.getOwnedAttributes().front();
    ASSERT_EQ(prop.getName(), "prop");
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    Operation& op = *artifact.getOwnedOperations().front();
    ASSERT_EQ(op.getName(), "op");
}

TEST_F(DeploymentParserTest, nestedArtifactTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/nestedArtifact.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::ARTIFACT);
    Artifact& artifact = el->as<Artifact>();
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    Artifact& nest = *artifact.getNestedArtifacts().front();
    ASSERT_EQ(nest.getName(), "nest");
}
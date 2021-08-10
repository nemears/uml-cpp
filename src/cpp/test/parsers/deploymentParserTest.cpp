#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/deployment.h"
#include "uml/package.h"
#include "uml/instanceSpecification.h"
#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/class.h"

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
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front().as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1).as<Artifact>();
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifact().front().getID(), artifact.getID());
}

TEST_F(DeploymentParserTest, basicArtifactTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/artifact.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front().as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1).as<Artifact>();
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifact().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifact().front().getID(), artifact.getID());
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    Property& prop = artifact.getOwnedAttributes().front();
    ASSERT_EQ(prop.getName(), "prop");
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    Operation& op = artifact.getOwnedOperations().front();
    ASSERT_EQ(op.getName(), "op");
}

TEST_F(DeploymentParserTest, nestedArtifactTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/nestedArtifact.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::ARTIFACT);
    Artifact& artifact = el->as<Artifact>();
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    Artifact& nest = artifact.getNestedArtifacts().front();
    ASSERT_EQ(nest.getName(), "nest");
}

TEST_F(DeploymentParserTest, emitDeploymentTest) {
    UmlManager m;
    Deployment& d = m.create<Deployment>();
    Artifact& a = m.create<Artifact>();
    d.setID("RP9VhYnGYcgWOqXxLt4_Xb3RAAM8");
    a.setID("bkwzmF3K0ddPG7CPwXVBZyyp8glc");
    d.getDeployedArtifact().add(a);
    string expectedEmit = R""""(deployment:
  id: RP9VhYnGYcgWOqXxLt4_Xb3RAAM8
  deployedArtifacts:
    - bkwzmF3K0ddPG7CPwXVBZyyp8glc)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(d));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentParserTest, emitDeploymentTargetTest) {
    UmlManager m;
    Deployment& d = m.create<Deployment>();
    Property& prop = m.create<Property>();
    d.setID("hZ6hYVt147nLvdm70bATtgmwlQqN");
    prop.setID("0gLOuagM6UjFYi2401zvnoCpMn0M");
    prop.getDeployments().add(d);
    string expectedEmit = R""""(property:
  id: 0gLOuagM6UjFYi2401zvnoCpMn0M
  deployments:
    - deployment:
        id: hZ6hYVt147nLvdm70bATtgmwlQqN)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(prop));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentParserTest, emitArtifactTest) {
    UmlManager m;
    Artifact& a = m.create<Artifact>();
    Property& p = m.create<Property>();
    Operation& o = m.create<Operation>();
    Artifact& n = m.create<Artifact>();
    a.setID("dzpr85AOkv_Z2mLs8cKLbHnR5DBq");
    p.setID("xr6rIEO8UBfSS2vlFWWNGPcNjVJv");
    o.setID("VdLJMfQodStjxL1RCzvyR6RLzCe5");
    n.setID("KWkfV0HFADssmGEBNUj1AwPB4SeC");
    a.getOwnedAttributes().add(p);
    a.getOwnedOperations().add(o);
    a.getNestedArtifacts().add(n);
    string expectedEmit = R""""(artifact:
  id: dzpr85AOkv_Z2mLs8cKLbHnR5DBq
  ownedAttributes:
    - property:
        id: xr6rIEO8UBfSS2vlFWWNGPcNjVJv
  ownedOperations:
    - operation:
        id: VdLJMfQodStjxL1RCzvyR6RLzCe5
  nestedArtifacts:
    - artifact:
        id: KWkfV0HFADssmGEBNUj1AwPB4SeC)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(a));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentParserTest, parseManifestationsTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/manifestations.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& c1 = pckg.getPackagedElements().front().as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1).as<Artifact>();
    ASSERT_EQ(artifact.getManifestations().size(), 2);
    Manifestation& m1 = artifact.getManifestations().front();
    Manifestation& m2 = artifact.getManifestations().back();
    ASSERT_TRUE(m2.getUtilizedElement() != 0);
    ASSERT_EQ(m2.getUtilizedElement()->getID(), c1.getID());
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    ASSERT_TRUE(m1.getUtilizedElement() != 0);
    ASSERT_EQ(m1.getUtilizedElement()->getID(), pckg.getPackagedElements().back().getID());
}

TEST_F(DeploymentParserTest, emitManifestationTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Manifestation& man = m.create<Manifestation>();
    Class& c = m.create<Class>();
    Artifact& a = m.create<Artifact>();
    pckg.setID("O4FknRxSbpxEJlw6HhHP&Wpq0AjD");
    man.setID("UfyRMRUyPnad&lJcpSBOD17VSHtn");
    c.setID("9mp2RmgjnYQrPtXIoOw9is1UUEyu");
    a.setID("Ihue7RPPRluLEpIUbTV8Xqb68ofQ");
    pckg.getPackagedElements().add(c);
    pckg.getPackagedElements().add(a);
    a.getManifestations().add(man);
    man.setUtilizedElement(&c);
    string expectedEmit = R""""(package:
  id: O4FknRxSbpxEJlw6HhHP&Wpq0AjD
  packagedElements:
    - class:
        id: 9mp2RmgjnYQrPtXIoOw9is1UUEyu
    - artifact:
        id: Ihue7RPPRluLEpIUbTV8Xqb68ofQ
        manifestations:
          - manifestation:
              id: UfyRMRUyPnad&lJcpSBOD17VSHtn
              utilizedElement: 9mp2RmgjnYQrPtXIoOw9is1UUEyu)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
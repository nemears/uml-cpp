#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(DeploymentTargetDeployment, Deployment, InstanceSpecification, &DeploymentTarget::getDeployments)
UML_SINGLETON_INTEGRATION_TEST(DeploymentLocation, Property, Deployment, &Deployment::getLocation, &Deployment::setLocation)
UML_SET_INTEGRATION_TEST(DeploymentDeployedArtifacts, Artifact, Deployment, &Deployment::getDeployedArtifacts)

class DeploymentTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DeploymentTest, basicDeploymentTest) {
    Manager<> m;
    Deployment& deployment = *m.create<Deployment>();
    DeploymentTarget& location = *m.create<DeploymentTarget>();
    DeployedArtifact& artifact = *m.create<DeployedArtifact>();
    deployment.setLocation(&location);
    deployment.getDeployedArtifacts().add(artifact);
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), location.getID());
    ASSERT_EQ(deployment.getClients().size(), 1);
    ASSERT_EQ(deployment.getClients().front()->getID(), location.getID());
    ASSERT_TRUE(deployment.getOwner());
    ASSERT_EQ(deployment.getOwner()->getID(), location.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front()->getID(), artifact.getID());
    ASSERT_EQ(deployment.getSuppliers().size(), 1);
    ASSERT_EQ(deployment.getSuppliers().front()->getID(), artifact.getID());
    deployment.setLocation(0);
    deployment.getDeployedArtifacts().remove(artifact);
    ASSERT_FALSE(deployment.getLocation());
    ASSERT_EQ(deployment.getClients().size(), 0);
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 0);
    ASSERT_EQ(deployment.getSuppliers().size(), 0);
    ASSERT_FALSE(deployment.getOwner());
}

TEST_F(DeploymentTest, artifactOperationAndAttributeTest) {
    Manager<> m;
    Artifact& artifact = *m.create<Artifact>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    artifact.getOwnedAttributes().add(prop);
    artifact.getOwnedOperations().add(op);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_EQ(artifact.getFeatures().size(), 2);
    ASSERT_EQ(artifact.getOwnedMembers().size(), 2);
    ASSERT_EQ(artifact.getMembers().size(), 2);
    ASSERT_EQ(artifact.getOwnedElements().size(), 2);
    ASSERT_TRUE(artifact.getOwnedAttributes().contains(prop));
    ASSERT_TRUE(artifact.getAttributes().contains(prop));
    ASSERT_TRUE(artifact.getOwnedOperations().contains(op));
    ASSERT_TRUE(artifact.getFeatures().contains(prop));
    ASSERT_TRUE(artifact.getFeatures().contains(op));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(prop));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(op));
    ASSERT_TRUE(artifact.getMembers().contains(prop));
    ASSERT_TRUE(artifact.getMembers().contains(op));
    ASSERT_TRUE(artifact.getOwnedElements().contains(prop));
    ASSERT_TRUE(artifact.getOwnedElements().contains(op));
    ASSERT_EQ(prop.getFeaturingClassifier().id(), artifact.getID());
    ASSERT_EQ(prop.getNamespace().id(), artifact.getID());
    ASSERT_EQ(prop.getOwner().id(), artifact.getID());
    ASSERT_EQ(op.getFeaturingClassifier().id(), artifact.getID());
    ASSERT_EQ(op.getNamespace().id(), artifact.getID());
    ASSERT_EQ(op.getOwner().id(), artifact.getID());

    artifact.getOwnedAttributes().remove(prop);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 0);
    ASSERT_EQ(artifact.getAttributes().size(), 0);
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_EQ(artifact.getFeatures().size(), 1);
    ASSERT_EQ(artifact.getOwnedMembers().size(), 1);
    ASSERT_EQ(artifact.getMembers().size(), 1);
    ASSERT_EQ(artifact.getOwnedElements().size(), 1);
    ASSERT_FALSE(artifact.getOwnedAttributes().contains(prop));
    ASSERT_FALSE(artifact.getAttributes().contains(prop));
    ASSERT_TRUE(artifact.getOwnedOperations().contains(op));
    ASSERT_FALSE(artifact.getFeatures().contains(prop));
    ASSERT_TRUE(artifact.getFeatures().contains(op));
    ASSERT_FALSE(artifact.getOwnedMembers().contains(prop));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(op));
    ASSERT_FALSE(artifact.getMembers().contains(prop));
    ASSERT_TRUE(artifact.getMembers().contains(op));
    ASSERT_FALSE(artifact.getOwnedElements().contains(prop));
    ASSERT_TRUE(artifact.getOwnedElements().contains(op));
    ASSERT_EQ(prop.getFeaturingClassifier().id(), ID::nullID());
    ASSERT_EQ(prop.getNamespace().id(), ID::nullID());
    ASSERT_EQ(prop.getOwner().id(), ID::nullID());
    ASSERT_EQ(op.getFeaturingClassifier().id(), artifact.getID());
    ASSERT_EQ(op.getNamespace().id(), artifact.getID());
    ASSERT_EQ(op.getOwner().id(), artifact.getID());

    artifact.getOwnedOperations().remove(op);
   ASSERT_EQ(artifact.getOwnedAttributes().size(), 0);
    ASSERT_EQ(artifact.getAttributes().size(), 0);
    ASSERT_EQ(artifact.getOwnedOperations().size(), 0);
    ASSERT_EQ(artifact.getFeatures().size(), 0);
    ASSERT_EQ(artifact.getOwnedMembers().size(), 0);
    ASSERT_EQ(artifact.getMembers().size(), 0);
    ASSERT_EQ(artifact.getOwnedElements().size(), 0);
    ASSERT_FALSE(artifact.getOwnedAttributes().contains(prop));
    ASSERT_FALSE(artifact.getAttributes().contains(prop));
    ASSERT_FALSE(artifact.getOwnedOperations().contains(op));
    ASSERT_FALSE(artifact.getFeatures().contains(prop));
    ASSERT_FALSE(artifact.getFeatures().contains(op));
    ASSERT_FALSE(artifact.getOwnedMembers().contains(prop));
    ASSERT_FALSE(artifact.getOwnedMembers().contains(op));
    ASSERT_FALSE(artifact.getMembers().contains(prop));
    ASSERT_FALSE(artifact.getMembers().contains(op));
    ASSERT_FALSE(artifact.getOwnedElements().contains(prop));
    ASSERT_FALSE(artifact.getOwnedElements().contains(op));
    ASSERT_EQ(prop.getFeaturingClassifier().id(), ID::nullID());
    ASSERT_EQ(prop.getNamespace().id(), ID::nullID());
    ASSERT_EQ(prop.getOwner().id(), ID::nullID());
    ASSERT_EQ(op.getFeaturingClassifier().id(), ID::nullID());
    ASSERT_EQ(op.getNamespace().id(), ID::nullID());
    ASSERT_EQ(op.getOwner().id(), ID::nullID());
}

TEST_F(DeploymentTest, artifactWithAttributeOperationAndNestedArtifact) {
    Manager<> m;
    Artifact& artifact = *m.create<Artifact>();
    Property& property = *m.create<Property>();
    Operation& operation = *m.create<Operation>();
    Artifact& nestedArtifact = *m.create<Artifact>();

    operation.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    property.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");

    artifact.getOwnedAttributes().add(property);
    artifact.getOwnedOperations().add(operation);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(artifact.getOwnedAttributes().contains(property));
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_TRUE(artifact.getOwnedOperations().contains(operation));
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_TRUE(artifact.getAttributes().contains(property));
    ASSERT_EQ(artifact.getFeatures().size(), 2);
    ASSERT_TRUE(artifact.getFeatures().contains(property));
    ASSERT_TRUE(artifact.getFeatures().contains(operation));
    ASSERT_EQ(artifact.getOwnedMembers().size(), 2);
    ASSERT_TRUE(artifact.getOwnedMembers().contains(property));
    ASSERT_TRUE(artifact.getOwnedMembers().contains(operation));
    ASSERT_EQ(artifact.getMembers().size(), 2);
    ASSERT_TRUE(artifact.getMembers().contains(property));
    ASSERT_TRUE(artifact.getMembers().contains(operation));
    ASSERT_EQ(artifact.getOwnedElements().size(), 2);
    ASSERT_TRUE(artifact.getOwnedElements().contains(property));
    ASSERT_TRUE(artifact.getOwnedElements().contains(operation));

    artifact.getNestedArtifacts().add(nestedArtifact);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(artifact.getOwnedAttributes().contains(property));
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    ASSERT_TRUE(artifact.getOwnedOperations().contains(operation));
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    ASSERT_FALSE(artifact.getNestedArtifacts().contains(property.getID()));
    ASSERT_FALSE(artifact.getNestedArtifacts().contains(operation.getID()));
    ASSERT_TRUE(artifact.getNestedArtifacts().contains(nestedArtifact));
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_TRUE(artifact.getAttributes().contains(property));
    ASSERT_FALSE(artifact.getAttributes().contains(operation.getID()));
    ASSERT_FALSE(artifact.getAttributes().contains(nestedArtifact.getID()));
    ASSERT_EQ(artifact.getFeatures().size(), 2);
    ASSERT_TRUE(artifact.getFeatures().contains(property));
    ASSERT_TRUE(artifact.getFeatures().contains(operation));
    ASSERT_FALSE(artifact.getFeatures().contains(nestedArtifact.getID()));
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

TEST_F(DeploymentTest, parseBasicDeploymentTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "deploymentTests/deployment.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = *inst.getDeployments().front();
    ASSERT_EQ((pckg.getPackagedElements().begin()++)->getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = (pckg.getPackagedElements().begin()++)->as<Artifact>();
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front()->getID(), artifact.getID());
}

TEST_F(DeploymentTest, basicArtifactTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "deploymentTests/artifact.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = *inst.getDeployments().front();
    ASSERT_EQ((pckg.getPackagedElements().begin()++)->getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = (pckg.getPackagedElements().begin()++)->as<Artifact>();
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front()->getID(), artifact.getID());
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    Property& prop = *artifact.getOwnedAttributes().front();
    ASSERT_EQ(prop.getName(), "prop");
    ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
    Operation& op = *artifact.getOwnedOperations().front();
    ASSERT_EQ(op.getName(), "op");
}

TEST_F(DeploymentTest, nestedArtifactTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "deploymentTests/nestedArtifact.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == ElementType::ARTIFACT);
    Artifact& artifact = m.getRoot()->as<Artifact>();
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    Artifact& nest = *artifact.getNestedArtifacts().front();
    ASSERT_EQ(nest.getName(), "nest");
}

TEST_F(DeploymentTest, emitDeploymentTest) {
    Manager<> m;
    Deployment& d = *m.create<Deployment>();
    Artifact& a = *m.create<Artifact>();
    d.setID("RP9VhYnGYcgWOqXxLt4_Xb3RAAM8");
    a.setID("bkwzmF3K0ddPG7CPwXVBZyyp8glc");
    d.getDeployedArtifacts().add(a);
    std::string expectedEmit = R""""(Deployment:
  id: RP9VhYnGYcgWOqXxLt4_Xb3RAAM8
  deployedArtifacts:
    - bkwzmF3K0ddPG7CPwXVBZyyp8glc)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(d, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentTest, emitDeploymentTargetTest) {
    Manager<> m;
    Deployment& d = *m.create<Deployment>();
    Property& prop = *m.create<Property>();
    d.setID("hZ6hYVt147nLvdm70bATtgmwlQqN");
    prop.setID("0gLOuagM6UjFYi2401zvnoCpMn0M");
    prop.getDeployments().add(d);
    std::string expectedEmit = R""""(Property:
  id: 0gLOuagM6UjFYi2401zvnoCpMn0M
  deployments:
    - Deployment:
        id: hZ6hYVt147nLvdm70bATtgmwlQqN)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(prop, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentTest, emitArtifactTest) {
    Manager<> m;
    Artifact& a = *m.create<Artifact>();
    Property& p = *m.create<Property>();
    Operation& o = *m.create<Operation>();
    Artifact& n = *m.create<Artifact>();
    a.setID("dzpr85AOkv_Z2mLs8cKLbHnR5DBq");
    p.setID("xr6rIEO8UBfSS2vlFWWNGPcNjVJv");
    o.setID("VdLJMfQodStjxL1RCzvyR6RLzCe5");
    n.setID("KWkfV0HFADssmGEBNUj1AwPB4SeC");
    a.getOwnedAttributes().add(p);
    a.getOwnedOperations().add(o);
    a.getNestedArtifacts().add(n);
    std::string expectedEmit = R""""(Artifact:
  id: dzpr85AOkv_Z2mLs8cKLbHnR5DBq
  ownedAttributes:
    - Property:
        id: xr6rIEO8UBfSS2vlFWWNGPcNjVJv
  ownedOperations:
    - Operation:
        id: VdLJMfQodStjxL1RCzvyR6RLzCe5
  nestedArtifacts:
    - Artifact:
        id: KWkfV0HFADssmGEBNUj1AwPB4SeC)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(a, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentTest, parseManifestationsTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "deploymentTests/manifestations.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("utilizedEl1")->getElementType(), ElementType::CLASS);
    Class& c1 = pckg.getPackagedElements().get("utilizedEl1")->as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("artifact")->getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get("artifact")->as<Artifact>();
    ASSERT_EQ(artifact.getManifestations().size(), 2);
    Manifestation& m2 = *artifact.getManifestations().get(ID::fromString("wO29nIB3PeOeFX0TaaHppiyUn83B"));
    Manifestation& m1 = *artifact.getManifestations().get(ID::fromString("elQJOxECf35h96KkZu2YEsie9kHJ"));
    ASSERT_TRUE(m2.getUtilizedElement());
    ASSERT_EQ(m2.getUtilizedElement()->getID(), c1.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("utilizedEl2")->getElementType(), ElementType::CLASS);
    ASSERT_TRUE(m1.getUtilizedElement());
    ASSERT_EQ(m1.getUtilizedElement()->getID(), pckg.getPackagedElements().get("utilizedEl2")->getID());
}

TEST_F(DeploymentTest, emitManifestationTest) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    Manifestation& man = *m.create<Manifestation>();
    Class& c = *m.create<Class>();
    Artifact& a = *m.create<Artifact>();
    pckg.setID("O4FknRxSbpxEJlw6HhHP&Wpq0AjD");
    man.setID("UfyRMRUyPnad&lJcpSBOD17VSHtn");
    c.setID("9mp2RmgjnYQrPtXIoOw9is1UUEyu");
    a.setID("Ihue7RPPRluLEpIUbTV8Xqb68ofQ");
    pckg.getPackagedElements().add(c);
    pckg.getPackagedElements().add(a);
    a.getManifestations().add(man);
    man.setUtilizedElement(&c);
    std::string expectedEmit = R""""(Package:
  id: "O4FknRxSbpxEJlw6HhHP&Wpq0AjD"
  packagedElements:
    - Artifact:
        id: Ihue7RPPRluLEpIUbTV8Xqb68ofQ
        clientDependencies:
          - "UfyRMRUyPnad&lJcpSBOD17VSHtn"
        manifestations:
          - Manifestation:
              id: "UfyRMRUyPnad&lJcpSBOD17VSHtn"
              utilizedElement: 9mp2RmgjnYQrPtXIoOw9is1UUEyu
    - Class:
        id: 9mp2RmgjnYQrPtXIoOw9is1UUEyu)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(DeploymentTest, mountAndEditArtifactTest) {
    Manager<> m;
    Package& root = *m.create<Package>();
    Artifact& artifact = *m.create<Artifact>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    Artifact& nest = *m.create<Artifact>();
    Manifestation& manifestation = *m.create<Manifestation>();
    Package& utilizedElement = *m.create<Package>();
    artifact.getOwnedAttributes().add(prop);
    artifact.getOwnedOperations().add(op);
    artifact.getNestedArtifacts().add(nest);
    manifestation.setUtilizedElement(utilizedElement);
    artifact.getManifestations().add(manifestation);
    root.getPackagedElements().add(artifact);
    m.setRoot(&root);
    m.mount(ymlPath + "deploymentTests");

    ID aID = artifact.getID();
    m.release(artifact);
    Artifact& artifact2 = root.getPackagedElements().front()->as<Artifact>();
    ASSERT_TRUE(artifact2.getOwningPackage());
    ASSERT_EQ(*artifact2.getOwningPackage(), root);
    ASSERT_TRUE(artifact2.getNamespace());
    ASSERT_EQ(*artifact2.getNamespace(), root);
    ASSERT_TRUE(artifact2.getOwner());
    ASSERT_EQ(*artifact2.getOwner(), root);
    ASSERT_TRUE(root.getPackagedElements().contains(artifact2.getID()));
    ASSERT_TRUE(root.getOwnedMembers().contains(artifact2.getID()));
    ASSERT_TRUE(root.getMembers().contains(aID));
    ASSERT_TRUE(root.getOwnedElements().contains(aID));

    m.release(artifact2, root);
    Artifact& artifact3 = m.get(aID)->as<Artifact>();
    ASSERT_TRUE(artifact3.getOwningPackage());
    Package& root2 = *artifact3.getOwningPackage();
    ASSERT_TRUE(artifact3.getNamespace());
    ASSERT_EQ(*artifact3.getNamespace(), root2);
    ASSERT_TRUE(artifact3.getOwner());
    ASSERT_EQ(*artifact3.getOwner(), root2);
    ASSERT_TRUE(root2.getPackagedElements().contains(artifact3.getID()));
    ASSERT_TRUE(root2.getOwnedMembers().contains(artifact3.getID()));
    ASSERT_TRUE(root2.getMembers().contains(aID));
    ASSERT_TRUE(root2.getOwnedElements().contains(aID));
    
    ID pid = prop.getID();
    m.release(prop);
    Property& prop2 = *artifact3.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.getFeaturingClassifier());
    ASSERT_EQ(*prop2.getFeaturingClassifier(), artifact3);
    ASSERT_TRUE(prop2.getNamespace());
    ASSERT_EQ(*prop2.getNamespace(), artifact3);
    ASSERT_TRUE(prop2.getOwner());
    ASSERT_EQ(*prop2.getOwner(), artifact3);
    ASSERT_TRUE(artifact3.getOwnedAttributes().contains(pid));
    ASSERT_TRUE(artifact3.getAttributes().contains(pid));
    ASSERT_TRUE(artifact3.getFeatures().contains(pid));
    ASSERT_TRUE(artifact3.getOwnedMembers().contains(pid));
    ASSERT_TRUE(artifact3.getMembers().contains(pid));
    ASSERT_TRUE(artifact3.getOwnedElements().contains(pid));

    m.release(artifact3, prop2);
    Property& prop3 = m.get(pid)->as<Property>();
    ASSERT_TRUE(prop3.getFeaturingClassifier());
    Artifact& artifact4 = prop3.getFeaturingClassifier()->as<Artifact>();
    ASSERT_TRUE(prop3.getFeaturingClassifier());
    ASSERT_EQ(*prop3.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(prop3.getNamespace());
    ASSERT_EQ(*prop3.getNamespace(), artifact4);
    ASSERT_TRUE(prop3.getOwner());
    ASSERT_EQ(*prop3.getOwner(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedAttributes().contains(pid));
    ASSERT_TRUE(artifact4.getAttributes().contains(pid));
    ASSERT_TRUE(artifact4.getFeatures().contains(pid));
    ASSERT_TRUE(artifact4.getOwnedMembers().contains(pid));
    ASSERT_TRUE(artifact4.getMembers().contains(pid));
    ASSERT_TRUE(artifact4.getOwnedElements().contains(pid));

    ID oid = op.getID();
    m.release(op);
    Operation& op2 = *artifact4.getOwnedOperations().front();
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(op2.getNamespace());
    ASSERT_EQ(*op2.getNamespace(), artifact4);
    ASSERT_TRUE(op2.getOwner());
    ASSERT_EQ(*op2.getOwner(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedOperations().contains(oid));
    ASSERT_TRUE(artifact4.getFeatures().contains(oid));
    ASSERT_TRUE(artifact4.getOwnedMembers().contains(oid));
    ASSERT_TRUE(artifact4.getMembers().contains(oid));
    ASSERT_TRUE(artifact4.getOwnedElements().contains(oid));

    m.release(artifact4, op2);
    Operation& op3 = m.get(oid)->as<Operation>();
    ASSERT_TRUE(op3.getFeaturingClassifier());
    Artifact& artifact5 = op3.getFeaturingClassifier()->as<Artifact>();
    ASSERT_TRUE(op3.getFeaturingClassifier());
    ASSERT_EQ(*op3.getFeaturingClassifier(), artifact5);
    ASSERT_TRUE(op3.getNamespace());
    ASSERT_EQ(*op3.getNamespace(), artifact5);
    ASSERT_TRUE(op3.getOwner());
    ASSERT_EQ(*op3.getOwner(), artifact5);
    ASSERT_TRUE(artifact5.getOwnedOperations().contains(oid));
    ASSERT_TRUE(artifact5.getFeatures().contains(oid));
    ASSERT_TRUE(artifact5.getOwnedMembers().contains(oid));
    ASSERT_TRUE(artifact5.getMembers().contains(oid));
    ASSERT_TRUE(artifact5.getOwnedElements().contains(oid));

    ID nid = nest.getID();
    m.release(nest);
    Artifact& nest2 = *artifact5.getNestedArtifacts().front();
    ASSERT_TRUE(nest2.getNamespace());
    ASSERT_EQ(*nest2.getNamespace(), artifact5);
    ASSERT_TRUE(nest2.getOwner());
    ASSERT_EQ(*nest2.getOwner(), artifact5);
    ASSERT_TRUE(artifact5.getNestedArtifacts().contains(nid));
    ASSERT_TRUE(artifact5.getOwnedMembers().contains(nid));
    ASSERT_TRUE(artifact5.getMembers().contains(nid));
    ASSERT_TRUE(artifact5.getOwnedElements().contains(nid));

    m.release(artifact5, nest2);
    Artifact& nest3 = m.get(nid)->as<Artifact>();
    ASSERT_TRUE(nest3.getNamespace());
    Artifact& artifact6 = nest3.getNamespace()->as<Artifact>();
    ASSERT_TRUE(nest3.getOwner());
    ASSERT_EQ(*nest3.getOwner(), artifact6);
    ASSERT_TRUE(artifact6.getNestedArtifacts().contains(nid));
    ASSERT_TRUE(artifact6.getOwnedMembers().contains(nid));
    ASSERT_TRUE(artifact6.getMembers().contains(nid));
    ASSERT_TRUE(artifact6.getOwnedElements().contains(nid));

    ID mid = manifestation.getID();
    ID uid = utilizedElement.getID();
    m.release(manifestation);
    Manifestation& manifestation2 = *artifact6.getManifestations().front();
    ASSERT_TRUE(manifestation2.getOwner());
    ASSERT_TRUE(manifestation2.getClients().contains(artifact6.getID()));
    ASSERT_EQ(*manifestation2.getOwner(), artifact6);
    ASSERT_TRUE(manifestation2.getClients().contains(aID));
    ASSERT_TRUE(manifestation2.getSources().contains(aID));
    ASSERT_TRUE(manifestation2.getRelatedElements().contains(aID));
    ASSERT_TRUE(manifestation2.getOwner());
    ASSERT_EQ(*manifestation2.getOwner(), artifact6);
    ASSERT_TRUE(manifestation2.getUtilizedElement());
    ASSERT_EQ(*manifestation2.getUtilizedElement(), utilizedElement);
    ASSERT_TRUE(manifestation2.getSuppliers().contains(uid));
    ASSERT_TRUE(manifestation2.getTargets().contains(uid));
    ASSERT_TRUE(manifestation2.getRelatedElements().contains(uid));
    ASSERT_TRUE(artifact6.getManifestations().contains(mid));
    ASSERT_TRUE(artifact6.getOwnedElements().contains(mid));

    m.release(artifact6, manifestation2, utilizedElement);
    Manifestation& manifestation3 = m.get(mid)->as<Manifestation>();
    ASSERT_TRUE(manifestation3.getUtilizedElement());
    Package& utilizedElement2 = manifestation3.getUtilizedElement()->as<Package>();
    ASSERT_TRUE(manifestation3.getOwner());
    ASSERT_TRUE(manifestation3.getClients().contains(aID));
    Artifact& artifact7 = manifestation3.getOwner()->as<Artifact>();
    ASSERT_EQ(*manifestation3.getOwner(), artifact7);
    ASSERT_TRUE(manifestation3.getClients().contains(aID));
    ASSERT_TRUE(manifestation3.getSources().contains(aID));
    ASSERT_TRUE(manifestation3.getRelatedElements().contains(aID));
    ASSERT_TRUE(manifestation3.getOwner());
    ASSERT_EQ(*manifestation3.getOwner(), artifact7);
    ASSERT_TRUE(manifestation3.getUtilizedElement());
    ASSERT_EQ(*manifestation3.getUtilizedElement(), utilizedElement2);
    ASSERT_TRUE(manifestation3.getSuppliers().contains(uid));
    ASSERT_TRUE(manifestation3.getTargets().contains(uid));
    ASSERT_TRUE(manifestation3.getRelatedElements().contains(uid));
    ASSERT_TRUE(artifact7.getManifestations().contains(mid));
    ASSERT_TRUE(artifact7.getOwnedElements().contains(mid));
}

TEST_F(DeploymentTest, mountDeploymentTest) {
    Manager<> m;
    InstanceSpecification& deploymentTarget = *m.create<InstanceSpecification>();
    Deployment& deployment = *m.create<Deployment>();
    Artifact& artifact = *m.create<Artifact>();
    Package& root = *m.create<Package>();
    deploymentTarget.getDeployments().add(deployment);
    deployment.getDeployedArtifacts().add(artifact);
    root.getPackagedElements().add(deploymentTarget, artifact);
    m.setRoot(&root);
    m.mount(ymlPath + "deploymentTests");

    ID deploymentID = deployment.getID();
    m.release(deployment);
    ASSERT_FALSE(m.loaded(deploymentID));
    Deployment& deployment2 = m.get(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment2.getLocation());
    ASSERT_EQ(*deployment2.getLocation(), deploymentTarget);
    ASSERT_EQ(deployment2.getClients().size(), 1);
    ASSERT_EQ(deployment2.getClients().front(), &deploymentTarget);
    ASSERT_TRUE(deployment2.getOwner());
    ASSERT_EQ(*deployment2.getOwner(), deploymentTarget);
    ASSERT_EQ(deployment2.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment2.getDeployedArtifacts().front(), &artifact);
    ASSERT_EQ(deployment2.getSuppliers().size(), 1);
    ASSERT_EQ(deployment2.getSuppliers().front(), &artifact);
    ASSERT_TRUE(deploymentTarget.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget.getDeployments().front(), &deployment2);

    ID deploymentTargetID = deploymentTarget.getID();
    ID artifactID = artifact.getID();
    m.release(deployment2, deploymentTarget);
    ASSERT_FALSE(m.loaded(deploymentTargetID));
    ASSERT_FALSE(m.loaded(deploymentID));
    Deployment& deployment3 = m.get(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment3.getLocation());
    ASSERT_EQ(deployment3.getLocation().id(), deploymentTargetID);
    ASSERT_EQ(deployment3.getClients().size(), 1);
    ASSERT_TRUE(deployment3.getClients().contains(deploymentTargetID));
    ASSERT_TRUE(deployment3.getOwner());
    ASSERT_EQ(deployment3.getOwner().id(), deploymentTargetID);
    ASSERT_EQ(deployment3.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment3.getDeployedArtifacts().front(), &artifact);
    ASSERT_EQ(deployment3.getSuppliers().size(), 1);
    ASSERT_EQ(deployment3.getSuppliers().front(), &artifact);
    InstanceSpecification& deploymentTarget2 = m.get(deploymentTargetID)->as<InstanceSpecification>();
    ASSERT_TRUE(deploymentTarget2.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget2.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget2.getDeployments().front(), &deployment3);
    ASSERT_EQ(deploymentTarget2.getClientDependencies().size(), 1);
    ASSERT_EQ(deploymentTarget2.getClientDependencies().front(), &deployment3);
    ASSERT_EQ(deploymentTarget2.getOwnedElements().size(), 1);
    ASSERT_EQ(*deploymentTarget2.getOwnedElements().begin(), deployment3);

    m.release(deployment3, deploymentTarget2);
    ASSERT_FALSE(m.loaded(deploymentTargetID));
    ASSERT_FALSE(m.loaded(deploymentID));
    InstanceSpecification& deploymentTarget3 = m.get(deploymentTargetID)->as<InstanceSpecification>();
    ASSERT_TRUE(deploymentTarget3.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getDeployments().size(), 1);
    ASSERT_TRUE(deploymentTarget3.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getClientDependencies().size(), 1);
    ASSERT_TRUE(deploymentTarget3.getClientDependencies().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getOwnedElements().size(), 1);
    ASSERT_EQ(*deploymentTarget3.getOwnedElements().ids().begin(), deploymentID);
    Deployment& deployment4 = m.get(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment4.getLocation());
    ASSERT_EQ(*deployment4.getLocation(), deploymentTarget3);
    ASSERT_EQ(deployment4.getClients().size(), 1);
    ASSERT_EQ(deployment4.getClients().front(), &deploymentTarget3);
    ASSERT_TRUE(deployment4.getOwner());
    ASSERT_EQ(*deployment4.getOwner(), deploymentTarget3);
    ASSERT_EQ(deployment4.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment4.getDeployedArtifacts().front(), &artifact);
    ASSERT_EQ(deployment4.getSuppliers().size(), 1);
    ASSERT_EQ(deployment4.getSuppliers().front(), &artifact);
    ASSERT_TRUE(deploymentTarget3.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget3.getDeployments().front(), &deployment4);

    m.release(deployment4, artifact);
    ASSERT_FALSE(m.loaded(deploymentID));
    ASSERT_FALSE(m.loaded(artifactID));
    Deployment& deployment5 = m.get(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment5.getLocation());
    ASSERT_EQ(*deployment5.getLocation(), deploymentTarget3);
    ASSERT_EQ(deployment5.getClients().size(), 1);
    ASSERT_EQ(deployment5.getClients().front(), &deploymentTarget3);
    ASSERT_TRUE(deployment5.getOwner());
    ASSERT_EQ(*deployment5.getOwner(), deploymentTarget3);
    ASSERT_EQ(deployment5.getDeployedArtifacts().size(), 1);
    ASSERT_TRUE(deployment5.getDeployedArtifacts().contains(artifactID));
    ASSERT_EQ(deployment5.getSuppliers().size(), 1);
    ASSERT_TRUE(deployment5.getSuppliers().contains(artifactID));
    Artifact& artifact2 = m.get(artifactID)->as<Artifact>();
    ASSERT_EQ(deployment5.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment5.getDeployedArtifacts().front(), &artifact2);
    ASSERT_EQ(deployment5.getSuppliers().size(), 1);
    ASSERT_EQ(deployment5.getSuppliers().front(), &artifact2);
}

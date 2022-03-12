#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/deployment.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front().as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1).as<Artifact>();
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front().getID(), artifact.getID());
}

TEST_F(DeploymentParserTest, basicArtifactTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/artifact.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = pckg.getPackagedElements().front().as<InstanceSpecification>();
    ASSERT_EQ(inst.getDeployments().size(), 1);
    Deployment& deployment = inst.getDeployments().front();
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get(1).as<Artifact>();
    ASSERT_TRUE(deployment.getLocation());
    ASSERT_EQ(deployment.getLocation()->getID(), inst.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front().getID(), artifact.getID());
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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/nestedArtifact.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::ARTIFACT);
    Artifact& artifact = el->as<Artifact>();
    ASSERT_EQ(artifact.getNestedArtifacts().size(), 1);
    Artifact& nest = artifact.getNestedArtifacts().front();
    ASSERT_EQ(nest.getName(), "nest");
}

TEST_F(DeploymentParserTest, emitDeploymentTest) {
    UmlManager m;
    Deployment& d = *m.create<Deployment>();
    Artifact& a = *m.create<Artifact>();
    d.setID("RP9VhYnGYcgWOqXxLt4_Xb3RAAM8");
    a.setID("bkwzmF3K0ddPG7CPwXVBZyyp8glc");
    d.getDeployedArtifacts().add(a);
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
    Deployment& d = *m.create<Deployment>();
    Property& prop = *m.create<Property>();
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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "deploymentTests/manifestations.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("utilizedEl1").getElementType(), ElementType::CLASS);
    Class& c1 = pckg.getPackagedElements().get("utilizedEl1").as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("artifact").getElementType(), ElementType::ARTIFACT);
    Artifact& artifact = pckg.getPackagedElements().get("artifact").as<Artifact>();
    ASSERT_EQ(artifact.getManifestations().size(), 2);
    Manifestation& m1 = artifact.getManifestations().front();
    Manifestation& m2 = artifact.getManifestations().back();
    ASSERT_TRUE(m2.getUtilizedElement());
    ASSERT_EQ(m2.getUtilizedElement()->getID(), c1.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("utilizedEl2").getElementType(), ElementType::CLASS);
    ASSERT_TRUE(m1.getUtilizedElement());
    ASSERT_EQ(m1.getUtilizedElement()->getID(), pckg.getPackagedElements().get("utilizedEl2").getID());
}

TEST_F(DeploymentParserTest, emitManifestationTest) {
    UmlManager m;
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

TEST_F(DeploymentParserTest, mountAndEditArtifactTest) {
    UmlManager m;
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
    Artifact& artifact2 = root.getPackagedElements().front().as<Artifact>();
    ASSERT_TRUE(artifact2.getOwningPackage());
    ASSERT_EQ(*artifact2.getOwningPackage(), root);
    ASSERT_TRUE(artifact2.getNamespace());
    ASSERT_EQ(*artifact2.getNamespace(), root);
    ASSERT_TRUE(artifact2.getOwner());
    ASSERT_EQ(*artifact2.getOwner(), root);
    ASSERT_TRUE(root.getPackagedElements().count(artifact2.getID()));
    ASSERT_TRUE(root.getOwnedMembers().count(artifact2.getID()));
    ASSERT_TRUE(root.getMembers().count(aID));
    ASSERT_TRUE(root.getOwnedElements().count(aID));

    m.release(artifact2, root);
    Artifact& artifact3 = m.aquire(aID)->as<Artifact>();
    ASSERT_TRUE(artifact3.getOwningPackage());
    Package& root2 = *artifact3.getOwningPackage();
    ASSERT_TRUE(artifact3.getNamespace());
    ASSERT_EQ(*artifact3.getNamespace(), root2);
    ASSERT_TRUE(artifact3.getOwner());
    ASSERT_EQ(*artifact3.getOwner(), root2);
    ASSERT_TRUE(root2.getPackagedElements().count(artifact3.getID()));
    ASSERT_TRUE(root2.getOwnedMembers().count(artifact3.getID()));
    ASSERT_TRUE(root2.getMembers().count(aID));
    ASSERT_TRUE(root2.getOwnedElements().count(aID));
    
    ID pid = prop.getID();
    m.release(prop);
    Property& prop2 = artifact3.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.getFeaturingClassifier());
    ASSERT_EQ(*prop2.getFeaturingClassifier(), artifact3);
    ASSERT_TRUE(prop2.getNamespace());
    ASSERT_EQ(*prop2.getNamespace(), artifact3);
    ASSERT_TRUE(prop2.getOwner());
    ASSERT_EQ(*prop2.getOwner(), artifact3);
    ASSERT_TRUE(artifact3.getOwnedAttributes().count(pid));
    ASSERT_TRUE(artifact3.getAttributes().count(pid));
    ASSERT_TRUE(artifact3.getFeatures().count(pid));
    ASSERT_TRUE(artifact3.getOwnedMembers().count(pid));
    ASSERT_TRUE(artifact3.getMembers().count(pid));
    ASSERT_TRUE(artifact3.getOwnedElements().count(pid));

    m.release(artifact3, prop2);
    Property& prop3 = m.aquire(pid)->as<Property>();
    ASSERT_TRUE(prop3.getFeaturingClassifier());
    Artifact& artifact4 = prop3.getFeaturingClassifier()->as<Artifact>();
    ASSERT_TRUE(prop3.getFeaturingClassifier());
    ASSERT_EQ(*prop3.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(prop3.getNamespace());
    ASSERT_EQ(*prop3.getNamespace(), artifact4);
    ASSERT_TRUE(prop3.getOwner());
    ASSERT_EQ(*prop3.getOwner(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedAttributes().count(pid));
    ASSERT_TRUE(artifact4.getAttributes().count(pid));
    ASSERT_TRUE(artifact4.getFeatures().count(pid));
    ASSERT_TRUE(artifact4.getOwnedMembers().count(pid));
    ASSERT_TRUE(artifact4.getMembers().count(pid));
    ASSERT_TRUE(artifact4.getOwnedElements().count(pid));

    ID oid = op.getID();
    m.release(op);
    Operation& op2 = artifact4.getOwnedOperations().front();
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), artifact4);
    ASSERT_TRUE(op2.getNamespace());
    ASSERT_EQ(*op2.getNamespace(), artifact4);
    ASSERT_TRUE(op2.getOwner());
    ASSERT_EQ(*op2.getOwner(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedOperations().count(oid));
    ASSERT_TRUE(artifact4.getFeatures().count(oid));
    ASSERT_TRUE(artifact4.getOwnedMembers().count(oid));
    ASSERT_TRUE(artifact4.getMembers().count(oid));
    ASSERT_TRUE(artifact4.getOwnedElements().count(oid));

    m.release(artifact4, op2);
    Operation& op3 = m.aquire(oid)->as<Operation>();
    ASSERT_TRUE(op3.getFeaturingClassifier());
    Artifact& artifact5 = op3.getFeaturingClassifier()->as<Artifact>();
    ASSERT_TRUE(op3.getFeaturingClassifier());
    ASSERT_EQ(*op3.getFeaturingClassifier(), artifact5);
    ASSERT_TRUE(op3.getNamespace());
    ASSERT_EQ(*op3.getNamespace(), artifact5);
    ASSERT_TRUE(op3.getOwner());
    ASSERT_EQ(*op3.getOwner(), artifact5);
    ASSERT_TRUE(artifact5.getOwnedOperations().count(oid));
    ASSERT_TRUE(artifact5.getFeatures().count(oid));
    ASSERT_TRUE(artifact5.getOwnedMembers().count(oid));
    ASSERT_TRUE(artifact5.getMembers().count(oid));
    ASSERT_TRUE(artifact5.getOwnedElements().count(oid));

    ID nid = nest.getID();
    m.release(nest);
    Artifact& nest2 = artifact5.getNestedArtifacts().front();
    ASSERT_TRUE(nest2.getNamespace());
    ASSERT_EQ(*nest2.getNamespace(), artifact5);
    ASSERT_TRUE(nest2.getOwner());
    ASSERT_EQ(*nest2.getOwner(), artifact5);
    ASSERT_TRUE(artifact5.getNestedArtifacts().count(nid));
    ASSERT_TRUE(artifact5.getOwnedMembers().count(nid));
    ASSERT_TRUE(artifact5.getMembers().count(nid));
    ASSERT_TRUE(artifact5.getOwnedElements().count(nid));

    m.release(artifact5, nest2);
    Artifact& nest3 = m.aquire(nid)->as<Artifact>();
    ASSERT_TRUE(nest3.getNamespace());
    Artifact& artifact6 = nest3.getNamespace()->as<Artifact>();
    ASSERT_TRUE(nest3.getOwner());
    ASSERT_EQ(*nest3.getOwner(), artifact6);
    ASSERT_TRUE(artifact6.getNestedArtifacts().count(nid));
    ASSERT_TRUE(artifact6.getOwnedMembers().count(nid));
    ASSERT_TRUE(artifact6.getMembers().count(nid));
    ASSERT_TRUE(artifact6.getOwnedElements().count(nid));

    ID mid = manifestation.getID();
    ID uid = utilizedElement.getID();
    m.release(manifestation);
    Manifestation& manifestation2 = artifact6.getManifestations().front();
    ASSERT_TRUE(manifestation2.getOwner());
    ASSERT_TRUE(manifestation2.getClient().contains(artifact6.getID()));
    ASSERT_EQ(*manifestation2.getOwner(), artifact6);
    ASSERT_TRUE(manifestation2.getClient().count(aID));
    ASSERT_TRUE(manifestation2.getSources().count(aID));
    ASSERT_TRUE(manifestation2.getRelatedElements().count(aID));
    ASSERT_TRUE(manifestation2.getOwner());
    ASSERT_EQ(*manifestation2.getOwner(), artifact6);
    ASSERT_TRUE(manifestation2.getUtilizedElement());
    ASSERT_EQ(*manifestation2.getUtilizedElement(), utilizedElement);
    ASSERT_TRUE(manifestation2.getSupplier().count(uid));
    ASSERT_TRUE(manifestation2.getTargets().count(uid));
    ASSERT_TRUE(manifestation2.getRelatedElements().count(uid));
    ASSERT_TRUE(artifact6.getManifestations().count(mid));
    ASSERT_TRUE(artifact6.getOwnedElements().count(mid));

    m.release(artifact6, manifestation2, utilizedElement);
    Manifestation& manifestation3 = m.aquire(mid)->as<Manifestation>();
    ASSERT_TRUE(manifestation3.getUtilizedElement());
    Package& utilizedElement2 = manifestation3.getUtilizedElement()->as<Package>();
    ASSERT_TRUE(manifestation3.getOwner());
    ASSERT_TRUE(manifestation3.getClient().contains(aID));
    Artifact& artifact7 = manifestation3.getOwner()->as<Artifact>();
    ASSERT_EQ(*manifestation3.getOwner(), artifact7);
    ASSERT_TRUE(manifestation3.getClient().count(aID));
    ASSERT_TRUE(manifestation3.getSources().count(aID));
    ASSERT_TRUE(manifestation3.getRelatedElements().count(aID));
    ASSERT_TRUE(manifestation3.getOwner());
    ASSERT_EQ(*manifestation3.getOwner(), artifact7);
    ASSERT_TRUE(manifestation3.getUtilizedElement());
    ASSERT_EQ(*manifestation3.getUtilizedElement(), utilizedElement2);
    ASSERT_TRUE(manifestation3.getSupplier().count(uid));
    ASSERT_TRUE(manifestation3.getTargets().count(uid));
    ASSERT_TRUE(manifestation3.getRelatedElements().count(uid));
    ASSERT_TRUE(artifact7.getManifestations().count(mid));
    ASSERT_TRUE(artifact7.getOwnedElements().count(mid));
}

TEST_F(DeploymentParserTest, mountDeploymentTest) {
    UmlManager m;
    InstanceSpecification& deploymentTarget = *m.create<InstanceSpecification>();
    Deployment& deployment = *m.create<Deployment>();
    Artifact& artifact = *m.create<Artifact>();
    Package& root = *m.create<Package>();
    deploymentTarget.getDeployments().add(deployment);
    deployment.getDeployedArtifacts().add(artifact);
    root.getPackagedElements().add(deploymentTarget, artifact);
    m.setRoot(root);
    m.mount(ymlPath + "deploymentTests");

    ID deploymentID = deployment.getID();
    m.release(deployment);
    ASSERT_FALSE(m.loaded(deploymentID));
    Deployment& deployment2 = m.aquire(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment2.getLocation());
    ASSERT_EQ(*deployment2.getLocation(), deploymentTarget);
    ASSERT_EQ(deployment2.getClient().size(), 1);
    ASSERT_EQ(deployment2.getClient().front(), deploymentTarget);
    ASSERT_TRUE(deployment2.getOwner());
    ASSERT_EQ(*deployment2.getOwner(), deploymentTarget);
    ASSERT_EQ(deployment2.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment2.getDeployedArtifacts().front(), artifact);
    ASSERT_EQ(deployment2.getSupplier().size(), 1);
    ASSERT_EQ(deployment2.getSupplier().front(), artifact);
    ASSERT_TRUE(deploymentTarget.getDeployments().count(deploymentID));
    ASSERT_EQ(deploymentTarget.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget.getDeployments().front(), deployment2);

    ID deploymentTargetID = deploymentTarget.getID();
    ID artifactID = artifact.getID();
    m.release(deployment2, deploymentTarget);
    ASSERT_FALSE(m.loaded(deploymentTargetID));
    ASSERT_FALSE(m.loaded(deploymentID));
    Deployment& deployment3 = m.aquire(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment3.getLocation());
    ASSERT_EQ(deployment3.getLocation().id(), deploymentTargetID);
    ASSERT_EQ(deployment3.getClient().size(), 1);
    ASSERT_TRUE(deployment3.getClient().contains(deploymentTargetID));
    ASSERT_TRUE(deployment3.getOwner());
    ASSERT_EQ(deployment3.getOwner().id(), deploymentTargetID);
    ASSERT_EQ(deployment3.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment3.getDeployedArtifacts().front(), artifact);
    ASSERT_EQ(deployment3.getSupplier().size(), 1);
    ASSERT_EQ(deployment3.getSupplier().front(), artifact);
    InstanceSpecification& deploymentTarget2 = m.aquire(deploymentTargetID)->as<InstanceSpecification>();
    ASSERT_TRUE(deploymentTarget2.getDeployments().count(deploymentID));
    ASSERT_EQ(deploymentTarget2.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget2.getDeployments().front(), deployment3);
    ASSERT_EQ(deploymentTarget2.getClientDependencies().size(), 1);
    ASSERT_EQ(deploymentTarget2.getClientDependencies().front(), deployment3);
    ASSERT_EQ(deploymentTarget2.getOwnedElements().size(), 1);
    ASSERT_EQ(*deploymentTarget2.getOwnedElements().begin(), deployment3);

    m.release(deployment3, deploymentTarget2);
    ASSERT_FALSE(m.loaded(deploymentTargetID));
    ASSERT_FALSE(m.loaded(deploymentID));
    InstanceSpecification& deploymentTarget3 = m.aquire(deploymentTargetID)->as<InstanceSpecification>();
    ASSERT_TRUE(deploymentTarget3.getDeployments().count(deploymentID));
    ASSERT_EQ(deploymentTarget3.getDeployments().size(), 1);
    ASSERT_TRUE(deploymentTarget3.getDeployments().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getClientDependencies().size(), 1);
    ASSERT_TRUE(deploymentTarget3.getClientDependencies().contains(deploymentID));
    ASSERT_EQ(deploymentTarget3.getOwnedElements().size(), 1);
    ASSERT_EQ(*deploymentTarget3.getOwnedElements().ids().begin(), deploymentID);
    Deployment& deployment4 = m.aquire(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment4.getLocation());
    ASSERT_EQ(*deployment4.getLocation(), deploymentTarget3);
    ASSERT_EQ(deployment4.getClient().size(), 1);
    ASSERT_EQ(deployment4.getClient().front(), deploymentTarget3);
    ASSERT_TRUE(deployment4.getOwner());
    ASSERT_EQ(*deployment4.getOwner(), deploymentTarget3);
    ASSERT_EQ(deployment4.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment4.getDeployedArtifacts().front(), artifact);
    ASSERT_EQ(deployment4.getSupplier().size(), 1);
    ASSERT_EQ(deployment4.getSupplier().front(), artifact);
    ASSERT_TRUE(deploymentTarget3.getDeployments().count(deploymentID));
    ASSERT_EQ(deploymentTarget3.getDeployments().size(), 1);
    ASSERT_EQ(deploymentTarget3.getDeployments().front(), deployment4);

    m.release(deployment4, artifact);
    ASSERT_FALSE(m.loaded(deploymentID));
    ASSERT_FALSE(m.loaded(artifactID));
    Deployment& deployment5 = m.aquire(deploymentID)->as<Deployment>();
    ASSERT_TRUE(deployment5.getLocation());
    ASSERT_EQ(*deployment5.getLocation(), deploymentTarget3);
    ASSERT_EQ(deployment5.getClient().size(), 1);
    ASSERT_EQ(deployment5.getClient().front(), deploymentTarget3);
    ASSERT_TRUE(deployment5.getOwner());
    ASSERT_EQ(*deployment5.getOwner(), deploymentTarget3);
    ASSERT_EQ(deployment5.getDeployedArtifacts().size(), 1);
    ASSERT_TRUE(deployment5.getDeployedArtifacts().contains(artifactID));
    ASSERT_EQ(deployment5.getSupplier().size(), 1);
    ASSERT_TRUE(deployment5.getSupplier().contains(artifactID));
    Artifact& artifact2 = m.aquire(artifactID)->as<Artifact>();
    ASSERT_EQ(deployment5.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment5.getDeployedArtifacts().front(), artifact2);
    ASSERT_EQ(deployment5.getSupplier().size(), 1);
    ASSERT_EQ(deployment5.getSupplier().front(), artifact2);
}
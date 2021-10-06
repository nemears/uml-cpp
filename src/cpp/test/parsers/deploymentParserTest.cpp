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

TEST_F(DeploymentParserTest, mountAndEditArtifactTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Artifact& artifact = m.create<Artifact>();
    Property& prop = m.create<Property>();
    Operation& op = m.create<Operation>();
    Artifact& nest = m.create<Artifact>();
    Manifestation& manifestation = m.create<Manifestation>();
    Package& utilizedElement = m.create<Package>();
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
    ASSERT_TRUE(artifact2.hasOwningPackage());
    ASSERT_EQ(artifact2.getOwningPackageRef(), root);
    ASSERT_TRUE(artifact2.hasNamespace());
    ASSERT_EQ(artifact2.getNamespaceRef(), root);
    ASSERT_TRUE(artifact2.getMemberNamespace().count(root.getID()));
    ASSERT_TRUE(artifact2.hasOwner());
    ASSERT_EQ(artifact2.getOwnerRef(), root);
    ASSERT_TRUE(root.getPackagedElements().count(artifact2.getID()));
    ASSERT_TRUE(root.getOwnedMembers().count(artifact2.getID()));
    ASSERT_TRUE(root.getMembers().count(aID));
    ASSERT_TRUE(root.getOwnedElements().count(aID));

    m.release(artifact2, root);
    Artifact& artifact3 = m.aquire(aID)->as<Artifact>();
    ASSERT_TRUE(artifact3.hasOwningPackage());
    Package& root2 = artifact3.getOwningPackageRef();
    ASSERT_TRUE(artifact3.hasNamespace());
    ASSERT_EQ(artifact3.getNamespaceRef(), root);
    ASSERT_TRUE(artifact3.getMemberNamespace().count(root.getID()));
    ASSERT_TRUE(artifact3.hasOwner());
    ASSERT_EQ(artifact3.getOwnerRef(), root2);
    ASSERT_TRUE(root2.getPackagedElements().count(artifact3.getID()));
    ASSERT_TRUE(root2.getOwnedMembers().count(artifact3.getID()));
    ASSERT_TRUE(root2.getMembers().count(aID));
    ASSERT_TRUE(root2.getOwnedElements().count(aID));
    
    ID pid = prop.getID();
    m.release(prop);
    Property& prop2 = artifact3.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.hasArtifact());
    ASSERT_EQ(prop2.getArtifactRef(), artifact3);
    ASSERT_TRUE(prop2.hasClassifier());
    ASSERT_EQ(prop2.getClassifierRef(), artifact3);
    ASSERT_TRUE(prop2.hasFeaturingClassifier());
    ASSERT_EQ(prop2.getFeaturingClassifierRef(), artifact3);
    ASSERT_TRUE(prop2.hasNamespace());
    ASSERT_EQ(prop2.getNamespaceRef(), artifact3);
    ASSERT_TRUE(prop2.getMemberNamespace().count(aID));
    ASSERT_TRUE(prop2.hasOwner());
    ASSERT_EQ(prop2.getOwnerRef(), artifact3);
    ASSERT_TRUE(artifact3.getOwnedAttributes().count(pid));
    ASSERT_TRUE(artifact3.getAttributes().count(pid));
    ASSERT_TRUE(artifact3.getFeatures().count(pid));
    ASSERT_TRUE(artifact3.getOwnedMembers().count(pid));
    ASSERT_TRUE(artifact3.getMembers().count(pid));
    ASSERT_TRUE(artifact3.getOwnedElements().count(pid));

    m.release(artifact3, prop2);
    Property& prop3 = m.aquire(pid)->as<Property>();
    ASSERT_TRUE(prop3.hasArtifact());
    Artifact& artifact4 = prop3.getArtifactRef();
    ASSERT_TRUE(prop3.hasClassifier());
    ASSERT_EQ(prop3.getClassifierRef(), artifact4);
    ASSERT_TRUE(prop3.hasFeaturingClassifier());
    ASSERT_EQ(prop3.getFeaturingClassifierRef(), artifact4);
    ASSERT_TRUE(prop3.hasNamespace());
    ASSERT_EQ(prop3.getNamespaceRef(), artifact4);
    ASSERT_TRUE(prop3.getMemberNamespace().count(aID));
    ASSERT_TRUE(prop3.hasOwner());
    ASSERT_EQ(prop3.getOwnerRef(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedAttributes().count(pid));
    ASSERT_TRUE(artifact4.getAttributes().count(pid));
    ASSERT_TRUE(artifact4.getFeatures().count(pid));
    ASSERT_TRUE(artifact4.getOwnedMembers().count(pid));
    ASSERT_TRUE(artifact4.getMembers().count(pid));
    ASSERT_TRUE(artifact4.getOwnedElements().count(pid));

    ID oid = op.getID();
    m.release(op);
    Operation& op2 = artifact4.getOwnedOperations().front();
    ASSERT_TRUE(op2.hasArtifact());
    ASSERT_EQ(op2.getArtifactRef(), artifact4);
    ASSERT_TRUE(op2.hasFeaturingClassifier());
    ASSERT_EQ(op2.getFeaturingClassifierRef(), artifact4);
    ASSERT_TRUE(op2.hasNamespace());
    ASSERT_EQ(op2.getNamespaceRef(), artifact4);
    ASSERT_TRUE(op2.getMemberNamespace().count(aID));
    ASSERT_TRUE(op2.hasOwner());
    ASSERT_EQ(op2.getOwnerRef(), artifact4);
    ASSERT_TRUE(artifact4.getOwnedOperations().count(oid));
    ASSERT_TRUE(artifact4.getFeatures().count(oid));
    ASSERT_TRUE(artifact4.getOwnedMembers().count(oid));
    ASSERT_TRUE(artifact4.getMembers().count(oid));
    ASSERT_TRUE(artifact4.getOwnedElements().count(oid));

    m.release(artifact4, op2);
    Operation& op3 = m.aquire(oid)->as<Operation>();
    ASSERT_TRUE(op3.hasArtifact());
    Artifact& artifact5 = op3.getArtifactRef();
    ASSERT_TRUE(op3.hasFeaturingClassifier());
    ASSERT_EQ(op3.getFeaturingClassifierRef(), artifact5);
    ASSERT_TRUE(op3.hasNamespace());
    ASSERT_EQ(op3.getNamespaceRef(), artifact5);
    ASSERT_TRUE(op3.getMemberNamespace().count(aID));
    ASSERT_TRUE(op3.hasOwner());
    ASSERT_EQ(op3.getOwnerRef(), artifact5);
    ASSERT_TRUE(artifact5.getOwnedOperations().count(oid));
    ASSERT_TRUE(artifact5.getFeatures().count(oid));
    ASSERT_TRUE(artifact5.getOwnedMembers().count(oid));
    ASSERT_TRUE(artifact5.getMembers().count(oid));
    ASSERT_TRUE(artifact5.getOwnedElements().count(oid));

    ID nid = nest.getID();
    m.release(nest);
    Artifact& nest2 = artifact5.getNestedArtifacts().front();
    ASSERT_TRUE(nest2.hasNamespace());
    ASSERT_EQ(nest2.getNamespaceRef(), artifact5);
    ASSERT_TRUE(nest2.getMemberNamespace().count(aID));
    ASSERT_TRUE(nest2.hasOwner());
    ASSERT_EQ(nest2.getOwnerRef(), artifact5);
    ASSERT_TRUE(artifact5.getNestedArtifacts().count(nid));
    ASSERT_TRUE(artifact5.getOwnedMembers().count(nid));
    ASSERT_TRUE(artifact5.getMembers().count(nid));
    ASSERT_TRUE(artifact5.getOwnedElements().count(nid));

    m.release(artifact5, nest2);
    Artifact& nest3 = m.aquire(nid)->as<Artifact>();
    ASSERT_TRUE(nest3.hasNamespace());
    Artifact& artifact6 = nest3.getNamespaceRef().as<Artifact>();
    ASSERT_TRUE(nest3.getMemberNamespace().count(aID));
    ASSERT_TRUE(nest3.hasOwner());
    ASSERT_EQ(nest3.getOwnerRef(), artifact6);
    ASSERT_TRUE(artifact6.getNestedArtifacts().count(nid));
    ASSERT_TRUE(artifact6.getOwnedMembers().count(nid));
    ASSERT_TRUE(artifact6.getMembers().count(nid));
    ASSERT_TRUE(artifact6.getOwnedElements().count(nid));
}
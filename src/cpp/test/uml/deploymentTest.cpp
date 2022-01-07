#include "gtest/gtest.h"
#include "uml/deployment.h"
#include "uml/deploymentTarget.h"
#include "uml/deployedArtifact.h"
#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "test/umlTestUtil.h"
#include "uml/manifestation.h"
#include "uml/package.h"
#include "uml/uml-stable.h"

using namespace UML;

class DeploymentTest : public ::testing::Test {};

TEST_F(DeploymentTest, basicDeploymentTest) {
    UmlManager m;
    Deployment& deployment = m.create<Deployment>();
    DeploymentTarget& location = m.create<DeploymentTarget>();
    DeployedArtifact& artifact = m.create<DeployedArtifact>();
    deployment.setLocation(&location);
    deployment.getDeployedArtifacts().add(artifact);
    ASSERT_TRUE(deployment.getLocation() != 0);
    ASSERT_EQ(deployment.getLocation()->getID(), location.getID());
    ASSERT_EQ(deployment.getClient().size(), 1);
    ASSERT_EQ(deployment.getClient().front().getID(), location.getID());
    ASSERT_TRUE(deployment.getOwner() != 0);
    ASSERT_EQ(deployment.getOwner()->getID(), location.getID());
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 1);
    ASSERT_EQ(deployment.getDeployedArtifacts().front().getID(), artifact.getID());
    ASSERT_EQ(deployment.getSupplier().size(), 1);
    ASSERT_EQ(deployment.getSupplier().front().getID(), artifact.getID());
    deployment.setLocation(0);
    deployment.getDeployedArtifacts().remove(artifact);
    ASSERT_TRUE(deployment.getLocation() == 0);
    ASSERT_EQ(deployment.getClient().size(), 0);
    ASSERT_EQ(deployment.getDeployedArtifacts().size(), 0);
    ASSERT_EQ(deployment.getSupplier().size(), 0);
    ASSERT_TRUE(deployment.getOwner() == 0);
}

TEST_F(DeploymentTest, artifactOperationAndAttributeTest) {
    UmlManager m;
    Artifact& artifact = m.create<Artifact>();
    Property& prop = m.create<Property>();
    Operation& op = m.create<Operation>();
    artifact.getOwnedAttributes().add(prop);
    artifact.getOwnedOperations().add(op);
    ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
    ASSERT_EQ(artifact.getAttributes().size(), 1);
    ASSERT_EQ(prop.getFeaturingClassifierRef().getID(), artifact.getID());
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
    Artifact& artifact = m.create<Artifact>();
    Property& property = m.create<Property>();
    Operation& operation = m.create<Operation>();
    Artifact& nestedArtifact = m.create<Artifact>();

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

// TEST_F(DeploymentTest, setArtifactTest) {
//     UmlManager m;
//     Artifact& artifact = m.create<Artifact>();
//     Property& prop = m.create<Property>();
//     Operation& op = m.create<Operation>();
//     prop.setArtifact(&artifact);
//     op.setArtifact(&artifact);
//     ASSERT_EQ(artifact.getOwnedAttributes().size(), 1);
//     ASSERT_EQ(artifact.getAttributes().size(), 1);
//     ASSERT_TRUE(prop.getArtifact() != 0);
//     ASSERT_EQ(prop.getArtifact()->getID(), artifact.getID());
//     ASSERT_EQ(prop.getClassifier()->getID(), artifact.getID());
//     ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
//     ASSERT_EQ(artifact.getOwnedOperations().size(), 1);
//     ASSERT_TRUE(op.getArtifact() != 0);
//     ASSERT_EQ(op.getArtifact()->getID(), artifact.getID());
//     prop.setArtifact(0);
//     op.setArtifact(0);
//     ASSERT_EQ(artifact.getOwnedAttributes().size(), 0);
//     ASSERT_EQ(artifact.getAttributes().size(), 0);
//     ASSERT_TRUE(prop.getArtifact() == 0);
//     ASSERT_EQ(artifact.getOwnedOperations().size(), 0);
//     ASSERT_TRUE(op.getArtifact() == 0);
// }

// TEST_F(DeploymentTest, nestedArtifactTest) {
//     UmlManager m;
//     Artifact& a1 = m.create<Artifact>();
//     Artifact& a2 = m.create<Artifact>();
//     a1.getNestedArtifacts().add(a2);
//     ASSERT_EQ(a1.getNestedArtifacts().size(), 1);
//     ASSERT_EQ(a1.getNestedArtifacts().front().getID(), a2.getID());
//     ASSERT_EQ(a1.getOwnedMembers().size(), 1);
//     ASSERT_EQ(a1.getOwnedMembers().front().getID(), a2.getID());
//     a1.getNestedArtifacts().remove(a2);
//     ASSERT_EQ(a1.getNestedArtifacts().size(), 0);
//     ASSERT_EQ(a1.getOwnedMembers().size(), 0);
//     ASSERT_THROW(a1.getNestedArtifacts().add(a1), NestedArtifactException);
// }

TEST_F(DeploymentTest, copyAndEditArtifactTest) {
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\nTODO: Uncomment me when we fix copying sets!" << std::endl;
    // UmlManager m;
    // Deployment& deployment = m.create<Deployment>();
    // DeploymentTarget& location = m.create<DeploymentTarget>();
    // Artifact& art = m.create<Artifact>();
    // Property& prop = m.create<Property>();
    // Operation& op = m.create<Operation>();
    // Artifact& nest = m.create<Artifact>();
    // Manifestation& man = m.create<Manifestation>();
    // Package& owner = m.create<Package>();
    // deployment.setLocation(&location);
    // deployment.getDeployedArtifacts().add(art);
    // art.getOwnedAttributes().add(prop);
    // art.getOwnedOperations().add(op);
    // art.getNestedArtifacts().add(nest);
    // art.getManifestations().add(man);
    // owner.getPackagedElements().add(art);
    // Artifact copy = art; // copy
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(art, copy, &Artifact::getOwnedAttributes,
    //                                  &Artifact::getOwnedOperations,
    //                                  &Artifact::getNestedArtifacts,
    //                                  &Artifact::getManifestations,
    //                                  &Classifier::getAttributes,
    //                                  &Classifier::getFeatures, 
    //                                  &Namespace::getOwnedMembers, 
    //                                  &Namespace::getMembers,
    //                                  &Element::getOwnedElements));
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(art, copy, &PackageableElement::getOwningPackage,
    //                                                                    &NamedElement::getNamespace,
    //                                                                    &Element::getOwner));
    // Property& prop2 = m.create<Property>();
    // copy.getOwnedAttributes().add(prop2);
    // Operation& op2 = m.create<Operation>();
    // copy.getOwnedOperations().add(op2);
    // Artifact& nest2 = m.create<Artifact>();
    // copy.getNestedArtifacts().add(nest2);
    // Manifestation& man2 = m.create<Manifestation>();
    // copy.getManifestations().add(man2);
    // Package& owner2 = m.create<Package>();
    // owner2.getPackagedElements().add(copy);
    // ASSERT_COPY_SEQUENCE_CORRECTLY(art, copy, &Artifact::getOwnedAttributes,
    //                                  &Artifact::getOwnedOperations,
    //                                  &Artifact::getNestedArtifacts,
    //                                  &Artifact::getManifestations,
    //                                  &Classifier::getAttributes,
    //                                  &Classifier::getFeatures, 
    //                                  &Namespace::getOwnedMembers, 
    //                                  &Namespace::getMembers,
    //                                  &Element::getOwnedElements);
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(art, copy, &Element::getOwner,
    //                                                                    &NamedElement::getNamespace,
    //                                                                    &PackageableElement::getOwningPackage));
    // copy.getOwnedAttributes().remove(prop2);
    // copy.getOwnedOperations().remove(op2);
    // copy.getNestedArtifacts().remove(nest2);
    // copy.getManifestations().remove(man2);
    // owner2.getPackagedElements().remove(copy);
    // ASSERT_COPY_SEQUENCE_CORRECTLY(art, copy, &Artifact::getOwnedAttributes,
    //                                  &Artifact::getOwnedOperations,
    //                                  &Artifact::getNestedArtifacts,
    //                                  &Artifact::getManifestations,
    //                                  &Classifier::getAttributes,
    //                                  &Classifier::getFeatures, 
    //                                  &Namespace::getOwnedMembers, 
    //                                  &Namespace::getMembers,
    //                                  &Element::getOwnedElements);
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(art, copy, &Element::getOwner,
    //                                                                    &NamedElement::getNamespace,
    //                                                                    &PackageableElement::getOwningPackage));
    // art.getOwnedAttributes().add(prop2);
    // art.getOwnedOperations().add(op2);
    // art.getNestedArtifacts().add(nest2);
    // art.getManifestations().add(man2);
    // owner.getPackagedElements().add(art);
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(art, copy, &Artifact::getOwnedAttributes,
    //                                  &Artifact::getOwnedOperations,
    //                                  &Artifact::getNestedArtifacts,
    //                                  &Artifact::getManifestations,
    //                                  &Classifier::getAttributes,
    //                                  &Classifier::getFeatures, 
    //                                  &Namespace::getOwnedMembers, 
    //                                  &Namespace::getMembers,
    //                                  &Element::getOwnedElements));
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(art, copy, &Element::getOwner,
    //                                                                    &NamedElement::getNamespace,
    //                                                                    &PackageableElement::getOwningPackage));
    // art.getOwnedAttributes().remove(prop2);
    // art.getOwnedOperations().remove(op2);
    // art.getNestedArtifacts().remove(nest2);
    // art.getManifestations().remove(man2);
    // owner.getPackagedElements().remove(art);
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(art, copy, &Artifact::getOwnedAttributes,
    //                                  &Artifact::getOwnedOperations,
    //                                  &Artifact::getNestedArtifacts,
    //                                  &Artifact::getManifestations,
    //                                  &Classifier::getAttributes,
    //                                  &Classifier::getFeatures, 
    //                                  &Namespace::getOwnedMembers, 
    //                                  &Namespace::getMembers,
    //                                  &Element::getOwnedElements));
    // ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(art, copy, &Element::getOwner,
    //                                                                    &NamedElement::getNamespace,
    //                                                                    &PackageableElement::getOwningPackage));
}
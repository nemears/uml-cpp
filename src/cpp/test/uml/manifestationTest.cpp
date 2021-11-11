#include "gtest/gtest.h"
#include "uml/manifestation.h"
#include "uml/artifact.h"

using namespace UML;

class ManifestationTest : public ::testing::Test {};

TEST_F(ManifestationTest, setUtilizedElementTest) {
    UmlManager m;
    Manifestation& man = m.create<Manifestation>();
    Dependency& d = m.create<Dependency>();
    man.setUtilizedElement(&d);
    ASSERT_TRUE(man.getUtilizedElement() != 0);
    ASSERT_EQ(man.getUtilizedElement(), &d);
    ASSERT_EQ(man.getSupplier().size(), 1);
    ASSERT_EQ(man.getSupplier().front().getID(), d.getID());
    Dependency& d2 = m.create<Dependency>();
    man.setUtilizedElement(&d2);
    ASSERT_TRUE(man.getUtilizedElement() != 0);
    ASSERT_EQ(man.getUtilizedElement(), & d2);
    ASSERT_EQ(man.getSupplier().size(), 1);
    ASSERT_EQ(man.getSupplier().front().getID(), d2.getID());
    man.setUtilizedElement(0);
    ASSERT_TRUE(man.getUtilizedElement() == 0);
    ASSERT_EQ(man.getSupplier().size(), 0);
}

TEST_F(ManifestationTest, artifactTest) {
    UmlManager m;
    Manifestation& man = m.create<Manifestation>();
    Artifact& a = m.create<Artifact>();
    man.setArtifact(&a);
    Artifact& a2 = m.create<Artifact>();
    ASSERT_TRUE(man.getArtifact() != 0);
    ASSERT_EQ(man.getArtifact(), &a);
    ASSERT_EQ(man.getClient().size(), 1);
    ASSERT_EQ(man.getClient().front().getID(), a.getID());
    ASSERT_EQ(a.getManifestations().size(),  1);
    ASSERT_EQ(a.getManifestations().front().getID(), man.getID());
    ASSERT_EQ(a.getOwnedElements().size(), 1);
    ASSERT_EQ((*a.getOwnedElements().begin()).getID(), man.getID());
    man.setArtifact(&a2);
    ASSERT_TRUE(man.getArtifact() != 0);
    ASSERT_EQ(man.getArtifact(), &a2);
    ASSERT_EQ(man.getClient().size(), 1);
    ASSERT_EQ(man.getClient().front().getID(), a2.getID());
    ASSERT_EQ(a2.getManifestations().size(),  1);
    ASSERT_EQ(a2.getManifestations().front().getID(), man.getID());
    ASSERT_EQ(a2.getOwnedElements().size(), 1);
    ASSERT_EQ((*a2.getOwnedElements().begin()).getID(), man.getID());
    ASSERT_EQ(a.getManifestations().size(), 0);
    ASSERT_EQ(a.getOwnedElements().size(), 0);
    man.setArtifact(0);
    ASSERT_TRUE(man.getArtifact() == 0);
    ASSERT_EQ(man.getClient().size(), 0);
    ASSERT_EQ(a2.getManifestations().size(), 0);
    a2.getManifestations().add(man);
    ASSERT_TRUE(man.getArtifact() != 0);
    ASSERT_EQ(man.getArtifact(), &a2);
    ASSERT_EQ(man.getClient().size(), 1);
    ASSERT_EQ(man.getClient().front().getID(), a2.getID());
    ASSERT_EQ(a2.getManifestations().size(),  1);
    ASSERT_EQ(a2.getManifestations().front().getID(), man.getID());
    ASSERT_EQ(a2.getOwnedElements().size(), 1);
    ASSERT_EQ((*a2.getOwnedElements().begin()).getID(), man.getID());
    a2.getManifestations().remove(man);
    ASSERT_TRUE(man.getArtifact() == 0);
    ASSERT_EQ(man.getClient().size(), 0);
    ASSERT_EQ(a2.getManifestations().size(), 0);
}
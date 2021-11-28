#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ClassifierTest : public ::testing::Test {};

TEST_F(ClassifierTest, GetOwnedAttributesTest) {
	UmlManager m;
	Class& classifier1 = m.create<Class>();
	Property& prop = m.create<Property>();
	classifier1.getOwnedAttributes().add(prop);
	ASSERT_FALSE(classifier1.getAttributes().empty());
	ASSERT_EQ(classifier1.getAttributes().front(), prop);
}

TEST_F(ClassifierTest, addAttributeFunctorTest) {
  UmlManager m;
  Class& c = m.create<Class>();
  Property& p = m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
}

TEST_F(ClassifierTest, setClassifierTest) {
  UmlManager m;
  Property& p = m.create<Property>();
  Class& c = m.create<Class>();
  p.setClass(&c);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClass() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
}

TEST_F(ClassifierTest, removeAttributeFunctorTest) {
  UmlManager m;
  Property& p = m.create<Property>();
  Class& c = m.create<Class>();
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(c.getOwnedAttributes().remove(p));
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!p.getFeaturingClassifier());
  ASSERT_TRUE(!p.getNamespace());
  ASSERT_TRUE(!p.getOwner());
}

TEST_F(ClassifierTest, copyClassifierTest) {
  UmlManager m;
  Class& c = m.create<Class>();
  c.setName("test");
  Package& d = m.create<Package>();
  Property& p = m.create<Property>();
  Class& b = m.create<Class>();
  Generalization& gen = m.create<Generalization>();
  c.getOwnedAttributes().add(p);
  c.getGeneralizations().add(gen);
  gen.setGeneral(b);
  d.getPackagedElements().add(c);
  d.getPackagedElements().add(b);
  Class c2 = c;
  ASSERT_TRUE(c2.getID() == c.getID());
  ASSERT_TRUE(c2.getName().compare(c.getName()) == 0);
  ASSERT_TRUE(c2.getAttributes().size() == 1);
  ASSERT_TRUE(&c2.getAttributes().front() == &p);
  ASSERT_TRUE(c2.getFeatures().size() == 1);
  ASSERT_TRUE(&c2.getFeatures().front() == &p);
  ASSERT_TRUE(c2.getMembers().size() == 1);
  ASSERT_TRUE(&c2.getMembers().front() == &p);
  ASSERT_TRUE(c2.getOwningPackage() == &d);
  ASSERT_EQ(c2.getGeneralizations().size(), 1);
  ASSERT_EQ(c2.getGeneralizations().front(), gen);
  ASSERT_EQ(c2.getOwnedElements().size(), 2);
}

TEST_F(ClassifierTest, inheritedMembersTest) {
  // UmlManager m;
  // Class& g1 = m.create<Class>();
  // Class& s1 = m.create<Class>();
  // Property& p1 = m.create<Property>();
  // g1.getOwnedAttributes().add(p1);
  // Generalization& gen1 = m.create<Generalization>();
  // gen1.setGeneral(&g1);
  // s1.getGeneralizations().add(gen1);
  // ASSERT_TRUE(s1.getInheritedMembers().size() == 1);
  // ASSERT_TRUE(&s1.getInheritedMembers().front() == &p1);
  // ASSERT_TRUE(s1.getMembers().size() == 1);
  // ASSERT_TRUE(&s1.getMembers().front() == &p1);
  // ASSERT_NO_THROW(s1.getGeneralizations().remove(gen1));
  // ASSERT_TRUE(s1.getInheritedMembers().size() == 0);
  // ASSERT_TRUE(s1.getMembers().size() == 0);

  // Class& g2 = m.create<Class>();
  // Class& s2 = m.create<Class>();
  // Property& p2 = m.create<Property>();
  // g2.getOwnedAttributes().add(p2);
  // s2.getGenerals().add(g2);
  // ASSERT_TRUE(s2.getInheritedMembers().size() == 1);
  // ASSERT_TRUE(&s2.getInheritedMembers().front() == &p2);
  // ASSERT_TRUE(s2.getMembers().size() == 1);
  // ASSERT_TRUE(&s2.getMembers().front() == &p2);
  // ASSERT_NO_THROW(s2.getGenerals().remove(g2));
  // ASSERT_TRUE(s2.getInheritedMembers().size() == 0);
  // ASSERT_TRUE(s2.getMembers().size() == 0);

  // Class& g3 = m.create<Class>();
  // Class& s3 = m.create<Class>();
  // Property& p3 = m.create<Property>();
  // g3.getOwnedAttributes().add(p3);
  // Generalization& gen3 = m.create<Generalization>();
  // s3.getGeneralizations().add(gen3);
  // gen3.setGeneral(&g3);
  // ASSERT_TRUE(s3.getInheritedMembers().size() == 1);
  // ASSERT_TRUE(&s3.getInheritedMembers().front() == &p3);
  // ASSERT_TRUE(s3.getMembers().size() == 1);
  // ASSERT_TRUE(&s3.getMembers().front() == &p3);
  // ASSERT_NO_THROW(s3.getGeneralizations().remove(gen3));
  // ASSERT_TRUE(s3.getInheritedMembers().size() == 0);
  // ASSERT_TRUE(s3.getMembers().size() == 0);

  // Class& g4 = m.create<Class>();
  // Class& s4 = m.create<Class>();
  // Property& p4 = m.create<Property>();
  // g4.getOwnedAttributes().add(p4);
  // p4.setVisibility(VisibilityKind::PRIVATE);
  // s4.getGenerals().add(g4);
  // ASSERT_TRUE(s4.getInheritedMembers().size() == 0);

  // Class& g5 = m.create<Class>();
  // Class& s5 = m.create<Class>();
  // Property& p5 = m.create<Property>();
  // g5.getOwnedAttributes().add(p5);
  // s5.getGenerals().add(g5);
  // p5.setVisibility(VisibilityKind::PRIVATE);
  // ASSERT_TRUE(s5.getInheritedMembers().size() == 0);
  // ASSERT_TRUE(s5.getMembers().size() == 0);

  // Class& g6 = m.create<Class>();
  // Class& s6 = m.create<Class>();
  // Property& p6 = m.create<Property>();
  // s6.getGenerals().add(g6);
  // g6.getOwnedAttributes().add(p6);
  // ASSERT_TRUE(s6.getInheritedMembers().size() == 1);
  // ASSERT_TRUE(s6.getMembers().size() == 1);
}

TEST_F(ClassifierTest, reindexClassifierID_test) {
	UmlManager m;
	DataType reindexed = m.create<DataType>();
	DataType general = m.create<DataType>();
	Generalization generalization = m.create<Generalization>();
	Property attribute = m.create<Property>();
	Package root = m.create<Package>();
	InstanceSpecification instance = m.create<InstanceSpecification>();
	reindexed.getGeneralizations().add(generalization);
	generalization.setGeneral(general);
	reindexed.getOwnedAttributes().add(attribute);
	instance.getClassifiers().add(reindexed);
	root.getPackagedElements().add(reindexed, general, instance);
	ID id = ID::fromString("YXA7t1zgj89FRZePjCmulq1h5s5s");
	reindexed.setID(id);
	ASSERT_NO_THROW(ASSERT_EQ(generalization.getSpecificRef(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(generalization.getSources().get(id), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(*generalization.getOwner(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(attribute.getDataTypeRef(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(attribute.getFeaturingClassifierRef(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(attribute.getNamespaceRef(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(*attribute.getOwner(), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(root.getPackagedElements().get(id), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(root.getOwnedMembers().get(id), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(root.getMembers().get(id), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(root.getOwnedElements().get(id), reindexed));
	ASSERT_NO_THROW(ASSERT_EQ(instance.getClassifiers().front(), reindexed));
}
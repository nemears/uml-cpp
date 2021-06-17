#include "gtest/gtest.h"
#include "uml/classifier.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/generalization.h"

using namespace UML;

class ClassifierTest : public ::testing::Test {
    public:
    Classifier classifier1;
    Property prop;
    Classifier* classifierPtr;
    Property prop2;
    Property* propPtr;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassifierTest() {
     classifierPtr = new Classifier;
     propPtr = new Property;
  }

  ~ClassifierTest() override {
     delete classifierPtr;
     delete propPtr;
  }

  void SetUp() override {
     // add element to owned element list
     classifier1.getAttributes().add(prop);
     classifier1.getAttributes().add(*propPtr);
     classifierPtr->getAttributes().add(prop2);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ClassifierTest, GetOwnedAttributesTest) {
  EXPECT_FALSE(classifier1.getAttributes().empty());
  EXPECT_EQ(classifier1.getAttributes().front(), &prop);
  EXPECT_EQ(classifier1.getAttributes().back(), propPtr);
  EXPECT_EQ(classifierPtr->getAttributes().front(), &prop2);
}

TEST_F(ClassifierTest, addAttributeFunctorTest) {
  Classifier c;
  Property p;
  c.getAttributes().add(p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
  ASSERT_TRUE(p.getMemberNamespace().count(c.getID()) == 1);
  ASSERT_TRUE(p.getMemberNamespace().size() == 1);
  ASSERT_TRUE(p.getMemberNamespace().front() == &c);
}

TEST_F(ClassifierTest, addAttributeFunctorTestW_Manager) {
  UmlManager m;
  Classifier& c = m.create<Class>();
  Property& p = m.create<Property>();
  c.getAttributes().add(p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
  ASSERT_TRUE(p.getMemberNamespace().count(c.getID()) == 1);
  ASSERT_TRUE(p.getMemberNamespace().size() == 1);
  ASSERT_TRUE(p.getMemberNamespace().front() == &c);
}

TEST_F(ClassifierTest, setClassifierTest) {
  Property p;
  Classifier c;
  p.setClassifier(&c);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
  ASSERT_TRUE(p.getMemberNamespace().count(c.getID()));
}

TEST_F(ClassifierTest, setClassifierTestW_Mananger) {
  UmlManager m;
  Property& p = m.create<Property>();
  Classifier& c = m.create<Class>();
  p.setClassifier(&c);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
  ASSERT_TRUE(p.getMemberNamespace().count(c.getID()));
}

TEST_F(ClassifierTest, removeAttributeFunctorTest) {
  Property p;
  Classifier c;
  c.getAttributes().add(p);
  ASSERT_NO_THROW(c.getAttributes().remove(p));
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!p.getClassifier());
  ASSERT_TRUE(!p.getFeaturingClassifier());
  ASSERT_TRUE(!p.getNamespace());
  ASSERT_TRUE(p.getMemberNamespace().size() == 0);
  ASSERT_TRUE(!p.getOwner());
}

TEST_F(ClassifierTest, removeAttributeFunctorTestW_Manager) {
  UmlManager m;
  Property& p = m.create<Property>();
  Classifier& c = m.create<Class>();
  c.getAttributes().add(p);
  ASSERT_NO_THROW(c.getAttributes().remove(p));
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!p.getClassifier());
  ASSERT_TRUE(!p.getFeaturingClassifier());
  ASSERT_TRUE(!p.getNamespace());
  ASSERT_TRUE(p.getMemberNamespace().size() == 0);
  ASSERT_TRUE(!p.getOwner());
}

TEST_F(ClassifierTest, copyClassifierTest) {
  UmlManager m;
  Classifier& c = m.create<Class>();
  c.setName("test");
  Package& d = m.create<Package>();
  Property& p = m.create<Property>();
  Classifier& b = m.create<Class>();
  c.getAttributes().add(p);
  c.getGenerals().add(b);
  d.getPackagedElements().add(c);
  d.getPackagedElements().add(b);
  Classifier c2 = c;
  ASSERT_TRUE(c2.getID() == c.getID());
  ASSERT_TRUE(c2.getName().compare(c.getName()) == 0);
  ASSERT_TRUE(c2.getAttributes().size() == 1);
  ASSERT_TRUE(c2.getAttributes().front() == &p);
  ASSERT_TRUE(c2.getFeatures().size() == 1);
  ASSERT_TRUE(c2.getFeatures().front() == &p);
  ASSERT_TRUE(c2.getMembers().size() == 1);
  ASSERT_TRUE(c2.getMembers().front() == &p);
  ASSERT_TRUE(c2.getOwningPackage() == &d);
}

TEST_F(ClassifierTest, inheritedMembersTest) {
  UmlManager m;
  Class& g1 = m.create<Class>();
  Class& s1 = m.create<Class>();
  Property& p1 = m.create<Property>();
  g1.getOwnedAttributes().add(p1);
  Generalization& gen1 = m.create<Generalization>();
  gen1.setGeneral(&g1);
  s1.getGeneralizations().add(gen1);
  ASSERT_TRUE(s1.getInheritedMembers().size() == 1);
  ASSERT_TRUE(s1.getInheritedMembers().front() == &p1);
  ASSERT_TRUE(s1.getMembers().size() == 1);
  ASSERT_TRUE(s1.getMembers().front() == &p1);
  ASSERT_NO_THROW(s1.getGeneralizations().remove(gen1));
  ASSERT_TRUE(s1.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s1.getMembers().size() == 0);

  Class& g2 = m.create<Class>();
  Class& s2 = m.create<Class>();
  Property& p2 = m.create<Property>();
  g2.getOwnedAttributes().add(p2);
  s2.getGenerals().add(g2);
  ASSERT_TRUE(s2.getInheritedMembers().size() == 1);
  ASSERT_TRUE(s2.getInheritedMembers().front() == &p2);
  ASSERT_TRUE(s2.getMembers().size() == 1);
  ASSERT_TRUE(s2.getMembers().front() == &p2);
  ASSERT_NO_THROW(s2.getGenerals().remove(g2));
  ASSERT_TRUE(s2.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s2.getMembers().size() == 0);

  Class& g3 = m.create<Class>();
  Class& s3 = m.create<Class>();
  Property& p3 = m.create<Property>();
  g3.getOwnedAttributes().add(p3);
  Generalization& gen3 = m.create<Generalization>();
  s3.getGeneralizations().add(gen3);
  gen3.setGeneral(&g3);
  ASSERT_TRUE(s3.getInheritedMembers().size() == 1);
  ASSERT_TRUE(s3.getInheritedMembers().front() == &p3);
  ASSERT_TRUE(s3.getMembers().size() == 1);
  ASSERT_TRUE(s3.getMembers().front() == &p3);
  ASSERT_NO_THROW(s3.getGeneralizations().remove(gen3));
  ASSERT_TRUE(s3.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s3.getMembers().size() == 0);

  Class& g4 = m.create<Class>();
  Class& s4 = m.create<Class>();
  Property& p4 = m.create<Property>();
  g4.getOwnedAttributes().add(p4);
  p4.setVisibility(VisibilityKind::PRIVATE);
  s4.getGenerals().add(g4);
  ASSERT_TRUE(s4.getInheritedMembers().size() == 0);

  Class& g5 = m.create<Class>();
  Class& s5 = m.create<Class>();
  Property& p5 = m.create<Property>();
  g5.getOwnedAttributes().add(p5);
  s5.getGenerals().add(g5);
  p5.setVisibility(VisibilityKind::PRIVATE);
  ASSERT_TRUE(s5.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s5.getMembers().size() == 0);

  Class& g6 = m.create<Class>();
  Class& s6 = m.create<Class>();
  Property& p6 = m.create<Property>();
  s6.getGenerals().add(g6);
  g6.getOwnedAttributes().add(p6);
  ASSERT_TRUE(s6.getInheritedMembers().size() == 1);
  ASSERT_TRUE(s6.getMembers().size() == 1);
}
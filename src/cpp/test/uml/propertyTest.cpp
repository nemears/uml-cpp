#include "gtest/gtest.h"
#include "uml/valueSpecification.h"
#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/class.h"

using namespace UML;

class PropertyTest : public ::testing::Test {
    public:

  PropertyTest() {

  }

  ~PropertyTest() override {

  }

  void SetUp() override {
     
  }

  void TearDown() override {
    // nothing right now
  }
};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
    Property p;
    PrimitiveType stringP;
    p.setType(&stringP);
    LiteralString ls;
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
}

// TEST_F(PropertyTest, setDefaultValueOfImproperTypeTestString) {
//     Property p;
//     PrimitiveType stringP;

//     // assign it improper type if expecting string
//     stringP.setPrimitiveType(PrimitiveType::Primitive::INT);

//     p.setType(&stringP);
    
//     LiteralString ls;
//     ls.setValue("test");
//     EXPECT_THROW(p.setDefaultValue(&ls), Property::InvalidValueException);
// }

TEST_F(PropertyTest, reindexID_forClassiferTest) {
  Class c;
  Property p;
  Element e;
  c.getAttributes().add(p);
  p.setClassifier(&c);
  e.getOwnedElements().add(p);
  p.setOwner(&e);
  p.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  ASSERT_TRUE(c.getAttributes().get(p.getID()) != NULL);
  ASSERT_TRUE(e.getOwnedElements().get(p.getID()) != NULL);
}

TEST_F(PropertyTest, reindexNameForClassifierTest) {
  Class c;
  Property p;
  Element e;
  c.getAttributes().add(p);
  p.setClassifier(&c);
  e.getOwnedElements().add(p);
  p.setOwner(&e);
  p.setName("test");
  ASSERT_TRUE(c.getAttributes().get("test") != NULL);
  ASSERT_TRUE(e.getOwnedElements().get("test") != NULL);
}

TEST_F(PropertyTest, overwriteClassifierTest) {
  Classifier p1;
  Classifier p2;
  Property c;
  p1.getAttributes().add(c);
  c.setClassifier(&p2);
  ASSERT_TRUE(p2.getAttributes().size() == 1);
  ASSERT_TRUE(p2.getAttributes().front() == &c);
  ASSERT_TRUE(c.getClassifier() == &p2);
  ASSERT_TRUE(p1.getAttributes().size() == 0);
}

TEST_F(PropertyTest, overwriteClassifierByAttributesAddTest) {
  Classifier p1;
  Classifier p2;
  Property c;
  p1.getAttributes().add(c);
  p2.getAttributes().add(c);
  ASSERT_TRUE(p2.getAttributes().size() == 1);
  ASSERT_TRUE(p2.getAttributes().front() == &c);
  ASSERT_TRUE(c.getClassifier() == &p2);
  ASSERT_TRUE(p1.getAttributes().size() == 0);
}

TEST_F(PropertyTest, copyPropertyTest) {
  Property p;
  p.setName("test");
  p.setStatic(true);
  Classifier c;
  c.getAttributes().add(p);
  Classifier t;
  p.setType(&t);
  Property p2 = p;
  ASSERT_TRUE(p2.getID() == p.getID());
  ASSERT_TRUE(p.getName().compare(p2.getName()) == 0);
  ASSERT_TRUE(p2.getType() == &t);
  ASSERT_TRUE(p2.getClassifier() == &c);
  ASSERT_TRUE(p2.getMemberNamespace().size() == 1);
  ASSERT_TRUE(p2.getMemberNamespace().front() == &c);
  ASSERT_TRUE(p2.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p2.isStatic());
}
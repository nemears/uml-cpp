#include "gtest/gtest.h"
#include "uml/valueSpecification.h"
#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/class.h"
#include "uml/association.h"

using namespace UML;

class PropertyTest : public ::testing::Test {};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
    Property p;
    PrimitiveType stringP;
    p.setType(&stringP);
    LiteralString ls;
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
    ASSERT_TRUE(p.getDefaultValue() == &ls);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().front() == &ls);
}

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestStringW_Manager) {
  UmlManager m;
    Property& p = m.create<Property>();
    PrimitiveType& stringP = m.create<PrimitiveType>();
    p.setType(&stringP);
    LiteralString& ls = m.create<LiteralString>();
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
    ASSERT_TRUE(p.getDefaultValue() == &ls);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(p.getOwnedElements().front() == &ls);
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
  p.setAggregation(AggregationKind::COMPOSITE);
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setID("190d1cb9_13dc_44e6_a064_1268"));
  ASSERT_TRUE(c.getOwnedElements().get(p.getID()) != 0);
  ASSERT_TRUE(c.getMembers().get(p.getID()) != 0);
  ASSERT_TRUE(c.getOwnedMembers().get(p.getID()) != 0);
  ASSERT_TRUE(c.getFeatures().get(p.getID()) != 0);
  ASSERT_TRUE(c.getAttributes().get(p.getID()) != NULL);
  ASSERT_TRUE(c.getOwnedAttributes().get(p.getID()) != 0);
  ASSERT_TRUE(c.getRole().get(p.getID()) != 0);
  ASSERT_TRUE(c.getParts().get(p.getID()) != 0);

  Association a;
  Property p2;
  a.getNavigableOwnedEnds().add(p2);
  ASSERT_NO_THROW(p2.setID("c0ab87cc_d00b_4afb_9558_5382"));
  ASSERT_TRUE(a.getNavigableOwnedEnds().get(p2.getID()));
  ASSERT_TRUE(a.getOwnedEnds().get(p2.getID()));
  ASSERT_TRUE(a.getMemberEnds().get(p2.getID()));
  ASSERT_TRUE(a.getFeatures().get(p2.getID()));
  ASSERT_TRUE(a.getOwnedMembers().get(p2.getID()));
  ASSERT_TRUE(a.getMembers().get(p2.getID()));
  ASSERT_TRUE(a.getOwnedElements().get(p2.getID()));
}

// TEST_F(PropertyTest, reindexNameForClassifierTest) {
//   // Class c;
//   // Property p;
//   // p.setAggregation(AggregationKind::COMPOSITE);
//   // c.getOwnedAttributes().add(p);
//   // ASSERT_NO_THROW(p.setName("test"));
//   // ASSERT_TRUE(c.getOwnedElements().get("test") != 0);
//   // ASSERT_TRUE(c.getMembers().get("test") != 0);
//   // ASSERT_TRUE(c.getOwnedMembers().get("test") != 0);
//   // ASSERT_TRUE(c.getFeatures().get("test") != 0);
//   // ASSERT_TRUE(c.getAttributes().get("test") != NULL);
//   // ASSERT_TRUE(c.getOwnedAttributes().get("test") != 0);
//   // ASSERT_TRUE(c.getRole().get("test") != 0);
//   // ASSERT_TRUE(c.getParts().get("test") != 0);

//   Association a;
//   Property p2;
//   a.getNavigableOwnedEnds().add(p2);
//   ASSERT_NO_THROW(p2.setName("test"));
//   ASSERT_TRUE(a.getNavigableOwnedEnds().get("test"));
//   ASSERT_TRUE(a.getOwnedEnds().get("test"));
//   ASSERT_TRUE(a.getMemberEnds().get("test"));
//   ASSERT_TRUE(a.getFeatures().get("test"));
//   ASSERT_TRUE(a.getOwnedMembers().get("test"));
//   ASSERT_TRUE(a.getMembers().get("test"));
//   ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

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
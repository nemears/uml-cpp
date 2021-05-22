#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/relationship.h"
#include "uml/package.h"

using namespace UML;

class ElementTest : public ::testing::Test {};

TEST_F(ElementTest, OverrideID_Test) {
  Package el1;
  el1.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  boost::uuids::uuid uuid = boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  EXPECT_EQ(el1.getID(), uuid);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  Package el2;
  Package el3;
  el2.getPackagedElements().add(el3);
  EXPECT_FALSE(el2.getOwnedElements().empty());
  EXPECT_EQ(el2.getOwnedElements().get(0), &el3);
}

TEST_F(ElementTest, InvalidID_Test) {
  Package el3;
  EXPECT_THROW(el3.setID("not a uuid4"), InvalidID_Exception);
  EXPECT_NO_THROW(el3.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
}

TEST_F(ElementTest, getNullOwnerTest) {
  Element e;
  ASSERT_TRUE(e.getOwner() == NULL);
}

TEST_F(ElementTest, setAndGetOwnerTest) {
  Package e;
  Package c;
  c.setOwningPackage(&e);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(c.getOwner()->getID() == e.getID());
}

TEST_F(ElementTest, getOwnedElementsBasicTest) {
  Package e;
  Package c;
  ASSERT_NO_THROW(e.getPackagedElements().add(c));
  ASSERT_TRUE(e.getOwnedElements().get(c.getID()));
}

TEST_F(ElementTest, getOwnedElementByNameTest) {
  Package e;
  Package n;
  n.setName("name");
  Package b;
  ASSERT_NO_THROW(e.getPackagedElements().add(b));
  ASSERT_NO_THROW(e.getPackagedElements().add(n));
  ASSERT_TRUE(e.getOwnedElements().get("name") == &n);
  ASSERT_TRUE(e.getOwnedElements().get(n.getID()) == &n);
  ASSERT_TRUE(e.getOwnedElements().get(b.getID()) == &b);
}

TEST_F(ElementTest, reIndexID_Test) {
  Package e1;
  Package e2;
  e1.getPackagedElements().add(e2);
  e2.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  ASSERT_TRUE(e1.getOwnedElements().get(e2.getID()) != NULL);
}

TEST_F(ElementTest, basicRelationshipTest) {
  Package e;
  Package a;
  Relationship r;
  r.getRelatedElements().add(e);
  r.getRelatedElements().add(a);
  ASSERT_TRUE(r.getRelatedElements().size() == 2);
  ASSERT_TRUE(r.getRelatedElements().front() == &e);
  ASSERT_TRUE(r.getRelatedElements().back() == &a);
  ASSERT_TRUE(e.getRelationships().size() == 1);
  ASSERT_TRUE(e.getRelationships().front() == &r);
  ASSERT_TRUE(a.getRelationships().size() == 1);
  ASSERT_TRUE(a.getRelationships().front() == &r);
}

TEST_F(ElementTest, reindexRelationshipID_test) {
  Element e;
  Element a;
  Relationship r;
  r.getRelatedElements().add(e);
  r.getRelatedElements().add(a);
  r.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  e.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
  ASSERT_TRUE(r.getRelatedElements().get(e.getID()));
  ASSERT_TRUE(e.getRelationships().get(r.getID()));
}

TEST_F(ElementTest, setOwnerFunctorTest) {
  Package e;
  Package c;
  e.getPackagedElements().add(c);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, setOwnerTest) {
  Package e;
  Package c;
  c.setOwningPackage(&e);
  ASSERT_TRUE(e.getOwnedElements().count(c.getID()));
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, overwriteOwnerTest) {
  Package p1;
  Package p2;
  Package c;
  p1.getPackagedElements().add(c);
  c.setOwningPackage(&p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTest) {
  Package p1;
  Package p2;
  Package c;
  p1.getPackagedElements().add(c);
  p2.getPackagedElements().add(c);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, CopyTest) {
  Package e1;
  Package c1;
  Package p1;
  e1.setOwningPackage(&p1);
  e1.getPackagedElements().add(c1);
  Element e2 = e1;
  ASSERT_TRUE(e2.getOwnedElements().size() == 1);
  ASSERT_TRUE(e2.getOwnedElements().front() == &c1);
  ASSERT_TRUE(e2.getID() == e1.getID());
  ASSERT_TRUE(e2.getOwner() == &p1);
}

TEST_F(ElementTest, doINeedAnAddRelationshipFunctorTest) { // answer is yes
  Package e;
  PackageMerge r;
  e.getPackageMerge().add(r);
  ASSERT_TRUE(r.getRelatedElements().size() == 1);
  ASSERT_TRUE(r.getRelatedElements().front() == &e);
}

// TEST_F(ElementTest, fullCopyTest) {
//   Package p;
//   Package c;
//   p.getPackagedElements().add(c);
//   Package p2 = p;
//   ASSERT_TRUE(p2.getPackagedElements().size() == 1);
//   ASSERT_TRUE(p2.getPackagedElements().front()->getID() == c.getID());
//   ASSERT_TRUE(p2.getPackagedElements().front() != &c);
// }

TEST_F(ElementTest, readOnlySequenceTest) {
  Package p;
  Package c1;
  Package c2;
  ASSERT_NO_THROW(p.getPackagedElements().add(c1));
  ASSERT_THROW(p.getOwnedElements().add(c2), ReadOnlySequenceException);
  ASSERT_THROW(p.getOwnedElements().remove(c1), ReadOnlySequenceException);
}
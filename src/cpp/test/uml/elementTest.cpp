#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/relationship.h"
#include "uml/package.h"

using namespace UML;

class ElementTest : public ::testing::Test {
    public:
    Element* el1;
    Element* el2;
    Element* el3;
    boost::uuids::uuid uuid;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementTest() {
     el1 = new Element;
     el2 = new Element;
     el3 = new Element;
  }

  ~ElementTest() override {
     delete el1;
     delete el2;
     delete el3;
  }

  void SetUp() override {
    // Override uuid on Element (not reccomended use of lib)
     uuid = boost::uuids::random_generator()();
     el1->setID(boost::lexical_cast<std::string>(uuid));

     // add element to owned element list
     el2->getOwnedElements().add(*el3);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementTest, OverrideID_Test) {
    EXPECT_EQ(el1->getID(), uuid);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  EXPECT_FALSE(el2->getOwnedElements().empty());
  EXPECT_EQ(el2->getOwnedElements().get(0), el3);
}

TEST_F(ElementTest, InvalidID_Test) {
  EXPECT_THROW(el3->setID("not a uuid4"), InvalidID_Exception);
  EXPECT_NO_THROW(el3->setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
}

TEST_F(ElementTest, getNullOwnerTest) {
  Element e;
  ASSERT_TRUE(e.getOwner() == NULL);
}

TEST_F(ElementTest, setAndGetOwnerTest) {
  Element e;
  Element c;
  c.setOwner(&e);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(c.getOwner()->getID() == e.getID());
}

TEST_F(ElementTest, getOwnedElementsBasicTest) {
  Element e;
  Element c;
  ASSERT_NO_THROW(e.getOwnedElements().add(c));
  ASSERT_TRUE(e.getOwnedElements().get(c.getID()));
}

TEST_F(ElementTest, getOwnedElementByNameTest) {
  Element e;
  NamedElement n;
  n.setName("name");
  Element b;
  ASSERT_NO_THROW(e.getOwnedElements().add(b));
  ASSERT_NO_THROW(e.getOwnedElements().add(n));
  ASSERT_TRUE(e.getOwnedElements().get("name") == &n);
  ASSERT_TRUE(e.getOwnedElements().get(n.getID()) == &n);
  ASSERT_TRUE(e.getOwnedElements().get(b.getID()) == &b);
}

TEST_F(ElementTest, reIndexID_Test) {
  Element e1;
  Element e2;
  e1.getOwnedElements().add(e2);
  e2.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  ASSERT_TRUE(e1.getOwnedElements().get(e2.getID()) != NULL);
}

TEST_F(ElementTest, basicRelationshipTest) {
  Element e;
  Element a;
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
  Element e;
  Element c;
  e.getOwnedElements().add(c);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, setOwnerTest) {
  Element e;
  Element c;
  c.setOwner(&e);
  ASSERT_TRUE(e.getOwnedElements().count(c.getID()));
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, overwriteOwnerTest) {
  Element p1;
  Element p2;
  Element c;
  p1.getOwnedElements().add(c);
  c.setOwner(&p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTest) {
  Element p1;
  Element p2;
  Element c;
  p1.getOwnedElements().add(c);
  p2.getOwnedElements().add(c);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, CopyTest) {
  Element e1;
  Element c1;
  Element p1;
  e1.setOwner(&p1);
  e1.getOwnedElements().add(c1);
  Element e2 = e1;
  ASSERT_TRUE(e2.getOwnedElements().size() == 1);
  ASSERT_TRUE(e2.getOwnedElements().front() == &c1);
  ASSERT_TRUE(e2.getID() == e1.getID());
  ASSERT_TRUE(e2.getOwner() == &p1);
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
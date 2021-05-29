#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/package.h"

using namespace UML;

class SequenceTest : public ::testing::Test {
   
};

TEST_F(SequenceTest, addGetAndRemoveElementTest) {
    Sequence<> seq;
    ASSERT_TRUE(seq.size() == 0);
    Element e;
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_TRUE(seq.get(e.getID()) == &e);
    ASSERT_TRUE(seq.size() == 1);
    seq.remove(e);
    ASSERT_TRUE(seq.size() == 0);
}

TEST_F(SequenceTest, addGetAndRemoveElementByNameTest) {
    Sequence<> seq;
    Element e;
    NamedElement n;
    n.setName("test");
    seq.add(e);
    seq.add(n);
    ASSERT_TRUE(seq.get("test") == &n);
    ASSERT_TRUE(seq.get(n.getID()) == &n);
    ASSERT_TRUE(seq.get(e.getID()) == &e);
}

TEST_F(SequenceTest, addElementTwiceTest) {
    Sequence<> seq;
    Element e;
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_TRUE(seq.get(e.getID()) == &e);
    ASSERT_TRUE(seq.size() == 2);
    ASSERT_TRUE(seq.get(0) == &e);
    ASSERT_TRUE(seq.get(1) == &e);
    ASSERT_NO_THROW(seq.remove(e));
    ASSERT_TRUE(seq.get(e.getID()) == NULL);
    ASSERT_TRUE(seq.size() == 0);
    ASSERT_TRUE(seq.empty());
}

TEST_F(SequenceTest, addNamedElementTwiceTest) {
    Sequence<> seq;
    NamedElement n;
    n.setName("name");
    ASSERT_NO_THROW(seq.add(n));
    ASSERT_NO_THROW(seq.add(n));
    ASSERT_TRUE(seq.get("name") == &n);
    ASSERT_TRUE(seq.get(n.getID()) == &n);
    ASSERT_NO_THROW(seq.remove(n));
    ASSERT_TRUE(seq.get("name") == NULL);
    ASSERT_TRUE(seq.get(n.getID()) == NULL);
}

TEST_F(SequenceTest, removeElementThatWasntAddedTest) {
    Sequence<> seq;
    Element e;
    ASSERT_THROW(seq.remove(e), ElementDoesntExistException);
}

TEST_F(SequenceTest, useAutoForLoop2) {
    Sequence<> seq;
    Element e;
    NamedElement f;
    Element b;
    Element c;
    seq.add(e);
    seq.add(f);
    seq.add(b);
    seq.add(c);
    for (auto const& e: seq) {
        ASSERT_TRUE(e != NULL);
    }
}

TEST_F(SequenceTest, getNonexistentElementByID_Test) {
    Package e;
    Package a;
    e.getPackagedElements().add(a);
    Package b;
    ASSERT_FALSE(e.getOwnedElements().get(b.getID()));
    ASSERT_TRUE(e.getOwnedElements().get(a.getID()));
}

TEST_F(SequenceTest, newSequenceTest) {
    UmlManager m;
    Sequence2<Package> s = m.createSequence<Package>();
    for (size_t i = 0; i < 100; i++) {
        s.add(m.create<Package>());
    }
    ASSERT_EQ(s.size(), 100);
    size_t i = 0;
    for (auto const& package: s) {
        i++;
    }
    ASSERT_EQ(i, 100);
}
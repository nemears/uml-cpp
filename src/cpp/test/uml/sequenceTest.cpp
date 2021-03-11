#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"

using namespace UML;

class SequenceTest : public ::testing::Test {
   
};

TEST_F(SequenceTest, addGetAndRemoveElementTest) {
    Sequence<> seq;
    ASSERT_TRUE(seq.size() == 0);
    Element e;
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_TRUE(seq.get(e.uuid) == &e);
    ASSERT_TRUE(seq.size() == 1);
    seq.remove(e);
    ASSERT_TRUE(seq.size() == 0);
}

TEST_F(SequenceTest, sequenceIteratorTest) {
    Sequence<> seq;
    Element e;
    Element f;
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_NO_THROW(seq.add(f));
    ASSERT_TRUE(seq.size() == 2);
    SequenceIterator<> it = seq.iterator();
    ASSERT_TRUE(it.hasNext());
    Element* first = it.getNext();
    ASSERT_TRUE(first == &e);
    ASSERT_TRUE(it.hasNext());
    Element* second = it.getNext();
    ASSERT_TRUE(second == &f);
    ASSERT_FALSE(it.hasNext());
}

TEST_F(SequenceTest, addGetAndRemoveElementByNameTest) {
    Sequence<> seq;
    Element e;
    NamedElement n;
    n.setName("test");
    seq.add(e);
    seq.add(n);
    ASSERT_TRUE(seq.get("test") == &n);
    ASSERT_TRUE(seq.get(n.uuid) == &n);
    ASSERT_TRUE(seq.get(e.uuid) == &e);
}

TEST_F(SequenceTest, addElementTwiceTest) {
    Sequence<> seq;
    Element e;
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_THROW(seq.add(e), ElementAlreadyExistsException);
}

TEST_F(SequenceTest, removeElementThatWasntAddedTest) {
    Sequence<> seq;
    Element e;
    ASSERT_THROW(seq.remove(e), ElementDoesntExistException);
}
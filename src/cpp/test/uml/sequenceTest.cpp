#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/package.h"
#include "uml/slot.h"
#include "uml/specialSequence.h"

using namespace UML;

class SequenceTest : public ::testing::Test {
   
};

TEST_F(SequenceTest, addGetAndRemoveElementTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& seq = p.getPackagedElements();
    ASSERT_TRUE(seq.size() == 0);
    Package e = m.create<Package>();
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_TRUE(seq.get(e.getID()) == e);
    ASSERT_TRUE(seq.size() == 1);
    seq.remove(e);
    ASSERT_TRUE(seq.size() == 0);
}

TEST_F(SequenceTest, addGetAndRemoveElementByNameTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& seq = p.getPackagedElements();
    Package& e = m.create<Package>();
    Package n = m.create<Package>();
    n.setName("test");
    seq.add(e);
    seq.add(n);
    ASSERT_TRUE(seq.get("test") == n);
    ASSERT_TRUE(seq.get(n.getID()) == n);
    ASSERT_TRUE(seq.get(e.getID()) == e);
}

TEST_F(SequenceTest, setNameLaterTest) {
    UmlManager m;
    Package& o = m.create<Package>();
    Package& p = m.create<Package>();
    o.getPackagedElements().add(p);
    ASSERT_THROW(o.getOwnedElements().get("test"), ID_doesNotExistException);
    p.setName("test");
    ASSERT_NO_THROW(o.getOwnedElements().get("test"));
    ASSERT_EQ(o.getOwnedElements().get("test").getID(), p.getID());
}

TEST_F(SequenceTest, addElementTwiceTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& seq = p.getPackagedElements();
    Package e = m.create<Package>();
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_NO_THROW(seq.add(e));
    ASSERT_TRUE(seq.get(e.getID()) == e);
    ASSERT_TRUE(seq.size() == 2);
    ASSERT_TRUE(seq.get(0) == e);
    ASSERT_TRUE(seq.get(1) == e);
    ASSERT_NO_THROW(seq.remove(e));
    ASSERT_THROW(seq.get(e.getID()), ID_doesNotExistException);
    ASSERT_TRUE(seq.size() == 0);
    ASSERT_TRUE(seq.empty());
}

// TEST_F(SequenceTest, addNamedElementTwiceTest) {
//     Sequence<> seq;
//     NamedElement n;
//     n.setName("name");
//     ASSERT_NO_THROW(seq.add(n));
//     ASSERT_NO_THROW(seq.add(n));
//     ASSERT_TRUE(seq.get("name") == &n);
//     ASSERT_TRUE(seq.get(n.getID()) == &n);
//     ASSERT_NO_THROW(seq.remove(n));
//     ASSERT_TRUE(seq.get("name") == NULL);
//     ASSERT_TRUE(seq.get(n.getID()) == NULL);
// }

TEST_F(SequenceTest, removeElementThatWasntAddedTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& seq = p.getPackagedElements();
    Package e = m.create<Package>();
    ASSERT_THROW(seq.remove(e), ElementDoesntExistException);
}

TEST_F(SequenceTest, useAutoForLoop2) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& seq = p.getPackagedElements();
    Package& e = m.create<Package>();
    Package& f = m.create<Package>();
    Package& b = m.create<Package>();
    Package& c = m.create<Package>();
    seq.add(e);
    seq.add(f);
    seq.add(b);
    seq.add(c);
    for (auto& e: seq) {
        ASSERT_TRUE(!e.getID().isNull());
    }
}

TEST_F(SequenceTest, getNonexistentElementByID_Test) {
    UmlManager m;
    Package e = m.create<Package>();
    Package a = m.create<Package>();
    e.getPackagedElements().add(a);
    Package b = m.create<Package>();
    ASSERT_THROW(e.getOwnedElements().get(b.getID()), ID_doesNotExistException);
    ASSERT_NO_THROW(e.getOwnedElements().get(a.getID()));
}

TEST_F(SequenceTest, newSequenceTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Sequence<PackageableElement>& s = p.getPackagedElements();
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

TEST_F(SequenceTest, variardicAddTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Package& c1 = m.create<Package>();
    Package& c2 = m.create<Package>();
    Package& c3 = m.create<Package>();
    ASSERT_NO_THROW(p.getPackagedElements().add(c1, c2, c3));
    ASSERT_EQ(p.getPackagedElements().size(), 3);
    ASSERT_EQ(p.getPackagedElements().front().getID(), c1.getID());
    ASSERT_EQ(p.getPackagedElements().get(1).getID(), c2.getID());
    ASSERT_EQ(p.getPackagedElements().back().getID(), c3.getID());
    ASSERT_NO_THROW(p.getPackagedElements().remove(c1, c2, c3));
    ASSERT_TRUE(p.getPackagedElements().empty());
}

TEST_F(SequenceTest, basicSpecialSequenceTest) {
    size_t numPackages = 20;
    SpecialSequence<Package> seq;
    UmlManager m;
    Package& pckg = m.create<Package>();
    seq.add(pckg);
    ASSERT_EQ(seq.get(pckg.getID()), pckg);
    std::vector<ID> ids(numPackages);
    ids[0] = pckg.getID();
    for (int i = 0; i < numPackages - 1; i++) {
        Package& p = m.create<Package>();
        ids[i+1] = p.getID();
        seq.add(p);
    }
    for (const ID id : ids) {
        ASSERT_TRUE(seq.contains(id));
    }
}

TEST_F(SequenceTest, basicSubsetsTest) {
    SpecialSequence<PackageableElement> rootSeq;
    SpecialSequence<Package> subSeq;
    
}
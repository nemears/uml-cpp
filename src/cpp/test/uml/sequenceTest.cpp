#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/package.h"
#include "uml/slot.h"
#include "uml/set.h"
#include "uml/class.h"
#include "uml/instanceSpecification.h"

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

TEST_F(SequenceTest, basicSetTest) {
    size_t numPackages = 20;
    Set<Package> seq;
    ASSERT_TRUE(seq.empty());
    UmlManager m;
    Package& pckg = m.create<Package>();
    seq.add(pckg);
    ASSERT_FALSE(seq.empty());
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
        ASSERT_EQ(seq.get(id).getID(), id);
    }
}

TEST_F(SequenceTest, basicRemoveTest) {
    Set<Package> seq;
    UmlManager m;
    const size_t constNumPackages = 8;
    size_t numPackages = constNumPackages;
    std::vector<ID> packages;
    for (int i = 0; i < numPackages; i++) {
        Package& temp = m.create<Package>();
        packages.push_back(temp.getID());
        seq.add(temp);
    }
    for (int i = 0; i < constNumPackages / 2; i++) {
        int index = rand() % numPackages;
        seq.remove(packages[index]);
        packages.erase(std::remove(packages.begin(), packages.end(), packages[index]), packages.end()) - packages.begin();
        numPackages--;
    }
    ASSERT_EQ(seq.size(), constNumPackages/2);
    for (int i = 0; i < constNumPackages/ 2; i++) {
        int index = rand() % numPackages;
        seq.remove(packages[index]);
        packages.erase(std::remove(packages.begin(), packages.end(), packages[index]), packages.end()) - packages.begin();
        numPackages--;
    }
    ASSERT_EQ(seq.size(), 0);
}

TEST_F(SequenceTest, basicSubsetsTest) {
    Set<PackageableElement> rootSeq;
    Set<Package> subSeq;
    subSeq.subsets(rootSeq);
    UmlManager m;
    Package& pckg = m.create<Package>();
    subSeq.add(pckg);
    ASSERT_EQ(subSeq.size(), 1);
    ASSERT_TRUE(subSeq.contains(pckg.getID()));
    ASSERT_EQ(subSeq.get(pckg.getID()), pckg);
    ASSERT_EQ(rootSeq.size(), 1);
    ASSERT_TRUE(rootSeq.contains(pckg.getID()));
    ASSERT_EQ(rootSeq.get(pckg.getID()), pckg);
    Class& clazz = m.create<Class>();
    rootSeq.add(clazz);
    ASSERT_EQ(rootSeq.size(), 2);
    ASSERT_TRUE(rootSeq.contains(clazz.getID()));
    ASSERT_EQ(rootSeq.get(clazz.getID()), clazz);
    ASSERT_EQ(subSeq.size(), 1);
    ASSERT_FALSE(subSeq.contains(clazz.getID()));
    ASSERT_THROW(subSeq.get(clazz.getID()), ID_doesNotExistException2);
}

TEST_F(SequenceTest, multiSubsetsTest) {
    Set<NamedElement> seq1;
    Set<PackageableElement> seq2;
    Set<Package> subSeq;
    subSeq.subsets(seq2);
    subSeq.subsets(seq1);
    UmlManager m;
    Class& clazz = m.create<Class>();
    seq1.add(clazz);
    ASSERT_FALSE(seq1.empty());
    ASSERT_EQ(seq1.size(), 1);
    ASSERT_TRUE(seq1.contains(clazz.getID()));
    ASSERT_TRUE(seq2.empty());
    ASSERT_EQ(seq2.size(), 0);
    ASSERT_FALSE(seq2.contains(clazz.getID()));
    ASSERT_TRUE(subSeq.empty());
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(clazz.getID()));
    InstanceSpecification& inst = m.create<InstanceSpecification>();
    seq2.add(inst);
    ASSERT_FALSE(seq1.empty());
    ASSERT_EQ(seq1.size(), 1);
    ASSERT_FALSE(seq1.contains(inst.getID()));
    ASSERT_FALSE(seq2.empty());
    ASSERT_EQ(seq2.size(), 1);
    ASSERT_TRUE(seq2.contains(inst.getID()));
    ASSERT_TRUE(subSeq.empty());
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(inst.getID()));
    Package& pckg = m.create<Package>();
    subSeq.add(pckg);
    ASSERT_FALSE(seq1.empty());
    ASSERT_EQ(seq1.size(), 2);
    ASSERT_TRUE(seq1.contains(pckg.getID()));
    ASSERT_FALSE(seq2.empty());
    ASSERT_EQ(seq2.size(), 2);
    ASSERT_TRUE(seq2.contains(pckg.getID()));
    ASSERT_FALSE(subSeq.empty());
    ASSERT_EQ(subSeq.size(), 1);
    ASSERT_TRUE(subSeq.contains(pckg.getID()));
}

TEST_F(SequenceTest, removeFromSubsettedSequenceTest) {
    Set<PackageableElement> rootSeq;
    Set<Package> subSeq;
    subSeq.subsets(rootSeq);
    UmlManager m;
    Package& pckg = m.create<Package>();
    subSeq.add(pckg);
    subSeq.remove(pckg);
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_EQ(rootSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(pckg.getID()));
    ASSERT_FALSE(rootSeq.contains(pckg.getID()));
    subSeq.add(pckg);
    rootSeq.remove(pckg);
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_EQ(rootSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(pckg.getID()));
    ASSERT_FALSE(rootSeq.contains(pckg.getID()));
}

TEST_F(SequenceTest, specialAutoForLoop) {
    Set<Package> seq;
    int numPackages = 10;
    UmlManager m;
    for (int i = 0; i < numPackages; i++) {
        Package& p = m.create<Package>(); 
        seq.add(p);
        std::hash<ID> hasher;
        std::cout << p.getID().string() << ", hash: " << hasher(p.getID()) << std::endl;
    }
    std::cout << "=========================================================" << std::endl;
    int i = 0;
    for (auto& p : seq) {
        i++;
        ASSERT_EQ(p.getElementType(), ElementType::PACKAGE);
        std::hash<ID> hasher;
        std::cout << p.getID().string() << ", hash: " << hasher(p.getID()) << std::endl;
        if (i > numPackages + 10) break;
    }
    ASSERT_EQ(i, 10);
}

TEST_F(SequenceTest, getFromSetByNameTest) {
    Set<PackageableElement> rootSeq;
    Set<Package> subSeq;
    subSeq.subsets(rootSeq);
    UmlManager m;
    Package& one = m.create<Package>();
    Class& two = m.create<Class>();
    one.setName("1");
    two.setName("2");
    subSeq.add(one);
    rootSeq.add(two);
    ASSERT_TRUE(rootSeq.contains("1"));
    ASSERT_TRUE(rootSeq.contains("2"));
    ASSERT_TRUE(subSeq.contains("1"));
    ASSERT_FALSE(subSeq.contains("2"));
    ASSERT_EQ(rootSeq.get("1"), one);
    ASSERT_EQ(rootSeq.get("2"), two);
    ASSERT_EQ(subSeq.get("1"), one);
    ASSERT_THROW(subSeq.get("2"), ManagerStateException);
}

TEST_F(SequenceTest, addToSetTwice) {
    Set<Package> set;
    UmlManager m;
    Package& p = m.create<Package>();
    set.add(p);
    ASSERT_THROW(set.add(p), DuplicateElementInSetException);
}

class TestElement : public Element {
    private:
        Set<TestElement, TestElement> m_others = Set<TestElement, TestElement>(this);
    public:
        TestElement() : Element(ElementType::ELEMENT) {
            m_others.opposite(&TestElement::getOthers);
        };
        Set<TestElement, TestElement>& getOthers() { return m_others; };
};

TEST_F(SequenceTest, oppositeTest) {
    TestElement t1;
    TestElement t2;
    t1.getOthers().add(t2);
    ASSERT_EQ(t2.getOthers().size(), 1);
    ASSERT_TRUE(t2.getOthers().contains(t1.getID()));
    ASSERT_EQ(t2.getOthers().get(t1.getID()), t1);
}

TEST_F(SequenceTest, setRedefinesTest) {
    Set<Package> ogSet;
    Set<Package> reSet;
    reSet.redefines(ogSet);
    UmlManager m;
    Package& p = m.create<Package>();
    reSet.add(p);
    ASSERT_FALSE(ogSet.empty());
    ASSERT_FALSE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 1);
    ASSERT_EQ(reSet.size(), 1);
    reSet.remove(p);
    ASSERT_TRUE(ogSet.empty());
    ASSERT_TRUE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 0);
    ASSERT_EQ(reSet.size(), 0);
    Package& p2 = m.create<Package>();
    ogSet.add(p);
    ASSERT_FALSE(ogSet.empty());
    ASSERT_FALSE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 1);
    ASSERT_EQ(reSet.size(), 1);
    ogSet.remove(p);
    ASSERT_TRUE(ogSet.empty());
    ASSERT_TRUE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 0);
    ASSERT_EQ(reSet.size(), 0);
    ogSet.add(p);
    reSet.add(p2);
    ASSERT_FALSE(ogSet.empty());
    ASSERT_FALSE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 2);
    ASSERT_EQ(reSet.size(), 2);
    Package& p3 = m.create<Package>();
    ogSet.add(p3);
    ASSERT_FALSE(ogSet.empty());
    ASSERT_FALSE(reSet.empty());
    ASSERT_EQ(ogSet.size(), 3);
    ASSERT_EQ(reSet.size(), 3);
}
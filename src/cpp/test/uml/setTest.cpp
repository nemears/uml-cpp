#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/package.h"
#include "uml/set.h"
#include "uml/orderedSet.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

using namespace UML;

class SetTest : public ::testing::Test {
   
};

TEST_F(SetTest, basicSetTest) {
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

TEST_F(SetTest, basicRemoveTest) {
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

TEST_F(SetTest, basicSubsetsTest) {
    Set<PackageableElement>* rootSeq = new Set<PackageableElement>();
    Set<Package>* subSeq = new Set<Package>();
    subSeq->subsets(*rootSeq);
    UmlManager m;
    Package& pckg = m.create<Package>();
    subSeq->add(pckg);
    ASSERT_EQ(subSeq->size(), 1);
    ASSERT_TRUE(subSeq->contains(pckg.getID()));
    ASSERT_EQ(subSeq->get(pckg.getID()), pckg);
    ASSERT_EQ(rootSeq->size(), 1);
    ASSERT_TRUE(rootSeq->contains(pckg.getID()));
    ASSERT_EQ(rootSeq->get(pckg.getID()), pckg);
    Package& clazz = m.create<Package>();
    rootSeq->add(clazz);
    ASSERT_EQ(rootSeq->size(), 2);
    ASSERT_TRUE(rootSeq->contains(clazz.getID()));
    ASSERT_EQ(rootSeq->get(clazz.getID()), clazz);
    ASSERT_EQ(subSeq->size(), 1);
    ASSERT_FALSE(subSeq->contains(clazz.getID()));
    ASSERT_THROW(subSeq->get(clazz.getID()), ID_doesNotExistException2);
    delete subSeq;
    delete rootSeq;
}

TEST_F(SetTest, multiSubsetsTest) {
    Set<NamedElement> seq1;
    Set<PackageableElement> seq2;
    Set<Package> subSeq;
    subSeq.subsets(seq2);
    subSeq.subsets(seq1);
    UmlManager m;
    Package& clazz = m.create<Package>();
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
    Package& inst = m.create<Package>();
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

TEST_F(SetTest, removeFromSubsettedSequenceTest) {
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

TEST_F(SetTest, specialAutoForLoop) {
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

TEST_F(SetTest, getFromSetByNameTest) {
    Set<PackageableElement> rootSeq;
    Set<Package> subSeq;
    subSeq.subsets(rootSeq);
    UmlManager m;
    Package& one = m.create<Package>();
    Package& two = m.create<Package>();
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

TEST_F(SetTest, addToSetTwice) {
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

TEST_F(SetTest, oppositeTest) {
    UmlManager m;
    TestElement t1;
    TestElement t2;
    t1.getOthers().add(t2);
    ASSERT_EQ(t2.getOthers().size(), 1);
    ASSERT_TRUE(t2.getOthers().contains(t1.getID()));
    ASSERT_EQ(t2.getOthers().get(t1.getID()), t1);
}

TEST_F(SetTest, setRedefinesTest) {
    Set<Package>* ogSet = new Set<Package>;
    Set<Package>* reSet = new Set<Package>;
    reSet->redefines(*ogSet);
    UmlManager m;
    Package& p = m.create<Package>();
    reSet->add(p);
    ASSERT_FALSE(ogSet->empty());
    ASSERT_FALSE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 1);
    ASSERT_EQ(reSet->size(), 1);
    reSet->remove(p);
    ASSERT_TRUE(ogSet->empty());
    ASSERT_TRUE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 0);
    ASSERT_EQ(reSet->size(), 0);
    Package& p2 = m.create<Package>();
    ogSet->add(p);
    ASSERT_FALSE(ogSet->empty());
    ASSERT_FALSE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 1);
    ASSERT_EQ(reSet->size(), 1);
    ogSet->remove(p);
    ASSERT_TRUE(ogSet->empty());
    ASSERT_TRUE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 0);
    ASSERT_EQ(reSet->size(), 0);
    ogSet->add(p);
    reSet->add(p2);
    ASSERT_FALSE(ogSet->empty());
    ASSERT_FALSE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 2);
    ASSERT_EQ(reSet->size(), 2);
    Package& p3 = m.create<Package>();
    ogSet->add(p3);
    ASSERT_FALSE(ogSet->empty());
    ASSERT_FALSE(reSet->empty());
    ASSERT_EQ(ogSet->size(), 3);
    ASSERT_EQ(reSet->size(), 3);
    delete reSet;
    delete ogSet;
}

class gFunc : public SetFunctor {
    private:
        void operator()(Element& el) const override {
            std::cout << "gFunc!" << std::endl;
        };
    public:
        gFunc(Element* el) : SetFunctor(el) {};
};

TEST_F(SetTest, setRedefinedWFunctors) {
    Set<Package>* oSet = new Set<Package>;
    Set<Package>* rSet = new Set<Package>;
    UmlManager m;
    Package& g = m.create<Package>();
    oSet->addFunctor(new gFunc(&g));
    rSet->redefines(*oSet);
    rSet->add(g);
    delete rSet;
    delete oSet;
}

TEST_F(SetTest, addToOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = m.create<Package>();
    p.setName("1");
    set.add(p);
    ASSERT_EQ(set.front(), p);
    ASSERT_EQ(set.back(), p);
    Package& p2 = m.create<Package>();
    p2.setName("2");
    set.add(p2);
    ASSERT_EQ(set.front(), p);
    ASSERT_EQ(set.back(), p2);
    Package& p3 = m.create<Package>();
    p3.setName("3");
    set.add(p3);
    ASSERT_EQ(set.front(), p);
    ASSERT_EQ(set.back(), p3);
    ASSERT_TRUE(set.contains(p.getID()));
    ASSERT_TRUE(set.contains("1"));
    ASSERT_EQ(set.get(p.getID()), p);
    ASSERT_EQ(set.get("1"), p);
    ASSERT_TRUE(set.contains(p2.getID()));
    ASSERT_TRUE(set.contains("2"));
    ASSERT_EQ(set.get(p2.getID()), p2);
    ASSERT_EQ(set.get("2"), p2);
    ASSERT_TRUE(set.contains(p3.getID()));
    ASSERT_TRUE(set.contains("3"));
    ASSERT_EQ(set.get(p3.getID()), p3);
    ASSERT_EQ(set.get("3"), p3);
    Package* ps[] = {&p, &p2, &p3};
    int i = 0;
    for (Package& pckg : set) {
        ASSERT_EQ(*ps[i], pckg);
        i++;
    }
    ASSERT_EQ(set.get(0), p);
    ASSERT_EQ(set.get(1), p2);
    ASSERT_EQ(set.get(2), p3);
}

TEST_F(SetTest, subsetOrderedSets) {
    OrderedSet<PackageableElement> rootSet;
    OrderedSet<Package> subSet;
    subSet.subsets(rootSet);
    UmlManager m;
    Package& p = m.create<Package>();
    subSet.add(p);
    ASSERT_EQ(subSet.front(), p);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_EQ(rootSet.front(), p);
    Package& c = m.create<Package>();
    rootSet.add(c);
    ASSERT_FALSE(subSet.contains(c.getID()));
    ASSERT_TRUE(rootSet.contains(c.getID()));
    ASSERT_EQ(rootSet.back(), c);
    int i = 0;
    for (Package& pckg : subSet) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (PackageableElement& pckg : rootSet) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

TEST_F(SetTest, orderedSetSubSetsSet) {
    Set<PackageableElement> rootSet;
    OrderedSet<Package> subSet;
    subSet.subsets(rootSet);
    UmlManager m;
    Package& p = m.create<Package>();
    subSet.add(p);
    ASSERT_EQ(subSet.front(), p);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_EQ(rootSet.get(p.getID()), p);
    Package& c = m.create<Package>();
    rootSet.add(c);
    ASSERT_FALSE(subSet.contains(c.getID()));
    ASSERT_TRUE(rootSet.contains(c.getID()));
    ASSERT_EQ(rootSet.get(c.getID()), c);
    int i = 0;
    for (Package& pckg : subSet) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (PackageableElement& pckg : rootSet) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

TEST_F(SetTest, testIdsMethodLoop) {
    Set<Package> set;
    UmlManager m;
    int numPackages = 10;
    std::unordered_set<ID> ids;
    for (int i = 0; i < numPackages; i++) {
        Package& p = m.create<Package>();
        set.add(p);
        ids.insert(p.getID());
    }
    for (const ID id : set.ids()) {
        ASSERT_TRUE(ids.count(id));
    }
}

TEST_F(SetTest, singletonTest) {
    Set<PackageableElement> rootSet;
    Singleton<Package> singleton;
    UmlManager m;
    singleton.subsets(rootSet);
    ASSERT_TRUE(singleton.empty());
    ASSERT_EQ(singleton.size(), 0);
    Package& p = m.create<Package>();
    singleton.set(p);
    ASSERT_EQ(singleton.size(), 1);
    ASSERT_FALSE(singleton.empty());
    ASSERT_TRUE(singleton.has());
    ASSERT_EQ(singleton.getRef(), p);
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.get(p.getID()), p);
    Package& c = m.create<Package>();
    rootSet.add(c);
    ASSERT_EQ(singleton.size(), 1);
    ASSERT_FALSE(singleton.empty());
    ASSERT_TRUE(singleton.has());
    ASSERT_EQ(singleton.getRef(), p);
    ASSERT_EQ(rootSet.size(), 2);
    ASSERT_TRUE(rootSet.contains(p.getID()));
    ASSERT_TRUE(rootSet.contains(c.getID()));
    ASSERT_EQ(rootSet.get(p.getID()), p);
    ASSERT_EQ(rootSet.get(c.getID()), c);
    singleton.set(0);
    ASSERT_FALSE(singleton.has());
    ASSERT_FALSE(singleton.get());
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_TRUE(rootSet.contains(c.getID()));
    ASSERT_FALSE(rootSet.contains(p.getID()));
}

TEST_F(SetTest, sharedSubsetEvenTreeTest) {
    Set<PackageableElement> superSet;
    Set<Package> set1;
    Set<Package> set2;
    set1.subsets(superSet);
    set2.subsets(superSet);
    UmlManager m;
    Package& p1 = m.create<Package>();
    Package& p2 = m.create<Package>();
    set1.add(p1);
    set2.add(p2);
    ASSERT_FALSE(set1.contains(p2.getID()));
    ASSERT_EQ(set1.front(), p1);
    ASSERT_EQ(set2.front(), p2);
    for (auto& el : superSet) {
        ASSERT_TRUE(superSet.contains(el.getID()));
    }
    for (const ID id : superSet.ids()) {
        ASSERT_TRUE(superSet.contains(id));
    }
    set1.remove(p1);
    ASSERT_EQ(set2.size(), 1);
    ASSERT_EQ(superSet.size(), 1);
    ASSERT_EQ(superSet.front(), p2);
    ASSERT_EQ(set2.front(), p2);
    set1.add(p1);
    ASSERT_FALSE(set1.contains(p2.getID()));
    ASSERT_EQ(set1.front(), p1);
    ASSERT_EQ(set2.front(), p2);
    for (auto& el : superSet) {
        ASSERT_TRUE(superSet.contains(el.getID()));
    }
    for (const ID id : superSet.ids()) {
        ASSERT_TRUE(superSet.contains(id));
    }
    set2.remove(p2);
    ASSERT_EQ(set1.size(), 1);
    ASSERT_EQ(superSet.size(), 1);
    ASSERT_EQ(superSet.front(), p1);
    ASSERT_EQ(set1.front(), p1);
}

TEST_F(SetTest, multiRootWithinRootTest) {
    Set<NamedElement>* r1 = new Set<NamedElement>();
    Set<PackageableElement>* r2 = new Set<PackageableElement>();
    Set<Package>* subSet = new Set<Package>();
    UmlManager m;
    subSet->subsets(*r1);
    subSet->subsets(*r2);
    Class& c = m.create<Class>();
    Package& p = m.create<Package>();
    r1->add(c);
    subSet->add(p);
    ASSERT_EQ(r1->size(), 2);
    ASSERT_EQ(r2->size(), 1);
    ASSERT_EQ(subSet->size(), 1);
    ASSERT_TRUE(r1->contains(c.getID()));
    ASSERT_FALSE(r2->contains(c.getID()));
    ASSERT_FALSE(subSet->contains(c.getID()));
    ASSERT_TRUE(r1->contains(p.getID()));
    ASSERT_TRUE(r2->contains(p.getID()));
    ASSERT_TRUE(subSet->contains(p.getID()));
    delete subSet;
    delete r2;
    delete r1;
}

TEST_F(SetTest, multiSubsetsOneElement) {
    Set<NamedElement>* r1 = new Set<NamedElement>();
    Set<PackageableElement>* r2 = new Set<PackageableElement>();
    Set<Package>* subSet = new Set<Package>();
    UmlManager m;
    subSet->subsets(*r1);
    subSet->subsets(*r2);
    Package& p = m.create<Package>();
    subSet->add(p);
    ASSERT_EQ(subSet->size(), 1);
    ASSERT_EQ(r2->size(), 1);
    ASSERT_EQ(r1->size(), 1);
    ASSERT_TRUE(subSet->contains(p.getID()));
    ASSERT_TRUE(r2->contains(p.getID()));
    ASSERT_TRUE(r1->contains(p.getID()));
    delete subSet;
    delete r2;
    delete r1;
}

TEST_F(SetTest, twoWayMultiSetSplitTest) {
    Set<Element>* rootSet = new Set<Element>();
    Set<NamedElement>* rightSet1 = new Set<NamedElement>();
    Set<NamedElement>* leftSet1 = new Set<NamedElement>();
    Set<PackageableElement>* rightSet2 = new Set<PackageableElement>();
    Set<PackageableElement>* leftSet2 = new Set<PackageableElement>();
    rightSet1->subsets(*rootSet);
    leftSet1->subsets(*rootSet);
    rightSet2->subsets(*rightSet1);
    leftSet2->subsets(*leftSet1);
    UmlManager m;
    Package& rightP = m.create<Package>();
    Package& leftP = m.create<Package>();
    rightSet2->add(rightP);
    leftSet2->add(leftP);
    ASSERT_EQ(rightSet2->size(), 1);
    ASSERT_EQ(rightSet1->size(), 1);
    ASSERT_EQ(leftSet2->size(), 1);
    ASSERT_EQ(leftSet1->size(), 1);
    ASSERT_EQ(rootSet->size(), 2);
    ASSERT_TRUE(rightSet2->contains(rightP.getID()));
    ASSERT_TRUE(rightSet1->contains(rightP.getID()));
    ASSERT_TRUE(rootSet->contains(rightP.getID()));
    ASSERT_TRUE(leftSet1->contains(leftP.getID()));
    ASSERT_TRUE(leftSet2->contains(leftP.getID()));
    ASSERT_TRUE(rootSet->contains(leftP.getID()));
    delete leftSet2;
    delete rightSet2;
    delete leftSet1;
    delete rightSet1;
    delete rootSet;
    // ASSERT_FALSE(rootRoot->m_id == ID::fromString("&&&&&&&&&&&&&&&&&&&&&&&&&&&&"));
}

TEST_F(SetTest, TODO_NAME) {
    Set<Element>* rootSet = new Set<Element>;
    Set<NamedElement>* superSet3 = new Set<NamedElement>;
    Set<NamedElement>* superSet2 = new Set<NamedElement>;
    Set<PackageableElement>* superSet1 = new Set<PackageableElement>;
    OrderedSet<Package>* set1 = new OrderedSet<Package>;
    OrderedSet<Package>* set2 = new OrderedSet<Package>;
    superSet2->subsets(*rootSet);
    superSet2->subsets(*superSet3);
    superSet1->subsets(*superSet3);
    set1->subsets(*superSet2);
    set1->subsets(*superSet1);
    set2->subsets(*superSet2);
    set2->subsets(*superSet1);
    UmlManager m;
    Package& p1 = m.create<Package>();
    Package& p2 = m.create<Package>();
    set1->add(p1);
    set2->add(p2);
    ASSERT_TRUE(superSet1->contains(p1.getID()));
    ASSERT_TRUE(superSet2->contains(p1.getID()));
    ASSERT_TRUE(superSet3->contains(p1.getID()));
    ASSERT_TRUE(rootSet->contains(p1.getID()));
    ASSERT_TRUE(superSet1->contains(p2.getID()));
    ASSERT_TRUE(superSet2->contains(p2.getID()));
    ASSERT_TRUE(superSet3->contains(p2.getID()));
    ASSERT_TRUE(rootSet->contains(p2.getID()));
    delete set2;
    delete set1;
    delete superSet1;
    delete superSet2;
    delete superSet3;
    delete rootSet;
}

TEST_F(SetTest, AddElementThatIsInSuperSet) {
    Set<PackageableElement> superSet;
    Set<Package> set;
    set.subsets(superSet);
    UmlManager m;
    Package& p = m.create<Package>();
    superSet.add(p);
    ASSERT_FALSE(superSet.empty());
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(superSet.size(), 1);
    ASSERT_EQ(set.size(), 0);
    ASSERT_TRUE(superSet.contains(p.getID()));
    ASSERT_FALSE(set.contains(p.getID()));
    set.add(p);
    ASSERT_FALSE(superSet.empty());
    ASSERT_FALSE(set.empty());
    ASSERT_EQ(superSet.size(), 1);
    ASSERT_EQ(set.size(), 1);
    ASSERT_TRUE(superSet.contains(p.getID()));
    ASSERT_TRUE(set.contains(p.getID()));
}

TEST_F(SetTest, subsetAddsFromRootSet) {
    Set<PackageableElement>* set = new Set<PackageableElement>;
    Set<Package>* subSet = new Set<Package>();
    subSet->subsets(*set);
    UmlManager m;
    Package& p1 = m.create<Package>();
    Package& p2 = m.create<Package>();
    Package& p3 = m.create<Package>();
    set->add(p1);
    set->add(p2);
    set->add(p3);
    ASSERT_FALSE(set->empty());
    ASSERT_TRUE(subSet->empty());
    ASSERT_EQ(set->size(), 3);
    ASSERT_EQ(subSet->size(), 0);
    subSet->add(p1);
    ASSERT_FALSE(subSet->empty());
    ASSERT_EQ(subSet->size(), 1);
    ASSERT_EQ(set->size(), 3);
    subSet->add(p3);
    ASSERT_EQ(subSet->size(), 2);
    ASSERT_EQ(set->size(), 3);
    subSet->add(p2);
    ASSERT_EQ(subSet->size(), 3);
    ASSERT_EQ(set->size(), 3);
    delete subSet;
    delete set;
}

TEST_F(SetTest, removeFirstElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = m.create<Package>();
    set.add(p);
    set.remove(p);
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(set.size(), 0);
    ASSERT_THROW(set.front(), ManagerStateException);
}

TEST_F(SetTest, removeLastElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = m.create<Package>();
    set.add(p);
    Package& p2 = m.create<Package>();
    set.add(p2);
    set.remove(p2);
    ASSERT_EQ(set.back(), p);
    ASSERT_FALSE(set.contains(p2.getID()));
    ASSERT_EQ(set.size(), 1);
}

TEST_F(SetTest, removeMiddleElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = m.create<Package>();
    set.add(p);
    Package& p2 = m.create<Package>();
    set.add(p2);
    Package& p3 = m.create<Package>();
    set.add(p3);
    set.remove(p2);
    OrderedSetIterator it = set.begin();
    it++;
    ASSERT_EQ(*it, p3);
}
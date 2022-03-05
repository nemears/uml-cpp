#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/package.h"
#include "uml/set.h"
#include "uml/orderedSet.h"
#include "test/yumlParsersTest.h"
#include "uml/umlPtr.h"
#include "uml/class.h"
#include "uml/generalization.h"
#include "uml/property.h"
#include "uml/dataType.h"

#include "uml/uml-stable.h"

using namespace UML;

class SetTest : public ::testing::Test {
   
};

TEST_F(SetTest, basicSetTest) {
    size_t numPackages = 20;
    Set<Package> seq;
    ASSERT_TRUE(seq.empty());
    UmlManager m;
    Package& pckg = *m.create<Package>();
    seq.add(pckg);
    ASSERT_FALSE(seq.empty());
    ASSERT_EQ(seq.get(pckg.getID()), pckg);
    std::vector<ID> ids(numPackages);
    ids[0] = pckg.getID();
    for (int i = 0; i < numPackages - 1; i++) {
        Package& p = *m.create<Package>();
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
        Package& temp = *m.create<Package>();
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
    Package& pckg = *m.create<Package>();
    subSeq->add(pckg);
    ASSERT_EQ(subSeq->size(), 1);
    ASSERT_TRUE(subSeq->contains(pckg.getID()));
    ASSERT_EQ(subSeq->get(pckg.getID()), pckg);
    ASSERT_EQ(rootSeq->size(), 1);
    ASSERT_TRUE(rootSeq->contains(pckg.getID()));
    ASSERT_EQ(rootSeq->get(pckg.getID()), pckg);
    Package& clazz = *m.create<Package>();
    rootSeq->add(clazz);
    ASSERT_EQ(rootSeq->size(), 2);
    ASSERT_TRUE(rootSeq->contains(clazz.getID()));
    ASSERT_EQ(rootSeq->get(clazz.getID()), clazz);
    ASSERT_EQ(subSeq->size(), 1);
    ASSERT_FALSE(subSeq->contains(clazz.getID()));
    ASSERT_THROW(subSeq->get(clazz.getID()), ID_doesNotExistException);
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
    Package& clazz = *m.create<Package>();
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
    Package& inst = *m.create<Package>();
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
    Package& pckg = *m.create<Package>();
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
    Set<NamedElement> rootRootSeq;
    Set<PackageableElement> rootSeq;
    Set<Package> subSeq;
    rootSeq.subsets(rootRootSeq);
    subSeq.subsets(rootSeq);
    UmlManager m;
    Package& pckg = *m.create<Package>();
    subSeq.add(pckg);
    subSeq.remove(pckg);
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_EQ(rootSeq.size(), 0);
    ASSERT_EQ(rootRootSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(pckg));
    ASSERT_FALSE(rootSeq.contains(pckg));
    ASSERT_FALSE(rootRootSeq.contains(pckg));
    subSeq.add(pckg);
    rootSeq.remove(pckg);
    ASSERT_EQ(subSeq.size(), 0);
    ASSERT_EQ(rootSeq.size(), 0);
    ASSERT_EQ(rootRootSeq.size(), 0);
    ASSERT_FALSE(subSeq.contains(pckg));
    ASSERT_FALSE(rootSeq.contains(pckg));
    ASSERT_FALSE(rootRootSeq.contains(pckg));
}

TEST_F(SetTest, specialAutoForLoop) {
    Set<Package> seq;
    int numPackages = 10;
    UmlManager m;
    for (int i = 0; i < numPackages; i++) {
        Package& p = *m.create<Package>(); 
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
    Package& one = *m.create<Package>();
    Package& two = *m.create<Package>();
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
    Package& p = *m.create<Package>();
    set.add(p);
    ASSERT_THROW(set.add(p), DuplicateElementInSetException);
}

class TestElement : public Element {

    friend class UmlManager;

    private:
        Set<TestElement, TestElement> m_others = Set<TestElement, TestElement>(this);
    public:
        TestElement() : Element(ElementType::ELEMENT) {
            m_others.opposite(&TestElement::getOthers);
        };
        virtual ~TestElement() {
            mountAndRelease();
        };
        Set<TestElement, TestElement>& getOthers() { return m_others; };
};

TEST_F(SetTest, oppositeTest) {
    UmlManager m;
    TestElement& t1 = *m.create<TestElement>();
    TestElement& t2 = *m.create<TestElement>();
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
    Package& p = *m.create<Package>();
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
    Package& p2 = *m.create<Package>();
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
    Package& p3 = *m.create<Package>();
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
    Package& g = *m.create<Package>();
    oSet->addFunctor(new gFunc(&g));
    rSet->redefines(*oSet);
    rSet->add(g);
    delete rSet;
    delete oSet;
}

TEST_F(SetTest, addToOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = *m.create<Package>();
    p.setName("1");
    set.add(p);
    ASSERT_EQ(set.front(), p);
    ASSERT_EQ(set.back(), p);
    Package& p2 = *m.create<Package>();
    p2.setName("2");
    set.add(p2);
    ASSERT_EQ(set.front(), p);
    ASSERT_EQ(set.back(), p2);
    Package& p3 = *m.create<Package>();
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
    Package& p = *m.create<Package>();
    subSet.add(p);
    ASSERT_EQ(subSet.front(), p);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_EQ(rootSet.front(), p);
    Package& c = *m.create<Package>();
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
    Package& p = *m.create<Package>();
    subSet.add(p);
    ASSERT_EQ(subSet.front(), p);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_EQ(rootSet.get(p.getID()), p);
    Package& c = *m.create<Package>();
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
        Package& p = *m.create<Package>();
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
    Package& p = *m.create<Package>();
    singleton.set(p);
    ASSERT_EQ(singleton.size(), 1);
    ASSERT_FALSE(singleton.empty());
    ASSERT_TRUE(singleton.get());
    ASSERT_EQ(*singleton.get(), p);
    ASSERT_EQ(rootSet.size(), 1);
    ASSERT_FALSE(rootSet.empty());
    ASSERT_EQ(rootSet.get(p.getID()), p);
    Package& c = *m.create<Package>();
    rootSet.add(c);
    ASSERT_EQ(singleton.size(), 1);
    ASSERT_FALSE(singleton.empty());
    ASSERT_TRUE(singleton.get());
    ASSERT_EQ(*singleton.get(), p);
    ASSERT_EQ(rootSet.size(), 2);
    ASSERT_TRUE(rootSet.contains(p.getID()));
    ASSERT_TRUE(rootSet.contains(c.getID()));
    ASSERT_EQ(rootSet.get(p.getID()), p);
    ASSERT_EQ(rootSet.get(c.getID()), c);
    singleton.set(0);
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
    Package& p1 = *m.create<Package>();
    Package& p2 = *m.create<Package>();
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
    Class& c = *m.create<Class>();
    Package& p = *m.create<Package>();
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
    Package& p = *m.create<Package>();
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
    Package& rightP = *m.create<Package>();
    Package& leftP = *m.create<Package>();
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

    Package& rightP2 = *m.create<Package>();
    Package& leftP2 = *m.create<Package>();
    rightSet1->add(rightP2);
    leftSet1->add(leftP2);
    ASSERT_EQ(rootSet->size(), 4);
    ASSERT_EQ(rightSet1->size(), 2);
    ASSERT_EQ(leftSet1->size(), 2);
    ASSERT_EQ(rightSet2->size(), 1);
    ASSERT_EQ(leftSet2->size(), 1);
    ASSERT_TRUE(rootSet->contains(rightP2));
    ASSERT_TRUE(rightSet1->contains(rightP2));
    ASSERT_FALSE(rightSet2->contains(rightP2));
    ASSERT_FALSE(leftSet1->contains(rightP2));
    ASSERT_FALSE(leftSet2->contains(rightP2));
    ASSERT_TRUE(rootSet->contains(leftP2));
    ASSERT_TRUE(leftSet1->contains(leftP2));
    ASSERT_FALSE(leftSet2->contains(leftP2));
    ASSERT_FALSE(rightSet1->contains(leftP2));
    ASSERT_FALSE(rightSet2->contains(leftP2));
    delete leftSet2;
    delete rightSet2;
    delete leftSet1;
    delete rightSet1;
    delete rootSet;
    // ASSERT_FALSE(rootRoot->m_id == ID::fromString("&&&&&&&&&&&&&&&&&&&&&&&&&&&&"));
}

/** This is a quick tree diagram of the subsetting at play here
 *      R   SS3
 *       \  /  \
 *        SS2   SS1
 *         | \  / |
 *        S1  S2  S1
 **/
// TEST_F(SetTest, TODO_NAME) {
//     Set<Element>* rootSet = new Set<Element>;
//     Set<NamedElement>* superSet3 = new Set<NamedElement>;
//     Set<NamedElement>* superSet2 = new Set<NamedElement>;
//     Set<PackageableElement>* superSet1 = new Set<PackageableElement>;
//     OrderedSet<Package>* set1 = new OrderedSet<Package>;
//     OrderedSet<Package>* set2 = new OrderedSet<Package>;
//     superSet2->subsets(*rootSet);
//     superSet2->subsets(*superSet3);
//     superSet1->subsets(*superSet3);
//     set1->subsets(*superSet2);
//     set1->subsets(*superSet1);
//     set2->subsets(*superSet2);
//     set2->subsets(*superSet1);
//     UmlManager m;
//     Package& p1 = m.create<Package>();
//     Package& p2 = m.create<Package>();
//     set1->add(p1);
//     set2->add(p2);
//     ASSERT_TRUE(superSet1->contains(p1.getID()));
//     ASSERT_TRUE(superSet2->contains(p1.getID()));
//     ASSERT_TRUE(superSet3->contains(p1.getID()));
//     ASSERT_TRUE(rootSet->contains(p1.getID()));
//     ASSERT_TRUE(superSet1->contains(p2.getID()));
//     ASSERT_TRUE(superSet2->contains(p2.getID()));
//     ASSERT_TRUE(superSet3->contains(p2.getID()));
//     ASSERT_TRUE(rootSet->contains(p2.getID()));
//     delete set2;
//     delete set1;
//     delete superSet1;
//     delete superSet2;
//     delete superSet3;
//     delete rootSet;
// }

TEST_F(SetTest, AddElementThatIsInSuperSet) {
    Set<PackageableElement> superSet;
    Set<Package> set;
    set.subsets(superSet);
    UmlManager m;
    Package& p = *m.create<Package>();
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
    std::cout << "TODO uncomment test" <<std::endl;
    // Set<PackageableElement>* set = new Set<PackageableElement>;
    // Set<Package>* subSet = new Set<Package>();
    // subSet->subsets(*set);
    // UmlManager m;
    // Package& p1 = m.create<Package>();
    // Package& p2 = m.create<Package>();
    // Package& p3 = m.create<Package>();
    // set->add(p1);
    // set->add(p2);
    // set->add(p3);
    // ASSERT_FALSE(set->empty());
    // ASSERT_TRUE(subSet->empty());
    // ASSERT_EQ(set->size(), 3);
    // ASSERT_EQ(subSet->size(), 0);std::cout << "TODO uncomment test" <<std::endl;
    // ASSERT_FALSE(subSet->contains(p1));
    // ASSERT_FALSE(subSet->contains(p2));
    // ASSERT_FALSE(subSet->contains(p3));
    // subSet->add(p1);
    // ASSERT_FALSE(subSet->empty());
    // ASSERT_EQ(subSet->size(), 1);
    // ASSERT_EQ(set->size(), 3);
    // ASSERT_TRUE(set->contains(p1));
    // ASSERT_TRUE(set->contains(p2));
    // ASSERT_TRUE(set->contains(p3));
    // ASSERT_TRUE(subSet->contains(p1));
    // ASSERT_FALSE(subSet->contains(p2));
    // ASSERT_FALSE(subSet->contains(p3));
    // subSet->add(p3);
    // ASSERT_EQ(subSet->size(), 2);
    // ASSERT_EQ(set->size(), 3);
    // ASSERT_TRUE(set->contains(p1));
    // ASSERT_TRUE(set->contains(p2));
    // ASSERT_TRUE(set->contains(p3));
    // ASSERT_TRUE(subSet->contains(p1));
    // ASSERT_FALSE(subSet->contains(p2));
    // ASSERT_TRUE(subSet->contains(p3));
    // subSet->add(p2);
    // ASSERT_EQ(subSet->size(), 3);
    // ASSERT_EQ(set->size(), 3);
    // ASSERT_TRUE(set->contains(p1));
    // ASSERT_TRUE(set->contains(p2));
    // ASSERT_TRUE(set->contains(p3));
    // ASSERT_TRUE(subSet->contains(p1));
    // ASSERT_TRUE(subSet->contains(p2));
    // ASSERT_TRUE(subSet->contains(p3));
    // delete subSet;
    // delete set;
}

TEST_F(SetTest, removeFirstElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = *m.create<Package>();
    set.add(p);
    set.remove(p);
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(set.size(), 0);
    ASSERT_THROW(set.front(), ManagerStateException);
}

TEST_F(SetTest, removeLastElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = *m.create<Package>();
    set.add(p);
    Package& p2 = *m.create<Package>();
    set.add(p2);
    set.remove(p2);
    ASSERT_EQ(set.back(), p);
    ASSERT_FALSE(set.contains(p2.getID()));
    ASSERT_EQ(set.size(), 1);
}

TEST_F(SetTest, removeMiddleElementFromOrderedSetTest) {
    OrderedSet<Package> set;
    UmlManager m;
    Package& p = *m.create<Package>();
    set.add(p);
    Package& p2 = *m.create<Package>();
    set.add(p2);
    Package& p3 = *m.create<Package>();
    set.add(p3);
    set.remove(p2);
    OrderedSetIterator it = set.begin();
    it++;
    ASSERT_EQ(*it, p3);
}

TEST_F(SetTest, removeFromSuperSetTest) {
    Set<PackageableElement>* set = new Set<PackageableElement>;
    Set<Package>* subSet = new Set<Package>;
    subSet->subsets(*set);
    UmlManager m;
    Package& pckg = *m.create<Package>();
    subSet->add(pckg);
    subSet->removeFromJustThisSet(pckg.getID());
    ASSERT_FALSE(set->empty());
    ASSERT_TRUE(subSet->empty());
    ASSERT_EQ(set->size(), 1);
    ASSERT_EQ(subSet->size(), 0);
    ASSERT_TRUE(set->contains(pckg.getID()));
    ASSERT_FALSE(subSet->contains(pckg.getID()));
    
    delete subSet;
    delete set;
}

TEST_F(SetTest, diamondSubsetsTest) {
    std::cout << "TODO uncomment test" <<std::endl;
    // Set<Element>* root = new Set<Element>;
    // Set<NamedElement>* set1 = new Set<NamedElement>;
    // Set<PackageableElement>* set2 = new Set<PackageableElement>;
    // Set<Package>* dSet = new Set<Package>;
    // set1->subsets(*root);
    // set2->subsets(*root);
    // dSet->subsets(*set1);
    // dSet->subsets(*set2);
    // UmlManager m;
    // Package& p1 = m.create<Package>();
    // Package& p2 = m.create<Package>();
    // Package& d1 = m.create<Package>();
    // Package& d2 = m.create<Package>();
    // set1->add(p1);
    // set2->add(p2);
    // set1->add(d1);
    // set2->add(d2);
    // dSet->add(d1);
    // ASSERT_EQ(root->size(), 4);
    // ASSERT_EQ(set1->size(), 2);
    // ASSERT_EQ(set2->size(), 2);
    // ASSERT_EQ(dSet->size(), 1);
    // ASSERT_TRUE(root->contains(p1));
    // ASSERT_TRUE(set1->contains(p1));
    // ASSERT_FALSE(set2->contains(p1));
    // ASSERT_FALSE(dSet->contains(p1));
    // ASSERT_TRUE(root->contains(p2));
    // ASSERT_FALSE(set1->contains(p2)); 
    // ASSERT_TRUE(set2->contains(p2));
    // ASSERT_FALSE(dSet->contains(p2));
    // ASSERT_TRUE(root->contains(d1));
    // ASSERT_TRUE(set1->contains(d1));
    // ASSERT_FALSE(set2->contains(d1));
    // ASSERT_TRUE(dSet->contains(d1));
    // dSet->add(d2);
    // ASSERT_EQ(root->size(), 4);
    // ASSERT_EQ(set1->size(), 2);
    // ASSERT_EQ(set2->size(), 2);
    // ASSERT_EQ(dSet->size(), 2);
    // ASSERT_TRUE(root->contains(p1));
    // ASSERT_TRUE(set1->contains(p1));
    // ASSERT_FALSE(set2->contains(p1));
    // ASSERT_FALSE(dSet->contains(p1));
    // ASSERT_TRUE(root->contains(p2));
    // ASSERT_FALSE(set1->contains(p2)); 
    // ASSERT_TRUE(set2->contains(p2));
    // ASSERT_FALSE(dSet->contains(p2));
    // ASSERT_TRUE(root->contains(d1));
    // ASSERT_TRUE(set1->contains(d1));
    // ASSERT_FALSE(set2->contains(d1));
    // ASSERT_TRUE(dSet->contains(d1));
    // ASSERT_TRUE(root->contains(d2));
    // ASSERT_FALSE(set1->contains(d2));
    // ASSERT_TRUE(set2->contains(d2));
    // ASSERT_TRUE(dSet->contains(d2));
    // delete dSet;
    // delete set2;
    // delete set1;
    // delete root;
}

TEST_F(SetTest, tripleRemovePlacholder) {
    Set<Element>* root = new Set<Element>;
    Set<NamedElement>* set1 = new Set<NamedElement>;
    Set<PackageableElement>* set2 = new Set<PackageableElement>;
    Set<Package>* set3 = new Set<Package>;
    set1->subsets(*root);
    set2->subsets(*root);
    set3->subsets(*root);
    UmlManager m;
    Package& pckg1 = *m.create<Package>();
    Package& pckg2 = *m.create<Package>();
    Package& pckg3 = *m.create<Package>();
    pckg1.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    pckg2.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
    pckg3.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");

    set1->add(pckg1);
    set2->add(pckg2);
    set3->add(pckg3);

    ASSERT_EQ(root->size(), 3);
    ASSERT_EQ(set1->size(), 1);
    ASSERT_EQ(set2->size(), 1);
    ASSERT_EQ(set3->size(), 1);

    ASSERT_TRUE(root->contains(pckg1));
    ASSERT_TRUE(set1->contains(pckg1));
    ASSERT_FALSE(set2->contains(pckg1));
    ASSERT_FALSE(set3->contains(pckg1));

    ASSERT_TRUE(root->contains(pckg2));
    ASSERT_FALSE(set1->contains(pckg2));
    ASSERT_TRUE(set2->contains(pckg2));
    ASSERT_FALSE(set3->contains(pckg2));

    ASSERT_TRUE(root->contains(pckg3));
    ASSERT_FALSE(set1->contains(pckg3));
    ASSERT_FALSE(set2->contains(pckg3));
    ASSERT_TRUE(set3->contains(pckg3));

    set1->remove(pckg1);

    ASSERT_EQ(root->size(), 2);
    ASSERT_EQ(set1->size(), 0);
    ASSERT_EQ(set2->size(), 1);
    ASSERT_EQ(set3->size(), 1);

    ASSERT_FALSE(root->contains(pckg1));
    ASSERT_FALSE(set1->contains(pckg1));
    ASSERT_FALSE(set2->contains(pckg1));
    ASSERT_FALSE(set3->contains(pckg1));

    ASSERT_TRUE(root->contains(pckg2));
    ASSERT_FALSE(set1->contains(pckg2));
    ASSERT_TRUE(set2->contains(pckg2));
    ASSERT_FALSE(set3->contains(pckg2));

    ASSERT_TRUE(root->contains(pckg3));
    ASSERT_FALSE(set1->contains(pckg3));
    ASSERT_FALSE(set2->contains(pckg3));
    ASSERT_TRUE(set3->contains(pckg3));

    set2->remove(pckg2);

    ASSERT_EQ(root->size(), 1);
    ASSERT_EQ(set1->size(), 0);
    ASSERT_EQ(set2->size(), 0);
    ASSERT_EQ(set3->size(), 1);

    ASSERT_FALSE(root->contains(pckg1));
    ASSERT_FALSE(set1->contains(pckg1));
    ASSERT_FALSE(set2->contains(pckg1));
    ASSERT_FALSE(set3->contains(pckg1));

    ASSERT_FALSE(root->contains(pckg2));
    ASSERT_FALSE(set1->contains(pckg2));
    ASSERT_FALSE(set2->contains(pckg2));
    ASSERT_FALSE(set3->contains(pckg2));

    ASSERT_TRUE(root->contains(pckg3));
    ASSERT_FALSE(set1->contains(pckg3));
    ASSERT_FALSE(set2->contains(pckg3));
    ASSERT_TRUE(set3->contains(pckg3));

    set3->remove(pckg3);

    ASSERT_EQ(root->size(), 0);
    ASSERT_EQ(set1->size(), 0);
    ASSERT_EQ(set2->size(), 0);
    ASSERT_EQ(set3->size(), 0);

    ASSERT_FALSE(root->contains(pckg1));
    ASSERT_FALSE(set1->contains(pckg1));
    ASSERT_FALSE(set2->contains(pckg1));
    ASSERT_FALSE(set3->contains(pckg1));

    ASSERT_FALSE(root->contains(pckg2));
    ASSERT_FALSE(set1->contains(pckg2));
    ASSERT_FALSE(set2->contains(pckg2));
    ASSERT_FALSE(set3->contains(pckg2));

    ASSERT_FALSE(root->contains(pckg3));
    ASSERT_FALSE(set1->contains(pckg3));
    ASSERT_FALSE(set2->contains(pckg3));
    ASSERT_FALSE(set3->contains(pckg3));

    delete set3;
    delete set2;
    delete set1;
    delete root;
}

TEST_F(SetTest, StructuredClassifierOwnedAttributesEmulationTest) {
    Set<>* ownedElements = new Set<>;
    Set<NamedElement>* members = new Set<NamedElement>;
    Set<NamedElement>* ownedMembers = new Set<NamedElement>;
    Set<Feature>* features = new Set<Feature>;
    Set<Property>* attributes = new Set<Property>;
    Set<Generalization>* generalizations = new Set<Generalization>;
    Set<NamedElement>* inheritedMembers = new Set<NamedElement>;
    Set<ConnectableElement>* roles = new Set<ConnectableElement>;
    Set<Property>* ownedAttributes = new Set<Property>;

    ownedMembers->subsets(*ownedElements);
    ownedMembers->subsets(*members);
    features->subsets(*members);
    attributes->subsets(*features);
    generalizations->subsets(*ownedElements);
    inheritedMembers->subsets(*members);
    roles->subsets(*members);
    ownedAttributes->subsets(*attributes);
    ownedAttributes->subsets(*roles);
    ownedAttributes->subsets(*ownedMembers);

    UmlManager m;
    
    Property& member = *m.create<Property>();
    Property& property = *m.create<Property>();
    Generalization& generalization = *m.create<Generalization>();

    generalizations->add(generalization);

    ASSERT_EQ(ownedElements->size(), 1);
    ASSERT_EQ(members->size(), 0);
    ASSERT_EQ(ownedMembers->size(), 0);
    ASSERT_EQ(features->size(), 0);
    ASSERT_EQ(attributes->size(), 0);
    ASSERT_EQ(generalizations->size(), 1);
    ASSERT_EQ(inheritedMembers->size(), 0);
    ASSERT_EQ(roles->size(), 0);
    ASSERT_EQ(ownedAttributes->size(), 0);
    ASSERT_TRUE(ownedElements->contains(generalization));
    ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
    ASSERT_FALSE(features->contains(generalization.getID()));
    ASSERT_FALSE(attributes->contains(generalization.getID()));
    ASSERT_TRUE(generalizations->contains(generalization));
    ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
    ASSERT_FALSE(roles->contains(generalization.getID()));
    ASSERT_FALSE(members->contains(generalization.getID()));
    ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));

    inheritedMembers->add(member);

    ASSERT_EQ(ownedElements->size(), 1);
    ASSERT_EQ(members->size(), 1);
    ASSERT_EQ(ownedMembers->size(), 0);
    ASSERT_EQ(features->size(), 0);
    ASSERT_EQ(attributes->size(), 0);
    ASSERT_EQ(generalizations->size(), 1);
    ASSERT_EQ(inheritedMembers->size(), 1);
    ASSERT_EQ(roles->size(), 0);
    ASSERT_EQ(ownedAttributes->size(), 0);
    ASSERT_FALSE(ownedElements->contains(member));
    ASSERT_FALSE(ownedMembers->contains(member));
    ASSERT_FALSE(features->contains(member));
    ASSERT_FALSE(attributes->contains(member));
    ASSERT_FALSE(generalizations->contains(member.getID()));
    ASSERT_FALSE(ownedAttributes->contains(member));
    ASSERT_FALSE(roles->contains(member));
    ASSERT_TRUE(members->contains(member));
    ASSERT_TRUE(inheritedMembers->contains(member));
    ASSERT_TRUE(ownedElements->contains(generalization));
    ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
    ASSERT_FALSE(features->contains(generalization.getID()));
    ASSERT_FALSE(attributes->contains(generalization.getID()));
    ASSERT_TRUE(generalizations->contains(generalization));
    ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
    ASSERT_FALSE(roles->contains(generalization.getID()));
    ASSERT_FALSE(members->contains(generalization.getID()));
    ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));
    
    ownedAttributes->add(property);

    ASSERT_EQ(ownedElements->size(), 2);
    ASSERT_EQ(members->size(), 2);
    ASSERT_EQ(inheritedMembers->size(), 1);
    ASSERT_EQ(features->size(), 1);
    ASSERT_EQ(attributes->size(), 1);
    ASSERT_EQ(generalizations->size(), 1);
    ASSERT_EQ(roles->size(), 1);
    ASSERT_EQ(ownedMembers->size(), 1);
    ASSERT_EQ(ownedAttributes->size(), 1);

    ASSERT_TRUE(ownedElements->contains(property));
    ASSERT_TRUE(members->contains(property));
    ASSERT_TRUE(ownedMembers->contains(property));
    ASSERT_TRUE(features->contains(property));
    ASSERT_TRUE(attributes->contains(property));
    ASSERT_FALSE(generalizations->contains(property.getID()));
    ASSERT_TRUE(roles->contains(property));
    ASSERT_TRUE(ownedAttributes->contains(property));
    ASSERT_FALSE(inheritedMembers->contains(property));
    ASSERT_FALSE(ownedElements->contains(member));
    ASSERT_FALSE(ownedMembers->contains(member));
    ASSERT_FALSE(features->contains(member));
    ASSERT_FALSE(attributes->contains(member));
    ASSERT_FALSE(generalizations->contains(member.getID()));
    ASSERT_FALSE(ownedAttributes->contains(member));
    ASSERT_FALSE(roles->contains(member));
    ASSERT_TRUE(members->contains(member));
    ASSERT_TRUE(inheritedMembers->contains(member));
    ASSERT_TRUE(ownedElements->contains(generalization));
    ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
    ASSERT_FALSE(features->contains(generalization.getID()));
    ASSERT_FALSE(attributes->contains(generalization.getID()));
    ASSERT_TRUE(generalizations->contains(generalization));
    ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
    ASSERT_FALSE(roles->contains(generalization.getID()));
    ASSERT_FALSE(members->contains(generalization.getID()));
    ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));

    delete ownedAttributes;
    delete roles;
    delete inheritedMembers;
    delete generalizations;
    delete attributes;
    delete features;
    delete ownedMembers;
    delete members;
    delete ownedElements;
}

// TEST_F(SetTest, redefineMoreComplexSet) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<PackageableElement>* packagedElements = new Set<PackageableElement>;
//     Set<Stereotype>* ownedStereotypes = new Set<Stereotype>;
//     Set<Stereotype>* redefinedStereotypes = new Set<Stereotype>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     packagedElements->subsets(*ownedMembers);
//     ownedStereotypes->subsets(*packagedElements);
//     redefinedStereotypes->redefines(*ownedStereotypes);

//     UmlManager m;
//     Stereotype& s = *m.create<Stereotype>();
//     s.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
//     Package& p = *m.create<Package>();
//     p.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");

//     redefinedStereotypes->add(s);

//     ASSERT_EQ(ownedElements->size(), 1);
//     ASSERT_EQ(members->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 1);
//     ASSERT_EQ(packagedElements->size(), 1);
//     ASSERT_EQ(ownedStereotypes->size(), 1);
//     ASSERT_EQ(redefinedStereotypes->size(), 1);

//     ASSERT_TRUE(ownedElements->contains(s));
//     ASSERT_TRUE(members->contains(s));
//     ASSERT_TRUE(ownedMembers->contains(s));
//     ASSERT_TRUE(packagedElements->contains(s));
//     ASSERT_TRUE(ownedStereotypes->contains(s));
//     ASSERT_TRUE(redefinedStereotypes->contains(s));

//     packagedElements->add(p);

//     ASSERT_EQ(ownedElements->size(), 2);
//     ASSERT_EQ(members->size(), 2);
//     ASSERT_EQ(ownedMembers->size(), 2);
//     ASSERT_EQ(packagedElements->size(), 2);
//     ASSERT_EQ(ownedStereotypes->size(), 1);
//     ASSERT_EQ(redefinedStereotypes->size(), 1);

//     ASSERT_TRUE(ownedElements->contains(s));
//     ASSERT_TRUE(members->contains(s));
//     ASSERT_TRUE(ownedMembers->contains(s));
//     ASSERT_TRUE(packagedElements->contains(s));
//     ASSERT_TRUE(ownedStereotypes->contains(s));
//     ASSERT_TRUE(redefinedStereotypes->contains(s));

//     ASSERT_TRUE(ownedElements->contains(p));
//     ASSERT_TRUE(members->contains(p));
//     ASSERT_TRUE(ownedMembers->contains(p));
//     ASSERT_TRUE(packagedElements->contains(p));
//     ASSERT_FALSE(ownedStereotypes->contains(p.getID()));
//     ASSERT_FALSE(redefinedStereotypes->contains(p.getID()));

//     redefinedStereotypes->remove(s);

//     ASSERT_EQ(ownedElements->size(), 1);
//     ASSERT_EQ(members->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 1);
//     ASSERT_EQ(packagedElements->size(), 1);
//     ASSERT_EQ(ownedStereotypes->size(), 0);
//     ASSERT_EQ(redefinedStereotypes->size(), 0);

//     ASSERT_FALSE(ownedElements->contains(s));
//     ASSERT_FALSE(members->contains(s));
//     ASSERT_FALSE(ownedMembers->contains(s));
//     ASSERT_FALSE(packagedElements->contains(s));
//     ASSERT_FALSE(ownedStereotypes->contains(s));
//     ASSERT_FALSE(redefinedStereotypes->contains(s));

//     ASSERT_TRUE(ownedElements->contains(p));
//     ASSERT_TRUE(members->contains(p));
//     ASSERT_TRUE(ownedMembers->contains(p));
//     ASSERT_TRUE(packagedElements->contains(p));
//     ASSERT_FALSE(ownedStereotypes->contains(p.getID()));
//     ASSERT_FALSE(redefinedStereotypes->contains(p.getID()));

//     delete redefinedStereotypes;
//     delete ownedStereotypes;
//     delete packagedElements;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

// TEST_F(SetTest, BehavioredClassifierEmulationTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<Behavior>* ownedBehaviors = new Set<Behavior>;
//     Set<Operation>* ownedOperations = new Set<Operation>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     ownedBehaviors->subsets(*ownedMembers);
//     ownedOperations->subsets(*features);
//     ownedOperations->subsets(*ownedMembers);

//     UmlManager m;
//     OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
//     Operation& op = m.*create<Operation>();

//     ownedBehaviors->add(bhv);

//     ASSERT_EQ(ownedElements->size(), 1);
//     ASSERT_EQ(members->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 1);
//     ASSERT_EQ(features->size(), 0);
//     ASSERT_EQ(ownedBehaviors->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 0);

//     ASSERT_TRUE(ownedElements->contains(bhv));
//     ASSERT_TRUE(members->contains(bhv));
//     ASSERT_TRUE(ownedMembers->contains(bhv));
//     ASSERT_FALSE(features->contains(bhv.getID()));
//     ASSERT_TRUE(ownedBehaviors->contains(bhv));
//     ASSERT_FALSE(ownedOperations->contains(bhv.getID()));

//     ownedOperations->add(op);

//     ASSERT_EQ(ownedElements->size(), 2);
//     ASSERT_EQ(members->size(), 2);
//     ASSERT_EQ(ownedMembers->size(), 2);
//     ASSERT_EQ(features->size(), 1);
//     ASSERT_EQ(ownedBehaviors->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 1);

//     ASSERT_TRUE(ownedElements->contains(bhv));
//     ASSERT_TRUE(members->contains(bhv));
//     ASSERT_TRUE(ownedMembers->contains(bhv));
//     ASSERT_FALSE(features->contains(bhv.getID()));
//     ASSERT_TRUE(ownedBehaviors->contains(bhv));
//     ASSERT_FALSE(ownedOperations->contains(bhv.getID()));

//     ASSERT_TRUE(ownedElements->contains(op));
//     ASSERT_TRUE(members->contains(op));
//     ASSERT_TRUE(ownedMembers->contains(op));
//     ASSERT_TRUE(features->contains(op));
//     ASSERT_FALSE(ownedBehaviors->contains(op.getID()));
//     ASSERT_TRUE(ownedOperations->contains(op));

//     delete ownedOperations;
//     delete ownedBehaviors;
//     delete features;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

// TEST_F(SetTest, ClassAttributeAndOperationEmulationTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<Property>* attributes = new Set<Property>;
//     Set<ConnectableElement>* roles =  new Set<ConnectableElement>;
//     Set<Property>* ownedAttributes = new Set<Property>;
//     Set<Operation>* ownedOperations = new Set<Operation>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     attributes->subsets(*features);
//     roles->subsets(*members);
//     ownedAttributes->subsets(*attributes);
//     ownedAttributes->subsets(*roles);
//     ownedAttributes->subsets(*ownedMembers);
//     ownedOperations->subsets(*ownedMembers);
//     ownedOperations->subsets(*features);

//     UmlManager m;
//     Property& p = *m.create<Property>();
//     Operation& o = *m.create<Operation>();
//     ownedAttributes->add(p);
//     ownedOperations->add(o);

//     ASSERT_EQ(ownedElements->size(), 2);
//     ASSERT_EQ(members->size(), 2);
//     ASSERT_EQ(ownedMembers->size(), 2);
//     ASSERT_EQ(features->size(), 2);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(roles->size(), 1);
//     ASSERT_EQ(ownedAttributes->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 1);

//     ASSERT_TRUE(ownedElements->contains(p));
//     ASSERT_TRUE(members->contains(p));
//     ASSERT_TRUE(ownedMembers->contains(p));
//     ASSERT_TRUE(features->contains(p));
//     ASSERT_TRUE(attributes->contains(p));
//     ASSERT_TRUE(roles->contains(p));
//     ASSERT_TRUE(ownedAttributes->contains(p));
//     ASSERT_FALSE(ownedOperations->contains(p.getID()));

//     ASSERT_TRUE(ownedElements->contains(o));
//     ASSERT_TRUE(members->contains(o));
//     ASSERT_TRUE(ownedMembers->contains(o));
//     ASSERT_TRUE(features->contains(o));
//     ASSERT_FALSE(attributes->contains(o.getID()));
//     ASSERT_FALSE(roles->contains(o.getID()));
//     ASSERT_FALSE(ownedAttributes->contains(o.getID()));
//     ASSERT_TRUE(ownedOperations->contains(o));

//     delete ownedOperations;
//     delete ownedAttributes;
//     delete roles;
//     delete attributes;
//     delete features;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

// TEST_F(SetTest, copyComplexTreeTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<Comment>* ownedComments =  new Set<Comment>;
//     Set<InstanceSpecification>* appliedStereotypes = new Set<InstanceSpecification>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
    
//     ownedComments->subsets(*ownedElements);
//     appliedStereotypes->subsets(*ownedElements);
//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);

//     UmlManager m;
//     InstanceSpecification& s = *m.create<InstanceSpecification>();
//     Comment& c = *m.create<Comment>();
//     Package& p = *m.create<Package>();

//     s.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
//     c.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");
//     p.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");

//     ownedMembers->add(p);
//     appliedStereotypes->add(s);
//     ownedComments->add(c);

//     Set<> ownedElements2;
//     Set<Comment> ownedComments2;
//     Set<InstanceSpecification> appliedStereotypes2;
//     Set<NamedElement> members2;
//     Set<NamedElement> ownedMembers2;

//     ownedComments2.subsets(ownedElements2);
//     appliedStereotypes2.subsets(ownedElements2);
//     ownedMembers2.subsets(ownedElements2);
//     ownedMembers2.subsets(members2);

//     ownedElements2 = *ownedElements;
//     ownedComments2 = *ownedComments;
//     appliedStereotypes2 = *appliedStereotypes;
//     members2 = *members;
//     ownedMembers2 = *ownedMembers;

//     ASSERT_EQ(ownedElements2.size(), 3);
//     ASSERT_EQ(ownedComments2.size(), 1);
//     ASSERT_EQ(appliedStereotypes2.size(), 1);
//     ASSERT_EQ(members2.size(), 1);
//     ASSERT_EQ(ownedMembers2.size(), 1);

//     ASSERT_TRUE(ownedElements2.contains(p));
//     ASSERT_FALSE(ownedComments2.contains(p.getID()));
//     ASSERT_FALSE(appliedStereotypes2.contains(p.getID()));
//     ASSERT_FALSE(ownedComments2.contains(p.getID()));
//     ASSERT_TRUE(members2.contains(p));
//     ASSERT_TRUE(ownedMembers2.contains(p));

//     ASSERT_TRUE(ownedElements2.contains(s));
//     ASSERT_FALSE(ownedComments2.contains(s.getID()));
//     ASSERT_TRUE(appliedStereotypes2.contains(s));
//     ASSERT_FALSE(members2.contains(s.getID()));
//     ASSERT_FALSE(ownedMembers2.contains(s.getID()));

//     ASSERT_TRUE(ownedElements2.contains(c));
//     ASSERT_TRUE(ownedComments2.contains(c));
//     ASSERT_FALSE(appliedStereotypes2.contains(c.getID()));
//     ASSERT_FALSE(members2.contains(c.getID()));
//     ASSERT_FALSE(ownedMembers2.contains(c.getID()));

//     delete ownedMembers;
//     delete members;
//     delete appliedStereotypes;
//     delete ownedComments;
//     delete ownedElements;
// }

// TEST_F(SetTest, EmulateFullArtifactTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<Dependency>* clientDependencies = new Set<Dependency>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<Property>* attributes = new Set<Property>;
//     Set<Artifact>* nestedArtifacts = new Set<Artifact>;
//     OrderedSet<Property>* ownedAttributes = new OrderedSet<Property>;
//     OrderedSet<Operation>* ownedOperations = new OrderedSet<Operation>;
//     Set<Manifestation>* manifestations = new Set<Manifestation>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     attributes->subsets(*features);
//     nestedArtifacts->subsets(*ownedMembers);
//     ownedAttributes->subsets(*attributes);
//     ownedAttributes->subsets(*ownedMembers);
//     ownedOperations->subsets(*features);
//     ownedOperations->subsets(*ownedMembers);
//     manifestations->subsets(*ownedMembers);
//     manifestations->subsets(*clientDependencies);

//     UmlManager m;
//     Property& attribute = *m.create<Property>();
//     attribute.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAD"));
//     Operation& operation = *m.create<Operation>();
//     operation.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAC"));
//     Artifact& nested = *m.create<Artifact>();
//     nested.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAB"));
//     Manifestation& manifestation = *m.create<Manifestation>();

//     ownedAttributes->add(attribute);

//     ASSERT_EQ(ownedAttributes->size(), 1);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 0);
//     ASSERT_EQ(features->size(), 1);
//     ASSERT_EQ(nestedArtifacts->size(), 0);
//     ASSERT_EQ(manifestations->size(), 0);
//     ASSERT_EQ(ownedMembers->size(), 1);
//     ASSERT_EQ(members->size(), 1);
//     ASSERT_EQ(clientDependencies->size(), 0);
//     ASSERT_EQ(ownedElements->size(), 1);

//     ASSERT_TRUE(ownedAttributes->contains(attribute));
//     ASSERT_TRUE(attributes->contains(attribute));
//     ASSERT_TRUE(features->contains(attribute));
//     ASSERT_TRUE(ownedMembers->contains(attribute));
//     ASSERT_TRUE(members->contains(attribute));
//     ASSERT_TRUE(ownedElements->contains(attribute));
//     ASSERT_FALSE(ownedOperations->contains(attribute.getID()));
//     ASSERT_FALSE(nestedArtifacts->contains(attribute.getID()));
//     ASSERT_FALSE(manifestations->contains(attribute.getID()));
//     ASSERT_FALSE(clientDependencies->contains(attribute.getID()));

//     ownedOperations->add(operation);

//     ASSERT_EQ(ownedAttributes->size(), 1);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 1);
//     ASSERT_EQ(features->size(), 2);
//     ASSERT_EQ(nestedArtifacts->size(), 0);
//     ASSERT_EQ(manifestations->size(), 0);
//     ASSERT_EQ(ownedMembers->size(), 2);
//     ASSERT_EQ(members->size(), 2);
//     ASSERT_EQ(clientDependencies->size(), 0);
//     ASSERT_EQ(ownedElements->size(), 2);

//     ASSERT_TRUE(ownedOperations->contains(operation));
//     ASSERT_TRUE(features->contains(operation));
//     ASSERT_TRUE(ownedMembers->contains(operation));
//     ASSERT_TRUE(members->contains(operation));
//     ASSERT_TRUE(ownedElements->contains(operation));
//     ASSERT_FALSE(ownedAttributes->contains(operation.getID()));
//     ASSERT_FALSE(attributes->contains(operation.getID()));
//     ASSERT_FALSE(nestedArtifacts->contains(operation.getID()));
//     ASSERT_FALSE(manifestations->contains(operation.getID()));
//     ASSERT_FALSE(clientDependencies->contains(operation.getID()));

//     nestedArtifacts->add(nested);

//     ASSERT_EQ(ownedAttributes->size(), 1);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 1);
//     ASSERT_EQ(features->size(), 2);
//     ASSERT_EQ(nestedArtifacts->size(), 1);
//     ASSERT_EQ(manifestations->size(), 0);
//     ASSERT_EQ(ownedMembers->size(), 3);
//     ASSERT_EQ(members->size(), 3);
//     ASSERT_EQ(clientDependencies->size(), 0);
//     ASSERT_EQ(ownedElements->size(), 3);
    
//     ASSERT_TRUE(nestedArtifacts->contains(nested));
//     ASSERT_TRUE(ownedMembers->contains(nested));
//     ASSERT_TRUE(members->contains(nested));
//     ASSERT_TRUE(ownedElements->contains(nested));
//     ASSERT_FALSE(ownedAttributes->contains(nested.getID()));
//     ASSERT_FALSE(ownedOperations->contains(nested.getID()));
//     ASSERT_FALSE(attributes->contains(nested.getID()));
//     ASSERT_FALSE(features->contains(nested.getID()));
//     ASSERT_FALSE(clientDependencies->contains(nested.getID()));

//     manifestations->add(manifestation);

//     ASSERT_EQ(ownedAttributes->size(), 1);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(ownedOperations->size(), 1);
//     ASSERT_EQ(features->size(), 2);
//     ASSERT_EQ(nestedArtifacts->size(), 1);
//     ASSERT_EQ(manifestations->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 4);
//     ASSERT_EQ(members->size(), 4);
//     ASSERT_EQ(clientDependencies->size(), 1);
//     ASSERT_EQ(ownedElements->size(), 4);

//     ASSERT_TRUE(manifestations->contains(manifestation));
//     ASSERT_TRUE(ownedMembers->contains(manifestation));
//     ASSERT_TRUE(members->contains(manifestation));
//     ASSERT_TRUE(clientDependencies->contains(manifestation));
//     ASSERT_TRUE(ownedElements->contains(manifestation));
//     ASSERT_FALSE(ownedAttributes->contains(manifestation.getID()));
//     ASSERT_FALSE(ownedOperations->contains(manifestation.getID()));
//     ASSERT_FALSE(attributes->contains(manifestation.getID()));
//     ASSERT_FALSE(features->contains(manifestation.getID()));
//     ASSERT_FALSE(nestedArtifacts->contains(manifestation.getID()));

//     ASSERT_TRUE(ownedAttributes->contains(attribute));
//     ASSERT_TRUE(attributes->contains(attribute));
//     ASSERT_TRUE(features->contains(attribute));
//     ASSERT_TRUE(ownedMembers->contains(attribute));
//     ASSERT_TRUE(members->contains(attribute));
//     ASSERT_TRUE(ownedElements->contains(attribute));
//     ASSERT_FALSE(ownedOperations->contains(attribute.getID()));
//     ASSERT_FALSE(nestedArtifacts->contains(attribute.getID()));
//     ASSERT_FALSE(manifestations->contains(attribute.getID()));
//     ASSERT_FALSE(clientDependencies->contains(attribute.getID()));

//     ASSERT_TRUE(ownedOperations->contains(operation));
//     ASSERT_TRUE(features->contains(operation));
//     ASSERT_TRUE(ownedMembers->contains(operation));
//     ASSERT_TRUE(members->contains(operation));
//     ASSERT_TRUE(ownedElements->contains(operation));
//     ASSERT_FALSE(ownedAttributes->contains(operation.getID()));
//     ASSERT_FALSE(attributes->contains(operation.getID()));
//     ASSERT_FALSE(nestedArtifacts->contains(operation.getID()));
//     ASSERT_FALSE(manifestations->contains(operation.getID()));
//     ASSERT_FALSE(clientDependencies->contains(operation.getID()));

//     ASSERT_TRUE(nestedArtifacts->contains(nested));
//     ASSERT_TRUE(ownedMembers->contains(nested));
//     ASSERT_TRUE(members->contains(nested));
//     ASSERT_TRUE(ownedElements->contains(nested));
//     ASSERT_FALSE(ownedAttributes->contains(nested.getID()));
//     ASSERT_FALSE(ownedOperations->contains(nested.getID()));
//     ASSERT_FALSE(attributes->contains(nested.getID()));
//     ASSERT_FALSE(features->contains(nested.getID()));
//     ASSERT_FALSE(clientDependencies->contains(nested.getID()));

//     delete manifestations;
//     delete ownedOperations;
//     delete ownedAttributes;
//     delete nestedArtifacts;
//     delete attributes;
//     delete features;
//     delete clientDependencies;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

// TEST_F(SetTest, emulateClassWConnectorTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<Property>* attributes = new Set<Property>;
//     Set<ConnectableElement>* roles =  new Set<ConnectableElement>;
//     Set<Property>* ownedAttributes = new Set<Property>;
//     Set<Property>* parts = new Set<Property>;
//     Set<Connector>* ownedConnectors = new Set<Connector>;
//     Set<Property>* classOwnedAttributes = new Set<Property>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     attributes->subsets(*features);
//     roles->subsets(*members);
//     ownedAttributes->subsets(*attributes);
//     ownedAttributes->subsets(*roles);
//     ownedAttributes->subsets(*ownedMembers);
//     parts->subsets(*ownedAttributes);
//     ownedConnectors->subsets(*ownedMembers);
//     ownedConnectors->subsets(*features);
//     classOwnedAttributes->redefines(*ownedAttributes);

//     UmlManager m;
//     Property& prop1 = *m.create<Property>();
//     Property& prop2 = *m.create<Property>();
//     Connector& connector = *m.create<Connector>();
//     classOwnedAttributes->add(prop1, prop2);
//     parts->add(prop2);
//     ownedConnectors->add(connector);

//     ASSERT_EQ(classOwnedAttributes->size(), 2);
//     ASSERT_EQ(parts->size(), 1);
//     ASSERT_EQ(ownedAttributes->size(), 2);
//     ASSERT_EQ(attributes->size(), 2);
//     ASSERT_EQ(roles->size(), 2);
//     ASSERT_EQ(ownedConnectors->size(), 1);
//     ASSERT_EQ(features->size(), 3);
//     ASSERT_EQ(ownedMembers->size(), 3);
//     ASSERT_EQ(members->size(), 3);
//     ASSERT_EQ(ownedElements->size(), 3);

//     ASSERT_TRUE(classOwnedAttributes->contains(prop1));
//     ASSERT_FALSE(parts->contains(prop1));
//     ASSERT_TRUE(ownedAttributes->contains(prop1));
//     ASSERT_TRUE(roles->contains(prop1));
//     ASSERT_TRUE(attributes->contains(prop1));
//     ASSERT_FALSE(ownedConnectors->contains(prop1.getID()));
//     ASSERT_TRUE(features->contains(prop1));
//     ASSERT_TRUE(ownedMembers->contains(prop1));
//     ASSERT_TRUE(members->contains(prop1));
//     ASSERT_TRUE(ownedElements->contains(prop1));

//     ASSERT_TRUE(classOwnedAttributes->contains(prop2));
//     ASSERT_TRUE(parts->contains(prop2));
//     ASSERT_TRUE(ownedAttributes->contains(prop2));
//     ASSERT_TRUE(roles->contains(prop2));
//     ASSERT_TRUE(attributes->contains(prop2));
//     ASSERT_FALSE(ownedConnectors->contains(prop2.getID()));
//     ASSERT_TRUE(features->contains(prop2));
//     ASSERT_TRUE(ownedMembers->contains(prop2));
//     ASSERT_TRUE(members->contains(prop2));
//     ASSERT_TRUE(ownedElements->contains(prop2));

//     ASSERT_FALSE(classOwnedAttributes->contains(connector.getID()));
//     ASSERT_FALSE(parts->contains(connector.getID()));
//     ASSERT_FALSE(ownedAttributes->contains(connector.getID()));
//     ASSERT_FALSE(roles->contains(connector.getID()));
//     ASSERT_FALSE(attributes->contains(connector.getID()));
//     ASSERT_TRUE(ownedConnectors->contains(connector));
//     ASSERT_TRUE(features->contains(connector));
//     ASSERT_TRUE(ownedMembers->contains(connector));
//     ASSERT_TRUE(members->contains(connector));
//     ASSERT_TRUE(ownedElements->contains(connector));

//     delete classOwnedAttributes;
//     delete ownedConnectors;
//     delete parts;
//     delete ownedAttributes;
//     delete roles;
//     delete attributes;
//     delete features;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

// TEST_F(SetTest, emulateAssociationTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<>* relatedElements = new Set<>;
//     OrderedSet<Property>* memberEnds = new OrderedSet<Property>;
//     OrderedSet<Property>* ownedEnds = new OrderedSet<Property>;
//     Set<Property>* navigableOwnedEnds = new Set<Property>;
//     Set<Type>* endTypes = new Set<Type>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     memberEnds->subsets(*members);
//     ownedEnds->subsets(*memberEnds);
//     ownedEnds->subsets(*ownedMembers);
//     ownedEnds->subsets(*features);
//     navigableOwnedEnds->subsets(*ownedEnds);
//     endTypes->subsets(*relatedElements);

//     UmlManager m;
//     Property& memberEnd = *m.create<Property>();
//     Property& ownedEnd = *m.create<Property>();
//     Class& endType1 = *m.create<Class>();
//     Class& endType2 = *m.create<Class>();

//     memberEnd.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");
//     ownedEnd.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
//     memberEnds->add(memberEnd);
//     endTypes->add(endType1);
//     navigableOwnedEnds->add(ownedEnd);
//     endTypes->add(endType2);

//     // test loops
//     ID id = ownedEnd.getID();
//     size_t i = 0;
//     for (auto& el : *ownedElements) {
//         i++;
//         ASSERT_TRUE(ownedElements->contains(el.getID()));
//         ASSERT_TRUE(members->contains(el.getID()));
//         ASSERT_TRUE(ownedMembers->contains(el.getID()));
//         ASSERT_TRUE(memberEnds->contains(el.getID()));
//         ASSERT_TRUE(ownedElements->contains(el.getID()));
//         ASSERT_EQ(id, el.getID());
//         ASSERT_EQ(i, 1);
//     }


//     delete endTypes;
//     delete navigableOwnedEnds;
//     delete ownedEnds;
//     delete memberEnds;
//     delete relatedElements;
//     delete features;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }
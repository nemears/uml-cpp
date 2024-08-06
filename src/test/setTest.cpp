#include "gtest/gtest.h"
#include "test/uml-cpp-paths.h"

#include "uml/types/package.h"
#include "uml/uml-stable.h"
#include "uml/set/orderedSet.h"

using namespace UML;

class SetTest : public ::testing::Test {
   
};

class TestPackageSetElement : public Element {
    public:
        Set<Package, TestPackageSetElement> set = Set<Package, TestPackageSetElement>(this);
        TestPackageSetElement() : Element(ElementType::ELEMENT) {}
};

TEST_F(SetTest, basicSetTest) {
    size_t numPackages = 20;
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    Package& pckg = *m.create<Package>();
    testEl->set.add(pckg);
    ASSERT_FALSE(testEl->set.empty());
    ASSERT_EQ(*testEl->set.get(pckg.getID()), pckg);
    std::vector<ID> ids(numPackages);
    ids[0] = pckg.getID();
    for (size_t i = 0; i < numPackages - 1; i++) {
        Package& p = *m.create<Package>();
        ids[i+1] = p.getID();
        testEl->set.add(p);
    }
    size_t i = 0;
    for (const ID id : ids) {
        ASSERT_TRUE(testEl->set.contains(id)) << "index at " << i;
        ASSERT_EQ(testEl->set.get(id)->getID(), id);
        i++;
    }
}

TEST_F(SetTest, basicRemoveTest) {
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    PackagePtr pckg3 = m.create<Package>();
    PackagePtr pckg4 = m.create<Package>();
    PackagePtr pckg5 = m.create<Package>();
    testEl->set.add(pckg1);
    testEl->set.add(pckg2);
    testEl->set.add(pckg3);
    testEl->set.add(pckg4);
    testEl->set.add(pckg5);
    testEl->set.remove(pckg1);
    ASSERT_EQ(testEl->set.size(), 4);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_TRUE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg5);
    ASSERT_EQ(testEl->set.size(), 3);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg2);
    ASSERT_EQ(testEl->set.size(), 2);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg4);
    ASSERT_EQ(testEl->set.size(), 1);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg3);
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
}

TEST_F(SetTest, consistentIdBasicRemoveTest) {
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    PackagePtr pckg1 = m.create<Package>();
    pckg1->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA1");
    PackagePtr pckg2 = m.create<Package>();
    pckg2->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA2");
    PackagePtr pckg3 = m.create<Package>();
    pckg3->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA3");
    PackagePtr pckg4 = m.create<Package>();
    pckg4->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA4");
    PackagePtr pckg5 = m.create<Package>();
    pckg5->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA5");
    testEl->set.add(pckg1);
    testEl->set.add(pckg2);
    testEl->set.add(pckg3);
    testEl->set.add(pckg4);
    testEl->set.add(pckg5);
    testEl->set.remove(pckg1);
    ASSERT_EQ(testEl->set.size(), 4);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_TRUE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg5);
    ASSERT_EQ(testEl->set.size(), 3);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg2);
    ASSERT_EQ(testEl->set.size(), 2);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg4);
    ASSERT_EQ(testEl->set.size(), 1);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    testEl->set.remove(pckg3);
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
}

TEST_F(SetTest, longerConsistentIdBasicRemoveTest) {
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    PackagePtr pckg1 = m.create<Package>();
    pckg1->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA1");
    PackagePtr pckg2 = m.create<Package>();
    pckg2->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA2");
    PackagePtr pckg3 = m.create<Package>();
    pckg3->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA3");
    PackagePtr pckg4 = m.create<Package>();
    pckg4->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA4");
    PackagePtr pckg5 = m.create<Package>();
    pckg5->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA5");
    PackagePtr pckg6 = m.create<Package>();
    pckg5->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA6");
    PackagePtr pckg7 = m.create<Package>();
    pckg5->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA7");
    PackagePtr pckg8 = m.create<Package>();
    pckg5->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAA8");
    testEl->set.add(pckg1);
    testEl->set.add(pckg2);
    testEl->set.add(pckg3);
    testEl->set.add(pckg4);
    testEl->set.add(pckg5);
    testEl->set.add(pckg6);
    testEl->set.add(pckg7);
    testEl->set.add(pckg8);
    testEl->set.remove(pckg1);
    ASSERT_EQ(testEl->set.size(), 7);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_TRUE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg5);
    ASSERT_EQ(testEl->set.size(), 6);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg2);
    ASSERT_EQ(testEl->set.size(), 5);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg4);
    ASSERT_EQ(testEl->set.size(), 4);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg7);
    ASSERT_EQ(testEl->set.size(), 3);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg3);
    ASSERT_EQ(testEl->set.size(), 2);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg8);
    ASSERT_EQ(testEl->set.size(), 1);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_FALSE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg6);
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_FALSE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_FALSE(testEl->set.contains(pckg8));
}

TEST_F(SetTest, longerBasicRemoveTest) {
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    PackagePtr pckg3 = m.create<Package>();
    PackagePtr pckg4 = m.create<Package>();
    PackagePtr pckg5 = m.create<Package>();
    PackagePtr pckg6 = m.create<Package>();
    PackagePtr pckg7 = m.create<Package>();
    PackagePtr pckg8 = m.create<Package>();
    testEl->set.add(pckg1);
    testEl->set.add(pckg2);
    testEl->set.add(pckg3);
    testEl->set.add(pckg4);
    testEl->set.add(pckg5);
    testEl->set.add(pckg6);
    testEl->set.add(pckg7);
    testEl->set.add(pckg8);
    testEl->set.remove(pckg1);
    ASSERT_EQ(testEl->set.size(), 7);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_TRUE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg5);
    ASSERT_EQ(testEl->set.size(), 6);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_TRUE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg2);
    ASSERT_EQ(testEl->set.size(), 5);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_TRUE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg4);
    ASSERT_EQ(testEl->set.size(), 4);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_TRUE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg7);
    ASSERT_EQ(testEl->set.size(), 3);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_TRUE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg3);
    ASSERT_EQ(testEl->set.size(), 2);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_TRUE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg8);
    ASSERT_EQ(testEl->set.size(), 1);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_TRUE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_FALSE(testEl->set.contains(pckg8));
    testEl->set.remove(pckg6);
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_FALSE(testEl->set.contains(pckg1));
    ASSERT_FALSE(testEl->set.contains(pckg2));
    ASSERT_FALSE(testEl->set.contains(pckg3));
    ASSERT_FALSE(testEl->set.contains(pckg4));
    ASSERT_FALSE(testEl->set.contains(pckg5));
    ASSERT_FALSE(testEl->set.contains(pckg6));
    ASSERT_FALSE(testEl->set.contains(pckg7));
    ASSERT_FALSE(testEl->set.contains(pckg8));
}

class TestReindexSetElement : public Element {
    public:
        Set<Package, TestReindexSetElement> set = Set<Package, TestReindexSetElement>(this);
        TestReindexSetElement() : Element(ElementType::ELEMENT) {
            set.subsets(*this->m_ownedElements);
        }
};

// TEST_F(SetTest, reindexSetElementTest) {
//     // Reference ids of this test: shown in order from least to greatest
//     //      Jp2IhMjC2qNN7cIYPXiFZU4vDdun
//     //      GqrX5Ta8KQDdFfaHrau08OS7Et3n
//     //      kbreSzh_ys_8SepvJR6Q58tzWdFI
//     //      TeIMyndF4nm_NOTbFZ&vZDLXxvtC
//     Manager<> m;
//     UmlPtr<TestReindexSetElement> testEl = m.create<TestReindexSetElement>();
//     PackagePtr pckg1 = m.create<Package>();
//     pckg1->setID("GqrX5Ta8KQDdFfaHrau08OS7Et3n");
//     PackagePtr pckg2 = m.create<Package>();
//     pckg2->setID("kbreSzh_ys_8SepvJR6Q58tzWdFI");
//     PackagePtr pckg3 = m.create<Package>();
//     pckg3->setID("TeIMyndF4nm_NOTbFZ&vZDLXxvtC");
//     testEl->set.add(pckg1);
//     testEl->set.add(pckg2);
//     testEl->set.add(pckg3);
//     ASSERT_EQ(testEl->set.front(), *pckg1);
//     ASSERT_EQ(testEl->set.back(), *pckg3);
//     SetIterator<Package> it1 = testEl->set.begin();
//     ASSERT_EQ(*it1, *pckg1);
//     ++it1;
//     ASSERT_EQ(*it1, *pckg2);
//     ++it1;
//     ASSERT_EQ(*it1, *pckg3);
//     ++it1;
//     ASSERT_EQ(it1, testEl->set.end());
//     // reindex
//     pckg3->setID("Jp2IhMjC2qNN7cIYPXiFZU4vDdun");
//     ASSERT_EQ(testEl->set.front(), *pckg3);
//     ASSERT_EQ(testEl->set.back(), *pckg1);
//     SetIterator<Package> it2 = testEl->set.begin();
//     ASSERT_EQ(*it2, *pckg3);
//     ++it2;
//     ASSERT_EQ(*it2, *pckg2);
//     ++it2;
//     ASSERT_EQ(*it2, *pckg1);
//     ++it2;
//     ASSERT_EQ(it2, testEl->set.end());
// }

class TestSubsetsElement : public Element {
    public:
        Set<PackageableElement, TestSubsetsElement> root = Set<PackageableElement, TestSubsetsElement>(this);
        Set<Package, TestSubsetsElement> sub = Set<Package, TestSubsetsElement>(this);
        TestSubsetsElement() : Element(ElementType::ELEMENT) {
            sub.subsets(root);
        }
};

TEST_F(SetTest, basicSubsetsTest) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    Package& pckg = *m.create<Package>();
    testEl->sub.add(pckg);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(pckg.getID()));
    ASSERT_EQ(*testEl->sub.get(pckg.getID()), pckg);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg.getID()));
    ASSERT_EQ(*testEl->root.get(pckg.getID()), pckg);
    Package& clazz = *m.create<Package>();
    testEl->root.add(clazz);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(clazz.getID()));
    ASSERT_EQ(*testEl->root.get(clazz.getID()), clazz);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_FALSE(testEl->sub.contains(clazz.getID()));
    ASSERT_FALSE(testEl->sub.get(clazz.getID()));
}

TEST_F(SetTest, iterateOverSubsettedElement) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    PackagePtr a = m.create<Package>();
    PackagePtr b = m.create<Package>();
    PackagePtr c = m.create<Package>();
    a->setID("&90IAqvc&wUnewHz0xLI4fPYNXUe");
    b->setID("04zH_c&oPfM5KXPqJXA0_7AzIzcy");
    c->setID("buttEyLdYRCk_zbuttYrQyy42yHr"); 
    // inorder
    //
    // &90IAqvc&wUnewHz0xLI4fPYNXUe
    // buttEyLdYRCk_zbuttYrQyy42yHr
    // 04zH_c&oPfM5KXPqJXA0_7AzIzcy
    //
    // WwowBIeuOqdXecMITJkHZWbnD94G
    testEl->root.add(a);
    std::unordered_set<PackageableElement*> pckgs;
    pckgs.insert(a.ptr());
    testEl->sub.add(b);
    pckgs.insert(b.ptr());
    testEl->sub.add(c);
    pckgs.insert(c.ptr());
    ASSERT_EQ(pckgs.size(), 3);
    auto it = testEl->root.begin();
    std::cout << "test setup done" << std::endl;
    std::cout << "first elment is " << it->getID().string() << std::endl;
    it++;
    std::cout << "incremented once!" << std::endl;
    std::cout << "second elment is " << it->getID().string() << std::endl;
    ASSERT_TRUE(it != testEl->root.end());
    it++;
    std::cout << "incremented twice!" << std::endl;
    std::cout << "third elment is " << it->getID().string() << std::endl;
    ASSERT_TRUE(it != testEl->root.end());
    it++;
    std::cout << "incremented three times (should be done)" << std::endl;
    ASSERT_EQ(it.m_hash, testEl->root.end().m_hash);
    ASSERT_EQ(it, testEl->root.end());
}

// TEST_F(SetTest, readjustRedBlackRootTest) {
//     Manager<> m;
//     UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
//     PackagePtr a = m.create<Package>();
//     PackagePtr b = m.create<Package>();
//     PackagePtr c = m.create<Package>();
//     a->setID("&90IAqvc&wUnewHz0xLI4fPYNXUe");
//     b->setID("buttEyLdYRCk_zbuttYrQyy42yHr");
//     c->setID("04zH_c&oPfM5KXPqJXA0_7AzIzcy");
//     testEl->sub.add(a);
//     ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
//     testEl->sub.add(b);
//     ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
//     testEl->sub.add(c);
//     ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
// }

class Test2SubsetsElement : public Element {
    public:
        Set<NamedElement, Test2SubsetsElement> set1 = Set<NamedElement, Test2SubsetsElement>(this);
        Set<PackageableElement, Test2SubsetsElement> set2 = Set<PackageableElement, Test2SubsetsElement>(this);
        Set<Package, Test2SubsetsElement> sub = Set<Package, Test2SubsetsElement>(this);
        Test2SubsetsElement() : Element(ElementType::ELEMENT) {
            sub.subsets(set1);
            sub.subsets(set2);
        }
        virtual ~Test2SubsetsElement() {
            getID();
        }
};

TEST_F(SetTest, multiSubsetsTest) {
    Manager<> m;
    UmlPtr<Test2SubsetsElement> testEl = m.create<Test2SubsetsElement>();
    Package& clazz = *m.create<Package>();
    testEl->set1.add(clazz);
    ASSERT_FALSE(testEl->set1.empty());
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_TRUE(testEl->set1.contains(clazz.getID()));
    ASSERT_TRUE(testEl->set2.empty());
    ASSERT_EQ(testEl->set2.size(), 0);
    ASSERT_FALSE(testEl->set2.contains(clazz.getID()));
    ASSERT_TRUE(testEl->sub.empty());
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(clazz.getID()));
    Package& inst = *m.create<Package>();
    testEl->set2.add(inst);
    ASSERT_FALSE(testEl->set1.empty());
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_FALSE(testEl->set1.contains(inst.getID()));
    ASSERT_FALSE(testEl->set2.empty());
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_TRUE(testEl->set2.contains(inst.getID()));
    ASSERT_TRUE(testEl->sub.empty());
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(inst.getID()));
    Package& pckg = *m.create<Package>();
    testEl->sub.add(pckg);
    ASSERT_FALSE(testEl->set1.empty());
    ASSERT_EQ(testEl->set1.size(), 2);
    ASSERT_TRUE(testEl->set1.contains(pckg.getID()));
    ASSERT_FALSE(testEl->set2.empty());
    ASSERT_EQ(testEl->set2.size(), 2);
    ASSERT_TRUE(testEl->set2.contains(pckg.getID()));
    ASSERT_FALSE(testEl->sub.empty());
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(pckg.getID()));
}

class Test3SubsetsElement : public Element {
    public:
        Set<NamedElement, Test3SubsetsElement> root = Set<NamedElement, Test3SubsetsElement>(this);
        Set<PackageableElement, Test3SubsetsElement> intermediate = Set<PackageableElement, Test3SubsetsElement>(this);
        Set<Package, Test3SubsetsElement> sub = Set<Package, Test3SubsetsElement>(this);
        Test3SubsetsElement() : Element(ElementType::ELEMENT) {
            intermediate.subsets(root);
            sub.subsets(intermediate);
        }
};

TEST_F(SetTest, removeFromSubsettedSequenceTest) {
    Manager<> m;
    UmlPtr<Test3SubsetsElement> testEl = m.create<Test3SubsetsElement>();
    Package& pckg = *m.create<Package>();
    testEl->sub.add(pckg);
    std::cout << "about to remove element" << std::endl;
    testEl->sub.remove(pckg);
    std::cout << "removed element" << std::endl;
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_EQ(testEl->intermediate.size(), 0);
    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(pckg));
    ASSERT_FALSE(testEl->intermediate.contains(pckg));
    ASSERT_FALSE(testEl->root.contains(pckg));
    testEl->sub.add(pckg);
    std::cout << "about to remove element" << std::endl;
    testEl->intermediate.remove(pckg);
    std::cout << "removed element" << std::endl;
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_EQ(testEl->intermediate.size(), 0);
    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(pckg));
    ASSERT_FALSE(testEl->intermediate.contains(pckg));
    ASSERT_FALSE(testEl->root.contains(pckg));
}

TEST_F(SetTest, addToSetTwice) {
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    Package& p = *m.create<Package>();
    testEl->set.add(p);
    ASSERT_THROW(testEl->set.add(p), SetStateException);
}

class TestElement2 : public Element {

    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    private:
        Set<TestElement2, TestElement2> m_others = Set<TestElement2, TestElement2>(this);
    protected:
    public:
        Set<TestElement2, TestElement2>& getOthers() { return m_others; };
        TestElement2() : Element(ElementType::ELEMENT) {
            m_others.opposite(&TestElement2::getOthers);
        };
};

TEST_F(SetTest, oppositeTest) {
    Manager<> m;
    TestElement2& t1 = *m.create<TestElement2>();
    TestElement2& t2 = *m.create<TestElement2>();
    t1.getOthers().add(t2);
    ASSERT_EQ(t2.getOthers().size(), 1);
    ASSERT_TRUE(t2.getOthers().contains(t1.getID()));
    // ASSERT_EQ(*t2.getOthers().get(t1.getID()), t1);
}

class RedefinedTestElement : public Element {

    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    public:
        Set<Package, RedefinedTestElement> rootSet = Set<Package, RedefinedTestElement>(this);
        Set<Package, RedefinedTestElement> redefiningSet = Set<Package, RedefinedTestElement>(this);
        RedefinedTestElement() : Element(ElementType::ELEMENT) {
            redefiningSet.redefines(rootSet);
        }
        virtual ~RedefinedTestElement() {
            getID();
        }
};

TEST_F(SetTest, setRedefinesTest) {
    Manager<> m;
    UmlPtr<RedefinedTestElement> testEl = m.create<RedefinedTestElement>();
    Package& p = *m.create<Package>();
    testEl->redefiningSet.add(p);
    ASSERT_FALSE(testEl->rootSet.empty());
    ASSERT_FALSE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 1);
    ASSERT_EQ(testEl->rootSet.size(), 1);
    testEl->redefiningSet.remove(p);
    ASSERT_TRUE(testEl->rootSet.empty());
    ASSERT_TRUE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 0);
    ASSERT_EQ(testEl->redefiningSet.size(), 0);
    Package& p2 = *m.create<Package>();
    testEl->rootSet.add(p);
    ASSERT_FALSE(testEl->rootSet.empty());
    ASSERT_FALSE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 1);
    ASSERT_EQ(testEl->redefiningSet.size(), 1);
    testEl->rootSet.remove(p);
    ASSERT_TRUE(testEl->rootSet.empty());
    ASSERT_TRUE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 0);
    ASSERT_EQ(testEl->redefiningSet.size(), 0);
    testEl->rootSet.add(p);
    testEl->redefiningSet.add(p2);
    ASSERT_FALSE(testEl->rootSet.empty());
    ASSERT_FALSE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 2);
    ASSERT_EQ(testEl->redefiningSet.size(), 2);
    Package& p3 = *m.create<Package>();
    testEl->rootSet.add(p3);
    ASSERT_FALSE(testEl->rootSet.empty());
    ASSERT_FALSE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 3);
    ASSERT_EQ(testEl->redefiningSet.size(), 3);
}

class PolicyTestElement;

class TestPolicy {
    public:
        void elementAdded(Package& el, PolicyTestElement& me);
        void elementRemoved(Package& el, PolicyTestElement& me);
};

class PolicyTestElement : public Element {

    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    public:
        Set<Package, PolicyTestElement, TestPolicy> policySet = Set<Package, PolicyTestElement, TestPolicy>(this);
        Set<Package, PolicyTestElement> redefinedSet = Set<Package, PolicyTestElement>(this);
        PolicyTestElement() : Element(ElementType::ELEMENT) {
            redefinedSet.redefines(policySet);
        }
        size_t count = 0;
};

void TestPolicy::elementAdded(__attribute__((unused)) Package& el, PolicyTestElement& me) {
    me.count++;
}

void TestPolicy::elementRemoved(__attribute__((unused)) Package& el, PolicyTestElement& me) {
    me.count--;
}

TEST_F(SetTest, setRedefinedWFunctors) {
    Manager<> m;
    UmlPtr<PolicyTestElement> testEl = m.create<PolicyTestElement>();
    Package& g = *m.create<Package>();
    testEl->redefinedSet.add(g);
    ASSERT_EQ(testEl->count, 1);
    testEl->redefinedSet.remove(g);
    ASSERT_EQ(testEl->count, 0);
}

class TestOrderedSetElement : public Element {
    
    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    public:
        OrderedSet<Package, TestOrderedSetElement> set = OrderedSet<Package, TestOrderedSetElement>(this);
        TestOrderedSetElement() : Element(ElementType::ELEMENT) {}
        ~TestOrderedSetElement() {}
};

TEST_F(SetTest, addToOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    p.setName("1");
    testEl->set.add(p);
    ASSERT_EQ(*testEl->set.front(), p);
    ASSERT_EQ(*testEl->set.back(), p);
    Package& p2 = *m.create<Package>();
    p2.setName("2");
    testEl->set.add(p2);
    ASSERT_EQ(*testEl->set.front(), p);
    ASSERT_EQ(*testEl->set.back(), p2);
    Package& p3 = *m.create<Package>();
    p3.setName("3");
    testEl->set.add(p3);
    ASSERT_EQ(*testEl->set.front(), p);
    ASSERT_EQ(*testEl->set.back(), p3);
    ASSERT_TRUE(testEl->set.contains(p.getID()));
    // ASSERT_TRUE(testEl->set.contains("1"));
    ASSERT_EQ(*testEl->set.get(p.getID()), p);
    // ASSERT_EQ(testEl->set.get("1"), p);
    ASSERT_TRUE(testEl->set.contains(p2.getID()));
    // ASSERT_TRUE(testEl->set.contains("2"));
    ASSERT_EQ(*testEl->set.get(p2.getID()), p2);
    // ASSERT_EQ(testEl->set.get("2"), p2);
    ASSERT_TRUE(testEl->set.contains(p3.getID()));
    // ASSERT_TRUE(testEl->set.contains("3"));
    ASSERT_EQ(*testEl->set.get(p3.getID()), p3);
    // ASSERT_EQ(testEl->set.get("3"), p3);
    Package* ps[] = {&p, &p2, &p3};
    int i = 0;
    for (Package& pckg : testEl->set) {
        ASSERT_EQ(*ps[i], pckg);
        i++;
    }
    ASSERT_EQ(*testEl->set.get(0), p);
    ASSERT_EQ(*testEl->set.get(1), p2);
    ASSERT_EQ(*testEl->set.get(2), p3);
}

class TestElementSubsetsOrderedSets : public Element {
    public:
        OrderedSet<Package, TestElementSubsetsOrderedSets> root = OrderedSet<Package, TestElementSubsetsOrderedSets>(this);
        OrderedSet<Package, TestElementSubsetsOrderedSets> sub = OrderedSet<Package, TestElementSubsetsOrderedSets>(this);
        TestElementSubsetsOrderedSets() : Element(ElementType::ELEMENT) {
            sub.subsets(root);
        }
};

TEST_F(SetTest, subsetOrderedSets) {
    Manager<> m;
    UmlPtr<TestElementSubsetsOrderedSets> testEl = m.create<TestElementSubsetsOrderedSets>();
    Package& p = *m.create<Package>();
    testEl->sub.add(p);
    ASSERT_EQ(*testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.front(), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(*testEl->root.back(), c);
    int i = 0;
    for (Package& pckg : testEl->sub) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (PackageableElement& pckg : testEl->root) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

class TestElementOrderedSubsetsSet : public Element {
    public:
        Set<Package, TestElementOrderedSubsetsSet> root = Set<Package, TestElementOrderedSubsetsSet>(this);
        OrderedSet<Package, TestElementOrderedSubsetsSet> sub = OrderedSet<Package, TestElementOrderedSubsetsSet>(this);
        TestElementOrderedSubsetsSet() : Element(ElementType::ELEMENT) {
            sub.subsets(root);
        }
};

TEST_F(SetTest, orderedSetSubSetsSet) {
    Manager<> m;
    UmlPtr<TestElementOrderedSubsetsSet> testEl = m.create<TestElementOrderedSubsetsSet>();
    Package& p = *m.create<Package>();
    testEl->sub.add(p);
    ASSERT_EQ(*testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.get(p.getID()), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(*testEl->root.get(c.getID()), c);
    int i = 0;
    for (Package& pckg : testEl->sub) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (Package& pckg : testEl->root) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

class TestSingletonElement : public Element {
    public:
        Set<Package, TestSingletonElement> root = Set<Package, TestSingletonElement>(this);
        Singleton<Package, TestSingletonElement> singleton = Singleton<Package, TestSingletonElement>(this);
        TestSingletonElement() : Element(ElementType::ELEMENT) {
            singleton.subsets(root);
        }
};

TEST_F(SetTest, singletonTest) {
    Manager<> m;
    UmlPtr<TestSingletonElement> testEl = m.create<TestSingletonElement>();
    ASSERT_TRUE(testEl->singleton.empty());
    ASSERT_EQ(testEl->singleton.size(), 0);
    Package& p = *m.create<Package>();
    testEl->singleton.set(p);
    ASSERT_EQ(testEl->singleton.size(), 1);
    ASSERT_FALSE(testEl->singleton.empty());
    ASSERT_TRUE(testEl->singleton.get());
    ASSERT_EQ(*testEl->singleton.get(), p);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(*testEl->root.get(p.getID()), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_EQ(testEl->singleton.size(), 1);
    ASSERT_FALSE(testEl->singleton.empty());
    ASSERT_TRUE(testEl->singleton.get());
    ASSERT_EQ(*testEl->singleton.get(), p);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(p.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(*testEl->root.get(p.getID()), p);
    ASSERT_EQ(*testEl->root.get(c.getID()), c);
    testEl->singleton.set(0);
    ASSERT_FALSE(testEl->singleton.get());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_FALSE(testEl->root.contains(p.getID()));
}

class TestSharedSubsetEvenTreeElement : public Element {
    public:
        Set<PackageableElement, TestSharedSubsetEvenTreeElement> root = Set<PackageableElement, TestSharedSubsetEvenTreeElement>(this);
        Set<Package, TestSharedSubsetEvenTreeElement> set1 = Set<Package, TestSharedSubsetEvenTreeElement>(this);
        Set<Package, TestSharedSubsetEvenTreeElement> set2 = Set<Package, TestSharedSubsetEvenTreeElement>(this);
        TestSharedSubsetEvenTreeElement() : Element(ElementType::ELEMENT) {
            set1.subsets(root);
            set2.subsets(root);
        }
};

TEST_F(SetTest, sharedSubsetEvenTreeTest) {
    Manager<> m;
    UmlPtr<TestSharedSubsetEvenTreeElement> testEl = m.create<TestSharedSubsetEvenTreeElement>();
    Package& p1 = *m.create<Package>();
    Package& p2 = *m.create<Package>();
    testEl->set1.add(p1);
    testEl->set2.add(p2);
    std::cout << "test setup finished" << std::endl;
    ASSERT_FALSE(testEl->set1.contains(p2.getID()));
    std::cout << "set1 front: " << testEl->set1.front().id().string() << std::endl;
    ASSERT_EQ(*testEl->set1.front(), p1);
    std::cout << "set2 front: " << testEl->set2.front().id().string() << std::endl;
    ASSERT_EQ(*testEl->set2.front(), p2);
    std::cout << "root front: " << testEl->set2.front().id().string() << std::endl;
    std::cout << "root begin: " << testEl->root.begin()->getID().string() << std::endl;
    ASSERT_EQ(*testEl->root.front(), *testEl->root.begin());
    for (auto& el : testEl->root) {
        std::cout << "current iteration: " << el.getID().string() << std::endl;
        ASSERT_TRUE(testEl->root.contains(el.getID()));
    }
    std::cout << "done with regular iterator" << std::endl;
    for (const ID id : testEl->root.ids()) {
        std::cout << "current iteration: " << id.string() << std::endl;
        ASSERT_TRUE(testEl->root.contains(id));
    }
    std::cout << "done with id iterator" << std::endl;
    testEl->set1.remove(p1);
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.front(), p2);
    ASSERT_EQ(*testEl->set2.front(), p2);
    testEl->set1.add(p1);
    ASSERT_FALSE(testEl->set1.contains(p2.getID()));
    ASSERT_EQ(*testEl->set1.front(), p1);
    ASSERT_EQ(*testEl->set2.front(), p2);
    for (auto& el : testEl->root) {
        ASSERT_TRUE(testEl->root.contains(el.getID()));
    }
    for (const ID id : testEl->root.ids()) {
        ASSERT_TRUE(testEl->root.contains(id));
    }
    testEl->set2.remove(p2);
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.front(), p1);
    ASSERT_EQ(*testEl->set1.front(), p1);
}

class TestTwoRootSubSetElement : public Element {
    public:
        Set<NamedElement, TestTwoRootSubSetElement> root1 = Set<NamedElement, TestTwoRootSubSetElement>(this);
        Set<PackageableElement, TestTwoRootSubSetElement> root2 = Set<PackageableElement, TestTwoRootSubSetElement>(this);
        Set<Package, TestTwoRootSubSetElement> sub = Set<Package, TestTwoRootSubSetElement>(this);
        TestTwoRootSubSetElement() : Element(ElementType::ELEMENT) {
            sub.subsets(root1);
            sub.subsets(root2);
        }
        virtual ~TestTwoRootSubSetElement() {
            getID();
        }
};

TEST_F(SetTest, multiRootWithinRootTest) {
    Manager<> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    Package& c = *m.create<Package>();
    Package& p = *m.create<Package>();
    testEl->root1.add(c);
    testEl->sub.add(p);
    ASSERT_EQ(testEl->root1.size(), 2);
    ASSERT_EQ(testEl->root2.size(), 1);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_TRUE(testEl->root1.contains(c.getID()));
    ASSERT_FALSE(testEl->root2.contains(c.getID()));
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root1.contains(p.getID()));
    ASSERT_TRUE(testEl->root2.contains(p.getID()));
    ASSERT_TRUE(testEl->sub.contains(p.getID()));
}

TEST_F(SetTest, multiSubsetsOneElement) {
    Manager<> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    Package& p = *m.create<Package>();
    testEl->sub.add(p);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_EQ(testEl->root1.size(), 1);
    ASSERT_EQ(testEl->root2.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(p.getID()));
    ASSERT_TRUE(testEl->root2.contains(p.getID()));
    ASSERT_TRUE(testEl->root1.contains(p.getID()));
}

TEST_F(SetTest, multiRootAddToSubsetTest) {
    Manager<> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    PackagePtr pckg = m.create<Package>();
    testEl->root1.add(pckg);
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_EQ(testEl->root1.size(), 1);
    ASSERT_EQ(testEl->root2.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(pckg.id()));
    ASSERT_FALSE(testEl->root2.contains(pckg.id()));
    ASSERT_TRUE(testEl->root1.contains(pckg.id()));
    testEl->sub.add(pckg);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_EQ(testEl->root1.size(), 1);
    ASSERT_EQ(testEl->root2.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(pckg.id()));
    ASSERT_TRUE(testEl->root2.contains(pckg.id()));
    ASSERT_TRUE(testEl->root1.contains(pckg.id()));
}

class TestComplexSubsetElement : public Element {
    template<typename SerializationPolicy, typename PersistencePolciy> friend class Manager;
    
    public:
        Set<Element, TestComplexSubsetElement> rootSet = Set<Element, TestComplexSubsetElement>(this);
        Set<NamedElement, TestComplexSubsetElement> rightSet1 = Set<NamedElement, TestComplexSubsetElement>(this);
        Set<NamedElement, TestComplexSubsetElement> leftSet1 = Set<NamedElement, TestComplexSubsetElement>(this);
        Set<PackageableElement, TestComplexSubsetElement> rightSet2 = Set<PackageableElement, TestComplexSubsetElement>(this);
        Set<PackageableElement, TestComplexSubsetElement> leftSet2 = Set<PackageableElement, TestComplexSubsetElement>(this);
        Set<PackageableElement, TestComplexSubsetElement> leftSet2a = Set<PackageableElement, TestComplexSubsetElement>(this);
        Set<PackageableElement, TestComplexSubsetElement> rightSet2a = Set<PackageableElement, TestComplexSubsetElement>(this);
        TestComplexSubsetElement() : Element(ElementType::ELEMENT) {
            rightSet1.subsets(rootSet);
            leftSet1.subsets(rootSet);
            rightSet2.subsets(rightSet1);
            leftSet2.subsets(leftSet1);
            rightSet2a.subsets(rightSet1);
            leftSet2a.subsets(leftSet1);
        }
        ~TestComplexSubsetElement() {}
};

TEST_F(SetTest, twoWayMultiSetSplitTest) {
    Manager<> m;
    UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
    Package& rightP = *m.create<Package>();
    Package& leftP = *m.create<Package>();
    testEl->rightSet2.add(rightP);
    testEl->leftSet2.add(leftP);
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_EQ(testEl->rightSet1.size(), 1);
    ASSERT_EQ(testEl->leftSet2.size(), 1);
    ASSERT_EQ(testEl->leftSet1.size(), 1);
    ASSERT_EQ(testEl->rootSet.size(), 2);
    ASSERT_TRUE(testEl->rightSet2.contains(rightP.getID()));
    ASSERT_TRUE(testEl->rightSet1.contains(rightP.getID()));
    ASSERT_TRUE(testEl->rootSet.contains(rightP.getID()));
    ASSERT_TRUE(testEl->leftSet1.contains(leftP.getID()));
    ASSERT_TRUE(testEl->leftSet2.contains(leftP.getID()));
    ASSERT_TRUE(testEl->rootSet.contains(leftP.getID()));
    ASSERT_FALSE(testEl->rightSet2.contains(leftP.getID()));
    ASSERT_FALSE(testEl->rightSet1.contains(leftP.getID()));
    ASSERT_FALSE(testEl->leftSet2.contains(rightP.getID()));
    ASSERT_FALSE(testEl->leftSet1.contains(rightP.getID()));
    ASSERT_FALSE(testEl->rightSet2a.contains(rightP.getID()));
    ASSERT_FALSE(testEl->rightSet2a.contains(leftP.getID()));
    ASSERT_FALSE(testEl->leftSet2a.contains(rightP.getID()));
    ASSERT_FALSE(testEl->leftSet2a.contains(leftP.getID()));

    Package& rightP2 = *m.create<Package>();
    Package& leftP2 = *m.create<Package>();
    testEl->rightSet1.add(rightP2);
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_EQ(testEl->rightSet1.size(), 2);
    ASSERT_EQ(testEl->leftSet2.size(), 1);
    ASSERT_EQ(testEl->leftSet1.size(), 1);
    ASSERT_EQ(testEl->rootSet.size(), 3);
    ASSERT_FALSE(testEl->rightSet2.contains(rightP2.getID()));
    ASSERT_TRUE(testEl->rightSet1.contains(rightP2.getID()));
    ASSERT_TRUE(testEl->rootSet.contains(rightP2.getID()));
    ASSERT_FALSE(testEl->rightSet2a.contains(rightP2.getID()));
    ASSERT_FALSE(testEl->leftSet1.contains(rightP2.getID()));
    ASSERT_FALSE(testEl->leftSet2.contains(rightP2.getID()));
    ASSERT_FALSE(testEl->leftSet2a.contains(rightP2.getID()));
    ASSERT_TRUE(testEl->rightSet2.contains(rightP.getID()));
    ASSERT_TRUE(testEl->rightSet1.contains(rightP.getID()));
    ASSERT_TRUE(testEl->rootSet.contains(rightP.getID()));
    ASSERT_TRUE(testEl->leftSet1.contains(leftP.getID()));
    ASSERT_TRUE(testEl->leftSet2.contains(leftP.getID()));
    ASSERT_TRUE(testEl->rootSet.contains(leftP.getID()));
    ASSERT_FALSE(testEl->rightSet2.contains(leftP.getID()));
    ASSERT_FALSE(testEl->rightSet1.contains(leftP.getID()));
    ASSERT_FALSE(testEl->leftSet2.contains(rightP.getID()));
    ASSERT_FALSE(testEl->leftSet1.contains(rightP.getID()));
    ASSERT_FALSE(testEl->rightSet2a.contains(rightP.getID()));
    ASSERT_FALSE(testEl->rightSet2a.contains(leftP.getID()));
    ASSERT_FALSE(testEl->leftSet2a.contains(rightP.getID()));
    ASSERT_FALSE(testEl->leftSet2a.contains(leftP.getID()));

    testEl->leftSet1.add(leftP2);
    ASSERT_EQ(testEl->rootSet.size(), 4);
    ASSERT_EQ(testEl->rightSet1.size(), 2);
    ASSERT_EQ(testEl->leftSet1.size(), 2);
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_EQ(testEl->leftSet2.size(), 1);
    ASSERT_TRUE(testEl->rootSet.contains(rightP2));
    ASSERT_TRUE(testEl->rightSet1.contains(rightP2));
    ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
    ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
    ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
    ASSERT_TRUE(testEl->rootSet.contains(leftP2));
    ASSERT_TRUE(testEl->leftSet1.contains(leftP2));
    ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
    ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
    ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
}

TEST_F(SetTest, complexDividerNodeTest) {
    Manager<> m;
    UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
    PackagePtr a = m.create<Package>();
    PackagePtr b = m.create<Package>();
    PackagePtr c = m.create<Package>();
    PackagePtr d = m.create<Package>();
    
    testEl->rightSet2.add(a);
    ASSERT_EQ(testEl->rootSet.size(), 1);
    ASSERT_TRUE(testEl->rootSet.contains(a));
    ASSERT_FALSE(testEl->rootSet.contains(b));
    ASSERT_FALSE(testEl->rootSet.contains(c));
    ASSERT_FALSE(testEl->rootSet.contains(d));
    ASSERT_EQ(testEl->rightSet1.size(), 1);
    ASSERT_TRUE(testEl->rightSet1.contains(a));
    ASSERT_FALSE(testEl->rightSet1.contains(b));
    ASSERT_FALSE(testEl->rightSet1.contains(c));
    ASSERT_FALSE(testEl->rightSet1.contains(d));
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_TRUE(testEl->rightSet2.contains(a));
    ASSERT_FALSE(testEl->rightSet2.contains(b));
    ASSERT_FALSE(testEl->rightSet2.contains(c));
    ASSERT_FALSE(testEl->rightSet2.contains(d));
    ASSERT_EQ(testEl->rightSet2a.size(), 0);
    ASSERT_FALSE(testEl->rightSet2a.contains(a));
    ASSERT_FALSE(testEl->rightSet2a.contains(b));
    ASSERT_FALSE(testEl->rightSet2a.contains(c));
    ASSERT_FALSE(testEl->rightSet2a.contains(d));
    ASSERT_EQ(testEl->leftSet1.size(), 0);
    ASSERT_FALSE(testEl->leftSet1.contains(a));
    ASSERT_FALSE(testEl->leftSet1.contains(b));
    ASSERT_FALSE(testEl->leftSet1.contains(c));
    ASSERT_FALSE(testEl->leftSet1.contains(d));
    ASSERT_EQ(testEl->leftSet2.size(), 0);
    ASSERT_FALSE(testEl->leftSet2.contains(a));
    ASSERT_FALSE(testEl->leftSet2.contains(b));
    ASSERT_FALSE(testEl->leftSet2.contains(c));
    ASSERT_FALSE(testEl->leftSet2.contains(d));
    ASSERT_EQ(testEl->leftSet2a.size(), 0);
    ASSERT_FALSE(testEl->leftSet2a.contains(a));
    ASSERT_FALSE(testEl->leftSet2a.contains(b));
    ASSERT_FALSE(testEl->leftSet2a.contains(c));
    ASSERT_FALSE(testEl->leftSet2a.contains(d));

    testEl->rightSet2a.add(b);
    ASSERT_EQ(testEl->rootSet.size(), 2);
    ASSERT_TRUE(testEl->rootSet.contains(a));
    ASSERT_TRUE(testEl->rootSet.contains(b));
    ASSERT_FALSE(testEl->rootSet.contains(c));
    ASSERT_FALSE(testEl->rootSet.contains(d));
    ASSERT_EQ(testEl->rightSet1.size(), 2);
    ASSERT_TRUE(testEl->rightSet1.contains(a));
    ASSERT_TRUE(testEl->rightSet1.contains(b));
    ASSERT_FALSE(testEl->rightSet1.contains(c));
    ASSERT_FALSE(testEl->rightSet1.contains(d));
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_TRUE(testEl->rightSet2.contains(a));
    ASSERT_FALSE(testEl->rightSet2.contains(b));
    ASSERT_FALSE(testEl->rightSet2.contains(c));
    ASSERT_FALSE(testEl->rightSet2.contains(d));
    ASSERT_EQ(testEl->rightSet2a.size(), 1);
    ASSERT_FALSE(testEl->rightSet2a.contains(a));
    ASSERT_TRUE(testEl->rightSet2a.contains(b));
    ASSERT_FALSE(testEl->rightSet2a.contains(c));
    ASSERT_FALSE(testEl->rightSet2a.contains(d));
    ASSERT_EQ(testEl->leftSet1.size(), 0);
    ASSERT_FALSE(testEl->leftSet1.contains(a));
    ASSERT_FALSE(testEl->leftSet1.contains(b));
    ASSERT_FALSE(testEl->leftSet1.contains(c));
    ASSERT_FALSE(testEl->leftSet1.contains(d));
    ASSERT_EQ(testEl->leftSet2.size(), 0);
    ASSERT_FALSE(testEl->leftSet2.contains(a));
    ASSERT_FALSE(testEl->leftSet2.contains(b));
    ASSERT_FALSE(testEl->leftSet2.contains(c));
    ASSERT_FALSE(testEl->leftSet2.contains(d));
    ASSERT_EQ(testEl->leftSet2a.size(), 0);
    ASSERT_FALSE(testEl->leftSet2a.contains(a));
    ASSERT_FALSE(testEl->leftSet2a.contains(b));
    ASSERT_FALSE(testEl->leftSet2a.contains(c));
    ASSERT_FALSE(testEl->leftSet2a.contains(d));

    testEl->leftSet2.add(c);
    ASSERT_EQ(testEl->rootSet.size(), 3);
    ASSERT_TRUE(testEl->rootSet.contains(a));
    ASSERT_TRUE(testEl->rootSet.contains(b));
    ASSERT_TRUE(testEl->rootSet.contains(c));
    ASSERT_FALSE(testEl->rootSet.contains(d));
    ASSERT_EQ(testEl->rightSet1.size(), 2);
    ASSERT_TRUE(testEl->rightSet1.contains(a));
    ASSERT_TRUE(testEl->rightSet1.contains(b));
    ASSERT_FALSE(testEl->rightSet1.contains(c));
    ASSERT_FALSE(testEl->rightSet1.contains(d));
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_TRUE(testEl->rightSet2.contains(a));
    ASSERT_FALSE(testEl->rightSet2.contains(b));
    ASSERT_FALSE(testEl->rightSet2.contains(c));
    ASSERT_FALSE(testEl->rightSet2.contains(d));
    ASSERT_EQ(testEl->rightSet2a.size(), 1);
    ASSERT_FALSE(testEl->rightSet2a.contains(a));
    ASSERT_TRUE(testEl->rightSet2a.contains(b));
    ASSERT_FALSE(testEl->rightSet2a.contains(c));
    ASSERT_FALSE(testEl->rightSet2a.contains(d));
    ASSERT_EQ(testEl->leftSet1.size(), 1);
    ASSERT_FALSE(testEl->leftSet1.contains(a));
    ASSERT_FALSE(testEl->leftSet1.contains(b));
    ASSERT_TRUE(testEl->leftSet1.contains(c));
    ASSERT_FALSE(testEl->leftSet1.contains(d));
    ASSERT_EQ(testEl->leftSet2.size(), 1);
    ASSERT_FALSE(testEl->leftSet2.contains(a));
    ASSERT_FALSE(testEl->leftSet2.contains(b));
    ASSERT_TRUE(testEl->leftSet2.contains(c));
    ASSERT_FALSE(testEl->leftSet2.contains(d));
    ASSERT_EQ(testEl->leftSet2a.size(), 0);
    ASSERT_FALSE(testEl->leftSet2a.contains(a));
    ASSERT_FALSE(testEl->leftSet2a.contains(b));
    ASSERT_FALSE(testEl->leftSet2a.contains(c));
    ASSERT_FALSE(testEl->leftSet2a.contains(d));

    testEl->leftSet2a.add(d);
    ASSERT_EQ(testEl->rootSet.size(), 4);
    ASSERT_TRUE(testEl->rootSet.contains(a));
    ASSERT_TRUE(testEl->rootSet.contains(b));
    ASSERT_TRUE(testEl->rootSet.contains(c));
    ASSERT_TRUE(testEl->rootSet.contains(d));
    ASSERT_EQ(testEl->rightSet1.size(), 2);
    ASSERT_TRUE(testEl->rightSet1.contains(a));
    ASSERT_TRUE(testEl->rightSet1.contains(b));
    ASSERT_FALSE(testEl->rightSet1.contains(c));
    ASSERT_FALSE(testEl->rightSet1.contains(d));
    ASSERT_EQ(testEl->rightSet2.size(), 1);
    ASSERT_TRUE(testEl->rightSet2.contains(a));
    ASSERT_FALSE(testEl->rightSet2.contains(b));
    ASSERT_FALSE(testEl->rightSet2.contains(c));
    ASSERT_FALSE(testEl->rightSet2.contains(d));
    ASSERT_EQ(testEl->rightSet2a.size(), 1);
    ASSERT_FALSE(testEl->rightSet2a.contains(a));
    ASSERT_TRUE(testEl->rightSet2a.contains(b));
    ASSERT_FALSE(testEl->rightSet2a.contains(c));
    ASSERT_FALSE(testEl->rightSet2a.contains(d));
    ASSERT_EQ(testEl->leftSet1.size(), 2);
    ASSERT_FALSE(testEl->leftSet1.contains(a));
    ASSERT_FALSE(testEl->leftSet1.contains(b));
    ASSERT_TRUE(testEl->leftSet1.contains(c));
    ASSERT_TRUE(testEl->leftSet1.contains(d));
    ASSERT_EQ(testEl->leftSet2.size(), 1);
    ASSERT_FALSE(testEl->leftSet2.contains(a));
    ASSERT_FALSE(testEl->leftSet2.contains(b));
    ASSERT_TRUE(testEl->leftSet2.contains(c));
    ASSERT_FALSE(testEl->leftSet2.contains(d));
    ASSERT_EQ(testEl->leftSet2a.size(), 1);
    ASSERT_FALSE(testEl->leftSet2a.contains(a));
    ASSERT_FALSE(testEl->leftSet2a.contains(b));
    ASSERT_FALSE(testEl->leftSet2a.contains(c));
    ASSERT_TRUE(testEl->leftSet2a.contains(d));
}

TEST_F(SetTest, AddElementThatIsInSuperSet) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    Package& p = *m.create<Package>();
    testEl->root.add(p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_TRUE(testEl->sub.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_TRUE(testEl->root.contains(p.getID()));
    ASSERT_FALSE(testEl->sub.contains(p.getID()));
    testEl->sub.add(p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_FALSE(testEl->sub.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_TRUE(testEl->root.contains(p.getID()));
    ASSERT_TRUE(testEl->sub.contains(p.getID()));
}

TEST_F(SetTest, removeFirstElementFromOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    testEl->set.add(p);
    testEl->set.remove(p);
    ASSERT_TRUE(testEl->set.empty());
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_EQ(testEl->set.front(), PackagePtr());
}

TEST_F(SetTest, removeLastElementFromOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    testEl->set.add(p);
    Package& p2 = *m.create<Package>();
    testEl->set.add(p2);
    testEl->set.remove(p2);
    ASSERT_EQ(*testEl->set.back(), p);
    ASSERT_FALSE(testEl->set.contains(p2.getID()));
    ASSERT_EQ(testEl->set.size(), 1);
}

TEST_F(SetTest, removeMiddleElementFromOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    testEl->set.add(p);
    Package& p2 = *m.create<Package>();
    testEl->set.add(p2);
    Package& p3 = *m.create<Package>();
    testEl->set.add(p3);
    testEl->set.remove(p2);
    auto it = testEl->set.begin();
    it++;
    ASSERT_EQ(*it, p3);
}

// uncomment?
// TEST_F(SetTest, removeFromSuperSetTest) {
//     Manager<> m;
//     UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
//     Package& pckg = *m.create<Package>();
//     testEl->sub.add(pckg);
//     testEl->sub.removeFromJustThisSet(pckg.getID());
//     ASSERT_FALSE(testEl->root.empty());
//     ASSERT_TRUE(testEl->sub.empty());
//     ASSERT_EQ(testEl->root.size(), 1);
//     ASSERT_EQ(testEl->sub.size(), 0);
//     ASSERT_TRUE(testEl->root.contains(pckg.getID()));
//     ASSERT_FALSE(testEl->sub.contains(pckg.getID()));
// }

class TestTripleSuperSetElement : public Element {
    public:
        Set<Element, TestTripleSuperSetElement> root = Set<Element, TestTripleSuperSetElement>(this);
        Set<NamedElement, TestTripleSuperSetElement> set1 = Set<NamedElement, TestTripleSuperSetElement>(this);
        Set<PackageableElement, TestTripleSuperSetElement> set2 = Set<PackageableElement, TestTripleSuperSetElement>(this);
        Set<Package, TestTripleSuperSetElement> set3 = Set<Package, TestTripleSuperSetElement>(this);
        TestTripleSuperSetElement() : Element(ElementType::ELEMENT) {
            set1.subsets(root);
            set2.subsets(root);
            set3.subsets(root);
        }
};

TEST_F(SetTest, tripleRemovePlacholder) {
    Manager<> m;
    UmlPtr<TestTripleSuperSetElement> testEl = m.create<TestTripleSuperSetElement>();
    Package& pckg1 = *m.create<Package>();
    Package& pckg2 = *m.create<Package>();
    Package& pckg3 = *m.create<Package>();
    pckg1.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    pckg2.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
    pckg3.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");

    testEl->set1.add(pckg1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg1));
    ASSERT_TRUE(testEl->set1.contains(pckg1));
    testEl->set2.add(pckg2);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg2));
    ASSERT_TRUE(testEl->set2.contains(pckg2));
    testEl->set3.add(pckg3);

    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_EQ(testEl->set3.size(), 1);

    ASSERT_TRUE(testEl->root.contains(pckg1));
    ASSERT_TRUE(testEl->set1.contains(pckg1));
    ASSERT_FALSE(testEl->set2.contains(pckg1));
    ASSERT_FALSE(testEl->set3.contains(pckg1));

    ASSERT_TRUE(testEl->root.contains(pckg2));
    ASSERT_FALSE(testEl->set1.contains(pckg2));
    ASSERT_TRUE(testEl->set2.contains(pckg2));
    ASSERT_FALSE(testEl->set3.contains(pckg2));

    ASSERT_TRUE(testEl->root.contains(pckg3));
    ASSERT_FALSE(testEl->set1.contains(pckg3));
    ASSERT_FALSE(testEl->set2.contains(pckg3));
    ASSERT_TRUE(testEl->set3.contains(pckg3));

    testEl->set1.remove(pckg1);

    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_EQ(testEl->set1.size(), 0);
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_EQ(testEl->set3.size(), 1);

    ASSERT_FALSE(testEl->root.contains(pckg1));
    ASSERT_FALSE(testEl->set1.contains(pckg1));
    ASSERT_FALSE(testEl->set2.contains(pckg1));
    ASSERT_FALSE(testEl->set3.contains(pckg1));

    ASSERT_TRUE(testEl->root.contains(pckg2));
    ASSERT_FALSE(testEl->set1.contains(pckg2));
    ASSERT_TRUE(testEl->set2.contains(pckg2));
    ASSERT_FALSE(testEl->set3.contains(pckg2));

    ASSERT_TRUE(testEl->root.contains(pckg3));
    ASSERT_FALSE(testEl->set1.contains(pckg3));
    ASSERT_FALSE(testEl->set2.contains(pckg3));
    ASSERT_TRUE(testEl->set3.contains(pckg3));

    testEl->set2.remove(pckg2);

    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->set1.size(), 0);
    ASSERT_EQ(testEl->set2.size(), 0);
    ASSERT_EQ(testEl->set3.size(), 1);

    ASSERT_FALSE(testEl->root.contains(pckg1));
    ASSERT_FALSE(testEl->set1.contains(pckg1));
    ASSERT_FALSE(testEl->set2.contains(pckg1));
    ASSERT_FALSE(testEl->set3.contains(pckg1));

    ASSERT_FALSE(testEl->root.contains(pckg2));
    ASSERT_FALSE(testEl->set1.contains(pckg2));
    ASSERT_FALSE(testEl->set2.contains(pckg2));
    ASSERT_FALSE(testEl->set3.contains(pckg2));

    ASSERT_TRUE(testEl->root.contains(pckg3));
    ASSERT_FALSE(testEl->set1.contains(pckg3));
    ASSERT_FALSE(testEl->set2.contains(pckg3));
    ASSERT_TRUE(testEl->set3.contains(pckg3));

    testEl->set3.remove(pckg3);

    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_EQ(testEl->set1.size(), 0);
    ASSERT_EQ(testEl->set2.size(), 0);
    ASSERT_EQ(testEl->set3.size(), 0);

    ASSERT_FALSE(testEl->root.contains(pckg1));
    ASSERT_FALSE(testEl->set1.contains(pckg1));
    ASSERT_FALSE(testEl->set2.contains(pckg1));
    ASSERT_FALSE(testEl->set3.contains(pckg1));

    ASSERT_FALSE(testEl->root.contains(pckg2));
    ASSERT_FALSE(testEl->set1.contains(pckg2));
    ASSERT_FALSE(testEl->set2.contains(pckg2));
    ASSERT_FALSE(testEl->set3.contains(pckg2));

    ASSERT_FALSE(testEl->root.contains(pckg3));
    ASSERT_FALSE(testEl->set1.contains(pckg3));
    ASSERT_FALSE(testEl->set2.contains(pckg3));
    ASSERT_FALSE(testEl->set3.contains(pckg3));
}

class TestDiamondSuperSetElement : public Element
{
public:
    Set<Element, TestDiamondSuperSetElement> root = Set<Element, TestDiamondSuperSetElement>(this);
    Set<NamedElement, TestDiamondSuperSetElement> left = Set<NamedElement, TestDiamondSuperSetElement>(this);
    Set<PackageableElement, TestDiamondSuperSetElement> right = Set<PackageableElement, TestDiamondSuperSetElement>(this);
    Set<Package, TestDiamondSuperSetElement> bottom = Set<Package, TestDiamondSuperSetElement>(this);
    TestDiamondSuperSetElement() : Element(ElementType::ELEMENT)
    {
        left.subsets(root);
        right.subsets(root);
        bottom.subsets(left);
        bottom.subsets(right);
    }
};

TEST_F(SetTest, VeryBasicDiamondSubsetTest)
{
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
}

TEST_F(SetTest, TwoElementsDiamondSubsetTest)
{
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));

    testEl->left.add(pckg2);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
}

TEST_F(SetTest, threeElementDiamondSubsetTest)
{
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    PackagePtr pckg3 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));

    testEl->left.add(pckg2);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));

    testEl->right.add(pckg3);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_FALSE(testEl->left.contains(pckg3.id()));
    ASSERT_TRUE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));
}

TEST_F(SetTest, fourElementDiamondSubsetTest)
{
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    PackagePtr pckg3 = m.create<Package>();
    PackagePtr pckg4 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));

    testEl->left.add(pckg2);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));

    testEl->right.add(pckg3);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_FALSE(testEl->left.contains(pckg3.id()));
    ASSERT_TRUE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));

    testEl->root.add(pckg4);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 4);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_TRUE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_FALSE(testEl->left.contains(pckg3.id()));
    ASSERT_TRUE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg4.id()));
    ASSERT_FALSE(testEl->left.contains(pckg4.id()));
    ASSERT_FALSE(testEl->right.contains(pckg4.id()));
    ASSERT_TRUE(testEl->root.contains(pckg4.id()));
}

TEST_F(SetTest, simpleRemoveFromDiamondSubset) {
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    testEl->bottom.remove(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 0);
    ASSERT_EQ(testEl->left.size(), 0);
    ASSERT_EQ(testEl->right.size(), 0);
    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_FALSE(testEl->bottom.contains(pckg1.id()));
    ASSERT_FALSE(testEl->left.contains(pckg1.id()));
    ASSERT_FALSE(testEl->right.contains(pckg1.id()));
    ASSERT_FALSE(testEl->root.contains(pckg1.id()));
}

TEST_F(SetTest, removefromDiamondSubset) {
    Manager<> m;
    PackagePtr pckg1 = m.create<Package>();
    PackagePtr pckg2 = m.create<Package>();
    PackagePtr pckg3 = m.create<Package>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->bottom.add(pckg1);
    testEl->right.add(pckg2);
    testEl->left.add(pckg3);
    testEl->bottom.remove(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 0);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_FALSE(testEl->bottom.contains(pckg1.id()));
    ASSERT_FALSE(testEl->left.contains(pckg1.id()));
    ASSERT_FALSE(testEl->right.contains(pckg1.id()));
    ASSERT_FALSE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_TRUE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_TRUE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));

    testEl->bottom.add(pckg1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_TRUE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_TRUE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));

    testEl->right.remove(pckg2);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_FALSE(testEl->right.contains(pckg2.id()));
    ASSERT_FALSE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_TRUE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));

    testEl->right.add(pckg2);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_TRUE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_TRUE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));

    testEl->left.remove(pckg3);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_TRUE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_FALSE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_FALSE(testEl->root.contains(pckg3.id()));

    testEl->left.add(pckg3);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_EQ(testEl->left.size(), 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_EQ(testEl->root.size(), 3);
    ASSERT_TRUE(testEl->bottom.contains(pckg1.id()));
    ASSERT_TRUE(testEl->left.contains(pckg1.id()));
    ASSERT_TRUE(testEl->right.contains(pckg1.id()));
    ASSERT_TRUE(testEl->root.contains(pckg1.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg2.id()));
    ASSERT_FALSE(testEl->left.contains(pckg2.id()));
    ASSERT_TRUE(testEl->right.contains(pckg2.id()));
    ASSERT_TRUE(testEl->root.contains(pckg2.id()));
    ASSERT_FALSE(testEl->bottom.contains(pckg3.id()));
    ASSERT_TRUE(testEl->left.contains(pckg3.id()));
    ASSERT_FALSE(testEl->right.contains(pckg3.id()));
    ASSERT_TRUE(testEl->root.contains(pckg3.id()));
}

// TODO more complex set

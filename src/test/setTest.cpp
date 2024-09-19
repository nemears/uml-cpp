#include "gtest/gtest.h"
#include <tuple>
#include "test/uml-cpp-paths.h"

#include "uml/managers/baseElement.h"
#include "uml/managers/manager.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"
#include "uml/types/package.h"
#include "uml/uml-stable.h"
#include "uml/set/orderedSet.h"

namespace UML {
class SetTest : public ::testing::Test {
   
};

class TestPackage;
class TestPackageSetElement;
class TestSubsetsElement;
class Test2SubsetsElement;
class Test3SubsetsElement;
class TestElement2;
class RedefinedTestElement;
class PolicyTestElement;
class TestOrderedSetElement;
class TestElementSubsetsOrderedSets;
class TestElementOrderedSubsetsSet;
class TestSingletonElement;
class TestSharedSubsetEvenTreeElement;
class TestTwoRootSubSetElement;
class TestComplexSubsetElement;
class TestTripleSuperSetElement;
class TestDiamondSuperSetElement;

typedef std::tuple<
        TestPackage, 
        TestPackageSetElement,
        TestSubsetsElement,
        Test2SubsetsElement,
        Test3SubsetsElement,
        TestElement2,
        RedefinedTestElement,
        PolicyTestElement,
        TestOrderedSetElement,
        TestElementSubsetsOrderedSets,
        TestElementOrderedSubsetsSet,
        TestSingletonElement,
        TestSharedSubsetEvenTreeElement,
        TestTwoRootSubSetElement,
        TestComplexSubsetElement,
        TestTripleSuperSetElement,
        TestDiamondSuperSetElement
    > TestTypes;

class TestPackage : public BaseElement<TestTypes> {
    public:
        TestPackage(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {}
        typedef TypeInfo<std::tuple<>, TestPackage> Info;
};

template <>
struct ElementInfo<TestPackage> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"TestPackage"};
};

class TestPackageSetElement : public BaseElement<TestTypes> {
    friend class Creator<TestPackageSetElement>;
    public:
        Set<TestPackage, TestPackageSetElement> set = Set<TestPackage, TestPackageSetElement>(this);
        TestPackageSetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {}
        typedef TypeInfo<std::tuple<>, TestPackageSetElement> Info;
};

template<>
struct ElementInfo<TestPackageSetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"TestPackageSetElement"};
};

TEST_F(SetTest, basicSetTest) {
    size_t numPackages = 20;
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    TestPackage& pckg = *m.create<TestPackage>();
    testEl->set.add(pckg);
    ASSERT_FALSE(testEl->set.empty());
    ASSERT_EQ(*testEl->set.get(pckg.getID()), pckg);
    std::vector<ID> ids(numPackages);
    ids[0] = pckg.getID();
    for (size_t i = 0; i < numPackages - 1; i++) {
        TestPackage& p = *m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg4 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg5 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    pckg1->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA1"));
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    pckg2->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA2"));
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
    pckg3->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA3"));
    UmlPtr<TestPackage> pckg4 = m.create<TestPackage>();
    pckg4->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA4"));
    UmlPtr<TestPackage> pckg5 = m.create<TestPackage>();
    pckg5->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA5"));
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
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    pckg1->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA1"));
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    pckg2->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA2"));
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
    pckg3->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA3"));
    UmlPtr<TestPackage> pckg4 = m.create<TestPackage>();
    pckg4->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA4"));
    UmlPtr<TestPackage> pckg5 = m.create<TestPackage>();
    pckg5->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA5"));
    UmlPtr<TestPackage> pckg6 = m.create<TestPackage>();
    pckg5->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA6"));
    UmlPtr<TestPackage> pckg7 = m.create<TestPackage>();
    pckg5->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA7"));
    UmlPtr<TestPackage> pckg8 = m.create<TestPackage>();
    pckg8->setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAA8"));
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
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg4 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg5 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg6 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg7 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg8 = m.create<TestPackage>();
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

class TestSubsetsElement : public BaseElement<TestTypes> {
    friend class Creator<TestSubsetsElement>;
    public:
        Set<TestPackage, TestSubsetsElement> root = Set<TestPackage, TestSubsetsElement>(this);
        Set<TestPackage, TestSubsetsElement> sub = Set<TestPackage, TestSubsetsElement>(this);
        TestSubsetsElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            sub.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestSubsetsElement> Info;
};

template <>
struct ElementInfo<TestSubsetsElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"TestSubsetsElement"};
};

TEST_F(SetTest, basicSubsetsTest) {
    Manager<TestTypes> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    TestPackage& pckg = *m.create<TestPackage>();
    testEl->sub.add(pckg);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(pckg.getID()));
    ASSERT_EQ(*testEl->sub.get(pckg.getID()), pckg);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg.getID()));
    ASSERT_EQ(*testEl->root.get(pckg.getID()), pckg);
    TestPackage& clazz = *m.create<TestPackage>();
    testEl->root.add(clazz);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(clazz.getID()));
    ASSERT_EQ(*testEl->root.get(clazz.getID()), clazz);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_FALSE(testEl->sub.contains(clazz.getID()));
    ASSERT_FALSE(testEl->sub.get(clazz.getID()));
}

TEST_F(SetTest, iterateOverSubsettedElement) {
    Manager<TestTypes> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    UmlPtr<TestPackage> a = m.create<TestPackage>();
    UmlPtr<TestPackage> b = m.create<TestPackage>();
    UmlPtr<TestPackage> c = m.create<TestPackage>();
    a->setID(ID::fromString("&90IAqvc&wUnewHz0xLI4fPYNXUe"));
    b->setID(ID::fromString("04zH_c&oPfM5KXPqJXA0_7AzIzcy"));
    c->setID(ID::fromString("buttEyLdYRCk_zbuttYrQyy42yHr")); 
    // inorder
    //
    // &90IAqvc&wUnewHz0xLI4fPYNXUe
    // buttEyLdYRCk_zbuttYrQyy42yHr
    // 04zH_c&oPfM5KXPqJXA0_7AzIzcy
    //
    // WwowBIeuOqdXecMITJkHZWbnD94G
    testEl->root.add(a);
    std::unordered_set<std::shared_ptr<TestPackage>> pckgs;
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
//     UmlManager m;
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

class Test2SubsetsElement : public BaseElement<TestTypes> {
    friend class Creator<Test2SubsetsElement>;
    public:
        Set<TestPackage, Test2SubsetsElement> set1 = Set<TestPackage, Test2SubsetsElement>(this);
        Set<TestPackage, Test2SubsetsElement> set2 = Set<TestPackage, Test2SubsetsElement>(this);
        Set<TestPackage, Test2SubsetsElement> sub = Set<TestPackage, Test2SubsetsElement>(this);
        Test2SubsetsElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            sub.subsets(set1);
            sub.subsets(set2);
        }
        typedef TypeInfo<std::tuple<>, Test2SubsetsElement> Info;
};

template <>
struct ElementInfo<Test2SubsetsElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"Test2SubsetsElement"};
};



TEST_F(SetTest, multiSubsetsTest) {
    Manager<TestTypes> m;
    UmlPtr<Test2SubsetsElement> testEl = m.create<Test2SubsetsElement>();
    TestPackage& clazz = *m.create<TestPackage>();
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
    TestPackage& inst = *m.create<TestPackage>();
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
    TestPackage& pckg = *m.create<TestPackage>();
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

class Test3SubsetsElement : public BaseElement<TestTypes> {
    friend class Creator<Test3SubsetsElement>;
    public:
        Set<TestPackage, Test3SubsetsElement> root = Set<TestPackage, Test3SubsetsElement>(this);
        Set<TestPackage, Test3SubsetsElement> intermediate = Set<TestPackage, Test3SubsetsElement>(this);
        Set<TestPackage, Test3SubsetsElement> sub = Set<TestPackage, Test3SubsetsElement>(this);
        Test3SubsetsElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            intermediate.subsets(root);
            sub.subsets(intermediate);
        }
        typedef TypeInfo<std::tuple<>, Test3SubsetsElement> Info;
};

template <>
struct ElementInfo<Test3SubsetsElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"Test3SubsetsElement"};
};

TEST_F(SetTest, removeFromSubsettedSequenceTest) {
    Manager<TestTypes> m;
    UmlPtr<Test3SubsetsElement> testEl = m.create<Test3SubsetsElement>();
    TestPackage& pckg = *m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->set.add(p);
    ASSERT_THROW(testEl->set.add(p), SetStateException);
}

class TestElement2 : public BaseElement<TestTypes> {
    friend class Creator<TestElement2>;
    private:
        Set<TestElement2, TestElement2> m_others = Set<TestElement2, TestElement2>(this);
    protected:
    public:
        Set<TestElement2, TestElement2>& getOthers() { return m_others; };
        TestElement2(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            m_others.opposite(&TestElement2::getOthers);
        };
        typedef TypeInfo<std::tuple<>,TestElement2> Info;
};

template <>
struct ElementInfo<TestElement2> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"TestElement2" };
};

TEST_F(SetTest, oppositeTest) {
    Manager<TestTypes> m;
    TestElement2& t1 = *m.create<TestElement2>();
    TestElement2& t2 = *m.create<TestElement2>();
    t1.getOthers().add(t2);
    ASSERT_EQ(t2.getOthers().size(), 1);
    ASSERT_TRUE(t2.getOthers().contains(t1.getID()));
    // ASSERT_EQ(*t2.getOthers().get(t1.getID()), t1);
}

class RedefinedTestElement : public BaseElement<TestTypes> {
    friend class Creator<RedefinedTestElement>;
    public:
        Set<TestPackage, RedefinedTestElement> rootSet = Set<TestPackage, RedefinedTestElement>(this);
        Set<TestPackage, RedefinedTestElement> redefiningSet = Set<TestPackage, RedefinedTestElement>(this);
        RedefinedTestElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            redefiningSet.redefines(rootSet);
        }
        typedef TypeInfo<std::tuple<>, RedefinedTestElement> Info;
};

template <>
struct ElementInfo<RedefinedTestElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"RedefinedTestElement"};
};

TEST_F(SetTest, setRedefinesTest) {
    Manager<TestTypes> m;
    UmlPtr<RedefinedTestElement> testEl = m.create<RedefinedTestElement>();
    TestPackage& p = *m.create<TestPackage>();
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
    TestPackage& p2 = *m.create<TestPackage>();
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
    TestPackage& p3 = *m.create<TestPackage>();
    testEl->rootSet.add(p3);
    ASSERT_FALSE(testEl->rootSet.empty());
    ASSERT_FALSE(testEl->redefiningSet.empty());
    ASSERT_EQ(testEl->rootSet.size(), 3);
    ASSERT_EQ(testEl->redefiningSet.size(), 3);
}


class TestPolicy {
    public:
        void elementAdded(TestPackage& el, PolicyTestElement& me);
        void elementRemoved(TestPackage& el, PolicyTestElement& me);
};

class PolicyTestElement : public BaseElement<TestTypes> {
    friend class Creator<PolicyTestElement>;
    public:
        Set<TestPackage, PolicyTestElement, TestPolicy> policySet = Set<TestPackage, PolicyTestElement, TestPolicy>(this);
        Set<TestPackage, PolicyTestElement> redefinedSet = Set<TestPackage, PolicyTestElement>(this);
        PolicyTestElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes> (elementType, manager) {
            redefinedSet.redefines(policySet);
        }
        size_t count = 0;
        typedef TypeInfo<std::tuple<>, PolicyTestElement> Info;
};

template <>
struct ElementInfo<PolicyTestElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"PolicyTestElement"};
};

void TestPolicy::elementAdded(__attribute__((unused)) TestPackage& el, PolicyTestElement& me) {
    me.count++;
}

void TestPolicy::elementRemoved(__attribute__((unused)) TestPackage& el, PolicyTestElement& me) {
    me.count--;
}

TEST_F(SetTest, setRedefinedWFunctors) {
    Manager<TestTypes> m;
    UmlPtr<PolicyTestElement> testEl = m.create<PolicyTestElement>();
    TestPackage& g = *m.create<TestPackage>();
    testEl->redefinedSet.add(g);
    ASSERT_EQ(testEl->count, 1);
    testEl->redefinedSet.remove(g);
    ASSERT_EQ(testEl->count, 0);
}

class TestOrderedSetElement : public BaseElement<TestTypes> {
    friend class Creator<TestOrderedSetElement>;    
    public:
        OrderedSet<TestPackage, TestOrderedSetElement> set = OrderedSet<TestPackage, TestOrderedSetElement>(this);
        TestOrderedSetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes> (elementType, manager) {}
        typedef TypeInfo<std::tuple<>, TestOrderedSetElement> Info;
};

template <>
struct ElementInfo<TestOrderedSetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"TestOrderedSetElement"};
};

TEST_F(SetTest, addToOrderedSetTest) {
    Manager<TestTypes> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->set.add(p);
    ASSERT_EQ(*testEl->set.front(), p);
    ASSERT_EQ(*testEl->set.back(), p);
    TestPackage& p2 = *m.create<TestPackage>();
    testEl->set.add(p2);
    ASSERT_EQ(*testEl->set.front(), p);
    ASSERT_EQ(*testEl->set.back(), p2);
    TestPackage& p3 = *m.create<TestPackage>();
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
    TestPackage* ps[] = {&p, &p2, &p3};
    int i = 0;
    for (TestPackage& pckg : testEl->set) {
        ASSERT_EQ(*ps[i], pckg);
        i++;
    }
    ASSERT_EQ(*testEl->set.get(0), p);
    ASSERT_EQ(*testEl->set.get(1), p2);
    ASSERT_EQ(*testEl->set.get(2), p3);
}

class TestElementSubsetsOrderedSets : public BaseElement<TestTypes> {
    friend class Creator<TestElementSubsetsOrderedSets>;
    public:
        OrderedSet<TestPackage, TestElementSubsetsOrderedSets> root = OrderedSet<TestPackage, TestElementSubsetsOrderedSets>(this);
        OrderedSet<TestPackage, TestElementSubsetsOrderedSets> sub = OrderedSet<TestPackage, TestElementSubsetsOrderedSets>(this);
        TestElementSubsetsOrderedSets(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            sub.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestElementSubsetsOrderedSets> Info;
};

template <>
struct ElementInfo<TestElementSubsetsOrderedSets> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"TestElementSubsetsOrderedSets"};
};

TEST_F(SetTest, subsetOrderedSets) {
    Manager<TestTypes> m;
    UmlPtr<TestElementSubsetsOrderedSets> testEl = m.create<TestElementSubsetsOrderedSets>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->sub.add(p);
    ASSERT_EQ(*testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.front(), p);
    TestPackage& c = *m.create<TestPackage>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(*testEl->root.back(), c);
    int i = 0;
    for (__attribute__((unused)) TestPackage& pckg : testEl->sub) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (__attribute__((unused)) TestPackage& pckg : testEl->root) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

class TestElementOrderedSubsetsSet : public BaseElement<TestTypes> {
    friend class Creator<TestElementOrderedSubsetsSet>;
    public:
        Set<TestPackage, TestElementOrderedSubsetsSet> root = Set<TestPackage, TestElementOrderedSubsetsSet>(this);
        OrderedSet<TestPackage, TestElementOrderedSubsetsSet> sub = OrderedSet<TestPackage, TestElementOrderedSubsetsSet>(this);
        TestElementOrderedSubsetsSet(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            sub.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestElementOrderedSubsetsSet> Info;
};

template <>
struct ElementInfo<TestElementOrderedSubsetsSet> : public DefaultInfo {
    static const bool abstract = true;
    inline static const std::string name{"TestElementOrderedSubsetsSet"};
};

TEST_F(SetTest, orderedSetSubSetsSet) {
    Manager<TestTypes> m;
    UmlPtr<TestElementOrderedSubsetsSet> testEl = m.create<TestElementOrderedSubsetsSet>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->sub.add(p);
    ASSERT_EQ(*testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(*testEl->root.get(p.getID()), p);
    TestPackage& c = *m.create<TestPackage>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(*testEl->root.get(c.getID()), c);
    int i = 0;
    for (__attribute__((unused)) TestPackage& pckg : testEl->sub) {
        i++;
    }
    ASSERT_EQ(i, 1);
    i = 0;
    for (__attribute__((unused)) TestPackage& pckg : testEl->root) {
        i++;
    }
    ASSERT_EQ(i, 2);
}

class TestSingletonElement : public BaseElement<TestTypes> {
    friend class Creator<TestSingletonElement>;
    public:
        Set<TestPackage, TestSingletonElement> root = Set<TestPackage, TestSingletonElement>(this);
        Singleton<TestPackage, TestSingletonElement> singleton = Singleton<TestPackage, TestSingletonElement>(this);
        TestSingletonElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            singleton.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestSingletonElement> Info;
};

template <>
struct ElementInfo<TestSingletonElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static std::string name{"TestSingletonElement"};
};

TEST_F(SetTest, singletonTest) {
    Manager<TestTypes> m;
    UmlPtr<TestSingletonElement> testEl = m.create<TestSingletonElement>();
    ASSERT_TRUE(testEl->singleton.empty());
    ASSERT_EQ(testEl->singleton.size(), 0);
    TestPackage& p = *m.create<TestPackage>();
    testEl->singleton.set(p);
    ASSERT_EQ(testEl->singleton.size(), 1);
    ASSERT_FALSE(testEl->singleton.empty());
    ASSERT_TRUE(testEl->singleton.get());
    ASSERT_EQ(*testEl->singleton.get(), p);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(*testEl->root.get(p.getID()), p);
    TestPackage& c = *m.create<TestPackage>();
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

class TestSharedSubsetEvenTreeElement : public BaseElement<TestTypes> {
    friend class Creator<TestSharedSubsetEvenTreeElement>;
    public:
        Set<TestPackage, TestSharedSubsetEvenTreeElement> root = Set<TestPackage, TestSharedSubsetEvenTreeElement>(this);
        Set<TestPackage, TestSharedSubsetEvenTreeElement> set1 = Set<TestPackage, TestSharedSubsetEvenTreeElement>(this);
        Set<TestPackage, TestSharedSubsetEvenTreeElement> set2 = Set<TestPackage, TestSharedSubsetEvenTreeElement>(this);
        TestSharedSubsetEvenTreeElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            set1.subsets(root);
            set2.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestSharedSubsetEvenTreeElement> Info;
};

template <>
struct ElementInfo<TestSharedSubsetEvenTreeElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static std::string name{"TestSharedSubsetsEvenTreeElement"};
};

TEST_F(SetTest, sharedSubsetEvenTreeTest) {
    Manager<TestTypes> m;
    UmlPtr<TestSharedSubsetEvenTreeElement> testEl = m.create<TestSharedSubsetEvenTreeElement>();
    TestPackage& p1 = *m.create<TestPackage>();
    TestPackage& p2 = *m.create<TestPackage>();
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

class TestTwoRootSubSetElement : public BaseElement<TestTypes> {
    friend class Creator<TestTwoRootSubSetElement>;
    public:
        Set<TestPackage, TestTwoRootSubSetElement> root1 = Set<TestPackage, TestTwoRootSubSetElement>(this);
        Set<TestPackage, TestTwoRootSubSetElement> root2 = Set<TestPackage, TestTwoRootSubSetElement>(this);
        Set<TestPackage, TestTwoRootSubSetElement> sub = Set<TestPackage, TestTwoRootSubSetElement>(this);
        TestTwoRootSubSetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes> (elementType, manager) {
            sub.subsets(root1);
            sub.subsets(root2);
        }
        typedef TypeInfo<std::tuple<>, TestTwoRootSubSetElement> Info;
};

template <>
struct ElementInfo<TestTwoRootSubSetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static std::string name{"TestTwoRootSubSetElement"};
};

TEST_F(SetTest, multiRootWithinRootTest) {
    Manager<TestTypes> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    TestPackage& c = *m.create<TestPackage>();
    TestPackage& p = *m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->sub.add(p);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_EQ(testEl->root1.size(), 1);
    ASSERT_EQ(testEl->root2.size(), 1);
    ASSERT_TRUE(testEl->sub.contains(p.getID()));
    ASSERT_TRUE(testEl->root2.contains(p.getID()));
    ASSERT_TRUE(testEl->root1.contains(p.getID()));
}

TEST_F(SetTest, multiRootAddToSubsetTest) {
    Manager<TestTypes> m;
    UmlPtr<TestTwoRootSubSetElement> testEl = m.create<TestTwoRootSubSetElement>();
    UmlPtr<TestPackage> pckg = m.create<TestPackage>();
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

class TestComplexSubsetElement : public BaseElement<TestTypes> {
    friend class Creator<TestComplexSubsetElement>;
    public:
        Set<TestPackage, TestComplexSubsetElement> rootSet = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> rightSet1 = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> leftSet1 = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> rightSet2 = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> leftSet2 = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> leftSet2a = Set<TestPackage, TestComplexSubsetElement>(this);
        Set<TestPackage, TestComplexSubsetElement> rightSet2a = Set<TestPackage, TestComplexSubsetElement>(this);
        TestComplexSubsetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            rightSet1.subsets(rootSet);
            leftSet1.subsets(rootSet);
            rightSet2.subsets(rightSet1);
            leftSet2.subsets(leftSet1);
            rightSet2a.subsets(rightSet1);
            leftSet2a.subsets(leftSet1);
        }
        typedef TypeInfo<std::tuple<>, TestComplexSubsetElement> Info;
};

template <>
struct ElementInfo<TestComplexSubsetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static std::string name{ "TestComplexSubsetElement" };
};

TEST_F(SetTest, twoWayMultiSetSplitTest) {
    Manager<TestTypes> m;
    UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
    TestPackage& rightP = *m.create<TestPackage>();
    TestPackage& leftP = *m.create<TestPackage>();
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

    TestPackage& rightP2 = *m.create<TestPackage>();
    TestPackage& leftP2 = *m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
    UmlPtr<TestPackage> a = m.create<TestPackage>();
    UmlPtr<TestPackage> b = m.create<TestPackage>();
    UmlPtr<TestPackage> c = m.create<TestPackage>();
    UmlPtr<TestPackage> d = m.create<TestPackage>();
    
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
    Manager<TestTypes> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    TestPackage& p = *m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->set.add(p);
    testEl->set.remove(p);
    ASSERT_TRUE(testEl->set.empty());
    ASSERT_EQ(testEl->set.size(), 0);
    ASSERT_EQ(testEl->set.front(), PackagePtr());
}

TEST_F(SetTest, removeLastElementFromOrderedSetTest) {
    Manager<TestTypes> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->set.add(p);
    TestPackage& p2 = *m.create<TestPackage>();
    testEl->set.add(p2);
    testEl->set.remove(p2);
    ASSERT_EQ(*testEl->set.back(), p);
    ASSERT_FALSE(testEl->set.contains(p2.getID()));
    ASSERT_EQ(testEl->set.size(), 1);
}

TEST_F(SetTest, removeMiddleElementFromOrderedSetTest) {
    Manager<TestTypes> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    TestPackage& p = *m.create<TestPackage>();
    testEl->set.add(p);
    TestPackage& p2 = *m.create<TestPackage>();
    testEl->set.add(p2);
    TestPackage& p3 = *m.create<TestPackage>();
    testEl->set.add(p3);
    testEl->set.remove(p2);
    auto it = testEl->set.begin();
    it++;
    ASSERT_EQ(*it, p3);
}

// uncomment?
// TEST_F(SetTest, removeFromSuperSetTest) {
//     UmlManager m;
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

class TestTripleSuperSetElement : public BaseElement<TestTypes> {
    friend class Creator<TestTripleSuperSetElement>;
    public:
        Set<TestPackage, TestTripleSuperSetElement> root = Set<TestPackage, TestTripleSuperSetElement>(this);
        Set<TestPackage, TestTripleSuperSetElement> set1 = Set<TestPackage, TestTripleSuperSetElement>(this);
        Set<TestPackage, TestTripleSuperSetElement> set2 = Set<TestPackage, TestTripleSuperSetElement>(this);
        Set<TestPackage, TestTripleSuperSetElement> set3 = Set<TestPackage, TestTripleSuperSetElement>(this);
        TestTripleSuperSetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager) {
            set1.subsets(root);
            set2.subsets(root);
            set3.subsets(root);
        }
        typedef TypeInfo<std::tuple<>, TestTripleSuperSetElement> Info;
};

template <>
struct ElementInfo<TestTripleSuperSetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name{"TestTripleSuperSetElement"};
};

TEST_F(SetTest, tripleRemovePlacholder) {
    Manager<TestTypes> m;
    UmlPtr<TestTripleSuperSetElement> testEl = m.create<TestTripleSuperSetElement>();
    TestPackage& pckg1 = *m.create<TestPackage>();
    TestPackage& pckg2 = *m.create<TestPackage>();
    TestPackage& pckg3 = *m.create<TestPackage>();
    pckg1.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAB"));
    pckg2.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAC"));
    pckg3.setID(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAD"));

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

class TestDiamondSuperSetElement : public BaseElement<TestTypes> 
{
    friend class Creator<TestDiamondSuperSetElement>;
public:
    Set<TestPackage, TestDiamondSuperSetElement> root = Set<TestPackage, TestDiamondSuperSetElement>(this);
    Set<TestPackage, TestDiamondSuperSetElement> left = Set<TestPackage, TestDiamondSuperSetElement>(this);
    Set<TestPackage, TestDiamondSuperSetElement> right = Set<TestPackage, TestDiamondSuperSetElement>(this);
    Set<TestPackage, TestDiamondSuperSetElement> bottom = Set<TestPackage, TestDiamondSuperSetElement>(this);
    TestDiamondSuperSetElement(std::size_t elementType, AbstractManager& manager) : BaseElement<TestTypes>(elementType, manager)
    {
        left.subsets(root);
        right.subsets(root);
        bottom.subsets(left);
        bottom.subsets(right);
    }
    typedef TypeInfo<std::tuple<>, TestDiamondSuperSetElement> Info;
};

template <>
struct ElementInfo<TestDiamondSuperSetElement> : public DefaultInfo {
    static const bool abstract = false;
    inline static const std::string name {"TestDiamondSuperSetElement"};
};

TEST_F(SetTest, VeryBasicDiamondSubsetTest)
{
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg4 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
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
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg3 = m.create<TestPackage>();
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

TEST_F(SetTest, addToRightFirst) {
    Manager<TestTypes> m;
    UmlPtr<TestPackage> pckg1 = m.create<TestPackage>();
    UmlPtr<TestPackage> pckg2 = m.create<TestPackage>();
    UmlPtr<TestDiamondSuperSetElement> testEl = m.create<TestDiamondSuperSetElement>();
    testEl->right.add(pckg1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg1));
    auto i = 0;
    for (__attribute__((unused)) auto& el : testEl->root) {
        i ++;
    }
    ASSERT_EQ(i, 1);
    ASSERT_EQ(testEl->right.size(), 1);
    ASSERT_TRUE(testEl->right.contains(pckg1));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->right) {
        i ++;
    }
    ASSERT_EQ(i, 1);
    ASSERT_EQ(testEl->left.size(), 0);
    ASSERT_FALSE(testEl->left.contains(pckg1));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->left) {
        i ++;
    }
    ASSERT_EQ(i, 0);
    ASSERT_EQ(testEl->bottom.size(), 0);
    ASSERT_FALSE(testEl->bottom.contains(pckg1));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->bottom) {
        i ++;
    }
    ASSERT_EQ(i, 0);

    testEl->bottom.add(pckg2);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(pckg2));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->root) {
        i ++;
    }
    ASSERT_EQ(i, 2);
    ASSERT_EQ(testEl->right.size(), 2);
    ASSERT_TRUE(testEl->right.contains(pckg2));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->right) {
        i ++;
    }
    ASSERT_EQ(i, 2);
    ASSERT_EQ(testEl->left.size(), 1);
    ASSERT_TRUE(testEl->left.contains(pckg2));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->left) {
        i ++;
    }
    ASSERT_EQ(i, 1);
    ASSERT_EQ(testEl->bottom.size(), 1);
    ASSERT_TRUE(testEl->bottom.contains(pckg2));
    i = 0;
    for (__attribute__((unused)) auto& el : testEl->bottom) {
        i ++;
    }
    ASSERT_EQ(i, 1);
}
}
// TODO more complex set

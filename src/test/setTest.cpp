#include "gtest/gtest.h"
#include "uml/types/element.h"
#include "uml/types/namedElement.h"
#include "uml/types/package.h"
#include "test/uml-cpp-paths.h"
#include "uml/umlPtr.h"
#include "uml/types/class.h"
#include "uml/types/generalization.h"
#include "uml/types/property.h"
#include "uml/types/dataType.h"

#include "uml/uml-stable.h"

#include "uml/set/set.h"
#include "uml/set/orderedSet.h"
#include "uml/set/singleton.h"
#include "uml/managers/manager.h"

using namespace UML;

class SetTest : public ::testing::Test {
   
};

class TestSet : public AbstractSet {
    protected:
        void runAddPolicy(Element& el) override {}
        void runRemovePolicy(Element& el) override {}
        bool oppositeEnabled() override {}
        void oppositeAdd(Element& el) override {}
        void oppositeRemove(Element& el) override {}
        void handleOppositeRemove(Element& el) override {}
        SetType setType() const override{};
        void adjustSuperSets(SetNode* node) override {}
        SetNode* createNode(Element& el) override {}
        SetNode* createNode(ID id) override {}
        void innerRemove(ID id) override {}
        void remove(ID id) override {}
    public:
        void setRoot(SetNode* node) override {
            m_root = node;
        }
        SetNode* getRoot() const override {
            return m_root;
        }
};

size_t getNumberOfBlackToLeafNodes(SetNode* node) {
    if (!node) {
        return 0;
    }
    size_t ret = 0;
    SetNode* currNode = node;
    do {
        if (currNode->m_color == SetNode::RedOrBlack::BLACK) {
            ret++;
        }
        if (currNode->m_left) {
            currNode = currNode->m_left;
        } else if (currNode->m_right) {
            currNode = currNode->m_right;
        } else {
            currNode = 0;
        }
    } while (currNode);
    return ret;
}

void checkAllPathsAreEqual(SetNode* node) {
    // traverse inorder
    if (node->m_left) {
        ASSERT_NO_FATAL_FAILURE(checkAllPathsAreEqual(node->m_left));
    }
    if (node->m_right) {
        ASSERT_NO_FATAL_FAILURE(checkAllPathsAreEqual(node->m_right));
    }
    ASSERT_EQ(getNumberOfBlackToLeafNodes(node->m_left), getNumberOfBlackToLeafNodes(node->m_right));
}

void checkNode(SetNode* node) {
    if (node->m_color == SetNode::RedOrBlack::RED) {
        ASSERT_TRUE((node->m_left && node->m_right) || (!node->m_right && !node->m_left));
        if (node->m_left) {
            ASSERT_TRUE(node->m_left->m_color == SetNode::RedOrBlack::BLACK);
        }
        if (node->m_right) {
            ASSERT_TRUE(node->m_right->m_color == SetNode::RedOrBlack::BLACK);
        }
        if (node->m_parent) {
            ASSERT_TRUE(node->m_parent->m_color == SetNode::RedOrBlack::BLACK);
        }
    }
    // check children number of black
    if (node->m_left && node->m_right) {
        ASSERT_NO_FATAL_FAILURE(checkAllPathsAreEqual(node));
        ASSERT_NO_FATAL_FAILURE(checkNode(node->m_left));
        ASSERT_NO_FATAL_FAILURE(checkNode(node->m_right));
    } else if (node->m_left) {
        ASSERT_TRUE(node->m_left->m_color == SetNode::RedOrBlack::RED);
        ASSERT_NO_FATAL_FAILURE(checkNode(node->m_left));
    } else if (node->m_right) {
        ASSERT_TRUE(node->m_right->m_color == SetNode::RedOrBlack::RED);
        ASSERT_NO_FATAL_FAILURE(checkNode(node->m_right));
    }
}

void isValidRedBlackTree(SetNode* root) {
    ASSERT_TRUE(root->m_color == SetNode::RedOrBlack::BLACK);
    ASSERT_NO_FATAL_FAILURE(checkNode(root));
}

TEST_F(SetTest, RedBlackAlwaysLeft) {
    Manager<> m;
    TestSet set;
    SetNode a;
    SetNode b;
    SetNode c;
    SetNode d;
    SetNode e;
    SetNode f;
    SetNode g;
    SetNode h;
    PackagePtr pckgA = m.create<Package>();
    a.m_ptr = pckgA;
    a.set = &set;
    PackagePtr pckgB = m.create<Package>();
    pckgB->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    b.m_ptr = pckgB;
    b.set = &set;
    set.setRoot(&a);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    set.getRoot()->insert(&b);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgC = m.create<Package>();
    c.m_ptr = pckgC;
    c.set = &set;
    set.getRoot()->insert(&c);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgD = m.create<Package>();
    pckgD->setID("BAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    d.m_ptr = pckgD;
    d.set = &set;
    set.getRoot()->insert(&d);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgE = m.create<Package>();
    pckgE->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    e.m_ptr = pckgE;
    e.set = &set;
    set.getRoot()->insert(&e);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgF = m.create<Package>();
    pckgF->setID("ABAAAAAAAAAAAAAAAAAAAAAAAAAA");
    f.m_ptr = pckgF;
    f.set = &set;
    set.getRoot()->insert(&f);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgG = m.create<Package>();
    pckgG->setID("ABBAAAAAAAAAAAAAAAAAAAAAAAAA");
    g.m_ptr = pckgG;
    g.set = &set;
    set.getRoot()->insert(&g);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgH = m.create<Package>();
    pckgH->setID("ABBBAAAAAAAAAAAAAAAAAAAAAAAA");
    h.m_ptr = pckgH;
    h.set = &set;
    set.getRoot()->insert(&h);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
}

TEST_F(SetTest, addAndRemoveALotOfElements) {
    size_t numElements = 100;
    Manager<> m;
    TestSet set;
    PackagePtr rootPckg = m.create<Package>();
    SetNode* root = new SetNode;
    root->m_ptr = rootPckg;
    root->set = &set;
    set.setRoot(root);
    std::vector<ID> nodes;
    for (size_t i = 0; i < numElements; i++) {
        PackagePtr pckg = m.create<Package>();
        SetNode* node = new SetNode;
        node->m_ptr = pckg;
        node->set = &set;
        set.getRoot()->insert(node);
        ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
        nodes.push_back(pckg.id());
    }
    // remove some elements, from front
    // 1
    delete set.getRoot()->search(nodes[0]);
    nodes.erase(nodes.begin());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    // 2
    delete set.getRoot()->search(nodes[0]);
    nodes.erase(nodes.begin());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    // 3
    delete set.getRoot()->search(nodes[0]);
    nodes.erase(nodes.begin());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    // 4
    delete set.getRoot()->search(nodes[0]);
    nodes.erase(nodes.begin());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // 5
    delete set.getRoot()->search(nodes[0]);
    nodes.erase(nodes.begin());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // remove some from the end
    // 1
    delete set.getRoot()->search(nodes.back());
    nodes.erase(--nodes.end());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // 2
    delete set.getRoot()->search(nodes.back());
    nodes.erase(--nodes.end());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // 3
    delete set.getRoot()->search(nodes.back());
    nodes.erase(--nodes.end());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // 4
    delete set.getRoot()->search(nodes.back());
    nodes.erase(--nodes.end());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    // 5
    delete set.getRoot()->search(nodes.back());
    nodes.erase(--nodes.end());
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));

    std::vector<ID>::iterator nodeIt = nodes.end();
    do {
        --nodeIt;
        delete set.getRoot()->search(*nodeIt);
        ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    } while (nodeIt != nodes.begin());
    ASSERT_TRUE(set.getRoot() == root);
    delete set.getRoot();
    ASSERT_TRUE(set.getRoot() == 0);
}


class TestPackageSetElement : public Element {
    public:
        CustomSet<Package, TestPackageSetElement> set = CustomSet<Package, TestPackageSetElement>(this);
        TestPackageSetElement() : Element(ElementType::ELEMENT) {}
};

TEST_F(SetTest, basicSetTest) {
    size_t numPackages = 20;
    Manager<> m;
    UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
    Package& pckg = *m.create<Package>();
    testEl->set.add(pckg);
    ASSERT_FALSE(testEl->set.empty());
    ASSERT_EQ(testEl->set.get(pckg.getID()), pckg);
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
        ASSERT_EQ(testEl->set.get(id).getID(), id);
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
        CustomSet<Package, TestReindexSetElement> set = CustomSet<Package, TestReindexSetElement>(this);
        TestReindexSetElement() : Element(ElementType::ELEMENT) {
            set.subsets(this->m_ownedElements);
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
        CustomSet<PackageableElement, TestSubsetsElement> root = CustomSet<PackageableElement, TestSubsetsElement>(this);
        CustomSet<Package, TestSubsetsElement> sub = CustomSet<Package, TestSubsetsElement>(this);
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
    ASSERT_EQ(testEl->sub.get(pckg.getID()), pckg);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(pckg.getID()));
    ASSERT_EQ(testEl->root.get(pckg.getID()), pckg);
    Package& clazz = *m.create<Package>();
    testEl->root.add(clazz);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(clazz.getID()));
    ASSERT_EQ(testEl->root.get(clazz.getID()), clazz);
    ASSERT_EQ(testEl->sub.size(), 1);
    ASSERT_FALSE(testEl->sub.contains(clazz.getID()));
    ASSERT_THROW(testEl->sub.get(clazz.getID()), SetStateException);
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
    testEl->sub.add(b);
    testEl->sub.add(c);
    std::unordered_set<PackageableElement*> pckgs;
    for (auto& pkg : testEl->root) {
        pckgs.insert(&pkg);
    }
    ASSERT_EQ(pckgs.size(), 3);
    SetIterator<PackageableElement> it = testEl->root.begin();
    it++;
    it++;
    ASSERT_EQ(*it, testEl->root.back());
}

TEST_F(SetTest, readjustRedBlackRootTest) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    PackagePtr a = m.create<Package>();
    PackagePtr b = m.create<Package>();
    PackagePtr c = m.create<Package>();
    a->setID("&90IAqvc&wUnewHz0xLI4fPYNXUe");
    b->setID("buttEyLdYRCk_zbuttYrQyy42yHr");
    c->setID("04zH_c&oPfM5KXPqJXA0_7AzIzcy");
    testEl->sub.add(a);
    ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
    testEl->sub.add(b);
    ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
    testEl->sub.add(c);
    ASSERT_EQ(testEl->sub.getRoot(), testEl->root.getRoot());
}

class Test2SubsetsElement : public Element {
    public:
        CustomSet<NamedElement, Test2SubsetsElement> set1 = CustomSet<NamedElement, Test2SubsetsElement>(this);
        CustomSet<PackageableElement, Test2SubsetsElement> set2 = CustomSet<PackageableElement, Test2SubsetsElement>(this);
        CustomSet<Package, Test2SubsetsElement> sub = CustomSet<Package, Test2SubsetsElement>(this);
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
        CustomSet<NamedElement, Test3SubsetsElement> root = CustomSet<NamedElement, Test3SubsetsElement>(this);
        CustomSet<PackageableElement, Test3SubsetsElement> intermediate = CustomSet<PackageableElement, Test3SubsetsElement>(this);
        CustomSet<Package, Test3SubsetsElement> sub = CustomSet<Package, Test3SubsetsElement>(this);
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
    testEl->sub.remove(pckg);
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_EQ(testEl->intermediate.size(), 0);
    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(pckg));
    ASSERT_FALSE(testEl->intermediate.contains(pckg));
    ASSERT_FALSE(testEl->root.contains(pckg));
    testEl->sub.add(pckg);
    testEl->intermediate.remove(pckg);
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_EQ(testEl->intermediate.size(), 0);
    ASSERT_EQ(testEl->root.size(), 0);
    ASSERT_FALSE(testEl->sub.contains(pckg));
    ASSERT_FALSE(testEl->intermediate.contains(pckg));
    ASSERT_FALSE(testEl->root.contains(pckg));
}

TEST_F(SetTest, getFromSetByNameTest) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    Package& one = *m.create<Package>();
    Package& two = *m.create<Package>();
    one.setName("1");
    two.setName("2");
    testEl->sub.add(one);
    testEl->root.add(two);
    ASSERT_TRUE(testEl->root.contains("1"));
    ASSERT_TRUE(testEl->root.contains("2"));
    ASSERT_TRUE(testEl->sub.contains("1"));
    ASSERT_FALSE(testEl->sub.contains("2"));
    ASSERT_EQ(testEl->root.get("1"), one);
    ASSERT_EQ(testEl->root.get("2"), two);
    ASSERT_EQ(testEl->sub.get("1"), one);
    ASSERT_THROW(testEl->sub.get("2"), SetStateException);
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
        CustomSet<TestElement2, TestElement2> m_others = CustomSet<TestElement2, TestElement2>(*this);
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
        CustomSet<Package, RedefinedTestElement> rootSet = CustomSet<Package, RedefinedTestElement>(this);
        CustomSet<Package, RedefinedTestElement> redefiningSet = CustomSet<Package, RedefinedTestElement>(this);
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
        static void apply(Package& el, PolicyTestElement& me);
};

class PolicyTestElement : public Element {

    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    public:
        CustomSet<Package, PolicyTestElement, TestPolicy> policySet = CustomSet<Package, PolicyTestElement, TestPolicy>(this);
        CustomSet<Package, PolicyTestElement> redefinedSet = CustomSet<Package, PolicyTestElement>(this);
        PolicyTestElement() : Element(ElementType::ELEMENT) {
            redefinedSet.redefines(policySet);
        }
        size_t count = 0;
};

void TestPolicy::apply(Package& el, PolicyTestElement& me) {
    me.count++;
}

TEST_F(SetTest, setRedefinedWFunctors) {
    Manager<> m;
    UmlPtr<PolicyTestElement> testEl = m.create<PolicyTestElement>();
    Package& g = *m.create<Package>();
    testEl->redefinedSet.add(g);
    ASSERT_EQ(testEl->count, 1);
}

class TestOrderedSetElement : public Element {
    
    template <typename SerializationPolicy, typename PersistencePolciy> friend class Manager;

    public:
        CustomOrderedSet<Package, TestOrderedSetElement> set = CustomOrderedSet<Package, TestOrderedSetElement>(this);
        TestOrderedSetElement() : Element(ElementType::ELEMENT) {}
        ~TestOrderedSetElement() {}
};

TEST_F(SetTest, addToOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    p.setName("1");
    testEl->set.add(p);
    ASSERT_EQ(testEl->set.front(), p);
    ASSERT_EQ(testEl->set.back(), p);
    Package& p2 = *m.create<Package>();
    p2.setName("2");
    testEl->set.add(p2);
    ASSERT_EQ(testEl->set.front(), p);
    ASSERT_EQ(testEl->set.back(), p2);
    Package& p3 = *m.create<Package>();
    p3.setName("3");
    testEl->set.add(p3);
    ASSERT_EQ(testEl->set.front(), p);
    ASSERT_EQ(testEl->set.back(), p3);
    ASSERT_TRUE(testEl->set.contains(p.getID()));
    ASSERT_TRUE(testEl->set.contains("1"));
    ASSERT_EQ(testEl->set.get(p.getID()), p);
    ASSERT_EQ(testEl->set.get("1"), p);
    ASSERT_TRUE(testEl->set.contains(p2.getID()));
    ASSERT_TRUE(testEl->set.contains("2"));
    ASSERT_EQ(testEl->set.get(p2.getID()), p2);
    ASSERT_EQ(testEl->set.get("2"), p2);
    ASSERT_TRUE(testEl->set.contains(p3.getID()));
    ASSERT_TRUE(testEl->set.contains("3"));
    ASSERT_EQ(testEl->set.get(p3.getID()), p3);
    ASSERT_EQ(testEl->set.get("3"), p3);
    Package* ps[] = {&p, &p2, &p3};
    int i = 0;
    for (Package& pckg : testEl->set) {
        ASSERT_EQ(*ps[i], pckg);
        i++;
    }
    ASSERT_EQ(testEl->set.get(0), p);
    ASSERT_EQ(testEl->set.get(1), p2);
    ASSERT_EQ(testEl->set.get(2), p3);
}

class TestElementSubsetsOrderedSets : public Element {
    public:
        CustomOrderedSet<Package, TestElementSubsetsOrderedSets> root = CustomOrderedSet<Package, TestElementSubsetsOrderedSets>(this);
        CustomOrderedSet<Package, TestElementSubsetsOrderedSets> sub = CustomOrderedSet<Package, TestElementSubsetsOrderedSets>(this);
        TestElementSubsetsOrderedSets() : Element(ElementType::ELEMENT) {
            sub.subsets(root);
        }
};

TEST_F(SetTest, subsetOrderedSets) {
    Manager<> m;
    UmlPtr<TestElementSubsetsOrderedSets> testEl = m.create<TestElementSubsetsOrderedSets>();
    Package& p = *m.create<Package>();
    testEl->sub.add(p);
    ASSERT_EQ(testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->root.front(), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(testEl->root.back(), c);
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
        CustomSet<Package, TestElementOrderedSubsetsSet> root = CustomSet<Package, TestElementOrderedSubsetsSet>(this);
        CustomOrderedSet<Package, TestElementOrderedSubsetsSet> sub = CustomOrderedSet<Package, TestElementOrderedSubsetsSet>(this);
        TestElementOrderedSubsetsSet() : Element(ElementType::ELEMENT) {
            sub.subsets(root);
        }
};

TEST_F(SetTest, orderedSetSubSetsSet) {
    Manager<> m;
    UmlPtr<TestElementOrderedSubsetsSet> testEl = m.create<TestElementOrderedSubsetsSet>();
    Package& p = *m.create<Package>();
    testEl->sub.add(p);
    ASSERT_EQ(testEl->sub.front(), p);
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->root.get(p.getID()), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_FALSE(testEl->sub.contains(c.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(testEl->root.get(c.getID()), c);
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
        CustomSet<Package, TestSingletonElement> root = CustomSet<Package, TestSingletonElement>(this);
        CustomSingleton<Package, TestSingletonElement> singleton = CustomSingleton<Package, TestSingletonElement>(this);
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
    ASSERT_EQ(testEl->root.get(p.getID()), p);
    Package& c = *m.create<Package>();
    testEl->root.add(c);
    ASSERT_EQ(testEl->singleton.size(), 1);
    ASSERT_FALSE(testEl->singleton.empty());
    ASSERT_TRUE(testEl->singleton.get());
    ASSERT_EQ(*testEl->singleton.get(), p);
    ASSERT_EQ(testEl->root.size(), 2);
    ASSERT_TRUE(testEl->root.contains(p.getID()));
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_EQ(testEl->root.get(p.getID()), p);
    ASSERT_EQ(testEl->root.get(c.getID()), c);
    testEl->singleton.set(0);
    ASSERT_FALSE(testEl->singleton.get());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_TRUE(testEl->root.contains(c.getID()));
    ASSERT_FALSE(testEl->root.contains(p.getID()));
}

class TestSharedSubsetEvenTreeElement : public Element {
    public:
        CustomSet<PackageableElement, TestSharedSubsetEvenTreeElement> root = CustomSet<PackageableElement, TestSharedSubsetEvenTreeElement>(this);
        CustomSet<Package, TestSharedSubsetEvenTreeElement> set1 = CustomSet<Package, TestSharedSubsetEvenTreeElement>(this);
        CustomSet<Package, TestSharedSubsetEvenTreeElement> set2 = CustomSet<Package, TestSharedSubsetEvenTreeElement>(this);
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
    ASSERT_FALSE(testEl->set1.contains(p2.getID()));
    ASSERT_EQ(testEl->set1.front(), p1);
    ASSERT_EQ(testEl->set2.front(), p2);
    for (auto& el : testEl->root) {
        ASSERT_TRUE(testEl->root.contains(el.getID()));
    }
    for (const ID id : testEl->root.ids()) {
        ASSERT_TRUE(testEl->root.contains(id));
    }
    testEl->set1.remove(p1);
    ASSERT_EQ(testEl->set2.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->root.front(), p2);
    ASSERT_EQ(testEl->set2.front(), p2);
    testEl->set1.add(p1);
    ASSERT_FALSE(testEl->set1.contains(p2.getID()));
    ASSERT_EQ(testEl->set1.front(), p1);
    ASSERT_EQ(testEl->set2.front(), p2);
    for (auto& el : testEl->root) {
        ASSERT_TRUE(testEl->root.contains(el.getID()));
    }
    for (const ID id : testEl->root.ids()) {
        ASSERT_TRUE(testEl->root.contains(id));
    }
    testEl->set2.remove(p2);
    ASSERT_EQ(testEl->set1.size(), 1);
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->root.front(), p1);
    ASSERT_EQ(testEl->set1.front(), p1);
}

class TestTwoRootSubSetElement : public Element {
    public:
        CustomSet<NamedElement, TestTwoRootSubSetElement> root1 = CustomSet<NamedElement, TestTwoRootSubSetElement>(this);
        CustomSet<PackageableElement, TestTwoRootSubSetElement> root2 = CustomSet<PackageableElement, TestTwoRootSubSetElement>(this);
        CustomSet<Package, TestTwoRootSubSetElement> sub = CustomSet<Package, TestTwoRootSubSetElement>(this);
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
    Class& c = *m.create<Class>();
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
        CustomSet<Element, TestComplexSubsetElement> rootSet = CustomSet<Element, TestComplexSubsetElement>(*this);
        CustomSet<NamedElement, TestComplexSubsetElement> rightSet1 = CustomSet<NamedElement, TestComplexSubsetElement>(*this);
        CustomSet<NamedElement, TestComplexSubsetElement> leftSet1 = CustomSet<NamedElement, TestComplexSubsetElement>(*this);
        CustomSet<PackageableElement, TestComplexSubsetElement> rightSet2 = CustomSet<PackageableElement, TestComplexSubsetElement>(*this);
        CustomSet<PackageableElement, TestComplexSubsetElement> leftSet2 = CustomSet<PackageableElement, TestComplexSubsetElement>(*this);
        CustomSet<PackageableElement, TestComplexSubsetElement> leftSet2a = CustomSet<PackageableElement, TestComplexSubsetElement>(*this);
        CustomSet<PackageableElement, TestComplexSubsetElement> rightSet2a = CustomSet<PackageableElement, TestComplexSubsetElement>(*this);
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
    ASSERT_THROW(testEl->set.front(), SetStateException);
}

TEST_F(SetTest, removeLastElementFromOrderedSetTest) {
    Manager<> m;
    UmlPtr<TestOrderedSetElement> testEl = m.create<TestOrderedSetElement>();
    Package& p = *m.create<Package>();
    testEl->set.add(p);
    Package& p2 = *m.create<Package>();
    testEl->set.add(p2);
    testEl->set.remove(p2);
    ASSERT_EQ(testEl->set.back(), p);
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
    OrderedSetIterator it = testEl->set.begin();
    it++;
    ASSERT_EQ(*it, p3);
}

TEST_F(SetTest, removeFromSuperSetTest) {
    Manager<> m;
    UmlPtr<TestSubsetsElement> testEl = m.create<TestSubsetsElement>();
    Package& pckg = *m.create<Package>();
    testEl->sub.add(pckg);
    testEl->sub.removeFromJustThisSet(pckg.getID());
    ASSERT_FALSE(testEl->root.empty());
    ASSERT_TRUE(testEl->sub.empty());
    ASSERT_EQ(testEl->root.size(), 1);
    ASSERT_EQ(testEl->sub.size(), 0);
    ASSERT_TRUE(testEl->root.contains(pckg.getID()));
    ASSERT_FALSE(testEl->sub.contains(pckg.getID()));
}

class TestTripleSuperSetElement : public Element {
    public:
        CustomSet<Element, TestTripleSuperSetElement> root = CustomSet<Element, TestTripleSuperSetElement>(this);
        CustomSet<NamedElement, TestTripleSuperSetElement> set1 = CustomSet<NamedElement, TestTripleSuperSetElement>(this);
        CustomSet<PackageableElement, TestTripleSuperSetElement> set2 = CustomSet<PackageableElement, TestTripleSuperSetElement>(this);
        CustomSet<Package, TestTripleSuperSetElement> set3 = CustomSet<Package, TestTripleSuperSetElement>(this);
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
    CustomSet<Element, TestDiamondSuperSetElement> root = CustomSet<Element, TestDiamondSuperSetElement>(this);
    CustomSet<NamedElement, TestDiamondSuperSetElement> left = CustomSet<NamedElement, TestDiamondSuperSetElement>(this);
    CustomSet<PackageableElement, TestDiamondSuperSetElement> right = CustomSet<PackageableElement, TestDiamondSuperSetElement>(this);
    CustomSet<Package, TestDiamondSuperSetElement> bottom = CustomSet<Package, TestDiamondSuperSetElement>(this);
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
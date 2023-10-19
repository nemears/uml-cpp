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
        void adjustSuperSets(SetNode* node, std::unordered_set<AbstractSet*>& allSuperSetsAndMe) override {}
        SetNode* createNode(Element& el) override {}
        SetNode* createNode(ID id) override {}
        void innerRemove(ID id) override {}
        void remove(ID id) override {}
    public:
        void setRoot(SetNode* node) override {
            // if (m_root && node) {
            //     // node->m_parent = this->m_root->m_parent;
            // }
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
    PackagePtr pckgA = m.create<Package>();
    SetNode a;
    a.m_ptr = pckgA;
    a.set = &set;
    PackagePtr pckgB = m.create<Package>();
    pckgB->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    SetNode b;
    b.m_ptr = pckgB;
    b.set = &set;
    set.setRoot(&a);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    set.getRoot()->insert(&b);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgC = m.create<Package>();
    SetNode c;
    c.m_ptr = pckgC;
    c.set = &set;
    set.getRoot()->insert(&c);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgD = m.create<Package>();
    pckgD->setID("BAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    SetNode d;
    d.m_ptr = pckgD;
    d.set = &set;
    set.getRoot()->insert(&d);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgE = m.create<Package>();
    pckgE->setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    SetNode e;
    e.m_ptr = pckgE;
    e.set = &set;
    set.getRoot()->insert(&e);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgF = m.create<Package>();
    pckgF->setID("ABAAAAAAAAAAAAAAAAAAAAAAAAAA");
    SetNode f;
    f.m_ptr = pckgF;
    f.set = &set;
    set.getRoot()->insert(&f);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgG = m.create<Package>();
    pckgG->setID("ABBAAAAAAAAAAAAAAAAAAAAAAAAA");
    SetNode g;
    g.m_ptr = pckgG;
    g.set = &set;
    set.getRoot()->insert(&g);
    ASSERT_NO_FATAL_FAILURE(isValidRedBlackTree(set.getRoot()));
    PackagePtr pckgH = m.create<Package>();
    pckgH->setID("ABBBAAAAAAAAAAAAAAAAAAAAAAAA");
    SetNode h;
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
    for (int i = 0; i < numPackages - 1; i++) {
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

// TEST_F(SetTest, basicRandomRemoveTest) {
//     Manager<> m;
//     const size_t constNumPackages = 40;
//     // TODO bug change 40 to 50 or higher
//     size_t numPackages = constNumPackages;
//     UmlPtr<TestPackageSetElement> testEl = m.create<TestPackageSetElement>();
//     std::vector<ID> packages;
//     for (size_t i = 0; i < numPackages; i++) {
//         Package& temp = *m.create<Package>();
//         packages.push_back(temp.getID());
//         testEl->set.add(temp);
//     }
//     for (size_t i = 0; i < constNumPackages / 2; i++) {
//         int index = rand() % numPackages;
//         std::cout << i << ", ";
//         testEl->set.remove(packages[index]);
//         packages.erase(std::remove(packages.begin(), packages.end(), packages[index]), packages.end()) - packages.begin();
//         numPackages--;
//     }
//     std::cout << std::endl;
//     ASSERT_EQ(testEl->set.size(), constNumPackages/2);
//     for (size_t i = 0; i < constNumPackages/ 2; i++) {
//         int index = rand() % numPackages;
//         testEl->set.remove(packages[index]);
//         packages.erase(std::remove(packages.begin(), packages.end(), packages[index]), packages.end()) - packages.begin();
//         numPackages--;
//     }
//     ASSERT_EQ(testEl->set.size(), 0);
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

// TEST_F(SetTest, specialAutoForLoop) {
//     Set<Package> seq;
//     int numPackages = 1;
//     Manager<> m;
//     for (int i = 0; i < numPackages; i++) {
//         Package& p = *m.create<Package>(); 
//         seq.add(p);
//         std::hash<ID> hasher;
//         std::cout << p.getID().string() << ", hash: " << hasher(p.getID()) << std::endl;
//     }
//     std::cout << "=========================================================" << std::endl;
//     int i = 0;
//     for (auto& p : seq) {
//         i++;
//         ASSERT_EQ(p.getElementType(), ElementType::PACKAGE);
//         std::hash<ID> hasher;
//         std::cout << p.getID().string() << ", hash: " << hasher(p.getID()) << std::endl;
//         if (i > numPackages + 10) break;
//     }
//     ASSERT_EQ(i, numPackages);
// }

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

// TEST_F(SetTest, testIdsMethodLoop) {
//     Set<Package> set;
//     Manager<> m;
//     int numPackages = 1;
//     std::unordered_set<ID> ids;
//     for (int i = 0; i < numPackages; i++) {
//         Package& p = *m.create<Package>();
//         set.add(p);
//         ids.insert(p.getID());
//     }
//     for (const ID id : set.ids()) {
//         ASSERT_TRUE(ids.count(id));
//     }
//  }

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

    Package& rightP2 = *m.create<Package>();
    Package& leftP2 = *m.create<Package>();
    testEl->rightSet1.add(rightP2);
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
//     Manager<> m;
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

TEST_F(SetTest, subsetAddsFromRootSet) {
    std::cout << "TODO uncomment test" <<std::endl;
    // Set<PackageableElement>* set = new Set<PackageableElement>;
    // Set<Package>* subSet = new Set<Package>();
    // subSet->subsets(*set);
    // Manager<> m;
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

// TEST_F(SetTest, StructuredClassifierOwnedAttributesEmulationTest) {
//     Set<>* ownedElements = new Set<>;
//     Set<NamedElement>* members = new Set<NamedElement>;
//     Set<NamedElement>* ownedMembers = new Set<NamedElement>;
//     Set<Feature>* features = new Set<Feature>;
//     Set<Property>* attributes = new Set<Property>;
//     Set<Generalization>* generalizations = new Set<Generalization>;
//     Set<NamedElement>* inheritedMembers = new Set<NamedElement>;
//     Set<ConnectableElement>* roles = new Set<ConnectableElement>;
//     Set<Property>* ownedAttributes = new Set<Property>;

//     ownedMembers->subsets(*ownedElements);
//     ownedMembers->subsets(*members);
//     features->subsets(*members);
//     attributes->subsets(*features);
//     generalizations->subsets(*ownedElements);
//     inheritedMembers->subsets(*members);
//     roles->subsets(*members);
//     ownedAttributes->subsets(*attributes);
//     ownedAttributes->subsets(*roles);
//     ownedAttributes->subsets(*ownedMembers);

//     Manager<> m;
    
//     Property& member = *m.create<Property>();
//     Property& property = *m.create<Property>();
//     Generalization& generalization = *m.create<Generalization>();

//     generalizations->add(generalization);

//     ASSERT_EQ(ownedElements->size(), 1);
//     ASSERT_EQ(members->size(), 0);
//     ASSERT_EQ(ownedMembers->size(), 0);
//     ASSERT_EQ(features->size(), 0);
//     ASSERT_EQ(attributes->size(), 0);
//     ASSERT_EQ(generalizations->size(), 1);
//     ASSERT_EQ(inheritedMembers->size(), 0);
//     ASSERT_EQ(roles->size(), 0);
//     ASSERT_EQ(ownedAttributes->size(), 0);
//     ASSERT_TRUE(ownedElements->contains(generalization));
//     ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
//     ASSERT_FALSE(features->contains(generalization.getID()));
//     ASSERT_FALSE(attributes->contains(generalization.getID()));
//     ASSERT_TRUE(generalizations->contains(generalization));
//     ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
//     ASSERT_FALSE(roles->contains(generalization.getID()));
//     ASSERT_FALSE(members->contains(generalization.getID()));
//     ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));

//     inheritedMembers->add(member);

//     ASSERT_EQ(ownedElements->size(), 1);
//     ASSERT_EQ(members->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 0);
//     ASSERT_EQ(features->size(), 0);
//     ASSERT_EQ(attributes->size(), 0);
//     ASSERT_EQ(generalizations->size(), 1);
//     ASSERT_EQ(inheritedMembers->size(), 1);
//     ASSERT_EQ(roles->size(), 0);
//     ASSERT_EQ(ownedAttributes->size(), 0);
//     ASSERT_FALSE(ownedElements->contains(member));
//     ASSERT_FALSE(ownedMembers->contains(member));
//     ASSERT_FALSE(features->contains(member));
//     ASSERT_FALSE(attributes->contains(member));
//     ASSERT_FALSE(generalizations->contains(member.getID()));
//     ASSERT_FALSE(ownedAttributes->contains(member));
//     ASSERT_FALSE(roles->contains(member));
//     ASSERT_TRUE(members->contains(member));
//     ASSERT_TRUE(inheritedMembers->contains(member));
//     ASSERT_TRUE(ownedElements->contains(generalization));
//     ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
//     ASSERT_FALSE(features->contains(generalization.getID()));
//     ASSERT_FALSE(attributes->contains(generalization.getID()));
//     ASSERT_TRUE(generalizations->contains(generalization));
//     ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
//     ASSERT_FALSE(roles->contains(generalization.getID()));
//     ASSERT_FALSE(members->contains(generalization.getID()));
//     ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));
    
//     ownedAttributes->add(property);

//     ASSERT_EQ(ownedElements->size(), 2);
//     ASSERT_EQ(members->size(), 2);
//     ASSERT_EQ(inheritedMembers->size(), 1);
//     ASSERT_EQ(features->size(), 1);
//     ASSERT_EQ(attributes->size(), 1);
//     ASSERT_EQ(generalizations->size(), 1);
//     ASSERT_EQ(roles->size(), 1);
//     ASSERT_EQ(ownedMembers->size(), 1);
//     ASSERT_EQ(ownedAttributes->size(), 1);

//     ASSERT_TRUE(ownedElements->contains(property));
//     ASSERT_TRUE(members->contains(property));
//     ASSERT_TRUE(ownedMembers->contains(property));
//     ASSERT_TRUE(features->contains(property));
//     ASSERT_TRUE(attributes->contains(property));
//     ASSERT_FALSE(generalizations->contains(property.getID()));
//     ASSERT_TRUE(roles->contains(property));
//     ASSERT_TRUE(ownedAttributes->contains(property));
//     ASSERT_FALSE(inheritedMembers->contains(property));
//     ASSERT_FALSE(ownedElements->contains(member));
//     ASSERT_FALSE(ownedMembers->contains(member));
//     ASSERT_FALSE(features->contains(member));
//     ASSERT_FALSE(attributes->contains(member));
//     ASSERT_FALSE(generalizations->contains(member.getID()));
//     ASSERT_FALSE(ownedAttributes->contains(member));
//     ASSERT_FALSE(roles->contains(member));
//     ASSERT_TRUE(members->contains(member));
//     ASSERT_TRUE(inheritedMembers->contains(member));
//     ASSERT_TRUE(ownedElements->contains(generalization));
//     ASSERT_FALSE(ownedMembers->contains(generalization.getID()));
//     ASSERT_FALSE(features->contains(generalization.getID()));
//     ASSERT_FALSE(attributes->contains(generalization.getID()));
//     ASSERT_TRUE(generalizations->contains(generalization));
//     ASSERT_FALSE(ownedAttributes->contains(generalization.getID()));
//     ASSERT_FALSE(roles->contains(generalization.getID()));
//     ASSERT_FALSE(members->contains(generalization.getID()));
//     ASSERT_FALSE(inheritedMembers->contains(generalization.getID()));

//     delete ownedAttributes;
//     delete roles;
//     delete inheritedMembers;
//     delete generalizations;
//     delete attributes;
//     delete features;
//     delete ownedMembers;
//     delete members;
//     delete ownedElements;
// }

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

//     Manager<> m;
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

//     Manager<> m;
//     OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
//     Operation& op = *m.create<Operation>();

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

//     Manager<> m;
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

//     Manager<> m;
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

//     Manager<> m;
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

//     Manager<> m;
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

//     Manager<> m;
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

// TEST_F(SetTest, removeFromSubsetOfOrderedSet) {
//     OrderedSet<Package>* rootSet = new OrderedSet<Package>;
//     Set<Package>* subSet = new Set<Package>;

//     subSet->subsets(*rootSet);

//     Manager<> m;
//     PackagePtr pckg = m.create<Package>();
    
//     subSet->add(*pckg);
//     ASSERT_EQ(rootSet->front(), *pckg);
//     ASSERT_EQ(rootSet->back(), *pckg);
//     ASSERT_EQ(subSet->front(), *pckg);
//     ASSERT_EQ(subSet->back(), *pckg);

//     subSet->remove(*pckg);
//     ASSERT_THROW(rootSet->front(), ManagerStateException);
//     ASSERT_THROW(rootSet->back(), ManagerStateException);

//     delete subSet;
//     delete rootSet;
// }

// class TestElement2 : public Element {

//     template<typename AccessPolicy, typename PersistencePolciy> friend class Manager;

//     private:
//         Set2<TestElement2, TestElement2> m_others = Set2<TestElement2, TestElement2>(*this);
//     protected:
//         TypedSet<TestElement2, TestElement2>& getOthersSet() {
//             return m_others;
//         }
//     public:
//         TestElement2() : Element(ElementType::ELEMENT) {
//             m_others.opposite(&TestElement2::getOthersSet);
//         };
//         virtual ~TestElement2() {
//             mountAndRelease();
//         };
//         Set2<TestElement2, TestElement2>& getOthers() { return m_others; };
// };

// TEST_F(SetTest, set2Test) {
//     Manager<> m;
//     UmlPtr<TestElement2> el = m.create<TestElement2>();
//     UmlPtr<TestElement2> otherEl = m.create<TestElement2>();
//     el->getOthers().add(otherEl);
//     ASSERT_EQ(el->getOthers().get(otherEl.id()), otherEl);
//     UmlPtr<TestElement2> otherEl2 = m.create<TestElement2>();
//     el->getOthers().add(otherEl2);
//     for (TestElement2& tt : el->getOthers()) {
//         ASSERT_TRUE(tt.getID() == otherEl.id() || tt.getID() == otherEl2.id());
//     }
// }

// class TestElementWOrderedSet : public Element {
//     template<typename AccessPolicy, typename PersistencePolciy> friend class Manager;

//     private:
//         OrderedSet2<TestElementWOrderedSet, TestElementWOrderedSet> m_others = OrderedSet2<TestElementWOrderedSet, TestElementWOrderedSet>(*this);
//     protected:
//         TypedSet<TestElementWOrderedSet, TestElementWOrderedSet>& getOthersSet() {
//             return m_others;
//         }
//     public:
//         TestElementWOrderedSet() : Element(ElementType::ELEMENT) {
//             m_others.opposite(&TestElementWOrderedSet::getOthersSet);
//         };
//         virtual ~TestElementWOrderedSet() {
//             mountAndRelease();
//         };
//         OrderedSet2<TestElementWOrderedSet, TestElementWOrderedSet>& getOthers() { return m_others; };
// };

// TEST_F(SetTest, orderedSet2Test) {
//     Manager<> m;
//     UmlPtr<TestElementWOrderedSet> el = m.create<TestElementWOrderedSet>();
//     UmlPtr<TestElementWOrderedSet> otherEl = m.create<TestElementWOrderedSet>();
//     UmlPtr<TestElementWOrderedSet> otherEl2 = m.create<TestElementWOrderedSet>();
//     el->getOthers().add(otherEl);
//     ASSERT_EQ(el->getOthers().get(otherEl.id()), otherEl);
//     el->getOthers().add(otherEl2);
//     ASSERT_EQ(el->getOthers().front(), otherEl);
//     ASSERT_EQ(el->getOthers().back(), otherEl2);
//     for (TestElementWOrderedSet& testEl : el->getOthers()) {
//         ASSERT_TRUE(testEl == otherEl || testEl == otherEl2);
//     }
// }

// class SubsetTestElement : public Element {
//     template<typename AccessPolicy, typename PersistencePolciy> friend class Manager;
    
//     private:
//         Set2<SubsetTestElement, SubsetTestElement> rootSet = Set2<SubsetTestElement, SubsetTestElement>(*this);
//         Set2<SubsetTestElement, SubsetTestElement> subSet = Set2<SubsetTestElement, SubsetTestElement>(*this);
//         TypedSet<SubsetTestElement, SubsetTestElement>& getRootSetSet() {
//             return rootSet;
//         }
//         TypedSet<SubsetTestElement, SubsetTestElement>& getSubSetSet() {
//             return subSet;
//         }
//     public:
//         SubsetTestElement() : Element(ElementType::ELEMENT) {
//             subSet.subsets(rootSet);
//             rootSet.opposite(&SubsetTestElement::getRootSetSet);
//             subSet.opposite(&SubsetTestElement::getSubSetSet);
//         }
//         Set2<SubsetTestElement, SubsetTestElement>& getRootSet() {
//             return rootSet;
//         }
//         Set2<SubsetTestElement, SubsetTestElement>& getSubSet() {
//             return subSet;
//         }
// };

// TEST_F(SetTest, set2SubsetTest) {
//     Manager<> m;
//     UmlPtr<SubsetTestElement> el1 = m.create<SubsetTestElement>();
//     UmlPtr<SubsetTestElement> el2 = m.create<SubsetTestElement>();
//     UmlPtr<SubsetTestElement> el3 = m.create<SubsetTestElement>();
//     el1->getRootSet().add(el2);
//     ASSERT_EQ(el1->getRootSet().get(el2.id()), el2);
//     ASSERT_EQ(el1->getRootSet().size(), 1);
//     ASSERT_EQ(el1->getSubSet().size(), 0);
//     el1->getSubSet().add(el3);
//     ASSERT_EQ(el1->getRootSet().get(el2.id()), el2);
//     ASSERT_EQ(el1->getRootSet().get(el3.id()), el3);
//     ASSERT_EQ(el1->getRootSet().size(), 2);
//     ASSERT_TRUE(el1->getSubSet().contains(el3));
//     ASSERT_FALSE(el1->getSubSet().contains(el2));
//     ASSERT_EQ(el1->getSubSet().size(), 1);
// }

// class TestComplexSubsetElement : public Element {
//     template<typename AccessPolicy, typename PersistencePolciy> friend class Manager;
    
//     public:
//         Set2<Element, TestComplexSubsetElement> rootSet = Set2<Element, TestComplexSubsetElement>(*this);
//         Set2<NamedElement, TestComplexSubsetElement> rightSet1 = Set2<NamedElement, TestComplexSubsetElement>(*this);
//         Set2<NamedElement, TestComplexSubsetElement> leftSet1 = Set2<NamedElement, TestComplexSubsetElement>(*this);
//         Set2<PackageableElement, TestComplexSubsetElement> rightSet2 = Set2<PackageableElement, TestComplexSubsetElement>(*this);
//         Set2<PackageableElement, TestComplexSubsetElement> leftSet2 = Set2<PackageableElement, TestComplexSubsetElement>(*this);
//         TestComplexSubsetElement() : Element(ElementType::ELEMENT) {
//             rightSet1.subsets(rootSet);
//             leftSet1.subsets(rootSet);
//             rightSet2.subsets(rightSet1);
//             leftSet2.subsets(leftSet1);
//         }
// };

// TEST_F(SetTest, complexSubsetTest) {
//     Manager<> m;
//     UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
//     UmlPtr<Package> rightP = m.create<Package>();
//     UmlPtr<Package> leftP = m.create<Package>();
//     testEl->rightSet2.add(rightP);
//     testEl->leftSet2.add(leftP);
//     ASSERT_EQ(testEl->rightSet2.size(), 1);
//     ASSERT_EQ(testEl->rightSet1.size(), 1);
//     ASSERT_EQ(testEl->leftSet2.size(), 1);
//     ASSERT_EQ(testEl->leftSet1.size(), 1);
//     ASSERT_EQ(testEl->rootSet.size(), 2);
//     ASSERT_EQ(testEl->rightSet2.get(rightP->getID()), rightP);
//     ASSERT_EQ(testEl->rightSet1.get(rightP->getID()), rightP);
//     ASSERT_EQ(testEl->rootSet.get(rightP->getID()), rightP);
//     ASSERT_EQ(testEl->leftSet1.get(leftP->getID()), leftP);
//     ASSERT_EQ(testEl->leftSet2.get(leftP->getID()), leftP);
//     ASSERT_EQ(testEl->rootSet.get(leftP->getID()), leftP);

//     UmlPtr<Package> rightP2 = m.create<Package>();
//     UmlPtr<Package> leftP2 = m.create<Package>();
//     testEl->rightSet1.add(rightP2);
//     testEl->leftSet1.add(leftP2);
//     ASSERT_EQ(testEl->rootSet.size(), 4);
//     ASSERT_EQ(testEl->rightSet1.size(), 2);
//     ASSERT_EQ(testEl->leftSet1.size(), 2);
//     ASSERT_EQ(testEl->rightSet2.size(), 1);
//     ASSERT_EQ(testEl->leftSet2.size(), 1);
//     ASSERT_TRUE(testEl->rootSet.contains(rightP2));
//     ASSERT_TRUE(testEl->rightSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
// }

// TEST_F(SetTest, testSet2Remove) {
//     Manager<> m;
//     UmlPtr<SubsetTestElement> el1 = m.create<SubsetTestElement>();
//     UmlPtr<SubsetTestElement> el2 = m.create<SubsetTestElement>();
//     UmlPtr<SubsetTestElement> el3 = m.create<SubsetTestElement>();
//     el1->getRootSet().add(el2);
//     ASSERT_EQ(el1->getRootSet().get(el2.id()), el2);
//     ASSERT_EQ(el1->getRootSet().size(), 1);
//     ASSERT_EQ(el1->getSubSet().size(), 0);
//     el1->getSubSet().add(el3);
//     ASSERT_EQ(el1->getRootSet().get(el2.id()), el2);
//     ASSERT_EQ(el1->getRootSet().get(el3.id()), el3);
//     ASSERT_EQ(el1->getRootSet().size(), 2);
//     ASSERT_TRUE(el1->getSubSet().contains(el3));
//     ASSERT_FALSE(el1->getSubSet().contains(el2));
//     ASSERT_EQ(el1->getSubSet().size(), 1);
//     el1->getRootSet().remove(el2);
//     ASSERT_EQ(el1->getRootSet().size(), 1);
//     ASSERT_EQ(el1->getSubSet().size(), 1);
//     ASSERT_FALSE(el1->getRootSet().contains(el2));
// }

// TEST_F(SetTest, complexSubsetRemoveTest) {
//     Manager<> m;
//     UmlPtr<TestComplexSubsetElement> testEl = m.create<TestComplexSubsetElement>();
//     UmlPtr<Package> rightP = m.create<Package>();
//     UmlPtr<Package> leftP = m.create<Package>();
//     testEl->rightSet2.add(rightP);
//     testEl->leftSet2.add(leftP);
//     UmlPtr<Package> rightP2 = m.create<Package>();
//     UmlPtr<Package> leftP2 = m.create<Package>();
//     testEl->rightSet1.add(rightP2);
//     testEl->leftSet1.add(leftP2);
    
//     // remove
//     testEl->rightSet1.remove(rightP2);
//     ASSERT_EQ(testEl->rootSet.size(), 3);
//     ASSERT_FALSE(testEl->rootSet.contains(rightP2));
//     ASSERT_TRUE(testEl->rootSet.contains(rightP));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP2));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP));
//     ASSERT_EQ(testEl->rightSet1.size(), 1);
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP2));
//     ASSERT_TRUE(testEl->rightSet1.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP));
//     ASSERT_EQ(testEl->rightSet2.size(), 1);
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
//     ASSERT_TRUE(testEl->rightSet2.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP));
//     ASSERT_EQ(testEl->leftSet1.size(), 2);
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP));
//     ASSERT_EQ(testEl->leftSet2.size(), 1);
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet2.contains(leftP));

//     testEl->rightSet2.remove(rightP);
//     ASSERT_EQ(testEl->rootSet.size(), 2);
//     ASSERT_FALSE(testEl->rootSet.contains(rightP2));
//     ASSERT_FALSE(testEl->rootSet.contains(rightP));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP2));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP));
//     ASSERT_EQ(testEl->rightSet1.size(), 0);
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP));
//     ASSERT_EQ(testEl->rightSet2.size(), 0);
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP));
//     ASSERT_EQ(testEl->leftSet1.size(), 2);
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP));
//     ASSERT_EQ(testEl->leftSet2.size(), 1);
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet2.contains(leftP));

//     testEl->leftSet1.remove(leftP2);
//     ASSERT_EQ(testEl->rootSet.size(), 1);
//     ASSERT_FALSE(testEl->rootSet.contains(rightP2));
//     ASSERT_FALSE(testEl->rootSet.contains(rightP));
//     ASSERT_FALSE(testEl->rootSet.contains(leftP2));
//     ASSERT_TRUE(testEl->rootSet.contains(leftP));
//     ASSERT_EQ(testEl->rightSet1.size(), 0);
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP));
//     ASSERT_EQ(testEl->rightSet2.size(), 0);
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP));
//     ASSERT_EQ(testEl->leftSet1.size(), 1);
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet1.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet1.contains(leftP));
//     ASSERT_EQ(testEl->leftSet2.size(), 1);
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
//     ASSERT_TRUE(testEl->leftSet2.contains(leftP));

//     testEl->leftSet2.remove(leftP);
//     ASSERT_EQ(testEl->rootSet.size(), 0);
//     ASSERT_FALSE(testEl->rootSet.contains(rightP2));
//     ASSERT_FALSE(testEl->rootSet.contains(rightP));
//     ASSERT_FALSE(testEl->rootSet.contains(leftP2));
//     ASSERT_FALSE(testEl->rootSet.contains(leftP));
//     ASSERT_EQ(testEl->rightSet1.size(), 0);
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet1.contains(leftP));
//     ASSERT_EQ(testEl->rightSet2.size(), 0);
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(rightP));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->rightSet2.contains(leftP));
//     ASSERT_EQ(testEl->leftSet1.size(), 0);
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet1.contains(leftP2));
//     ASSERT_FALSE(testEl->leftSet1.contains(leftP));
//     ASSERT_EQ(testEl->leftSet2.size(), 0);
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(rightP));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP2));
//     ASSERT_FALSE(testEl->leftSet2.contains(leftP));
// }

// TEST_F(SetTest, set2RedefinesTest) {
//     Manager<> m;
//     UmlPtr<RedefinedTestElement> testEl = m.create<RedefinedTestElement>();
//     Package& p = *m.create<Package>();
//     testEl->redefiningSet.add(p);
//     ASSERT_EQ(testEl->rootSet.size(), 1);
//     ASSERT_EQ(testEl->redefiningSet.size(), 1);
//     ASSERT_TRUE(testEl->rootSet.contains(p));
//     ASSERT_TRUE(testEl->redefiningSet.contains(p));
//     testEl->redefiningSet.remove(p);
//     ASSERT_EQ(testEl->rootSet.size(), 0);
//     ASSERT_EQ(testEl->redefiningSet.size(), 0);
//     ASSERT_FALSE(testEl->rootSet.contains(p));
//     ASSERT_FALSE(testEl->redefiningSet.contains(p));
//     Package& p2 = *m.create<Package>();
//     testEl->redefiningSet.add(p);
//     ASSERT_EQ(testEl->rootSet.size(), 1);
//     ASSERT_EQ(testEl->redefiningSet.size(), 1);
//     testEl->rootSet.remove(p);
//     ASSERT_EQ(testEl->rootSet.size(), 0);
//     ASSERT_EQ(testEl->redefiningSet.size(), 0);
//     ASSERT_FALSE(testEl->rootSet.contains(p));
//     ASSERT_FALSE(testEl->redefiningSet.contains(p));
//     testEl->rootSet.add(p);
//     ASSERT_EQ(testEl->rootSet.size(), 1);
//     ASSERT_EQ(testEl->redefiningSet.size(), 1);
//     ASSERT_TRUE(testEl->rootSet.contains(p));
//     ASSERT_TRUE(testEl->redefiningSet.contains(p));
//     testEl->redefiningSet.add(p2);
//     ASSERT_EQ(testEl->rootSet.size(), 2);
//     ASSERT_EQ(testEl->redefiningSet.size(), 2);
//     Package& p3 = *m.create<Package>();
//     testEl->rootSet.add(p3);
//     ASSERT_EQ(testEl->rootSet.size(), 3);
//     ASSERT_EQ(testEl->redefiningSet.size(), 3);
// }

// TEST_F(SetTest, orderedSet2AllocationPolicy) {
//     Manager<> m;
//     UmlPtr<TestElementWOrderedSet> el = m.create<TestElementWOrderedSet>();
//     UmlPtr<TestElementWOrderedSet> otherEl = m.create<TestElementWOrderedSet>();
//     UmlPtr<TestElementWOrderedSet> otherEl2 = m.create<TestElementWOrderedSet>();
//     UmlPtr<TestElementWOrderedSet> otherEl3 = m.create<TestElementWOrderedSet>();
//     el->getOthers().add(otherEl);
//     ASSERT_EQ(el->getOthers().get(otherEl.id()), otherEl);
//     ASSERT_EQ(el->getOthers().front(), otherEl);
//     ASSERT_EQ(el->getOthers().back(), otherEl);
//     el->getOthers().add(otherEl2);
//     ASSERT_EQ(el->getOthers().front(), otherEl);
//     ASSERT_EQ(el->getOthers().back(), otherEl2);
//     el->getOthers().add(otherEl3);
//     ASSERT_EQ(el->getOthers().front(), otherEl);
//     ASSERT_EQ(el->getOthers().back(), otherEl3);
//     el->getOthers().remove(otherEl3);
//     ASSERT_EQ(el->getOthers().front(), otherEl);
//     ASSERT_EQ(el->getOthers().back(), otherEl2);
//     el->getOthers().remove(otherEl);
//     ASSERT_EQ(el->getOthers().front(), otherEl2);
//     ASSERT_EQ(el->getOthers().back(), otherEl2);
// }

// class TestElementWSingleton : public Element {
//     public:
//         Singleton2<TestElementWSingleton, TestElementWSingleton> singleton = Singleton2<TestElementWSingleton, TestElementWSingleton>(this);
//         TestElementWSingleton() : Element(ElementType::ELEMENT) {};
// };

// TEST_F(SetTest, singleton2Test) {
//     Manager<> m;
//     UmlPtr<TestElementWSingleton> testEl = m.create<TestElementWSingleton>();
//     UmlPtr<TestElementWSingleton> el = m.create<TestElementWSingleton>();
//     UmlPtr<TestElementWSingleton> otherEl = m.create<TestElementWSingleton>();
//     testEl->singleton.set(el);
//     ASSERT_EQ(testEl->singleton.get(), el);
//     testEl->singleton.set(0);
//     ASSERT_FALSE(testEl->singleton.get());
//     testEl->singleton.set(el);
//     ASSERT_EQ(testEl->singleton.get(), el);
//     testEl->singleton.set(otherEl);
//     ASSERT_EQ(testEl->singleton.get(), otherEl);
// }

// class TestElementWFunctor : public Element {
//     public:
//         Set2<TestElementWFunctor, TestElementWFunctor> set = Set2<TestElementWFunctor, TestElementWFunctor>(this);

//         int count = 0;
        
//         class TestFunctor : public SetFunctor2<TestElementWFunctor, TestElementWFunctor> {
//             public:
//                 TestFunctor(TestElementWFunctor& f) : SetFunctor2<TestElementWFunctor, TestElementWFunctor>(f) {}
//                 void operator()(TestElementWFunctor& el) override {
//                     std::cout << "PP" << std::endl;
//                     el.count++;
//                 }
//         };

//         TestFunctor functor = TestFunctor(*this);

//         TestElementWFunctor() : Element(ElementType::ELEMENT) {
//             set.setAddFunctor(functor);
//             set.setRemoveFunctor(functor);
//         }
// };

// TEST_F(SetTest, testFunctorsW2) {
//     Manager<> m;
//     UmlPtr<TestElementWFunctor> el = m.create<TestElementWFunctor>();
//     UmlPtr<TestElementWFunctor> otherEl = m.create<TestElementWFunctor>();
//     el->set.add(otherEl);
//     ASSERT_EQ(otherEl->count, 1);
//     el->set.remove(otherEl);
//     ASSERT_EQ(otherEl->count, 2);
// }

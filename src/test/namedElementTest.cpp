#include "gtest/gtest.h"
#include "uml/uml-stable.h"
// #include "test/umlTestUtil.h"
#include <filesystem>

using namespace UML;

class NamedElementTest : public ::testing::Test {
public:
    std::string ymlPath;
    void SetUp() override {
        ymlPath = "../examples/";
    };
};

TEST_F(NamedElementTest, SetNameTest) {
    UmlManager m;
    auto& namedEl = *m.create<Package>();
    namedEl.setName("test");
    EXPECT_EQ(namedEl.getName(), "test");
}

TEST_F(NamedElementTest, GetNullNameTest) {
    UmlManager m;
    auto& ne = *m.create<Package>();
    ASSERT_NO_THROW(ne.getName());
    EXPECT_TRUE(ne.getName().compare("") == 0);
}

// TEST_F(NamedElementTest, reIndexNameTest) {
//     UmlManager m;
//     Package& e1 = *m.create<Package>();
//     Package& e2 = *m.create<Package>();
//     e1.getPackagedElements().add(e2);
//     // e2.setOwningPackage(&e1); // TODO do we want this to work?
//     e2.setName("test");
//     ASSERT_NO_THROW(e1.getOwnedElements().get("test"));
//     ASSERT_EQ(e1.getOwnedElements().get("test"), e2);
// }

TEST_F(NamedElementTest, overwriteNamespaceTest) {
    UmlManager m;
    auto& p1 = *m.create<Package>();
    auto& p2 = *m.create<Package>();
    auto& c = *m.create<Package>();
    p1.getPackagedElements().add(c);
    c.setOwningPackage(p2);
    ASSERT_EQ(p2.getOwnedMembers().size(), 1);
    ASSERT_EQ(*p2.getOwnedMembers().front(), c);
    ASSERT_EQ(*c.getNamespace(), p2);
    ASSERT_EQ(p1.getOwnedMembers().size(), 0);
}

TEST_F(NamedElementTest, overwriteNamespaceByOwnedMemebersAddTest) {
    UmlManager m;
    auto& p1 = *m.create<Package>();
    auto& p2 = *m.create<Package>();
    auto& c = *m.create<Package>();
    p1.getPackagedElements().add(c);
    p2.getPackagedElements().add(c);
    ASSERT_EQ(p2.getOwnedMembers().size(), 1);
    ASSERT_EQ(*p2.getOwnedMembers().front(), c);
    ASSERT_EQ(*c.getNamespace(), p2);
    ASSERT_EQ(p1.getOwnedMembers().size(), 0);
}

TEST_F(NamedElementTest, visibilityTest) {
    UmlManager m;
    auto& n = *m.create<Package>();
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::NONE);
    ASSERT_NO_THROW(n.setVisibility(VisibilityKind::PRIVATE));
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PRIVATE);
}

TEST_F(NamedElementTest, singletonMethodsTest) {
    UmlManager m;
    auto& p = *m.create<Package>();
    auto& c = *m.create<Package>();
    ASSERT_FALSE(c.getNamespace());
    c.setOwningPackage(p);
    ASSERT_TRUE(c.getNamespace());
    ASSERT_EQ(*c.getNamespace(), p);
    c.setOwningPackage(0);
    ASSERT_FALSE(c.getNamespace());
}

TEST_F(NamedElementTest, eraseNamepaceTest) {
    UmlManager m;
    auto& package = *m.create<Package>();
    auto& nmspc = *m.create<Package>();
    nmspc.getPackagedElements().add(package);
    m.setRoot(&nmspc);
    m.mount(ymlPath + "namedElementTests");
    EGM::ID nmspcID = nmspc.getID();
    m.erase(nmspc);
    ASSERT_FALSE(std::filesystem::exists((ymlPath + "namedElementTests/" + nmspcID.string() + ".yml")));
    ASSERT_FALSE(m.loaded(nmspcID));
    ASSERT_FALSE(package.getNamespace());
    ASSERT_FALSE(package.getOwner());
}

TEST_F(NamedElementTest, testQualifiedName) {
    UmlManager m;
    auto& one = *m.create<Package>();
    auto& two = *m.create<Package>();
    auto& three = *m.create<Package>();
    one.setName("1");
    two.setName("2");
    one.getPackagedElements().add(two);
    ASSERT_EQ(one.getQualifiedName(), "1");
    ASSERT_EQ(two.getQualifiedName(), "1::2");
    two.getPackagedElements().add(three);
    ASSERT_EQ(three.getQualifiedName(), "1::2::");
    three.setName("3");
    ASSERT_EQ(three.getQualifiedName(), "1::2::3");
    two.getPackagedElements().remove(three);
    ASSERT_EQ(three.getQualifiedName(), "3");
    two.setName("");
    ASSERT_EQ(two.getQualifiedName(), "1::");
}

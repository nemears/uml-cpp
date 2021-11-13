#include "gtest/gtest.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/package.h"
#include "uml/umlManager.h"
#include "test/umlTestUtil.h"
#include "test/yumlParsersTest.h"

using namespace UML;

class NamedElementTest : public ::testing::Test {
public:
    std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(NamedElementTest, SetNameTest) {
    UmlManager m;
    Package namedEl = m.create<Package>();
    namedEl.setName("test");
    EXPECT_EQ(namedEl.getName(), "test");
}

TEST_F(NamedElementTest, GetNullNameTest) {
    UmlManager m;
    Package ne = m.create<Package>();
    ASSERT_NO_THROW(ne.getName());
    EXPECT_TRUE(ne.getName().compare("") == 0);
}

// TEST_F(NamedElementTest, reIndexNameTest) {
//   Package e1;
//   Package e2;
//   e1.getPackagedElements().add(e2);
//   e2.setOwningPackage(&e1);
//   e2.setName("test");
//   ASSERT_TRUE(e1.getOwnedElements().get("test") != NULL);
// }

TEST_F(NamedElementTest, overwriteNamespaceTest) {
    UmlManager m;
    Package p1 = m.create<Package>();
    Package p2 = m.create<Package>();
    Package c = m.create<Package>();
    p1.getPackagedElements().add(c);
    c.setOwningPackage(p2);
    ASSERT_EQ(p2.getOwnedMembers().size(), 1);
    ASSERT_EQ(p2.getOwnedMembers().front(), c);
    ASSERT_EQ(*c.getNamespace(), p2);
    ASSERT_EQ(p1.getOwnedMembers().size(), 0);
}

TEST_F(NamedElementTest, overwriteNamespaceByOwnedMemebersAddTest) {
    UmlManager m;
    Package p1 = m.create<Package>();
    Package p2 = m.create<Package>();
    Package c = m.create<Package>();
    p1.getPackagedElements().add(c);
    p2.getPackagedElements().add(c);
    ASSERT_EQ(p2.getOwnedMembers().size(), 1);
    ASSERT_EQ(p2.getOwnedMembers().front(), c);
    ASSERT_EQ(*c.getNamespace(), p2);
    ASSERT_EQ(p1.getOwnedMembers().size(), 0);
}

TEST_F(NamedElementTest, copyNamedElementTest) {
    UmlManager m;
    Package n = m.create<Package>();
    n.setName("test");
    Package p = m.create<Package>();
    Package c = m.create<Package>();
    n.setOwningPackage(&p);
    n.getPackagedElements().add(c);
    Package n2 = n;
    ASSERT_TRUE(n2.getName().compare("test") == 0);
    ASSERT_EQ(n2, n);
    ASSERT_EQ(*n2.getOwner(), p);
    ASSERT_TRUE(n2.hasNamespace());
    ASSERT_TRUE(n2.getNamespace());
    ASSERT_EQ(n2.getNamespaceRef(), p);
    ASSERT_EQ(n2.getOwnedElements().size(), 1);
    ASSERT_EQ(n2.getOwnedElements().get(c.getID()), c);
    ASSERT_EQ(*c.getOwner(), n);
}

TEST_F(NamedElementTest, visibilityTest) {
    UmlManager m;
    Package n = m.create<Package>();
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PUBLIC);
    ASSERT_NO_THROW(n.setVisibility(VisibilityKind::PRIVATE));
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PRIVATE);
}

TEST_F(NamedElementTest, copyAndEditTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Package& c = m.create<Package>();
    p.getPackagedElements().add(c);
    c.setName("test");
    c.setVisibility(VisibilityKind::PRIVATE);
    Package copy = c;
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
    ASSERT_EQ(c.getVisibility(), copy.getVisibility());
    p.getPackagedElements().remove(copy);
    c.setVisibility(VisibilityKind::PROTECTED);
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
    ASSERT_EQ(c.getVisibility(), copy.getVisibility());
    copy.setOwningPackage(&p);
    copy.setVisibility(VisibilityKind::PUBLIC);
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
    ASSERT_EQ(c.getVisibility(), copy.getVisibility());
    p.setOwningPackage(0);
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
    ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
}

TEST_F(NamedElementTest, singletonMethodsTest) {
    UmlManager m;
    Package p = m.create<Package>();
    Package c = m.create<Package>();
    ASSERT_FALSE(c.hasNamespace());
    ASSERT_FALSE(c.getNamespace());
    ASSERT_THROW(c.getNamespaceRef(), NullReferenceException);
    c.setOwningPackage(p);
    ASSERT_TRUE(c.hasNamespace());
    ASSERT_EQ(*c.getNamespace(), p);
    ASSERT_EQ(c.getNamespaceRef(), p);
    c.setOwningPackage(0);
    ASSERT_FALSE(c.hasNamespace());
    ASSERT_FALSE(c.getNamespace());
    ASSERT_THROW(c.getNamespaceRef(), NullReferenceException);
}

TEST_F(NamedElementTest, eraseNamepaceTest) {
    UmlManager m;
    Package& package = m.create<Package>();
    Package& nmspc = m.create<Package>();
    nmspc.getPackagedElements().add(package);
    m.setRoot(nmspc);
    m.mount(ymlPath + "namedElementTests");
    ID nmspcID = nmspc.getID();
    m.erase(nmspc);
    ASSERT_FALSE(std::filesystem::exists((ymlPath + "namedElementTests/" + nmspcID.string() + ".yml")));
    ASSERT_FALSE(m.loaded(nmspcID));
    ASSERT_FALSE(package.hasNamespace());
    ASSERT_TRUE(package.getMemberNamespace().empty());
    ASSERT_FALSE(package.hasOwner());
}

TEST_F(NamedElementTest, testQualifiedName) {
    UmlManager m;
    Package one = m.create<Package>();
    Package two = m.create<Package>();
    Package three = m.create<Package>();
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
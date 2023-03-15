#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(ExtensionOwnedEnd, ExtensionEnd, Extension, &Extension::getOwnedEnd, &Extension::setOwnedEnd);
// UML_SINGLETON_INTEGRATION_TEST(ExtensionEndType, Stereotype, ExtensionEnd, &ExtensionEnd::getType, &ExtensionEnd::setType);

class ExtensionTest : public ::testing::Test {};

TEST_F(ExtensionTest, basicExtensionTest) {
    Manager<> m;
    Extension& ext = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(end);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd());
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getOwningAssociation());
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
    ASSERT_TRUE(end.getOwningAssociation());
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
}

TEST_F(ExtensionTest, setOwnedEndToNullTest) {
    Manager<> m;
    Extension& ext = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(end);
    ext.setOwnedEnd(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_FALSE(ext.getOwnedEnd());
    ASSERT_EQ(ext.getOwnedEnds().size(), 0);
    ASSERT_FALSE(end.getOwningAssociation());
}

TEST_F(ExtensionTest, setExtensionToNullTest) {
    Manager<> m;
    Extension& ext = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    end.setOwningAssociation(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_FALSE(ext.getOwnedEnd());
    ASSERT_EQ(ext.getOwnedEnds().size(), 0);
    ASSERT_FALSE(end.getOwningAssociation());
}

TEST_F(ExtensionTest, setEndTypeTest) {
    Manager<> m;
    Extension& ext = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    Stereotype& t = *m.create<Stereotype>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(end);
    end.setType(t);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd());
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getOwningAssociation());
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
    ASSERT_TRUE(end.getType());
    ASSERT_EQ(end.getType()->getID(), t.getID());
    ASSERT_EQ(ext.getEndTypes().size(), 1);
    ASSERT_EQ(ext.getEndTypes().front().getID(), t.getID());
}

TEST_F(ExtensionTest, setEndTypeNullTest) {
    Manager<> m;
    Extension& ext = *m.create<Extension>();
    ExtensionEnd& end = *m.create<ExtensionEnd>();
    Stereotype& t = *m.create<Stereotype>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(end);
    end.setType(t);
    end.setType(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd());
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getOwningAssociation());
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
    ASSERT_FALSE(end.getType());
    ASSERT_EQ(ext.getEndTypes().size(), 0);
}
#include "gtest/gtest.h"
#include "uml/extension.h"
#include "uml/extensionEnd.h"

using namespace UML;

class ExtensionTest : public ::testing::Test {};

TEST_F(ExtensionTest, basicExtensionTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() != 0);
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getExtension() != 0);
    ASSERT_EQ(end.getExtension()->getID(), ext.getID());
    ASSERT_TRUE(end.getOwningAssociation() != 0);
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
}

TEST_F(ExtensionTest, setOwnedEndToNullTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    ext.setOwnedEnd(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() == 0);
    ASSERT_EQ(ext.getOwnedEnds().size(), 0);
    ASSERT_TRUE(end.getExtension() == 0);
    ASSERT_TRUE(end.getOwningAssociation() == 0);
}

TEST_F(ExtensionTest, setExtensionToNullTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    end.setExtension(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() == 0);
    ASSERT_EQ(ext.getOwnedEnds().size(), 0);
    ASSERT_TRUE(end.getExtension() == 0);
    ASSERT_TRUE(end.getOwningAssociation() == 0);
}

TEST_F(ExtensionTest, setEndTypeTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    Stereotype& t = m.create<Stereotype>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    end.setType(&t);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() != 0);
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getExtension() != 0);
    ASSERT_EQ(end.getExtension()->getID(), ext.getID());
    ASSERT_TRUE(end.getOwningAssociation() != 0);
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
    ASSERT_TRUE(end.getType() != 0);
    ASSERT_EQ(end.getType()->getID(), t.getID());
    ASSERT_EQ(ext.getEndType().size(), 1);
    ASSERT_EQ(ext.getEndType().front()->getID(), t.getID());
}

TEST_F(ExtensionTest, setEndTypeNullTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    Stereotype& t = m.create<Stereotype>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    end.setType(&t);
    end.setType(0);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() != 0);
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
    ASSERT_TRUE(end.getExtension() != 0);
    ASSERT_EQ(end.getExtension()->getID(), ext.getID());
    ASSERT_TRUE(end.getOwningAssociation() != 0);
    ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
    ASSERT_TRUE(end.getType() == 0);
    ASSERT_EQ(ext.getEndType().size(), 0);
}
#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(ExtensionOwnedEnd, ExtensionEnd, Extension, &Extension::getOwnedEnd, &Extension::setOwnedEnd)
// UML_SINGLETON_INTEGRATION_TEST(ExtensionEndType, Stereotype, ExtensionEnd, &ExtensionEnd::getType, &ExtensionEnd::setType);

class ExtensionTest : public ::testing::Test {};

TEST_F(ExtensionTest, extensionMemberEndsTest) {
    Manager<> m;
    ExtensionPtr e = m.create<Extension>();
    ExtensionEndPtr end = m.create<ExtensionEnd>();
    PropertyPtr memberEnd = m.create<Property>();
    e->setOwnedEnd(end);
    e->getMemberEnds().add(memberEnd);
    ASSERT_EQ(e->getMemberEnds().size(), 2);
    ASSERT_EQ(e->Association::getMemberEnds().size(), 2);
    ASSERT_EQ(e->getOwnedEnds().size(), 1);
    ASSERT_EQ(e->getMemberEnds().front(), end);
    ASSERT_EQ(e->getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->Association::getMemberEnds().front(), end);
    ASSERT_EQ(e->Association::getMemberEnds().back(), memberEnd);
    ASSERT_EQ(e->getOwnedEnds().front(), end);
    ASSERT_EQ(e->getOwnedEnds().back(), end);
    ASSERT_EQ(e->getOwnedEnd(), end);
    e->getMemberEnds().clear();
    ASSERT_EQ(e->getMemberEnds().front(), 0);
    ASSERT_EQ(e->getMemberEnds().back(), 0);
    ASSERT_EQ(e->Association::getMemberEnds().front(), 0);
    ASSERT_EQ(e->Association::getMemberEnds().back(), 0);
    ASSERT_EQ(e->getOwnedEnds().front(), 0);
    ASSERT_EQ(e->getOwnedEnds().back(), 0);
    ASSERT_EQ(e->getOwnedEnd(), 0);
}

// TEST_F(ExtensionTest, basicExtensionTest) {
//     Manager<> m;
//     Extension& ext = *m.create<Extension>();
//     ExtensionEnd& end = *m.create<ExtensionEnd>();
//     ext.setMetaClass(ElementType::PACKAGE);
//     ext.setOwnedEnd(end);
//     ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
//     ASSERT_TRUE(ext.getOwnedEnd());
//     ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
//     ASSERT_TRUE(end.getOwningAssociation());
//     ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
//     ASSERT_TRUE(end.getOwningAssociation());
//     ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
// }

// TEST_F(ExtensionTest, setOwnedEndToNullTest) {
//     Manager<> m;
//     Extension& ext = *m.create<Extension>();
//     ExtensionEnd& end = *m.create<ExtensionEnd>();
//     ext.setMetaClass(ElementType::PACKAGE);
//     ext.setOwnedEnd(end);
//     ext.setOwnedEnd(0);
//     ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
//     ASSERT_FALSE(ext.getOwnedEnd());
//     ASSERT_EQ(ext.getOwnedEnds().size(), 0);
//     ASSERT_FALSE(end.getOwningAssociation());
// }

// TEST_F(ExtensionTest, setExtensionToNullTest) {
//     Manager<> m;
//     Extension& ext = *m.create<Extension>();
//     ExtensionEnd& end = *m.create<ExtensionEnd>();
//     ext.setMetaClass(ElementType::PACKAGE);
//     ext.setOwnedEnd(&end);
//     end.setOwningAssociation(0);
//     ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
//     ASSERT_FALSE(ext.getOwnedEnd());
//     ASSERT_EQ(ext.getOwnedEnds().size(), 0);
//     ASSERT_FALSE(end.getOwningAssociation());
// }

// TEST_F(ExtensionTest, setEndTypeTest) {
//     Manager<> m;
//     Extension& ext = *m.create<Extension>();
//     ExtensionEnd& end = *m.create<ExtensionEnd>();
//     Stereotype& t = *m.create<Stereotype>();
//     ext.setMetaClass(ElementType::PACKAGE);
//     ext.setOwnedEnd(end);
//     end.setType(t);
//     ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
//     ASSERT_TRUE(ext.getOwnedEnd());
//     ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
//     ASSERT_TRUE(end.getOwningAssociation());
//     ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
//     ASSERT_TRUE(end.getType());
//     ASSERT_EQ(end.getType()->getID(), t.getID());
//     ASSERT_EQ(ext.getEndTypes().size(), 1);
//     ASSERT_EQ(ext.getEndTypes().front().getID(), t.getID());
// }

// TEST_F(ExtensionTest, setEndTypeNullTest) {
//     Manager<> m;
//     Extension& ext = *m.create<Extension>();
//     ExtensionEnd& end = *m.create<ExtensionEnd>();
//     Stereotype& t = *m.create<Stereotype>();
//     ext.setMetaClass(ElementType::PACKAGE);
//     ext.setOwnedEnd(end);
//     end.setType(t);
//     end.setType(0);
//     ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
//     ASSERT_TRUE(ext.getOwnedEnd());
//     ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
//     ASSERT_TRUE(end.getOwningAssociation());
//     ASSERT_EQ(end.getOwningAssociation()->getID(), ext.getID());
//     ASSERT_FALSE(end.getType());
//     ASSERT_EQ(ext.getEndTypes().size(), 0);
// }
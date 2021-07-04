#include "gtest/gtest.h"
#include "uml/extension.h"
#include "uml/extensionEnd.h"

using namespace UML;

class ProfileApplicationTest : public ::testing::Test {};

TEST_F(ProfileApplicationTest, basicExtensionTest) {
    UmlManager m;
    Extension& ext = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    ext.setMetaClass(ElementType::PACKAGE);
    ext.setOwnedEnd(&end);
    ASSERT_EQ(ext.getMetaClass(), ElementType::PACKAGE);
    ASSERT_TRUE(ext.getOwnedEnd() != 0);
    ASSERT_EQ(ext.getOwnedEnd()->getID(), end.getID());
}
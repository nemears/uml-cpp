#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/profile.h"
#include "uml/extensionEnd.h"

using namespace std;
using namespace UML;

class ProfileParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ProfileParserTest, stereotypeWithExtensionTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "profileTests/stereotypeW_Extension.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PROFILE);
    Profile& profile = dynamic_cast<Profile&>(*el);
    ASSERT_EQ(profile.getOwnedStereotypes().size(), 1);
    Stereotype& s = *profile.getOwnedStereotypes().front();
    ASSERT_EQ(profile.getPackagedElements().size(), 2);
    ASSERT_EQ(profile.getPackagedElements().front()->getElementType(), ElementType::EXTENSION);
    Extension& ext = dynamic_cast<Extension&>(*profile.getPackagedElements().front());
    ASSERT_EQ(ext.getMetaClass(), ElementType::CLASS);
    ASSERT_TRUE(ext.getOwnedEnd() != 0);
    ExtensionEnd& end = *ext.getOwnedEnd();
    ASSERT_TRUE(end.getType() != 0);
    ASSERT_EQ(end.getType()->getID(), s.getID());
}

TEST_F(ProfileParserTest, internalProfileapplication) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "profileTests/internalProfileApplication.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = *dynamic_cast<Package*>(el);
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::PACKAGE);
    Package& applying = *dynamic_cast<Package*>(pckg.getPackagedElements().front());
    ASSERT_EQ(applying.getProfileApplications().size(), 1);
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::PROFILE);
    Profile& profile = *dynamic_cast<Profile*>(pckg.getPackagedElements().back());
    ProfileApplication& application = *applying.getProfileApplications().front();
    ASSERT_EQ(application.getAppliedProfile()->getID(), profile.getID());
}
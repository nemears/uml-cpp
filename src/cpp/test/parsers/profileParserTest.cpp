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

TEST_F(ProfileParserTest, externalProfileApplicationTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "profileTests/externalProfileApplication.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = *dynamic_cast<Package*>(el);
    ASSERT_EQ(pckg.getProfileApplications().size(), 1);
    ProfileApplication& application = *pckg.getProfileApplications().front();
    ASSERT_EQ(application.getAppliedProfile()->getID(), ID::fromString("XIf5yPHTzLz4NDkVLLwDamOWscKb"));
    //lazy
}

TEST_F(ProfileParserTest, emitProfileTest) {
    UmlManager m;
    Profile& profile = m.create<Profile>();
    Stereotype& stereotype = m.create<Stereotype>();
    Extension& extension = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    profile.setID("83lphS&gucqvJwW&KSzVmTSMMG1z");
    extension.setID("&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ");
    end.setID("t&ZWitKKpMcvG9Dzwh23wSbP1hr5");
    stereotype.setID("7PJxQhyjuuWylik9y2fgpNDXmMdv");
    end.setType(&stereotype);
    extension.setOwnedEnd(&end);
    extension.setMetaClass(ElementType::OPERATION);
    profile.getOwnedStereotypes().add(stereotype);
    profile.getPackagedElements().add(extension);
    string expectedEmit = R""""(profile:
  id: 83lphS&gucqvJwW&KSzVmTSMMG1z
  packagedElements:
    - extension:
        id: "&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ"
        metaClass: OPERATION
        ownedEnd:
          extensionEnd:
            id: t&ZWitKKpMcvG9Dzwh23wSbP1hr5
            type: 7PJxQhyjuuWylik9y2fgpNDXmMdv
  ownedStereotypes:
    - stereotype:
        id: 7PJxQhyjuuWylik9y2fgpNDXmMdv)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(profile));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
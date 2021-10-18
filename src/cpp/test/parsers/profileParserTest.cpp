#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/profile.h"
#include "uml/extensionEnd.h"
#include "uml/profileApplication.h"
#include "uml/extension.h"
#include "uml/instanceSpecification.h"
#include "test/umlTestUtil.h"

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
    Stereotype& s = profile.getOwnedStereotypes().front();
    ASSERT_EQ(profile.getPackagedElements().size(), 2);
    ASSERT_EQ(profile.getPackagedElements().front().getElementType(), ElementType::EXTENSION);
    Extension& ext = dynamic_cast<Extension&>(profile.getPackagedElements().front());
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
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::PACKAGE);
    Package& applying = dynamic_cast<Package&>(pckg.getPackagedElements().front());
    ASSERT_EQ(applying.getProfileApplications().size(), 1);
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::PROFILE);
    Profile& profile = dynamic_cast<Profile&>(pckg.getPackagedElements().back());
    ProfileApplication& application = applying.getProfileApplications().front();
    ASSERT_EQ(application.getAppliedProfile()->getID(), profile.getID());
}

TEST_F(ProfileParserTest, externalProfileApplicationTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "profileTests/externalProfileApplication.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = *dynamic_cast<Package*>(el);
    ASSERT_EQ(pckg.getProfileApplications().size(), 1);
    ProfileApplication& application = pckg.getProfileApplications().front();
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
            association: "&nOhZzwgZ9xoJVAtXDUVQpLf7LTZ"
  ownedStereotypes:
    - stereotype:
        id: 7PJxQhyjuuWylik9y2fgpNDXmMdv
        profile: 83lphS&gucqvJwW&KSzVmTSMMG1z)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(profile));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ProfileParserTest, emitProfileApplication) {
    UmlManager m;
    Package& root = m.create<Package>();
    Package& applying = m.create<Package>();
    Profile& profile = m.create<Profile>();
    ProfileApplication& application  = m.create<ProfileApplication>();
    root.setID("BW5iaVf_WdBebuIH3yi9beXpG5Yi");
    applying.setID("BtO&7RDq4sOe2Cb3hl_bByknWtDU");
    application.setID("QbTzWJmjUCFjrufpPQc9qyeQdK3R");
    profile.setID("R12X_VJHWWUKmJS_F8JotXJZzsNB");
    application.setAppliedProfile(&profile);
    applying.getProfileApplications().add(application);
    root.getPackagedElements().add(applying);
    root.getPackagedElements().add(profile);
    string expectedEmit = R""""(package:
  id: BW5iaVf_WdBebuIH3yi9beXpG5Yi
  packagedElements:
    - package:
        id: BtO&7RDq4sOe2Cb3hl_bByknWtDU
        profileApplications:
          - profileApplication:
              id: QbTzWJmjUCFjrufpPQc9qyeQdK3R
              appliedProfile: R12X_VJHWWUKmJS_F8JotXJZzsNB
    - profile:
        id: R12X_VJHWWUKmJS_F8JotXJZzsNB)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ProfileParserTest, parseAppliedStereotypeTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "profileTests/appliedStereotype.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& root = *dynamic_cast<Package*>(el);
    ASSERT_EQ(root.getPackagedElements().size(), 2);
    ASSERT_EQ(root.getPackagedElements().front().getElementType(), ElementType::PACKAGE);
    Package& applying = dynamic_cast<Package&>(root.getPackagedElements().front());
    ASSERT_EQ(applying.getProfileApplications().size(), 1);
    ProfileApplication& application = applying.getProfileApplications().front();
    ASSERT_TRUE(application.getAppliedProfile() != 0);
    ASSERT_EQ(root.getPackagedElements().back().getElementType(), ElementType::PROFILE);
    Profile& profile = dynamic_cast<Profile&>(root.getPackagedElements().back());
    ASSERT_EQ(application.getAppliedProfile()->getID(), profile.getID());
    ASSERT_EQ(applying.getPackagedElements().size(), 1);
    ASSERT_EQ(applying.getPackagedElements().front().getElementType(), ElementType::PACKAGE);
    Package& typed = dynamic_cast<Package&>(applying.getPackagedElements().front());
    ASSERT_EQ(typed.getAppliedStereotypes().size(), 1);
    InstanceSpecification& stereotypeInst = typed.getAppliedStereotypes().front();
    ASSERT_TRUE(stereotypeInst.getClassifier() != 0);
    ASSERT_EQ(stereotypeInst.getClassifier()->getElementType(), ElementType::STEREOTYPE);
    ASSERT_EQ(profile.getOwnedStereotypes().size(), 1);
    Stereotype& stereotype = profile.getOwnedStereotypes().front();
    ASSERT_EQ(stereotypeInst.getClassifier()->getID(), stereotype.getID());
}

TEST_F(ProfileParserTest, emitAppliedStereotypeTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Package& applying = m.create<Package>();
    ProfileApplication& application = m.create<ProfileApplication>();
    Package& stereotypedEl = m.create<Package>();
    InstanceSpecification& stereotypeInst = m.create<InstanceSpecification>();
    Profile& profile = m.create<Profile>();
    Stereotype& stereotype = m.create<Stereotype>();
    Extension& extension = m.create<Extension>();
    ExtensionEnd& end = m.create<ExtensionEnd>();
    root.setID("jswJELYwKd_wleha5klF&GJFcU_0");
    applying.setID("Lf963Dxo5MFIqi9ip7&Nj4l1f1yj");
    application.setID("MUiSKR6gArugHOb1RqZtF5_uhflV");
    profile.setID("I3QrZblFek6tdX&j70kCP8u4QNAh");
    stereotypedEl.setID("wX&KNwgtwFYOQ0B4eIweaaRz&QC1");
    stereotypeInst.setID("l3q&INpC6kqcdavsgIMSrSNNpGEt");
    extension.setID("jjf&mHlwFSAjJXsBqng4IlxfYIJh");
    end.setID("FK1SGxJ2lV&5RtbRhiGU9jR0zAsw");
    stereotype.setID("x5r8XggyW2DI5c3RyAS8r_arWh79");
    profile.getOwnedStereotypes().add(stereotype);
    extension.setMetaClass(ElementType::PACKAGE);
    end.setType(&stereotype);
    extension.setOwnedEnd(&end);
    profile.getPackagedElements().add(extension);
    application.setAppliedProfile(&profile);
    applying.getProfileApplications().add(application);
    stereotypeInst.setClassifier(&stereotype);
    stereotypedEl.getAppliedStereotypes().add(stereotypeInst);
    applying.getPackagedElements().add(stereotypedEl);
    root.getPackagedElements().add(applying);
    root.getPackagedElements().add(profile);
    string expectedEmit = R""""(package:
  id: jswJELYwKd_wleha5klF&GJFcU_0
  packagedElements:
    - package:
        id: Lf963Dxo5MFIqi9ip7&Nj4l1f1yj
        profileApplications:
          - profileApplication:
              id: MUiSKR6gArugHOb1RqZtF5_uhflV
              appliedProfile: I3QrZblFek6tdX&j70kCP8u4QNAh
        packagedElements:
          - package:
              id: wX&KNwgtwFYOQ0B4eIweaaRz&QC1
              appliedStereotypes:
                - instanceSpecification:
                    id: l3q&INpC6kqcdavsgIMSrSNNpGEt
                    classifier: x5r8XggyW2DI5c3RyAS8r_arWh79
    - profile:
        id: I3QrZblFek6tdX&j70kCP8u4QNAh
        packagedElements:
          - extension:
              id: jjf&mHlwFSAjJXsBqng4IlxfYIJh
              metaClass: PACKAGE
              ownedEnd:
                extensionEnd:
                  id: FK1SGxJ2lV&5RtbRhiGU9jR0zAsw
                  type: x5r8XggyW2DI5c3RyAS8r_arWh79
                  association: jjf&mHlwFSAjJXsBqng4IlxfYIJh
        ownedStereotypes:
          - stereotype:
              id: x5r8XggyW2DI5c3RyAS8r_arWh79
              profile: I3QrZblFek6tdX&j70kCP8u4QNAh)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ProfileParserTest, mountProfileTest) {
  UmlManager m;
  Profile& profile = m.create<Profile>();
  Stereotype& stereotype = m.create<Stereotype>();
  Extension& extension = m.create<Extension>();
  ExtensionEnd& end = m.create<ExtensionEnd>();
  Class& applying = m.create<Class>();
  InstanceSpecification& stereotypeInst = m.create<InstanceSpecification>();
  Package& root = m.create<Package>();
  Package& pckg = m.create<Package>();
  profile.getOwnedStereotypes().add(stereotype);
  profile.getPackagedElements().add(extension);
  extension.setOwnedEnd(end);
  extension.setMetaClass(ElementType::CLASS);
  end.setType(stereotype);
  stereotypeInst.setClassifier(stereotype);
  applying.getAppliedStereotypes().add(stereotypeInst);
  pckg.getPackagedElements().add(applying);
  root.getPackagedElements().add(pckg, profile);
  m.setRoot(root);
  m.mount(ymlPath + "profileTests");

  ID profileID = profile.getID();
  m.release(profile);
  ASSERT_FALSE(m.loaded(profileID));
  Profile& profile2 = m.aquire(profileID)->as<Profile>();
  ASSERT_EQ(profile2.getPackagedElements().size(), 2);
  ASSERT_EQ(profile2.getPackagedElements().front(), extension);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(extension, profile2));
  ASSERT_EQ(profile2.getPackagedElements().back(), stereotype);
  ASSERT_EQ(profile2.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile2.getOwnedStereotypes().front(), stereotype);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype, profile2));

  ID stereotypeID = stereotype.getID();
  m.release(stereotype, profile2);
  ASSERT_FALSE(m.loaded(profileID));
  ASSERT_FALSE(m.loaded(stereotypeID));
  Profile& profile3 = m.aquire(profileID)->as<Profile>();
  ASSERT_EQ(profile3.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile3.getOwnedStereotypes().frontID(), stereotypeID);
  ASSERT_EQ(profile3.getPackagedElements().size(), 2);
  ASSERT_EQ(profile3.getPackagedElements().backID(), stereotypeID);
  Stereotype& stereotype2 = m.aquire(stereotypeID)->as<Stereotype>();
  ASSERT_TRUE(stereotype2.hasProfile());
  ASSERT_EQ(stereotype2.getProfileRef(), profile3);
  ASSERT_TRUE(stereotype2.hasOwningPackage());
  ASSERT_EQ(stereotype2.getOwningPackageRef(), profile3);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype2, profile3));

  m.release(stereotype2, profile3);
  ASSERT_FALSE(m.loaded(profileID));
  ASSERT_FALSE(m.loaded(stereotypeID));
  Stereotype& stereotype3 = m.aquire(stereotypeID)->as<Stereotype>();
  ASSERT_TRUE(stereotype3.hasProfile());
  ASSERT_EQ(stereotype3.getProfileID(), profileID);
  ASSERT_TRUE(stereotype3.hasOwningPackage());
  ASSERT_EQ(stereotype3.getOwningPackageID(), profileID);
  ASSERT_TRUE(stereotype3.hasNamespace());
  ASSERT_EQ(stereotype3.getNamespaceID(), profileID);
  ASSERT_TRUE(stereotype3.getMemberNamespace().count(profileID));
  ASSERT_TRUE(stereotype3.hasOwner());
  ASSERT_EQ(stereotype3.getOwnerID(), profileID);
  Profile& profile4 = m.aquire(profileID)->as<Profile>();
  ASSERT_EQ(profile4.getOwnedStereotypes().size(), 1);
  ASSERT_EQ(profile4.getOwnedStereotypes().front(), stereotype3);
  ASSERT_EQ(profile4.getPackagedElements().size(), 2);
  ASSERT_EQ(profile4.getPackagedElements().back(), stereotype3);
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(stereotype3, profile4));
}
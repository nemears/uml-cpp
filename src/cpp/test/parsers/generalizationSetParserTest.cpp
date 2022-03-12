#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/uml-cpp-paths.h"
#include "uml/parsers/parser.h"
#include "test/umlTestUtil.h"

using namespace UML;

class GeneralizationSetParserTest : public ::testing::Test {
   public:
    std::string ymlPath = YML_FILES_PATH;
};

TEST_F(GeneralizationSetParserTest, parseBasicGeneralizationSetTest) {
  UmlManager m;
  Element* el;
  ASSERT_NO_THROW(el = m.parse(ymlPath + "generalizationSetTests/basicGeneralizationSet.yml").ptr());
  ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
  Package& pckg = el->as<Package>();
  ASSERT_EQ(pckg.getPackagedElements().size(), 3);
  ASSERT_EQ(pckg.getPackagedElements().get("general").getElementType(), ElementType::CLASS);
  Class& general = pckg.getPackagedElements().get("general").as<Class>();
  ASSERT_EQ(general.getPowerTypeExtent().size(), 1);
  ASSERT_EQ(pckg.getPackagedElements().get("specific").getElementType(), ElementType::CLASS);
  Class& specific = pckg.getPackagedElements().get("specific").as<Class>();
  ASSERT_EQ(specific.getGeneralizations().size(), 1);
  Generalization& generalization = specific.getGeneralizations().front();
  ASSERT_EQ(generalization.getGeneralizationSets().size(), 1);
  ASSERT_EQ(pckg.getPackagedElements().get("set").getElementType(), ElementType::GENERALIZATION_SET);
  GeneralizationSet& set = pckg.getPackagedElements().get("set").as<GeneralizationSet>();
  ASSERT_EQ(general.getPowerTypeExtent().front(), set);
  ASSERT_EQ(generalization.getGeneralizationSets().front(), set);
  ASSERT_TRUE(set.getPowerType());
  ASSERT_EQ(*set.getPowerType(), general);
  ASSERT_EQ(set.getGeneralizations().size(), 1);
  ASSERT_EQ(set.getGeneralizations().front(), generalization);
  ASSERT_TRUE(set.isCovering());
  ASSERT_TRUE(set.isDisjoint());
}

TEST_F(GeneralizationSetParserTest, emitGeneralizationSetTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Class& general = *m.create<Class>();
    Class& specific = *m.create<Class>();
    Generalization& generalization = *m.create<Generalization>();
    GeneralizationSet& set = *m.create<GeneralizationSet>();
    root.setID("UpJ207YoGcD0zWHbmtYZhLAYEhRP");
    general.setID("mmUnLGAGcUocJQlNkF2BxGUzadjY");
    specific.setID("wJ7Y3K6BmTpN3D2pEtbbBt5aMhuo");
    generalization.setID("vGAiKV8tZmvkxePhhEns36Z654xF");
    set.setID("uLHn5GsNBUhrk9cgTO&qLw5LO068");
    specific.getGeneralizations().add(generalization);
    generalization.setGeneral(general);
    generalization.getGeneralizationSets().add(set);
    set.setPowerType(general);
    root.getPackagedElements().add(general, specific, set);
    std::string expectedEmit = R""""(package:
  id: UpJ207YoGcD0zWHbmtYZhLAYEhRP
  packagedElements:
    - class:
        id: mmUnLGAGcUocJQlNkF2BxGUzadjY
        powerTypeExtent:
          - uLHn5GsNBUhrk9cgTO&qLw5LO068
    - class:
        id: wJ7Y3K6BmTpN3D2pEtbbBt5aMhuo
        generalizations:
          - generalization:
              id: vGAiKV8tZmvkxePhhEns36Z654xF
              general: mmUnLGAGcUocJQlNkF2BxGUzadjY
              generalizationSets:
                - uLHn5GsNBUhrk9cgTO&qLw5LO068
    - generalizationSet:
        id: uLHn5GsNBUhrk9cgTO&qLw5LO068
        covering: false
        disjoint: false
        powerType: mmUnLGAGcUocJQlNkF2BxGUzadjY
        generalizations:
          - vGAiKV8tZmvkxePhhEns36Z654xF)"""";
    std::string expectedEmit2 = R""""(package:
  id: UpJ207YoGcD0zWHbmtYZhLAYEhRP
  packagedElements:
    - class:
        id: mmUnLGAGcUocJQlNkF2BxGUzadjY
        powerTypeExtent:
          - uLHn5GsNBUhrk9cgTO&qLw5LO068
    - generalizationSet:
        id: uLHn5GsNBUhrk9cgTO&qLw5LO068
        covering: false
        disjoint: false
        powerType: mmUnLGAGcUocJQlNkF2BxGUzadjY
        generalizations:
          - vGAiKV8tZmvkxePhhEns36Z654xF
    - class:
        id: wJ7Y3K6BmTpN3D2pEtbbBt5aMhuo
        generalizations:
          - generalization:
              id: vGAiKV8tZmvkxePhhEns36Z654xF
              general: mmUnLGAGcUocJQlNkF2BxGUzadjY
              generalizationSets:
                - uLHn5GsNBUhrk9cgTO&qLw5LO068)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    std::cout << generatedEmit << '\n';
    ASSERT_TRUE(expectedEmit == generatedEmit || expectedEmit2 == generatedEmit);
}

TEST_F(GeneralizationSetParserTest, mountGeneralizationSet) {
  UmlManager m;
  Package& root = *m.create<Package>();
  Class& general = *m.create<Class>();
  Class& specific = *m.create<Class>();
  Generalization& generalization = *m.create<Generalization>();
  GeneralizationSet& set = *m.create<GeneralizationSet>();
  specific.getGeneralizations().add(generalization);
  generalization.setGeneral(general);
  generalization.getGeneralizationSets().add(set);
  set.setPowerType(general);
  root.getPackagedElements().add(general, specific, set);
  m.setRoot(root);
  m.mount(ymlPath + "generalizationSetTests");

  ID setID = set.getID();
  m.release(set);
  ASSERT_FALSE(m.loaded(setID));
  GeneralizationSet& set2 = m.aquire(setID)->as<GeneralizationSet>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(set2, root));
  ASSERT_TRUE(set2.getPowerType());
  ASSERT_EQ(*set2.getPowerType(), general);
  ASSERT_EQ(set2.getGeneralizations().size(), 1);
  ASSERT_EQ(set2.getGeneralizations().front(), generalization);

  ID generalID = general.getID();
  m.release(set2, general);
  ASSERT_FALSE(m.loaded(generalID));
  ASSERT_FALSE(m.loaded(setID));
  GeneralizationSet& set3 = m.aquire(setID)->as<GeneralizationSet>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(set3, root));
  ASSERT_TRUE(set3.getPowerType());
  ASSERT_EQ(set3.getPowerType().id(), generalID);
  ASSERT_EQ(set3.getGeneralizations().size(), 1);
  ASSERT_EQ(set3.getGeneralizations().front(), generalization);
  Class& general2 = m.aquire(generalID)->as<Class>();
  ASSERT_EQ(general2.getPowerTypeExtent().size(), 1);
  ASSERT_EQ(general2.getPowerTypeExtent().front(), set3);

  m.release(set3, general2);
  ASSERT_FALSE(m.loaded(generalID));
  ASSERT_FALSE(m.loaded(setID));
  Class& general3 = m.aquire(generalID)->as<Class>();
  ASSERT_EQ(general3.getPowerTypeExtent().size(), 1);
  ASSERT_EQ(general3.getPowerTypeExtent().ids().front(), setID);
  GeneralizationSet& set4 = m.aquire(setID)->as<GeneralizationSet>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(set4, root));
  ASSERT_TRUE(set4.getPowerType());
  ASSERT_EQ(*set4.getPowerType(), general3);
  ASSERT_EQ(set4.getGeneralizations().size(), 1);
  ASSERT_EQ(set4.getGeneralizations().front(), generalization);

  ID generalizationID = generalization.getID();
  m.release(set4, generalization);
  ASSERT_FALSE(m.loaded(generalizationID));
  ASSERT_FALSE(m.loaded(setID));
  GeneralizationSet& set5 = m.aquire(setID)->as<GeneralizationSet>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(set5, root));
  ASSERT_TRUE(set5.getPowerType());
  ASSERT_EQ(*set5.getPowerType(), general3);
  ASSERT_EQ(set5.getGeneralizations().size(), 1);
  ASSERT_EQ(set5.getGeneralizations().ids().front(), generalizationID);
  Generalization& generalization2 = m.aquire(generalizationID)->as<Generalization>();
  ASSERT_EQ(generalization2.getGeneralizationSets().size(), 1);
  ASSERT_EQ(generalization2.getGeneralizationSets().front(), set5);

  m.release(set5, generalization2);
  ASSERT_FALSE(m.loaded(generalizationID));
  ASSERT_FALSE(m.loaded(setID));
  Generalization& generalization3 = m.aquire(generalizationID)->as<Generalization>();
  ASSERT_EQ(generalization3.getGeneralizationSets().size(), 1);
  ASSERT_EQ(generalization3.getGeneralizationSets().ids().front(), setID);
  GeneralizationSet& set6 = m.aquire(setID)->as<GeneralizationSet>();
  ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(set6, root));
  ASSERT_TRUE(set6.getPowerType());
  ASSERT_EQ(*set6.getPowerType(), general3);
  ASSERT_EQ(set6.getGeneralizations().size(), 1);
  ASSERT_EQ(set6.getGeneralizations().front(), generalization3);
}
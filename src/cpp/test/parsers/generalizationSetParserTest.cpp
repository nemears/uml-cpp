#include "gtest/gtest.h"
#include "uml/generalization.h"
#include "uml/generalizationSet.h"
#include "uml/classifier.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"
#include "uml/parsers/parser.h"

using namespace UML;

class GeneralizationSetParserTest : public ::testing::Test {
   public:
    std::string ymlPath = YML_FILES_PATH;
};

TEST_F(GeneralizationSetParserTest, parseBasicGeneralizationSetTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "generalizationSetTests/basicGeneralizationSet.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& general = pckg.getPackagedElements().front().as<Class>();
    ASSERT_EQ(general.getPowerTypeExtent().size(), 1);
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::CLASS);
    Class& specific = pckg.getPackagedElements().get(1).as<Class>();
    ASSERT_EQ(specific.getGeneralizations().size(), 1);
    Generalization& generalization = specific.getGeneralizations().front();
    ASSERT_EQ(generalization.getGeneralizationSets().size(), 1);
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::GENERALIZATION_SET);
    GeneralizationSet& set = pckg.getPackagedElements().back().as<GeneralizationSet>();
    ASSERT_EQ(general.getPowerTypeExtent().front(), set);
    ASSERT_EQ(generalization.getGeneralizationSets().front(), set);
    ASSERT_TRUE(set.hasPowerType());
    ASSERT_EQ(set.getPowerTypeRef(), general);
    ASSERT_EQ(set.getGeneralizations().size(), 1);
    ASSERT_EQ(set.getGeneralizations().front(), generalization);
    ASSERT_TRUE(set.isCovering());
    ASSERT_TRUE(set.isDisjoint());
}

TEST_F(GeneralizationSetParserTest, emitGeneralizationSetTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Class& general = m.create<Class>();
    Class& specific = m.create<Class>();
    Generalization& generalization = m.create<Generalization>();
    GeneralizationSet& set = m.create<GeneralizationSet>();
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
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(root));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
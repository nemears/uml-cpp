#include "gtest/gtest.h"
#include "uml/generalization.h"
#include "uml/generalizationSet.h"
#include "uml/classifier.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"

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
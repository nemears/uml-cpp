#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class GeneralizationSetTest : public ::testing::Test {
   
};

TEST_F(GeneralizationSetTest, testGetElementType) {
    UmlManager m;
    GeneralizationSet& set = *m.create<GeneralizationSet>();
    ASSERT_EQ(set.getElementType(), ElementType::GENERALIZATION_SET);
}

TEST_F(GeneralizationSetTest, AddAndRemoveTosequencesTest) {
    UmlManager m;
    GeneralizationSet& set = *m.create<GeneralizationSet>();
    Generalization& generalization = *m.create<Generalization>();
    Class& general = *m.create<Class>();
    set.getGeneralizations().add(generalization);
    ASSERT_EQ(generalization.getGeneralizationSets().size(), 1);
    ASSERT_EQ(generalization.getGeneralizationSets().front(), set);
    generalization.getGeneralizationSets().remove(set);
    ASSERT_EQ(set.getGeneralizations().size(), 0);
    generalization.getGeneralizationSets().add(set);
    ASSERT_EQ(set.getGeneralizations().size(), 1);
    ASSERT_EQ(set.getGeneralizations().front(), generalization);

    set.setPowerType(general);
    ASSERT_EQ(general.getPowerTypeExtent().size(), 1);
    ASSERT_EQ(general.getPowerTypeExtent().front(), set);
    Class& general2 = *m.create<Class>();
    set.setPowerType(general2);
    ASSERT_EQ(general.getPowerTypeExtent().size(), 0);
    general2.getPowerTypeExtent().remove(set);
    ASSERT_FALSE(set.getPowerType());
}
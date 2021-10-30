#include "gtest/gtest.h"
#include "uml/generalization.h"
#include "uml/generalizationSet.h"
#include "uml/classifier.h"
#include "uml/class.h"

using namespace UML;

class GeneralizationSetTest : public ::testing::Test {
   
};

TEST_F(GeneralizationSetTest, testGetElementType) {
    UmlManager m;
    GeneralizationSet set = m.create<GeneralizationSet>();
    std::cout << set.getElementTypeString() << std::endl;
    ASSERT_EQ(set.getElementType(), ElementType::GENERALIZATION_SET);
}
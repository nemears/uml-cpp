#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/package.h"
#include "uml/instanceSpecification.h"
#include "uml/classifier.h"
#include "uml/slot.h"
#include "uml/structuralFeature.h"
#include "uml/literalInt.h"

using namespace std;
using namespace UML;
using namespace CPP;

class CppNamespaceTest : public ::testing::Test {
    public:
    const string profilePath = "../../../../../src/yml/uml/cpp/";
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

TEST_F(CppNamespaceTest, bunchOfTypesTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "namespaceTests/bunchOfTypes.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 2);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(pckg->getPackagedElements().get(1)->getElementType(), ElementType::PACKAGE);
    Package& FOO = pckg->getPackagedElements().get(1)->as<Package>();
    ASSERT_EQ(FOO.getAppliedStereotypes().size(), 1);
    ASSERT_EQ(FOO.getAppliedStereotypes().front()->getClassifier()->getID(), ID::fromString("Cpp_NAMESPACE_3FloKgLhiH2P0t"));
    ASSERT_EQ(FOO.getPackagedElements().size(), 2);
    ASSERT_EQ(FOO.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b = FOO.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(b.getName(), "b");
    ASSERT_TRUE(b.getClassifier() != 0);
    ASSERT_EQ(b.getClassifier()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(FOO.getPackagedElements().get(1)->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_array = FOO.getPackagedElements().get(1)->as<InstanceSpecification>();
    ASSERT_EQ(b_array.getName(), "b_array");
    ASSERT_EQ(b_array.getClassifier()->getID(), ID::fromString("Cpp_CONST_ARRAY_Nw3c30z1PCo3"));
    ASSERT_EQ(b_array.getSlots().size(), 1);
    Slot& sizeSlot= *b_array.getSlots().front();
    ASSERT_EQ(sizeSlot.getDefiningFeature()->getName(), "size");
    ASSERT_EQ(sizeSlot.getValues().size(), 1);
    ASSERT_EQ(sizeSlot.getValues().front()->getElementType(), ElementType::LITERAL_INT);
    ASSERT_EQ(sizeSlot.getValues().front()->as<LiteralInt>().getValue(), 10);
}
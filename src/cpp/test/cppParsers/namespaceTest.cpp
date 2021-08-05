#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/package.h"
#include "uml/instanceSpecification.h"
#include "uml/classifier.h"
#include "uml/slot.h"
#include "uml/structuralFeature.h"
#include "uml/literalInt.h"
#include "uml/property.h"

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
    ASSERT_EQ(FOO.getAppliedStereotypes().front()->getClassifier()->getID(), Profile::cppNamespaceID);
    ASSERT_EQ(FOO.getPackagedElements().size(), 4);
    ASSERT_EQ(FOO.getPackagedElements().front()->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b = FOO.getPackagedElements().front()->as<InstanceSpecification>();
    ASSERT_EQ(b.getName(), "b");
    ASSERT_TRUE(b.getClassifier() != 0);
    ASSERT_EQ(b.getClassifier()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(FOO.getPackagedElements().get(1)->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_array = FOO.getPackagedElements().get(1)->as<InstanceSpecification>();
    ASSERT_EQ(b_array.getName(), "b_array");
    ASSERT_EQ(b_array.getAppliedStereotypes().size(), 1);
    InstanceSpecification& b_arrayStereotype = *b_array.getAppliedStereotypes().front();
    ASSERT_TRUE(b_arrayStereotype.getClassifier() != 0);
    ASSERT_EQ(b_arrayStereotype.getClassifier()->getID(), Profile::cppConstArrayID);
    ASSERT_EQ(b_arrayStereotype.getSlots().size(), 1);
    Slot& b_arraySizeSlot = *b_arrayStereotype.getSlots().front();
    ASSERT_TRUE(b_arraySizeSlot.getDefiningFeature() != 0);
    ASSERT_EQ(b_arraySizeSlot.getDefiningFeature()->getID(), b_arrayStereotype.getClassifier()->getAttributes().get("size")->getID());
    ASSERT_EQ(b_arraySizeSlot.getValues().size(), 1);
    ASSERT_EQ(b_arraySizeSlot.getValues().front()->getElementType(), ElementType::LITERAL_INT);
    ASSERT_EQ(b_arraySizeSlot.getValues().front()->as<LiteralInt>().getValue(), 10);
    ASSERT_TRUE(b_array.getClassifier() != 0);
    ASSERT_EQ(b_array.getClassifier()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(FOO.getPackagedElements().get(2)->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_ptr = FOO.getPackagedElements().get(2)->as<InstanceSpecification>();
    ASSERT_TRUE(b_ptr.getClassifier() != 0);
    ASSERT_EQ(b_ptr.getClassifier()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(b_ptr.getAppliedStereotypes().size(), 1);
    ASSERT_TRUE(b_ptr.getAppliedStereotypes().front()->getClassifier() != 0);
    ASSERT_EQ(b_ptr.getAppliedStereotypes().front()->getClassifier()->getID(), Profile::cppPointerID);
    ASSERT_EQ(FOO.getPackagedElements().get(3)->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_ref = FOO.getPackagedElements().get(3)->as<InstanceSpecification>();
    ASSERT_TRUE(b_ref.getClassifier() != 0);
    ASSERT_EQ(b_ref.getClassifier()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(b_ref.getAppliedStereotypes().size(), 1);
    ASSERT_TRUE(b_ref.getAppliedStereotypes().front()->getClassifier() != 0);
    ASSERT_EQ(b_ref.getAppliedStereotypes().front()->getClassifier()->getID(), Profile::cppReferenceID);
}
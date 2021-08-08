#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/package.h"
#include "uml/instanceSpecification.h"
#include "uml/classifier.h"
#include "uml/slot.h"
#include "uml/structuralFeature.h"
#include "uml/literalInt.h"
#include "uml/property.h"
#include "uml/literalBool.h"
#include "uml/literalReal.h"
#include "testUtils.h"

using namespace std;
using namespace UML;
using namespace CPP;

class CppNamespaceTest : public ::testing::Test {
    public:
    const string profilePath = "../../../../../src/yml/uml/cpp/";
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

void testVariablesOfType(size_t i, string c, Package& FOO, ID typeID, size_t arraySize) {
    ASSERT_EQ(FOO.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b = FOO.getPackagedElements().get(i).as<InstanceSpecification>();
    ASSERT_EQ(b.getName(), c);
    ASSERT_TRUE(b.getClassifier() != 0);
    ASSERT_EQ(b.getClassifier()->getID(), typeID);
    ASSERT_EQ(FOO.getPackagedElements().get(i + 1).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_array = FOO.getPackagedElements().get(i + 1).as<InstanceSpecification>();
    ASSERT_EQ(b_array.getName(), c + "_array");
    ASSERT_EQ(b_array.getAppliedStereotypes().size(), 1);
    InstanceSpecification& b_arrayStereotype = b_array.getAppliedStereotypes().front();
    ASSERT_TRUE(b_arrayStereotype.getClassifier() != 0);
    ASSERT_EQ(b_arrayStereotype.getClassifier()->getID(), Profile::cppConstArrayID);
    ASSERT_EQ(b_arrayStereotype.getSlots().size(), 1);
    Slot& b_arraySizeSlot = b_arrayStereotype.getSlots().front();
    ASSERT_TRUE(b_arraySizeSlot.getDefiningFeature() != 0);
    ASSERT_EQ(b_arraySizeSlot.getDefiningFeature()->getID(), b_arrayStereotype.getClassifier()->getAttributes().get("size").getID());
    ASSERT_EQ(b_arraySizeSlot.getValues().size(), 1);
    ASSERT_EQ(b_arraySizeSlot.getValues().front().getElementType(), ElementType::LITERAL_INT);
    ASSERT_EQ(b_arraySizeSlot.getValues().front().as<LiteralInt>().getValue(), arraySize);
    ASSERT_TRUE(b_array.getClassifier() != 0);
    ASSERT_EQ(b_array.getClassifier()->getID(), typeID);
    ASSERT_EQ(FOO.getPackagedElements().get(i + 2).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_ptr = FOO.getPackagedElements().get(i + 2).as<InstanceSpecification>();
    ASSERT_TRUE(b_ptr.getClassifier() != 0);
    ASSERT_EQ(b_ptr.getClassifier()->getID(), typeID);
    ASSERT_EQ(b_ptr.getAppliedStereotypes().size(), 1);
    ASSERT_TRUE(b_ptr.getAppliedStereotypes().front().getClassifier() != 0);
    ASSERT_EQ(b_ptr.getAppliedStereotypes().front().getClassifier()->getID(), Profile::cppPointerID);
    ASSERT_EQ(FOO.getPackagedElements().get(i + 3).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b_ref = FOO.getPackagedElements().get(i + 3).as<InstanceSpecification>();
    ASSERT_TRUE(b_ref.getClassifier() != 0);
    ASSERT_EQ(b_ref.getClassifier()->getID(), typeID);
    ASSERT_EQ(b_ref.getAppliedStereotypes().size(), 1);
    ASSERT_TRUE(b_ref.getAppliedStereotypes().front().getClassifier() != 0);
    ASSERT_EQ(b_ref.getAppliedStereotypes().front().getClassifier()->getID(), Profile::cppReferenceID);
}

TEST_F(CppNamespaceTest, bunchOfTypesTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "namespaceTests/bunchOfTypes.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 2);
    ASSERT_EQ(pckg->getPackagedElements().front().getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(pckg->getPackagedElements().get(1).getElementType(), ElementType::PACKAGE);
    Package& FOO = pckg->getPackagedElements().get(1).as<Package>();
    ASSERT_EQ(FOO.getAppliedStereotypes().size(), 1);
    ASSERT_EQ(FOO.getAppliedStereotypes().front().getClassifier()->getID(), Profile::cppNamespaceID);
    ASSERT_EQ(FOO.getPackagedElements().size(), 20);

    testVariablesOfType(0, "b", FOO, Profile::cppBoolID, 10);
    testVariablesOfType(4, "i", FOO, Profile::cppIntID, 100);
    testVariablesOfType(8, "c", FOO, Profile::cppCharID, 1000);
    testVariablesOfType(12, "f", FOO, Profile::cppFloatID, 1);
    testVariablesOfType(16, "d", FOO, Profile::cppDoubleID, 3);
}

TEST_F(CppNamespaceTest, defaultValueTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "namespaceTests/primitiveDefaults.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 2);
    ASSERT_EQ(pckg->getPackagedElements().front().getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(pckg->getPackagedElements().get(1).getElementType(), ElementType::PACKAGE);
    Package& FOO = pckg->getPackagedElements().get(1).as<Package>();
    ASSERT_EQ(FOO.getAppliedStereotypes().size(), 1);
    ASSERT_EQ(FOO.getAppliedStereotypes().front().getClassifier()->getID(), Profile::cppNamespaceID);
    ASSERT_EQ(FOO.getPackagedElements().size(), 5);
    ASSERT_EQ(FOO.getPackagedElements().front().getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& b = FOO.getPackagedElements().front().as<InstanceSpecification>();
    ASSERT_EQ(b.getSlots().size(), 1);
    ASSERT_TRUE(b.getSlots().front().getDefiningFeature() != 0);
    ASSERT_EQ(b.getSlots().front().getValues().size(), 1);
    ASSERT_EQ(b.getSlots().front().getValues().front().getElementType(), ElementType::LITERAL_BOOL);
    ASSERT_EQ(b.getSlots().front().getValues().front().as<LiteralBool>().getValue(), false);

    ASSERT_EQ(FOO.getPackagedElements().get(1).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& i = FOO.getPackagedElements().get(1).as<InstanceSpecification>();
    ASSERT_EQ(i.getSlots().size(), 1);
    ASSERT_TRUE(i.getSlots().front().getDefiningFeature() != 0);
    ASSERT_EQ(i.getSlots().front().getValues().size(), 1);
    ASSERT_EQ(i.getSlots().front().getValues().front().getElementType(), ElementType::LITERAL_INT);
    ASSERT_EQ(i.getSlots().front().getValues().front().as<LiteralInt>().getValue(), 420);

    ASSERT_EQ(FOO.getPackagedElements().get(2).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& c = FOO.getPackagedElements().get(2).as<InstanceSpecification>();
    ASSERT_EQ(c.getSlots().size(), 1);
    ASSERT_TRUE(c.getSlots().front().getDefiningFeature() != 0);
    ASSERT_EQ(c.getSlots().front().getValues().size(), 1);
    ASSERT_EQ(c.getSlots().front().getValues().front().getElementType(), ElementType::LITERAL_INT);
    ASSERT_EQ(c.getSlots().front().getValues().front().as<LiteralInt>().getValue(), 'x');

    ASSERT_EQ(FOO.getPackagedElements().get(3).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& f = FOO.getPackagedElements().get(3).as<InstanceSpecification>();
    ASSERT_EQ(f.getSlots().size(), 1);
    ASSERT_TRUE(f.getSlots().front().getDefiningFeature() != 0);
    ASSERT_EQ(f.getSlots().front().getValues().size(), 1);
    ASSERT_EQ(f.getSlots().front().getValues().front().getElementType(), ElementType::LITERAL_REAL);
    ASSERT_EQ((float) f.getSlots().front().getValues().front().as<LiteralReal>().getValue(), (float) 3.14159); 
    /** NOTE: so this cast to float is necessary cause double goes a lil long and comparison is weird, shouldnt
     * affect roundtripping i think**/

    ASSERT_EQ(FOO.getPackagedElements().get(4).getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& d = FOO.getPackagedElements().get(4).as<InstanceSpecification>();
    ASSERT_EQ(d.getSlots().size(), 1);
    ASSERT_TRUE(d.getSlots().front().getDefiningFeature() != 0);
    ASSERT_EQ(d.getSlots().front().getValues().size(), 1);
    ASSERT_EQ(d.getSlots().front().getValues().front().getElementType(), ElementType::LITERAL_REAL);
    ASSERT_EQ(d.getSlots().front().getValues().front().as<LiteralReal>().getValue(), 2.7);
}
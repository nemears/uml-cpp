#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/class.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/association.h"

using namespace std;
using namespace UML;
using namespace CPP;

class CppClassTest : public ::testing::Test {
    public:
    const string profilePath = "../../../../../src/yml/uml/cpp/";
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

TEST_F(CppClassTest, attributesW_DifferentVisibilityTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "classTests/attributesW_DifferentVisibility.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 1);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& clazz = pckg->getPackagedElements().front()->as<Class>();
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 4);
    Property& i = *clazz.getOwnedAttributes().front();
    ASSERT_EQ(i.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(i.getName(), "i");
    ASSERT_EQ(i.getVisibility(), VisibilityKind::PRIVATE);
    Property& d = *clazz.getOwnedAttributes().get(1);
    ASSERT_EQ(d.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(d.getName(), "d");
    ASSERT_EQ(d.getVisibility(), VisibilityKind::PROTECTED);
    Property& c = *clazz.getOwnedAttributes().get(2);
    ASSERT_EQ(c.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(c.getName(), "c");
    ASSERT_EQ(c.getVisibility(), VisibilityKind::PUBLIC);
    Property& f = *clazz.getOwnedAttributes().back();
    ASSERT_EQ(f.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(f.getName(), "f");
    ASSERT_EQ(f.getVisibility(), VisibilityKind::PRIVATE);
}

TEST_F(CppClassTest, classW_MethodsTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "classTests/someMethods.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 1);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& clazz = pckg->getPackagedElements().front()->as<Class>();
    ASSERT_EQ(clazz.getOperations().size(), 3);
    Operation& foo = *clazz.getOperations().front();
    ASSERT_EQ(foo.getVisibility(), VisibilityKind::PRIVATE);
    ASSERT_EQ(foo.getOwnedParameters().size(), 0);
    Operation& bar = *clazz.getOperations().get(1);
    ASSERT_EQ(bar.getVisibility(), VisibilityKind::PROTECTED);
    ASSERT_EQ(bar.getOwnedParameters().size(), 1);
    Parameter& barRet = *bar.getOwnedParameters().front();
    ASSERT_EQ(barRet.getName(), "return");
    ASSERT_EQ(barRet.getDirection(), ParameterDirectionKind::RETURN);
    ASSERT_EQ(barRet.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    Operation& tt = *clazz.getOperations().get(2);
    ASSERT_EQ(tt.getVisibility(), VisibilityKind::PUBLIC);
    ASSERT_EQ(tt.getOwnedParameters().size(), 1);
    Parameter& ttArg = *tt.getOwnedParameters().front();
    ASSERT_EQ(ttArg.getName(), "d");
    ASSERT_EQ(ttArg.getDirection(), ParameterDirectionKind::IN);
    ASSERT_EQ(ttArg.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
}

TEST_F(CppClassTest, bunchOfTypesInClassTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "classTests/bunchOfTypes.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 11);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& clazz = pckg->getPackagedElements().front()->as<Class>();
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 20);

    // bool properties
    Property& boolProp = *clazz.getOwnedAttributes().front();
    ASSERT_EQ(boolProp.getName(), "b");
    ASSERT_EQ(boolProp.getType()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(boolProp.getAggregation(), AggregationKind::COMPOSITE);
    Property& boolArray = *clazz.getOwnedAttributes().get(1);
    ASSERT_EQ(boolArray.getName(), "b_array");
    ASSERT_EQ(boolArray.getType()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(boolArray.getLower(), 0);
    ASSERT_EQ(boolArray.getUpper(), 10);
    ASSERT_EQ(boolArray.getAggregation(), AggregationKind::COMPOSITE);
    Property& boolPtr = *clazz.getOwnedAttributes().get(2);
    ASSERT_EQ(boolPtr.getName(), "b_ptr");
    ASSERT_EQ(boolPtr.getType()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(boolPtr.getAggregation(), AggregationKind::NONE);
    Association& ptrAssoc = pckg->getPackagedElements().get(1)->as<Association>();
    ASSERT_EQ(ptrAssoc.getMemberEnds().front()->getID(), boolPtr.getID());
    ASSERT_EQ(ptrAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
    Property& boolRef = *clazz.getOwnedAttributes().get(3);
    ASSERT_EQ(boolRef.getName(), "b_ref");
    ASSERT_EQ(boolRef.getType()->getID(), ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG"));
    ASSERT_EQ(boolRef.getAggregation(), AggregationKind::SHARED);
    ASSERT_EQ(boolRef.getLower(), 1);
    ASSERT_EQ(boolRef.getUpper(), 1);
    Association& refAssoc = pckg->getPackagedElements().get(2)->as<Association>();
    ASSERT_EQ(refAssoc.getMemberEnds().front()->getID(), boolRef.getID());
    ASSERT_EQ(refAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());

    // char properties
    Property& charProp = *clazz.getOwnedAttributes().get(4);
    ASSERT_EQ(charProp.getName(), "c");
    ASSERT_EQ(charProp.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(charProp.getAggregation(), AggregationKind::COMPOSITE);
    Property& charArray = *clazz.getOwnedAttributes().get(5);
    ASSERT_EQ(charArray.getName(), "c_array");
    ASSERT_EQ(charArray.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(charArray.getLower(), 0);
    ASSERT_EQ(charArray.getUpper(), 11);
    ASSERT_EQ(charArray.getAggregation(), AggregationKind::COMPOSITE);
    Property& charPtr = *clazz.getOwnedAttributes().get(6);
    ASSERT_EQ(charPtr.getName(), "c_ptr");
    ASSERT_EQ(charPtr.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(charPtr.getAggregation(), AggregationKind::NONE);
    Association& charPtrAssoc = pckg->getPackagedElements().get(3)->as<Association>();
    ASSERT_EQ(charPtrAssoc.getMemberEnds().front()->getID(), charPtr.getID());
    ASSERT_EQ(charPtrAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
    Property& charRef = *clazz.getOwnedAttributes().get(7);
    ASSERT_EQ(charRef.getName(), "c_ref");
    ASSERT_EQ(charRef.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(charRef.getAggregation(), AggregationKind::SHARED);
    ASSERT_EQ(charRef.getLower(), 1);
    ASSERT_EQ(charRef.getUpper(), 1);
    Association& charRefAssoc = pckg->getPackagedElements().get(4)->as<Association>();
    ASSERT_EQ(charRefAssoc.getMemberEnds().front()->getID(), charRef.getID());
    ASSERT_EQ(charRefAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());

    // int properties
    Property& intProp = *clazz.getOwnedAttributes().get(8);
    ASSERT_EQ(intProp.getName(), "i");
    ASSERT_EQ(intProp.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(intProp.getAggregation(), AggregationKind::COMPOSITE);
    Property& intArray = *clazz.getOwnedAttributes().get(9);
    ASSERT_EQ(intArray.getName(), "i_array");
    ASSERT_EQ(intArray.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(intArray.getLower(), 0);
    ASSERT_EQ(intArray.getUpper(), 100);
    ASSERT_EQ(intArray.getAggregation(), AggregationKind::COMPOSITE);
    Property& intPtr = *clazz.getOwnedAttributes().get(10);
    ASSERT_EQ(intPtr.getName(), "i_ptr");
    ASSERT_EQ(intPtr.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(intPtr.getAggregation(), AggregationKind::NONE);
    Association& intPtrAssoc = pckg->getPackagedElements().get(5)->as<Association>();
    ASSERT_EQ(intPtrAssoc.getMemberEnds().front()->getID(), intPtr.getID());
    ASSERT_EQ(intPtrAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
    Property& intRef = *clazz.getOwnedAttributes().get(11);
    ASSERT_EQ(intRef.getName(), "i_ref");
    ASSERT_EQ(intRef.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(intRef.getAggregation(), AggregationKind::SHARED);
    ASSERT_EQ(intRef.getLower(), 1);
    ASSERT_EQ(intRef.getUpper(), 1);
    Association& intRefAssoc = pckg->getPackagedElements().get(6)->as<Association>();
    ASSERT_EQ(intRefAssoc.getMemberEnds().front()->getID(), intRef.getID());
    ASSERT_EQ(intRefAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());

    // float properties
    Property& floatProp = *clazz.getOwnedAttributes().get(12);
    ASSERT_EQ(floatProp.getName(), "f");
    ASSERT_EQ(floatProp.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(floatProp.getAggregation(), AggregationKind::COMPOSITE);
    Property& floatArray = *clazz.getOwnedAttributes().get(13);
    ASSERT_EQ(floatArray.getName(), "f_array");
    ASSERT_EQ(floatArray.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(floatArray.getLower(), 0);
    ASSERT_EQ(floatArray.getUpper(), 1000);
    ASSERT_EQ(floatArray.getAggregation(), AggregationKind::COMPOSITE);
    Property& floatPtr = *clazz.getOwnedAttributes().get(14);
    ASSERT_EQ(floatPtr.getName(), "f_ptr");
    ASSERT_EQ(floatPtr.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(floatPtr.getAggregation(), AggregationKind::NONE);
    Association& floatPtrAssoc = pckg->getPackagedElements().get(7)->as<Association>();
    ASSERT_EQ(floatPtrAssoc.getMemberEnds().front()->getID(), floatPtr.getID());
    ASSERT_EQ(floatPtrAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
    Property& floatRef = *clazz.getOwnedAttributes().get(15);
    ASSERT_EQ(floatRef.getName(), "f_ref");
    ASSERT_EQ(floatRef.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(floatRef.getAggregation(), AggregationKind::SHARED);
    ASSERT_EQ(floatRef.getLower(), 1);
    ASSERT_EQ(floatRef.getUpper(), 1);
    Association& floatRefAssoc = pckg->getPackagedElements().get(8)->as<Association>();
    ASSERT_EQ(floatRefAssoc.getMemberEnds().front()->getID(), floatRef.getID());
    ASSERT_EQ(floatRefAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());

    // double properties
    Property& doubleProp = *clazz.getOwnedAttributes().get(16);
    ASSERT_EQ(doubleProp.getName(), "d");
    ASSERT_EQ(doubleProp.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(doubleProp.getAggregation(), AggregationKind::COMPOSITE);
    Property& doubleArray = *clazz.getOwnedAttributes().get(17);
    ASSERT_EQ(doubleArray.getName(), "d_array");
    ASSERT_EQ(doubleArray.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(doubleArray.getLower(), 0);
    ASSERT_EQ(doubleArray.getUpper(), 999);
    ASSERT_EQ(doubleArray.getAggregation(), AggregationKind::COMPOSITE);
    Property& doublePtr = *clazz.getOwnedAttributes().get(18);
    ASSERT_EQ(doublePtr.getName(), "d_ptr");
    ASSERT_EQ(doublePtr.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(doublePtr.getAggregation(), AggregationKind::NONE);
    Association& doublePtrAssoc = pckg->getPackagedElements().get(9)->as<Association>();
    ASSERT_EQ(doublePtrAssoc.getMemberEnds().front()->getID(), doublePtr.getID());
    ASSERT_EQ(doublePtrAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
    Property& doubleRef = *clazz.getOwnedAttributes().get(19);
    ASSERT_EQ(doubleRef.getName(), "d_ref");
    ASSERT_EQ(doubleRef.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(doubleRef.getAggregation(), AggregationKind::SHARED);
    ASSERT_EQ(doubleRef.getLower(), 1);
    ASSERT_EQ(doubleRef.getUpper(), 1);
    Association& doubleRefAssoc = pckg->getPackagedElements().get(10)->as<Association>();
    ASSERT_EQ(doubleRefAssoc.getMemberEnds().front()->getID(), doubleRef.getID());
    ASSERT_EQ(doubleRefAssoc.getNavigableOwnedEnds().front()->getType()->getID(), clazz.getID());
}

TEST_F(CppClassTest, predefinedTypeTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "classTests/predefinedType.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 2);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& foo = pckg->getPackagedElements().front()->as<Class>();
    ASSERT_EQ(foo.getName(), "FOO");
    ASSERT_EQ(pckg->getPackagedElements().get(1)->getElementType(), ElementType::CLASS);
    Class& bar = pckg->getPackagedElements().get(1)->as<Class>();
    ASSERT_EQ(bar.getName(), "BAR");
    ASSERT_EQ(bar.getOwnedAttributes().size(), 1);
    Property& prop = *bar.getOwnedAttributes().front();
    ASSERT_EQ(prop.getName(), "f");
    ASSERT_TRUE(prop.getClass() != 0);
    ASSERT_EQ(prop.getType()->getID(), foo.getID());
}
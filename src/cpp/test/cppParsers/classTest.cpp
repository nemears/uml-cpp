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
    ASSERT_EQ(pckg->getPackagedElements().size(), 3);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& clazz = pckg->getPackagedElements().front()->as<Class>();
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 4);
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
}
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

class CppHeaderTest : public ::testing::Test {
    public:
    const string profilePath = "../../../../../src/yml/uml/cpp/";
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

TEST_F(CppHeaderTest, bunchOfTypesTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "headerTests/globalVars.h", m));
    ASSERT_EQ(pckg->getPackagedElements().size(), 21);
    ASSERT_EQ(pckg->getPackagedElements().front().getElementType(), ElementType::ARTIFACT);

    testVariablesOfType(1, "b", *pckg, Profile::cppBoolID, 10);
    testVariablesOfType(5, "i", *pckg, Profile::cppIntID, 100);
    testVariablesOfType(9, "c", *pckg, Profile::cppCharID, 1000);
    testVariablesOfType(13, "f", *pckg, Profile::cppFloatID, 1);
    testVariablesOfType(17, "d", *pckg, Profile::cppDoubleID, 3);
}
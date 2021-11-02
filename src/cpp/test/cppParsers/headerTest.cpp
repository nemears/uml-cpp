#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/uml-stable.h"
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

    testVariablesOfType(1, "b", *pckg, CPP::Profile::cppBoolID, 10);
    testVariablesOfType(5, "i", *pckg, CPP::Profile::cppIntID, 100);
    testVariablesOfType(9, "c", *pckg, CPP::Profile::cppCharID, 1000);
    testVariablesOfType(13, "f", *pckg, CPP::Profile::cppFloatID, 1);
    testVariablesOfType(17, "d", *pckg, CPP::Profile::cppDoubleID, 3);
}
TEST_F(CppHeaderTest, basic_parseCppTest) {
    UmlManager m;
    Model& model = m.create<Model>();
    m.setRoot(&model);
    model.getPackagedElements().add(m.parse(profilePath + "cppProfile.yml")->as<UML::Profile>());
    parseCpp("test", {testPath + "headerTests/includeTest.h", testPath + "classTests/classW_Generalization.h"}, {}, m);
    ASSERT_EQ(model.getPackagedElements().size(), 3);
    ASSERT_EQ(model.getPackagedElements().get(1).as<Package>().getPackagedElements().size(), 3);
    ASSERT_EQ(model.getPackagedElements().get(1).as<Package>().getPackagedElements().front().getElementType(), ElementType::DEPLOYMENT);
    ASSERT_EQ(model.getPackagedElements().get(1).as<Package>().getPackagedElements().get(1).getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(model.getPackagedElements().get(1).as<Package>().getPackagedElements().back().getElementType(), ElementType::ARTIFACT);
}
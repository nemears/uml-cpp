#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class ModelParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ModelParserTest, parsedAndCreatedElementTest) {
    
    // Parse
    Model* m;
    ASSERT_NO_THROW(m = Parsers::parseModel(ymlPath + "modelTests/modelW_Packages.yml"));

    // Validate
    ASSERT_TRUE(m->getPackagedElements().size() == 1);
    ASSERT_TRUE(m->getPackagedElements().front()->getElementType() == ElementType::PACKAGE);
    Package* parsedPckg = dynamic_cast<Package*>(m->getPackagedElements().front());

    // Add some elements
    Class c;
    Property p;
    c.getOwnedAttributes().add(p);
    parsedPckg->getPackagedElements().add(c);
}
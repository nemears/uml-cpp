#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

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
    UmlManager mm;
    mm.open(ymlPath + "modelTests/modelW_Packages.yml");

    m = mm.getModel();

    // Validate
    ASSERT_TRUE(m->getPackagedElements().size() == 1);
    ASSERT_TRUE(m->getPackagedElements().front().getElementType() == ElementType::PACKAGE);
    Package* parsedPckg = dynamic_cast<Package*>(&m->getPackagedElements().front());

    // Add some elements
    Class& c = mm.create<Class>();
    Property& p = mm.create<Property>();
    c.getOwnedAttributes().add(p);
    parsedPckg->getPackagedElements().add(c);
}
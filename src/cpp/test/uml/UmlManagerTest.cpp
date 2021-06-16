#include "gtest/gtest.h"
#include "uml/umlManager.h"
#include "uml/expression.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class UmlManagerTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(UmlManagerTest, FactoryTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    ASSERT_TRUE(c.getElementType() == ElementType::CLASS);
}

TEST_F(UmlManagerTest, openAndSaveTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "umlManagerTests/simpleModel.yml"));
    ASSERT_EQ(m.getModel()->getID(), ID::fromString("GAfdua&ubXfsR1EgdB3HeVglkaor"));
    ASSERT_EQ(m.getModel()->getName(), "test");
    ASSERT_EQ(m.getModel(), &m.get<Model>(m.getModel()->getID()));
}
#include "gtest/gtest.h"
#include "uml/umlManager.h"
#include "uml/expression.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"
#include "uml/profileApplication.h"

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

    Package& p = m.create<Package>();
    ID pID = p.getID();
    m.getModel()->getPackagedElements().add(p);
    m.save();
    m.open();

    EXPECT_EQ(m.getModel()->getID(), ID::fromString("GAfdua&ubXfsR1EgdB3HeVglkaor"));
    EXPECT_EQ(m.getModel()->getName(), "test");
    EXPECT_EQ(m.getModel(), &m.get<Model>(m.getModel()->getID()));
    EXPECT_TRUE(m.getModel()->getPackagedElements().size() == 1);
    //ASSERT_EQ(m.getModel()->getPackagedElements().front()->getID(), p.getID());

    Package& p2 = m.get<Package>(pID);
    m.getModel()->getPackagedElements().remove(p2);
    m.save();
}

TEST_F(UmlManagerTest, multipleFileTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "umlManagerTests/multipleFiles.yml"));
    ASSERT_TRUE(m.getRoot() != 0);
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = dynamic_cast<Package&>(*m.getRoot());
    ASSERT_EQ(pckg.getProfileApplications().size(), 1);
    ProfileApplication& application = pckg.getProfileApplications().front();
    ASSERT_TRUE(application.getAppliedProfile() != 0);
    m.save();
}

TEST_F(UmlManagerTest, simpleMountTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    p.setName("mountedRoot");
    m.setRoot(&p);
    ASSERT_NO_THROW(m.mount("."));
}
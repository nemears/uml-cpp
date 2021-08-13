#include "gtest/gtest.h"
#include "uml/umlManager.h"
#include "uml/expression.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"
#include "uml/profileApplication.h"
#include "uml/parsers/parser.h"
#include "uml/instanceSpecification.h"

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
    UmlManager* m = new UmlManager;
    Package& p = m->create<Package>();
    p.setName("mountedRoot");
    m->setRoot(&p);
    ASSERT_NO_THROW(m->mount(ymlPath + "umlManagerTests"));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount"));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string()));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / (p.getID().string() + ".yml")));
    UmlManager* m2 = Parsers::parse(filesystem::path(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / (p.getID().string() + ".yml")));
    ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
    Package& p2 = m2->getRoot()->as<Package>();
    ASSERT_EQ(p2.getID(), p.getID());
    ASSERT_EQ(p2.getName(), p.getName());
    delete m2;
    delete m;
    ASSERT_FALSE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount"));
}

TEST_F(UmlManagerTest, multiLayerMountTest) {
    UmlManager* m = new UmlManager;
    Package& p = m->create<Package>();
    Package& c = m->create<Package>();
    p.setName("mountedRoot");
    m->setRoot(&p);
    p.getPackagedElements().add(c);
    c.setName("child");
    ASSERT_NO_THROW(m->mount(ymlPath + "umlManagerTests"));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount"));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string()));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / (p.getID().string() + ".yml")));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / c.getID().string()));
    ASSERT_TRUE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / c.getID().string() / (c.getID().string() + ".yml")));
    UmlManager* m2 = Parsers::parse(filesystem::path(filesystem::path(ymlPath + "umlManagerTests") / "mount" / p.getID().string() / (p.getID().string() + ".yml")));
    ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
    Package& p2 = m2->getRoot()->as<Package>();
    ASSERT_EQ(p2.getID(), p.getID());
    ASSERT_EQ(p2.getName(), p.getName());
    ASSERT_EQ(p2.getPackagedElements().size(), p.getPackagedElements().size());
    ASSERT_EQ(p2.getPackagedElements().front().getID(), c.getID());
    delete m;
}

TEST_F(UmlManagerTest, releaseTest) {
    UmlManager m;
    Package& p = m.create<Package>();
    Package& c = m.create<Package>();
    p.getPackagedElements().add(c);
    m.setRoot(&p);
    ASSERT_NO_THROW(m.mount(ymlPath + "umlManagerTests"));
    p.setName("name");
    ID pid = p.getID();
    ASSERT_NO_THROW(m.release(p.getID()));
    UmlManager* m2 = Parsers::parse(filesystem::path(ymlPath + "umlManagerTests") / "mount" / pid.string() / (pid.string() + ".yml"));
    ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
    Package& p2 = m2->getRoot()->as<Package>();
    ASSERT_EQ(p.getName(), "name");
}

TEST_F(UmlManagerTest, releaseTestW_RefInOther) {
    UmlManager m;
    Package& p = m.create<Package>();
    Class& c = m.create<Class>();
    InstanceSpecification& i = m.create<InstanceSpecification>();
    i.setClassifier(&c);
    p.getPackagedElements().add(c);
    p.getPackagedElements().add(i);
    m.setRoot(&p);
    ASSERT_NO_THROW(m.mount(ymlPath + "umlManagerTests"));
    ASSERT_TRUE(i.getClassifier() != 0);
    ASSERT_EQ(i.getClassifier()->getID(), c.getID());
    ASSERT_NO_THROW(m.release(c.getID()));
    ASSERT_TRUE(i.getClassifier() != 0);
    //c = i.getClassifier()->as<Class>();
    i.getOwner();
    c.getOwner();
    ASSERT_NO_THROW(m.release(p.getID()));
    ASSERT_TRUE(c.getOwner() != 0);
    ASSERT_TRUE(i.getOwner() != 0);
}
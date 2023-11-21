#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/uml-cpp-paths.h"
#include "test/umlTestUtil.h"
#include "uml/managers/manager.h"

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
    Manager<> m;
    Class& c = *m.create<Class>();
    ASSERT_TRUE(c.getElementType() == ElementType::CLASS);
}

TEST_F(UmlManagerTest, openAndSaveTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "umlManagerTests/simpleModel.yml"));
    ASSERT_EQ(m.getRoot()->getID(), ID::fromString("GAfdua&ubXfsR1EgdB3HeVglkaor"));
    ASSERT_EQ(m.getRoot()->as<NamedElement>().getName(), "test");

    Package& p = *m.create<Package>();
    ID pID = p.getID();
    m.getRoot()->as<Package>().getPackagedElements().add(p);
    m.save();
    m.open();

    EXPECT_EQ(m.getRoot()->getID(), ID::fromString("GAfdua&ubXfsR1EgdB3HeVglkaor"));
    EXPECT_EQ(m.getRoot()->as<Package>().getName(), "test");
    EXPECT_EQ(m.getRoot()->as<Package>().getPackagedElements().size(), 2);
    //ASSERT_EQ(m.getModel()->getPackagedElements().front()->getID(), p.getID());

    Package& p2 = m.get(pID)->as<Package>();
    m.getRoot()->as<Package>().getPackagedElements().remove(p2);
    m.save();
}

// TEST_F(UmlManagerTest, multipleFileTest) {
//     Manager<> m;
//     ASSERT_NO_THROW(m.open(ymlPath + "umlManagerTests/multipleFiles.yml"));
//     ASSERT_TRUE(m.getRoot());
//     ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
//     Package& pckg = dynamic_cast<Package&>(*m.getRoot());
//     ASSERT_EQ(pckg.getProfileApplications().size(), 1);
//     ProfileApplication& application = pckg.getProfileApplications().front();
//     ASSERT_TRUE(application.getAppliedProfile());
//     m.save();
// }

// TEST_F(UmlManagerTest, simpleMountTest) {
//     UmlManager* m = new UmlManager;
//     Package& p = *m->create<Package>();
//     p.setName("mountedRoot");
//     m->setRoot(&p);
//     ASSERT_NO_THROW(m->mount(ymlPath + "umlManagerTests"));
//     UmlManager* m2 = Parsers::parse((filesystem::path(filesystem::path(ymlPath + "umlManagerTests") / "mount" / (p.getID().string() + ".yml"))).string());
//     ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
//     Package& p2 = m2->getRoot()->as<Package>();
//     ASSERT_EQ(p2.getID(), p.getID());
//     ASSERT_EQ(p2.getName(), p.getName());
//     delete m2;
//     delete m;
//     ASSERT_FALSE(filesystem::exists(filesystem::path(ymlPath + "umlManagerTests") / "mount"));
// }

// TEST_F(UmlManagerTest, multiLayerMountTest) {
//     UmlManager* m = new UmlManager;
//     Package& p = *m->create<Package>();
//     Package& c = *m->create<Package>();
//     p.setName("mountedRoot");
//     m->setRoot(&p);
//     p.getPackagedElements().add(c);
//     c.setName("child");
//     ASSERT_NO_THROW(m->mount(ymlPath + "umlManagerTests"));
//     UmlManager* m2 = Parsers::parse((filesystem::path(filesystem::path(ymlPath + "umlManagerTests") / "mount" / (p.getID().string() + ".yml"))).string());
//     ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
//     Package& p2 = m2->getRoot()->as<Package>();
//     ASSERT_EQ(p2.getID(), p.getID());
//     ASSERT_EQ(p2.getName(), p.getName());
//     ASSERT_EQ(p2.getPackagedElements().size(), p.getPackagedElements().size());
//     ASSERT_EQ(p2.getPackagedElements().front().getID(), c.getID());
//     delete m;
//     delete m2;
// }

// TEST_F(UmlManagerTest, releaseTest) {
//     UmlManager<> m;
//     Package& p = *m.create<Package>();
//     Package& c = *m.create<Package>();
//     p.getPackagedElements().add(c);
//     m.setRoot(&p);
//     ASSERT_NO_THROW(m.mount(ymlPath + "umlManagerTests"));
//     p.setName("name");
//     ID pid = p.getID();
//     ASSERT_NO_THROW(m.release(p.getID()));
//     ASSERT_FALSE(m.loaded(pid));
//     UmlManager* m2 = Parsers::parse((filesystem::path(ymlPath + "umlManagerTests") / "mount" / (pid.string() + ".yml")).string());
//     ASSERT_EQ(m2->getRoot()->getElementType(), ElementType::PACKAGE);
//     Package& p2 = m2->getRoot()->as<Package>();
//     ASSERT_EQ(p2.getName(), "name");
//     delete m2;
// }

TEST_F(UmlManagerTest, releaseTestW_MoreRefs) {
    Manager<> m;
    Package& p = *m.create<Package>();
    Class& c = *m.create<Class>();
    InstanceSpecification& i = *m.create<InstanceSpecification>();
    i.getClassifiers().add(c);
    p.getPackagedElements().add(c);
    p.getPackagedElements().add(i);
    m.setRoot(&p);
    ASSERT_NO_THROW(m.mount(ymlPath + "umlManagerTests"));
    ASSERT_TRUE(i.getClassifiers().size() != 0);
    ASSERT_TRUE(i.getClassifiers().contains(c.getID()));
    ASSERT_NO_THROW(m.release(c));
    ASSERT_TRUE(i.getClassifiers().size() != 0);
    Class* c2 = &i.getClassifiers().front().as<Class>();
    ASSERT_TRUE(i.getOwner());
    ASSERT_TRUE(c2->getOwner());
    ASSERT_EQ(c2->getNamespace(), &p);
    ASSERT_EQ(c2->getOwningPackage(), &p);
    ASSERT_TRUE(p.getOwnedElements().contains(*c2));
    ASSERT_EQ(p.getMembers().size(), 2);
    ASSERT_TRUE(p.getMembers().contains(*c2));
    ASSERT_TRUE(p.getMembers().contains(*c2));
    ASSERT_TRUE(p.getPackagedElements().contains(*c2));
    ASSERT_NO_THROW(m.release(p));
    ASSERT_TRUE(c2->getOwner());
    ASSERT_TRUE(i.getOwner());
    Package* p2 = &i.getOwner()->as<Package>();
    ASSERT_EQ(c2->getOwner(), p2);
    ASSERT_TRUE(c2->getNamespace());
    ASSERT_EQ(c2->getNamespace(), p2);
    ASSERT_TRUE(c2->getOwningPackage());
    ASSERT_EQ(c2->getOwningPackage(), p2);

    ASSERT_EQ(i.getOwner(), p2);
    ASSERT_TRUE(i.getNamespace());
    ASSERT_EQ(i.getNamespace(), p2);
    ASSERT_TRUE(i.getOwningPackage());
    ASSERT_EQ(i.getOwningPackage(), p2);

    ASSERT_EQ(p2->getPackagedElements().size(), 2);
    ASSERT_TRUE(p2->getPackagedElements().contains(c2->getID()));
    ASSERT_EQ(p2->getOwnedMembers().size(), 2);
    ASSERT_TRUE(p2->getOwnedMembers().contains(*c2));
    ASSERT_EQ(p2->getMembers().size(), 2);
    ASSERT_TRUE(p2->getMembers().contains(*c2));
    ASSERT_EQ(p2->getOwnedElements().size(), 2);
    ASSERT_TRUE(p2->getOwnedElements().contains(*c2));
}

TEST_F(UmlManagerTest, addToManagerAfterMountedTest) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    m.setRoot(&pckg);
    m.mount(ymlPath + "umlManagerTests");
    Package& child = *m.create<Package>();   
    pckg.getPackagedElements().add(child);
    ASSERT_NO_THROW(m.release(child));
}

TEST_F(UmlManagerTest, ManagerMountStressTest) {
    const size_t numElements = 20;
    Manager<> m;
    Package& root = *m.create<Package>();
    ID rootID = root.getID();
    m.setRoot(&root);
    ASSERT_NO_THROW(m.mount(ymlPath + "umlManagerTests"));
    std::vector<ID> ids(numElements);
    PackagePtr pckg(&root);
    for (size_t i = 0; i < numElements; i++) {
        PackagePtr child = m.create<Package>();
        pckg->getPackagedElements().add(*child);
        ids.push_back(pckg->getID());
        m.release(*pckg); // release or segfault
        pckg = child;
    }
    m.release(*pckg);
    for (size_t i = 0; i < numElements; i++) {
        EXPECT_FALSE(m.loaded(ids[i])) << "at index" << i;
    }
    pckg = &m.get(rootID)->as<Package>(); // try to only aquire root
    for (size_t i = 0; i < numElements; i++) {
        EXPECT_FALSE(m.loaded(*pckg->getPackagedElements().ids().begin())) << "at index " << i;
        ASSERT_EQ(pckg->getPackagedElements().size(), 1) << "at index " << i;
        ASSERT_EQ(pckg->getOwnedMembers().size(), 1) << "at index " << i;
        ASSERT_EQ(pckg->getMembers().size(), 1) << "at index " << i;
        ASSERT_EQ(pckg->getOwnedElements().size(), 1) << "at index " << i;
        if (i > 0) {
            ASSERT_TRUE(pckg->getOwningPackage());
            m.release(*pckg->getOwningPackage());
        }
        pckg = &pckg->getPackagedElements().front().as<Package>();
    }
    Package& root3 = m.get(rootID)->as<Package>();
    ASSERT_EQ(root3.getID(), rootID);
}

TEST_F(UmlManagerTest, basicEraseFunctionalityTest) {
    Manager<> m;
    Package& package = *m.create<Package>();
    Package& child = *m.create<Package>();
    package.getPackagedElements().add(child);
    m.setRoot(&package);
    m.mount(ymlPath + "umlManagerTests");
    ID childID = child.getID();
    m.erase(child);
    // ASSERT_FALSE(std::filesystem::exists(std::filesystem::path(ymlPath + "umlManagerTests/" + childID.string() + ".yml")));
    ASSERT_TRUE(package.getOwnedElements().empty());
    ASSERT_FALSE(m.loaded(childID));
}

TEST_F(UmlManagerTest, eraseReferenceWhileReleasedTest) {
    Manager<> m;
    ClassPtr c = m.create<Class>();
    InstanceSpecificationPtr i = m.create<InstanceSpecification>();
    i->getClassifiers().add(*c);
    m.mount(".");
    c.release();
    i.release();
    c.aquire();
    m.erase(*c);
    i.aquire();
    ASSERT_EQ(i->getClassifiers().size(), 0);
}

TEST_F(UmlManagerTest, policyManagerTest) {
    Manager<> m;
    PackagePtr p = m.create<Package>();
    m.mount(".");
    m.release(*p);
    ASSERT_FALSE(p.loaded());
    ASSERT_EQ(m.get(p.id()).ptr(), p.ptr());
    PackagePtr child = m.create<Package>();
    p->getPackagedElements().add(*child);
    p.release();
    ASSERT_FALSE(p.loaded());
    ASSERT_FALSE(child->getOwningPackage().loaded());
}

// TEST_F(UmlManagerTest, threadSafeManagerTest) {
//     ThreadSafeManager m;
//     PackagePtr p = m.create<Package>();
//     m.mount(".");
//     m.release(*p);
//     ASSERT_FALSE(p.loaded());
//     ASSERT_EQ(m.get(p.id()).ptr(), p.ptr());
//     PackagePtr child = m.create<Package>();
//     p->getPackagedElements().add(*child);
//     p.release();
//     ASSERT_FALSE(p.loaded());
//     ASSERT_FALSE(child->getOwningPackage().loaded());
// }

TEST_F(UmlManagerTest, overwriteRootTest) {
    Manager<> m;
    m.setRoot(m.create<Package>().ptr());
    m.mount(".");
    m.getRoot().release();
    m.get(m.getRoot().id());
}
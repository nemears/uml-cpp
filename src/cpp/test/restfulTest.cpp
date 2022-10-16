#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include <stdlib.h>
#include <thread>

using namespace UML;

class RestfulTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(RestfulTest, clientsConnectToServerTest) {
    UmlClient client1;
    UmlClient client2;
}

TEST_F(RestfulTest, postAndGetTest) {
    UmlClient client;
    Class& clazz = *client.create<Class>();
    ID clazzID = clazz.getID();
    clazz.setName("clazz");
    ASSERT_TRUE(client.loaded(clazz.getID()));
    client.release(clazz);
    Class& clazz2 = client.get(clazzID)->as<Class>();
    ASSERT_EQ(clazz2.getName(), "clazz");
}

TEST_F(RestfulTest, basicEraseTest) {
    UmlClient client;
    Class& clazz = *client.create<Class>();
    ID clazzID = clazz.getID();
    client.erase(clazz);
    ASSERT_FALSE(client.loaded(clazzID));
}

TEST_F(RestfulTest, basicGetByQualifiedName) {
    UmlClient client;
    Class& clazz = *client.create<Class>();
    Package& root = *client.create<Package>();
    root.setName("A");
    clazz.setName("B");
    root.getPackagedElements().add(clazz);
    ASSERT_EQ(clazz.getQualifiedName(), "A::B");
    ID clazzID = clazz.getID();
    client.release(clazz);
    ASSERT_EQ(client.get("A::B")->getID(), clazzID);
}

TEST_F(RestfulTest, bigMessageTest) {
    int numChildren = 125;
    UmlClient client;
    PackagePtr root = client.create<Package>();
    for (int i = 0; i < numChildren; i++) {
        root->getPackagedElements().add(*client.create<Package>());
    }
    ID id = root.id();
    root->setName("foo");
    client.release(*root);
    ASSERT_EQ(client.get(id)->as<Package>().getPackagedElements().size(), numChildren);
    client.release(*root);
    ASSERT_EQ(client.get("foo")->as<Package>().getPackagedElements().size(), numChildren);
}

TEST_F(RestfulTest, headTest) {
    UmlClient client;
    PackagePtr root = client.create<Package>();
    PackagePtr child = client.create<Package>();
    root->setName("root");
    child->setName("test");
    root->getPackagedElements().add(*child);
    client.release(*child);
    client.setRoot(root.ptr());
    // ASSERT_EQ(*root, client.get(""));
    ASSERT_EQ(client.get(""), root);
}

TEST_F(RestfulTest, saveTest) {
    UmlClient client;
    PackagePtr newRoot = client.create<Package>();
    ClassPtr clazz = client.create<Class>();
    newRoot->getPackagedElements().add(*clazz);
    client.setRoot(newRoot.ptr());
    client.release(*clazz);
    clazz->setName("clzz");
    client.release(*clazz);
    client.save();
    // TODO can misbehave and still pass
    // TODO shutdown server and startup server somehow and check
}

void raceConditionThread(ID id) {
    UmlClient client;
    PackagePtr pckg = &client.get(id)->as<Package>();
    PackagePtr child = client.create<Package>();
    pckg->getPackagedElements().add(*child);
    client.release(*pckg);
    client.release(*child);
    // client.putAll();
}

TEST_F(RestfulTest, raceConditionTest) {
    UmlClient client;
    PackagePtr pckg = client.create<Package>();
    std::thread rcThread(&raceConditionThread, pckg.id());
    PackagePtr child = client.create<Package>();
    pckg->getPackagedElements().add(*child);
    // client.putAll();
    client.release(*pckg);
    client.release(*child);
    rcThread.join();
    // TODO 
    pckg.release();
    pckg.aquire();
    ASSERT_EQ(pckg->getPackagedElements().size(), 1);
    // ASSERT_EQ(pckg->getPackagedElements().front(), *child);
}

TEST_F(RestfulTest, badReferenceTest) {
    UmlClient client;
    ID clazzID;
    ID instID;

    // dereference clazz from inst by releasing both, changing clazz's id, and releasing, then aquiring inst
    {
        ClassPtr clazz = client.create<Class>();
        InstanceSpecificationPtr inst = client.create<InstanceSpecification>();
        inst->getClassifiers().add(*clazz);
        clazzID = clazz.id();
        instID = inst.id();
        clazz.release();
        inst.release();
    }
    {
        ClassPtr clazz = &client.get(clazzID)->as<Class>();
        clazz->setID(ID::randomID());
        clazz.release();
    }
    InstanceSpecificationPtr inst;
    ClassPtr clazz;
    ASSERT_NO_THROW(inst = &client.get(instID)->as<InstanceSpecification>());
    ASSERT_NO_THROW(clazz = &client.get(clazzID)->as<Class>());
    ClassifierPtr instClassifier;
    ASSERT_NO_THROW(instClassifier = &inst->getClassifiers().front());
    ASSERT_EQ(clazz.id(), instClassifier.id());
    ASSERT_EQ(clazz.ptr(), instClassifier.ptr());
}
#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"
#include "uml/class.h"
#include "test/yumlParsersTest.h"
#include "uml/package.h"

using namespace UML;

class RestfulTest : public ::testing::Test {
   public:
    std::string ymlPath = YML_FILES_PATH;
};

TEST_F(RestfulTest, clientsConnectToServerTest) {
    UmlServer server;
    UmlClient client1;
    UmlClient client2;
    sleep(0.02);
    ASSERT_EQ(server.numClients(), 2);
}

TEST_F(RestfulTest, postTest) {
    UmlServer server;
    {
        UmlClient client;
        Class& clazz = client.post<Class>();
        ASSERT_TRUE(client.count(clazz.getID()));
        ASSERT_TRUE(server.count(clazz.getID()));
    }
}

TEST_F(RestfulTest, basicGetTest) {
    UmlServer server;
    {
        UmlClient client;
        ID id = server.create<Class>().getID();
        server.get<Class>(id).setName("test");
        ASSERT_EQ(client.get<NamedElement>(id).getName(), "test");
    }
}

TEST_F(RestfulTest, basicPutTest) {
    UmlServer server;
    Package& root = server.create<Package>();
    server.setRoot(root);
    server.mount(ymlPath + "umlManagerTests");
    UmlClient client;
    Class& clazz = client.post<Class>();
    ID id = clazz.getID();
    clazz.setName("test");
    clazz.setOwningPackage(root);
    UmlClient client2;
    ASSERT_TRUE(client2.get<Class>(id).getName().empty());
    client.put(clazz);
    ASSERT_EQ(client2.get<Class>(id).getName(), "test");
}

TEST_F(RestfulTest, basicEraseTest) {
    UmlServer server;
    UmlClient client;
    Class& clazz = client.post<Class>();
    ID clazzID = clazz.getID();
    client.erase(clazz);
    ASSERT_FALSE(client.loaded(clazzID));
    sleep(0.01);
    ASSERT_FALSE(server.loaded(clazzID));
}

TEST_F(RestfulTest, basicGetByQualifiedName) {
    UmlServer server;
    UmlClient client;
    Class& clazz = client.post<Class>();
    Package& root = client.post<Package>();
    root.setName("A");
    clazz.setName("B");
    root.getPackagedElements().add(clazz);
    ASSERT_EQ(clazz.getQualifiedName(), "A::B");
    client.put(clazz);
    UmlClient client2;
    Class& clazz2 = client2.get<Class>("A::B");
    ASSERT_EQ(clazz2.getOwningPackageRef(), root);
}

TEST_F(RestfulTest, bigMessageTest) {
    int numChildren = 25;
    UmlServer server;
    UmlClient client;
    Package& root = client.post<Package>();
    for (int i = 0; i < numChildren; i++) {
        root.getPackagedElements().add(client.post<Package>());
    }
    ID id = root.getID();
    client.release(root);
    ASSERT_EQ(client.get<Package>(id).getPackagedElements().size(), numChildren);
}
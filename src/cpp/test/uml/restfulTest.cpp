#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"
#include <stdlib.h>
#include <thread>

using namespace UML;

class RestfulTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
        UmlServer server;

        void TearDown() override {
            server.reset();
        };
};

TEST_F(RestfulTest, clientsConnectToServerTest) {
    UmlClient client1;
    UmlClient client2;
    ASSERT_EQ(server.numClients(), 2);
}

TEST_F(RestfulTest, postTest) {
    UmlClient client;
    Class& clazz = client.post<Class>();
    ASSERT_TRUE(client.count(clazz.getID()));
    ASSERT_TRUE(server.count(clazz.getID()));
}

TEST_F(RestfulTest, basicGetTest) {
    UmlClient client;
    ID id = server.create<Class>().getID();
    server.get<Class>(id).setName("test");
    ASSERT_EQ(client.get<NamedElement>(id).getName(), "test");
}

TEST_F(RestfulTest, basicPutTest) {
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
    UmlClient client;
    Class& clazz = client.post<Class>();
    ID clazzID = clazz.getID();
    client.erase(clazz);
    ASSERT_FALSE(client.loaded(clazzID));
    server.waitForProcessing();
    ASSERT_FALSE(server.loaded(clazzID));
}

TEST_F(RestfulTest, basicGetByQualifiedName) {
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
    UmlClient client;
    Package& root = client.post<Package>();
    for (int i = 0; i < numChildren; i++) {
        root.getPackagedElements().add(client.post<Package>());
    }
    ID id = root.getID();
    root.setName("foo");
    client.release(root);
    ASSERT_EQ(client.get<Package>(id).getPackagedElements().size(), numChildren);
    client.release(id);
    ASSERT_EQ(client.get<Package>("foo").getPackagedElements().size(), numChildren);
}

/**
 * TODO: 
 *  expand testing to test using client with server in seperate terminal, can't currently get test to work
 *  definitely look into more wholesome threading procedures and locks so blocking is less likely in debug and valgrind
 **/

// void runServer() {
//     FILE* f = popen("../../../../../build/src/cpp/uml/./uml-server -l ../../../../../src/yml/umlManagerTests/server.yml", "r");
//     if (f == 0) {
//         std::cout << "did not run server from commandline correctly" << std::endl;
//     }
//     std::cout << (char*)f << std::endl;
// }

// TEST_F(RestfulTest, serverMainTest) {
//     server.shutdown();
//     server.waitTillShutDown();
//     std::thread cmd = std::thread(runServer);
//     cmd.detach();
//     UmlClient client;
//     Package& test = client.get<Package>("Model::test");
//     Package& child = client.post<Package>();
//     child.setName("child");
//     client.release(test, child);
//     ASSERT_EQ(client.get<Package>("Model::test").getPackagedElements().front().getName(), "child");
//     client.shutdownServer();
//     //cmd.join();
// }
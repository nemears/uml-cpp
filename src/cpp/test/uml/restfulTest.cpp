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
    Class& clazz = client.post<Class>();
    ID clazzID = clazz.getID();
    clazz.setName("clazz");
    ASSERT_TRUE(client.count(clazz.getID()));
    client.release(clazz);
    Class& clazz2 = client.get<Class>(clazzID);
    ASSERT_EQ(clazz2.getName(), "clazz");
}

TEST_F(RestfulTest, basicEraseTest) {
    UmlClient client;
    Class& clazz = client.post<Class>();
    ID clazzID = clazz.getID();
    client.erase(clazz);
    ASSERT_FALSE(client.loaded(clazzID));
}

TEST_F(RestfulTest, basicGetByQualifiedName) {
    UmlClient client;
    Class& clazz = client.post<Class>();
    Package& root = client.post<Package>();
    root.setName("A");
    clazz.setName("B");
    root.getPackagedElements().add(clazz);
    ASSERT_EQ(clazz.getQualifiedName(), "A::B");
    ID clazzID = clazz.getID();
    client.release(clazz);
    ASSERT_EQ(client.get("A::B").getID(), clazzID);
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
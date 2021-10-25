#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"
#include "uml/class.h"

using namespace UML;

class RestfulTest : public ::testing::Test {
   
};

TEST_F(RestfulTest, clientsConnectToServerTest) {
    UmlServer server;
    UmlClient client1;
    UmlClient client2;
    sleep(0.1);
    ASSERT_EQ(server.numClients(), 2);
}

TEST_F(RestfulTest, postTest) {
    UmlServer server;
    UmlClient client;
    Class& clazz = client.post<Class>();
    ASSERT_TRUE(client.count(clazz.getID()));
    ASSERT_TRUE(server.count(clazz.getID()));
}
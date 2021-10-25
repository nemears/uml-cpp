#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"

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
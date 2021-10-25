#include "gtest/gtest.h"
#include "uml/managers/umlServer.h"
#include "uml/managers/umlClient.h"

using namespace UML;

class RestfulTest : public ::testing::Test {
   
};

TEST_F(RestfulTest, clientConnectsToManagerTest) {
    UmlServer server;
    UmlClient client;
    sleep(1);
}
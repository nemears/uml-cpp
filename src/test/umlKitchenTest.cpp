#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "uml/managers/protocol/umlKitchenClient.h"

using namespace UML;

class UmlKitchenTest : public testing::Test {};

TEST_F(UmlKitchenTest, createClass) {
    UmlKitchenClient client;
    client.login("wss://uml.cafe", "uml-cpp-tests", "uml-cpp-test-runner", "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08");
    ClassPtr clazz = client.create<Class>();
    clazz.release();
}

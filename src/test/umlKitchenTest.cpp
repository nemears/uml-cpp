#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "uml/managers/protocol/umlKitchenClient.h"

using namespace UML;

class UmlKitchenTest : public testing::Test {};

TEST_F(UmlKitchenTest, createClass) {
    NoTlsUmlKitchenClient client;
    client.login(
            "ws://localhost:1372", // address 
            "uml-cpp-tests", // project
            "uml-cpp-test-runner", // user
            "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08" // password hash
    );
    ClassPtr clazz = client.create<Class>();
    clazz.release();
    ASSERT_FALSE(clazz.loaded());
    clazz.aquire();
    ASSERT_TRUE(clazz.loaded());
}

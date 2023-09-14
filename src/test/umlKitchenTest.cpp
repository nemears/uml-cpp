#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "uml/managers/protocol/umlKitchenClient.h"

using namespace UML;

class UmlKitchenTest : public testing::Test {};

TEST_F(UmlKitchenTest, createClass) {
    UmlKitchenClient client("ws://localhost", "test", "", "");
    ClassPtr clazz = client.create<Class>();
    clazz.release();
}

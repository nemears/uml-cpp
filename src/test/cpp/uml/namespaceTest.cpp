#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/namespace.h"

class NamespaceTest : public ::testing::Test {
    public:

        Namespace namespc;
        Namespace* namespcPtr;

    protected:

        NamespaceTest() {
            namespcPtr = new Namespace;
        }

        ~NamespaceTest() override {
            delete namespcPtr;
        }

        void SetUp() override {
            namespc.setName("test");
            namespcPtr->setName("ptrTest");
        }
};

// Really this test isn't necessary just did it for posterity and getting used to gtest
TEST_F(NamespaceTest, SetNameTest) {
    EXPECT_EQ(namespc.getName(), "test");
    EXPECT_EQ(namespcPtr->getName(), "ptrTest");
}
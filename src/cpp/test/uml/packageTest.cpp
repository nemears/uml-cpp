#include "gtest/gtest.h"
#include "uml/package.h"
#include "uml/packageableElement.h"

using namespace UML;

class PackageTest : public ::testing::Test {
   
};

TEST_F(PackageTest, addPackagedElement) {
    Package p;
    PackageableElement e;
    ASSERT_NO_THROW(p.getPackagedElements().add(e));
    ASSERT_TRUE(p.getPackagedElements().size() == 1);
    ASSERT_TRUE(p.getPackagedElements().front() == &e);
}
#include "gtest/gtest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class ID_Test : public ::testing::Test {};

TEST_F(ID_Test, idTest) {
    ID id;
    cout << id.string() << endl;
    ASSERT_FALSE(0);
}
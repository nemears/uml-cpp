#include "gtest/gtest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class ID_Test : public ::testing::Test {};

TEST_F(ID_Test, idTest) {
    srand (time(NULL));
    ID id;
    cout << id.string() << endl;
    ID randomID = ID::randomID();
    ID rid = ID::randomID();
    cout << randomID.string() << endl;
    cout << rid.string() << endl;
    ASSERT_FALSE(0);
}
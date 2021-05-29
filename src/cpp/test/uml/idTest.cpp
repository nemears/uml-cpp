#include "gtest/gtest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class ID_Test : public ::testing::Test {};

TEST_F(ID_Test, idTest) {
    srand (time(NULL));
    ID id;
    ID id2;
    cout << id.string() << endl;
    ID randomID = ID::randomID();
    ID rid = ID::randomID();
    cout << randomID.string() << endl;
    cout << "rid: " << rid.string() << endl;
    ASSERT_TRUE(id == id2);
    ASSERT_TRUE(id != rid);
    ASSERT_TRUE(randomID != rid);
    ID ridFromString = ID::fromString(rid.string());
    cout << "ridFromString: " << ridFromString.string() << endl;
    ASSERT_EQ(ID::fromString("AAAAAAAAAAAAAAAAAAAAAAAAAAAA"), id);
    ASSERT_EQ(rid, ID::fromString(rid.string()));
    ID rid2 = rid;
    ASSERT_EQ(rid, rid2);
    uint8_t* ridBytes = rid.getBytes();
    ID rid3;
    rid3.setBytes(ridBytes);
    ID ampersandID;
    uint8_t ampersands[21] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
    ampersandID.setBytes(ampersands);
    ASSERT_EQ("&&&&&&&&&&&&&&&&&&&&&&&&&&&&", ampersandID.string());
    hash<ID> idHash;
    cout << "hashing rid: " << idHash(rid) << endl;
}
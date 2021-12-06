#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class ID_Test : public ::testing::Test {};

TEST_F(ID_Test, idTest) {
    srand (time(NULL));
    ID id;
    ID id2;
    std::cout << id.string() << std::endl;
    ID randomID = ID::randomID();
    ID rid = ID::randomID();
    std::cout << randomID.string() << std::endl;
    std::cout << "rid: " << rid.string() << std::endl;
    ASSERT_TRUE(id == id2);
    ASSERT_TRUE(id != rid);
    ASSERT_TRUE(randomID != rid);
    ID ridFromString = ID::fromString(rid.string());
    std::cout << "ridFromString: " << ridFromString.string() << std::endl;
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
    std::hash<ID> idHash;
    std::cout << "hashing rid: " << idHash(rid) << std::endl;
}
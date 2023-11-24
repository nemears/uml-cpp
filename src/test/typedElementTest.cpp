#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

class TypedElementTest : public ::testing::Test {
	public:
		std::string ymlPath = YML_FILES_PATH;
};

TEST_F(TypedElementTest, GetTypeTest) {
	Manager<> m;
	Property& p = *m.create<Property>();
	Class& t = *m.create<Class>();
	p.setType(t);
	EXPECT_EQ(*p.getType(), t);
}

TEST_F(TypedElementTest, GetTypeReturnsNullTest) {
	Manager<> m;
	Property& t = *m.create<Property>();
    ASSERT_FALSE(t.getType());
}
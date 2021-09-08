#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/property.h"

using namespace UML;

class TypedElementTest : public ::testing::Test {};

TEST_F(TypedElementTest, GetTypeTest) {
	UmlManager m;
	Property p = m.create<Property>();
	Class t = m.create<Class>();
	p.setType(t);
	EXPECT_EQ(p.getTypeRef(), t);
}

TEST_F(TypedElementTest, GetTypeReturnsNullTest) {
	UmlManager m;
	Property t = m.create<Property>();
  ASSERT_TRUE(t.getType() == NULL);
}
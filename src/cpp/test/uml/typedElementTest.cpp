#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/property.h"
#include "uml/package.h"
#include "test/yumlParsersTest.h"

using namespace UML;

class TypedElementTest : public ::testing::Test {
	public:
		std::string ymlPath = YML_FILES_PATH;
};

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

TEST_F(TypedElementTest, reindexTypeTest) {
	UmlManager m;
	Class t = m.create<Class>();
	Property p = m.create<Property>();
	Class c = m.create<Class>();
	Package root = m.create<Package>();
	p.setType(t);
	c.getOwnedAttributes().add(p);
	root.getPackagedElements().add(t, c);
	m.setRoot(&root);
	m.mount(ymlPath + "typedElementTests");
	ID id = ID::fromString("iera5tJozUcDPwn2dcJ_0ZCP&goY");
	t.setID(id);
	ASSERT_TRUE(p.hasType());
	ASSERT_TRUE(p.getType());
	ASSERT_EQ(p.getTypeRef().getID(), id);
	m.release(id);
	ASSERT_TRUE(p.hasType());
	ASSERT_TRUE(p.getType());
	ASSERT_EQ(p.getTypeRef().getID(), id);
}
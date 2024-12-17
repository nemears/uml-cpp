#include "gtest/gtest.h"
#include "uml/managers/manager.h"
#include "uml/managers/typeInfo.h"
#include "uml/managers/dummyManager.h"

using namespace UML;

class ManagerTest : public ::testing::Test {};

template <class T>
struct TClass1 {};

template <class T>
struct TClass2 {};

TEST_F(ManagerTest, TemplateTypeListTypeTest) {

    using TestTypes = TemplateTypeList<TClass1, TClass2>;

    auto isTypeResultSame = std::is_same<
            TClass1<int>,
            TemplateTypeListType<0, TestTypes>::result<int>
        >::value;

    ASSERT_TRUE(isTypeResultSame);

    auto indexResult1 = TemplateTypeListIndex<TClass1, TestTypes>::result;
    ASSERT_EQ(indexResult1, 0);
    auto indexResult2 = TemplateTypeListIndex<TClass2, TestTypes>::result;
    ASSERT_EQ(indexResult2, 1);
    auto indexResult3 = TemplateTypeListIndex<TemplateTypeListType<0, TestTypes>::result, TestTypes>::result;
    ASSERT_EQ(indexResult3, 0);
}

namespace UML {
    template <class ManagerPolicy>
    struct Base : public ManagerPolicy {
        using Info = TypeInfo<Base>;
        Base() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Base(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Base() {}
    };

    template <>
    struct ElementInfo<Base> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Right : public ManagerPolicy {
        using Info = TypeInfo<Right, TemplateTypeList<Base>>;
        Right() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Right(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Right() {}
    };
    
    template <>
    struct ElementInfo<Right> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Left : public ManagerPolicy {
        using Info = TypeInfo<Left, TemplateTypeList<Base>>;
        Left () :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Left (std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Left() {}
    };
    
    template <>
    struct ElementInfo<Left> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Diamond : public ManagerPolicy {
        using Info = TypeInfo<Diamond, TemplateTypeList<Right, Left>>;
        Diamond() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Diamond(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Diamond() {}
    };
    
    template <>
    struct ElementInfo<Diamond> : DefaultInfo {
        static constexpr bool abstract = false;
    };

    using DiamondTypes = TemplateTypeList<Base, Right, Left, Diamond>;
    using DiamondManager = Manager<DiamondTypes>;
}

TEST_F(ManagerTest, diamondElementTest) {
    DiamondManager m;
    auto diamond = m.create<Diamond>();
    auto& right = diamond->as<Right>();
    ASSERT_EQ(right, dynamic_cast<Right<DiamondManager::GenBaseHierarchy<Right>>&>(*diamond));
    auto& left = diamond->as<Left>();
    ASSERT_EQ(left, dynamic_cast<Left<DiamondManager::GenBaseHierarchy<Left>>&>(*diamond));
    auto& base = diamond->as<Base>();
    ASSERT_EQ(base, dynamic_cast<Base<DiamondManager::GenBaseHierarchy<Base>>&>(*diamond));
}

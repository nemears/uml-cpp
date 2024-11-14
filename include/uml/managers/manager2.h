#pragma once

#include <mutex>
#include <tuple>
#include <memory>
#include <unordered_map>
#include "../umlPtr.h"

namespace UML {

    
    
    
    
    // template class typelist
    template <class ManagedType, template <class> class ... TemplateTypes>
    struct TemplateTypeList {};

    template <class TypeList>
    struct TemplateTypeListSize;

    template <template <class> class Last, template <class> class ... RestOfTypeList, class ManagedType>
    struct TemplateTypeListSize<TemplateTypeList<ManagedType, Last, RestOfTypeList...>> {
        static const std::size_t result = 1 + TemplateTypeListSize<TemplateTypeList<ManagedType, RestOfTypeList...>>::result;
    };

    template <template <class> class Last, class ManagedType>
    struct TemplateTypeListSize<Last<ManagedType>> {
        static const std::size_t result = 1;
    };

    template <std::size_t I, class TypeList>
    struct TemplateTypeListType;

    template <template <class> class First, template <class> class ... RestOfTypeList, class ManagedType>
    struct TemplateTypeListType<0, TemplateTypeList<ManagedType, First, RestOfTypeList...>> {
        using result = First<ManagedType>;
    };

    template <std::size_t I, template <class> class First, template <class> class ... RestOfTypeList, class ManagedType>
    struct TemplateTypeListType<I, TemplateTypeList<ManagedType, First, RestOfTypeList...>> {
        using result = TemplateTypeListType<I - 1, TemplateTypeList<ManagedType, RestOfTypeList...>>::result;
    };

    template <class TypePolicyList, class StoragePolicy>
    class Manager2 : public StoragePolicy {
        private: 
            // tuple utility struct to find index of type
            template <class T, class Tuple>
            struct Index;
            
            // template specialization for Index
            template <class T, class... Types>
            struct Index<T, std::tuple<T, Types...>> {
                static const std::size_t value = 0;
            };

            template <class T, class U, class... Types>
            struct Index<T, std::tuple<U, Types...>> {
                static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
            };

            // data
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            std::mutex m_graphMtx;

        public:
            template <class T>
            class ManagedType;
            // GenBaseHierarchy definition
            template <class T, std::size_t I = 0, bool IsEmpty = I < std::tuple_size<typename T::Info::BaseList>{}>
            class GenBaseHierarchy;

            // GenBaseHierarchy template specialization
            template <class T, std::size_t I>
            class GenBaseHierarchy<T, I, true> : public T {
                template <class U>
                bool is() {
                    return Index<T, TypePolicyList>::value == Index<U, TypePolicyList>::value;
                }
            };

            template <class T, std::size_t I>
            class GenBaseHierarchy<T, I, false> : 
                public GenBaseHierarchy<typename T::Info::BaseList, 0, std::tuple_size<std::tuple_element_t<I, typename T::Info::BaseList>>{} == 0>, 
                public GenBaseHierarchy<T, I + 1, I + 1 < std::tuple_size<typename T::Info::BaseList>{}> 
            {
                using Base1 = GenBaseHierarchy<typename T::Info::BaseList, 0, std::tuple_size<std::tuple_element_t<I, typename T::Info::BaseList>>{} == 0>;
                using Base2 = GenBaseHierarchy<T, I + 1, I + 1 < std::tuple_size<typename T::Info::BaseList>{}>;
                template <class U>
                bool is() {
                    bool ret = Base1::template is<U>();
                    if (ret) {
                        return ret;
                    }
                    return Base2::template is<U>();
                }
            };
            
            template <class T>
            class ManagedType : public GenBaseHierarchy<T> {
            
            };
            
            // create function
            template <class T>
            UmlPtr<GenBaseHierarchy<T>> create() {
                auto ret = std::make_shared<GenBaseHierarchy<T>>(Index<T, TypePolicyList>::value, this);
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                ret->m_node = m_graph.emplace(ret->getID(), std::make_shared<ManagerNode>(ret)).first->second;
                ret->m_node.lock()->m_myPtr = ret->m_node;
                return ret.get();
            }
    };
}

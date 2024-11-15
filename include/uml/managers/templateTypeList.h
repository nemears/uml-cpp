#pragma once

#include <cstddef>

namespace UML {
    // template class typelist
    template <template <class> class ... TemplateTypes>
    struct TemplateTypeList {};

    // utility types for TemplateTypeList
    // size of typelist
    template <class TypeList>
    struct TemplateTypeListSize;

    template <template <class> class First, template <class> class ... RestOfTypeList>
    struct TemplateTypeListSize<TemplateTypeList<First, RestOfTypeList...>> {
        static const std::size_t result = 1 + TemplateTypeListSize<TemplateTypeList<RestOfTypeList...>>::result;
    };

    template <template <class> class Last>
    struct TemplateTypeListSize<TemplateTypeList<Last>> {
        static const std::size_t result = 1;
    };
    
    // type at index of typelist
    // template <std::size_t I, class TypeList, class ManagedType>
    // struct TemplateTypeListType;

    // template <template <class> class First, template <class> class ... RestOfTypeList, class ManagedType>
    // struct TemplateTypeListType<0, TemplateTypeList<First, RestOfTypeList...>, ManagedType> {
    //     using result = First<ManagedType>;
    // };

    // template <std::size_t I, template <class> class First, template <class> class ... RestOfTypeList, class ManagedType>
    // struct TemplateTypeListType<I, TemplateTypeList<First, RestOfTypeList...>, ManagedType> {
    //     using result = typename TemplateTypeListType<I - 1, TemplateTypeList<RestOfTypeList...>, ManagedType>::result;
    // };

    // redo
    template <std::size_t I, class TypeList>
    struct TemplateTypeListType;

    template <template <class> class First, template <class> class ... RestOfTypeList>
    struct TemplateTypeListType<0, TemplateTypeList<First, RestOfTypeList...>> {
        template <class T>
        struct result : public First<T> {};
    };

    template <std::size_t I, template <class> class First, template <class> class ... RestOfTypeList>
    struct TemplateTypeListType<I, TemplateTypeList<First, RestOfTypeList...>> : 
        public TemplateTypeListType<I - 1, TemplateTypeList<RestOfTypeList...>> 
    {};

    // index of type in typelist
    template <template <class> class TypeToFind, class TypeList>
    struct TemplateTypeListIndex;

    template <template <class> class TypeToFind, template <class> class ... RestOfTypes>
    struct TemplateTypeListIndex<TypeToFind, TemplateTypeList<TypeToFind, RestOfTypes ...>> {
        static const std::size_t result = 0;
    };

    template <template <class> class TypeToFind, template <class> class First, template <class> class ... RestOfTypes>
    struct TemplateTypeListIndex<TypeToFind, TemplateTypeList<First, RestOfTypes...>> {
        static const std::size_t result = TemplateTypeListIndex<TypeToFind, TemplateTypeList<RestOfTypes...>>::result + 1;
    };
}

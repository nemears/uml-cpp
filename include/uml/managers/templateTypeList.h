#pragma once

#include <cstddef>
#include <type_traits>

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

    template <>
    struct TemplateTypeListSize<TemplateTypeList<>> {
        static const std::size_t result = 0;
    };
    
    // type at index of typelist
    template <std::size_t I, class TypeList>
    struct TemplateTypeListType;

    template <template <class> class First, template <class> class ... RestOfTypeList>
    struct TemplateTypeListType<0, TemplateTypeList<First, RestOfTypeList...>> {
        template <class T>
        using result = First<T>;
    };

    template <std::size_t I, template <class> class First, template <class> class ... RestOfTypeList>
    struct TemplateTypeListType<I, TemplateTypeList<First, RestOfTypeList...>> {
        template <class T>
        using result = typename TemplateTypeListType<I - 1, TemplateTypeList<RestOfTypeList...>>::template result<T>;
    };

    // index of type in typelist
    template <template <class> class TypeToFind, class TypeList>
    struct TemplateTypeListIndex;

    template <template <class> class TypeToFind, template <class> class First, template <class> class ... RestOfTypes>
    struct TemplateTypeListIndex<TypeToFind, TemplateTypeList<First, RestOfTypes...>> {
        private:
            using ResultOfPrior = TemplateTypeListIndex<TypeToFind, TemplateTypeList<RestOfTypes...>>;
            // necessary sometimes because of type.alias see
            // https://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1286
            struct Dummy {};
            static constexpr int IsMatch = std::is_same<TypeToFind<Dummy>, First<Dummy>>::value;
        public:
            static const int result =  IsMatch ? 0 : ResultOfPrior::result == -1 ? -1 : ResultOfPrior::result + 1;
    };

    template <template <class> class TypeToFind>
    struct TemplateTypeListIndex<TypeToFind, TemplateTypeList<>> {
        static const int result = -1;
    };

    // concatenate two typelists
    template <class TypeList1, class TypeList2>
    struct TemplateTypeListCat;

    template <template <class> class ... LeftTypes, template <class> class ... RightTypes>
    struct TemplateTypeListCat<TemplateTypeList<LeftTypes...>, TemplateTypeList<RightTypes...>> {
        using result = TemplateTypeList<LeftTypes..., RightTypes...>;
    };
}

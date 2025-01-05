#pragma once

#include "templateTypeList.h"
#include "integerList.h"
#include "dummyManager.h"

namespace UML {
    template <bool IsFirst, class First, class Second>
    struct Choose;
    template <class First, class Second>
    struct Choose<true, First, Second> {
        using result = First;
    };
    template <class First, class Second>
    struct Choose<false, First, Second> {
        using result = Second;
    };

    template <class List, template <class> class Curr, class AllTypesList, class Bases = Curr<DummyManager::BaseElement>::Info::BaseList>
    struct AddChildrenTypes;

    template <class List, template <class> class Curr, class AllTypesList, template <class> class Front, template <class> class ... Bases>
    struct AddChildrenTypes<List, Curr, AllTypesList, TemplateTypeList<Front, Bases...>> {
        static const std::size_t front_type = TemplateTypeListIndex<Front, AllTypesList>::result;
        using ListAndFront = Choose<HasInt<front_type, List>::value, List ,typename IntAppend<List, front_type>::type>::result;
        using result = typename AddChildrenTypes<
                typename AddChildrenTypes<
                    ListAndFront, 
                    Front,
                    AllTypesList
                >::result, 
                Curr, 
                AllTypesList,
                TemplateTypeList<Bases...>
            >::result;
    };

    template <class List, template <class> class Curr, class AllTypesList>
    struct AddChildrenTypes<List, Curr, AllTypesList, TemplateTypeList<>> {
        using result = List;
    };

    template <class Visitor, template <class> class Curr, class AllTypesList, class Visited = IntList<>, class Bases = Curr<DummyManager::BaseElement>::Info::BaseList>
    struct VisitAllTypesDFS;

    template <class Visitor, template <class> class Curr, class AllTypesList, class Visited, template <class> class Front, template <class> class ... Rest>
    struct VisitAllTypesDFS<Visitor, Curr, AllTypesList, Visited, TemplateTypeList<Front, Rest...>> {
        using CurrentTypeList = TemplateTypeList<Front, Rest...>;
        static void visit(Visitor& visitor) {
            if constexpr  (TemplateTypeListSize<typename Curr<DummyManager::BaseElement>::Info::BaseList>::result == TemplateTypeListSize<CurrentTypeList>::result) {
                visitor.template visit<Curr>();
            }
            constexpr std::size_t BaseID = TemplateTypeListIndex<Front, AllTypesList>::result;
            constexpr std::size_t CurrID = TemplateTypeListIndex<Curr, AllTypesList>::result;
            using VisitedAndCurr = Choose<HasInt<CurrID, Visited>::value, Visited, typename IntAppend<Visited, CurrID>::type>::result;
            if constexpr (!HasInt<BaseID, VisitedAndCurr>::value) {
                VisitAllTypesDFS<Visitor, Front, AllTypesList, VisitedAndCurr>::visit(visitor);
                using NewVisited = AddChildrenTypes<typename IntAppend<VisitedAndCurr, BaseID>::type, Front, AllTypesList>::result;
                VisitAllTypesDFS<Visitor, Curr, AllTypesList, NewVisited, TemplateTypeList<Rest...>>::visit(visitor);
            } else {
                VisitAllTypesDFS<Visitor, Curr, AllTypesList, VisitedAndCurr, TemplateTypeList<Rest...>>::visit(visitor);
            }
        }
    };

    template <class Visitor, template <class> class Curr, class AllTypesList, class Visited>
    struct VisitAllTypesDFS<Visitor, Curr, AllTypesList, Visited, TemplateTypeList<>> {
        static void visit(Visitor&) {}
    };

    template <class Visitor, template <class> class Start, class AllTypesList>
    void visitAllTypesDFS(Visitor& visitor) {
        VisitAllTypesDFS<Visitor, Start, AllTypesList>::visit(visitor);
    }
}
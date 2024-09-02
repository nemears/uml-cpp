#pragma once

#include <tuple>
#include <vector>
#include <unordered_set>
#include <uml/managers/baseElement.h>

namespace UML {
    // typeinfo should be TypeInfo<TList<pair<set name, set function ptr>>, TList<set BaseTypeInfos>>

    class AbstractSet;

    template <class BaseList>
    class BaseInfo;
   
    template <class ... Bases, class Base>
    class BaseInfo<std::tuple<Base, Bases...>> : public Base::Info, BaseInfo<std::tuple<Bases...>> {
        typedef std::tuple<Base, Bases...> BaseList;
        typedef typename Base::Info LeftBase;
        typedef BaseInfo<std::tuple<Bases...>> RightBase;
        template <class T>
        constexpr bool is() {
            auto curr = LeftBase::template is<T>();
            if (curr) {
                return true;
            }

            return RightBase::template is<T>();
        }
    };

    template <class Base>
    class BaseInfo<std::tuple<Base>> : public Base::Info {
        typedef typename Base::Info LeftBase;
        template <class T>
        constexpr bool is() {
            return Base::template FindIdOf<T>::value  == Base::template FindIdOf<Base>;
        }
    };

    template <>
    class BaseInfo<std::tuple<>> {
        constexpr bool is() {
            return false;
        }
    };

    template <class T>
    constexpr bool is(const std::size_t elementType) {
        return T::Info::template is<T::template GetType<elementType>::type>();
    }

    template <class ElWithSets>
    class SetInfo;

    template <>
    class SetInfo<AbstractElement> {};

    template <class BaseList = std::tuple<>, class ElementType = AbstractElement>
    class TypeInfo : public BaseInfo<BaseList>{
        typedef ElementType Type;
        typedef SetInfo<ElementType> Sets;
    };

    // get all sets
    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;

    template <class T>
    void addSetsToQueue(SetList& queue, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
        if (!visited.count(T::template TypeIdOf<T>::value)) {
            queue.push_back(T::Info::Sets::sets(el));
            visited.insert(T::template TypeIdOf<T>::value);
        }
    }

    template <std::size_t I = 0, class BaseList>
    void addSetsToQueueHelper(SetList& queue, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
        if (std::tuple_size<BaseList>{} == I) {
            return;
        }
        addSetsToQueue<std::tuple_element<I, BaseList>>(queue, visited, el);
        addSetsToQueueHelper<I + 1, BaseList>(queue, el);
    }
    
    template <class T>
    constexpr SetList getAllSetsHelper(std::unordered_set<std::size_t>& visited, T& el) {
        typedef typename T::template GetType<el.getElementType()>::type ElementType;
        SetList ret = ElementType::Info::Sets::sets(el);
        addSetsToQueue<ElementType>(ret, visited, el);
        return ret;
 
    }

    template <class T>
    SetList getAllSets(T& el) {
        std::unordered_set<std::size_t> visited = { el.getElementType() };
        return getAllSetsHelper(visited, el);
    }
}

#pragma once

#include <tuple>
#include "abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {
    class NullType {};

    // below is implementation of TypeIDHierarchy from Alex Andrescu's C++ Generic Programming and Design Patterns Applied
    // with variardic templates from c++11

    // type id
    template <std::size_t ElementType, class Tlist, template<std::size_t, class> class Unit>
    class TypeIDHierarchy;

    template <std::size_t ElementType, class ... Ts, class T, template<std::size_t, class> class Unit>
    class TypeIDHierarchy<ElementType, std::tuple<T, Ts...>, Unit> : public TypeIDHierarchy<ElementType, T, Unit>, public TypeIDHierarchy<ElementType - 1, Ts..., Unit> {
        public:
            typedef std::tuple<T, Ts...> Tlist;
            typedef TypeIDHierarchy<ElementType, T, Unit> LeftBase;
            typedef TypeIDHierarchy<ElementType - 1, Ts..., Unit> RightBase;
    };

    template <std::size_t ElementType, class AtomicType, template <std::size_t, class> class Unit>
    class TypeIDHierarchy: public Unit<ElementType, AtomicType> {
        typedef Unit<ElementType, AtomicType> LeftBase;
    };

    template <std::size_t ElementType, template <std::size_t, class> class Unit> 
    class TypeIDHierarchy<ElementType, NullType, Unit> {};

    template <std::size_t ElementType, class T>
    struct TypeID {
        static std::size_t elementType() {
            return ElementType;
        }
    };

    template <class ... Ts>
    class ManagerTypes : public TypeIDHierarchy<std::tuple_size<std::tuple<Ts...>>{}, std::tuple<Ts...>, TypeID> {
        typedef std::tuple<Ts...> Types;

        // utility class to get type given size_t
        template <std::size_t ElementType> 
        struct GetType {
            using type = std::tuple_element<ElementType, Types>;
        };

        // utility class to get elementType given type
        template<class Tlist, class T>
        struct TypeIdOfHelper;

        template <class T>
        struct TypeIdOfHelper<NullType, T> {
            enum { value = -1 };
        };

        template <class T, class Tail> 
        struct TypeIdOfHelper<std::tuple<T>, Tail> {
            enum { value = 0 };
        };

        template <class ... TTs, class T>
        struct TypeIdOfHelper<std::tuple<TTs...>, T> {
            private:
                enum { temp = TypeIdOfHelper<Ts...>::value };
            public:
                enum { value = temp == -1 ? -1 : 1 + temp }; 
        };
        
        template <class T>
        struct TypeIdOf : public TypeIdOfHelper<Types, T> {};
    };

    // factory
    template <class Tlist, template<class> class Unit>
    struct CreatorHierarchy;

    template <class ...Ts, class T, template<class> class Unit>
    struct CreatorHierarchy<std::tuple<T, Ts...>, Unit> : public CreatorHierarchy<T, Unit> ,CreatorHierarchy<std::tuple<Ts...>, Unit> {
        typedef std::tuple<T, Ts...> Tlist;
        typedef CreatorHierarchy<T, Unit> LeftBase;
        typedef CreatorHierarchy<std::tuple<Ts...>, Unit> RightBase;
    };

    template <class AtomicType, template<class> class Unit>
    struct CreatorHierarchy : public Unit<AtomicType> {
        typedef Unit<AtomicType> LeftBase;
    };

    template <template <class> class Unit>
    struct CreatorHierarchy<NullType, Unit> {};

    template <class T>
    class Creator {
        protected:
            T* createPtr(std::size_t elementType) {
                return new T(elementType);
            }
    };

    template <class ... Ts>
    class AbstractFactory : public CreatorHierarchy<std::tuple<Ts...>, Creator> {
        public:
            template <class T>
            T* factoryCreate(std::size_t elementType) {
                // TODO
                return Creator<T>::createPtr(elementType);
            }
    };
    
    template <class SerializationPolicy, class PersistencePolicy, class ... Ts> 
    class Manager : public ManagerTypes<Ts...>, public AbstractFactory<Ts...>, public AbstractManager , protected SerializationPolicy, protected PersistencePolicy {
        protected:
            std::unordered_map<ID, ManagerNode> m_graph;
            void registerPtr(Element* ptr) {
                m_graph.emplace(ptr->getID(), ptr);
            }
        public:
            // create by type id
            ElementPtr create(std::size_t elementType) override {
                return AbstractFactory<Ts...>::template factoryCreate<ManagerTypes<Ts...>::template GetType<elementType>::type>(elementType);
            }

            // create by type
            template <class T>
            UmlPtr<T> create() {
                return AbstractFactory<T>::template factoryCreate<T>(ManagerTypes<Ts...>::template TypeIdOf<T>::value);
            }

            // TODO the rest
    };
}

#pragma once

#include <mutex>
#include <tuple>
#include "abstractManager.h"
#include "typeID.h"
#include "uml/umlPtr.h"
#include "managerNode.h"

namespace UML {

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
    struct CreatorHierarchy<std::tuple<>, Unit> {};

    template <class T>
    class Creator {
        protected:
            T* createPtr(std::size_t elementType, AbstractManager& manager) {
                return new T(elementType, manager);
            }
    };

    template <class ... Ts>
    class AbstractFactory : public CreatorHierarchy<std::tuple<Ts...>, Creator> {
        public:
            template <class T>
            T* factoryCreate(std::size_t elementType, AbstractManager& manager) {
                // TODO
                return Creator<T>::createPtr(elementType, manager);
            }
    };


    // manager
    template <class SerializationPolicy, class PersistencePolicy, class ... Ts> 
    class Manager : public ManagerTypes<Ts...>, public AbstractFactory<Ts...>, public AbstractManager , protected SerializationPolicy, protected PersistencePolicy {
        protected:
            std::mutex m_graphMutex;
            std::unordered_map<ID, ManagerNode> m_graph;
            template<class T>
            UmlPtr<T> registerPtr(T* ptr) {
                // create Node by emplace in graph
                std::lock_guard<std::mutex> graphLock(m_graphMutex);
                ManagerNode& node = m_graph.emplace(ptr->getID(), ptr).first->second;

                // Node Reference
                ptr->m_node = std::make_shared<NodeReference>(node);

                // initialize ptr through copy
                return ptr;
            }
        public:
            // create by type id
            ElementPtr create(std::size_t elementType) override {
                return registerPtr(AbstractFactory<Ts...>::template factoryCreate<ManagerTypes<Ts...>::template GetType<elementType>::type>(elementType, *this));
            }

            // create by type
            template <class T>
            UmlPtr<T> create() {
                return registerPtr(AbstractFactory<T>::template factoryCreate<T>(ManagerTypes<Ts...>::template TypeIdOf<T>::value, *this));
            }

            // get by id
            ElementPtr get(ID id) override {

                // local memory look up
                {
                    std::lock_guard<std::mutex> graphLock(m_graphMutex);
                    auto it = m_graph.find(id);
                    if (it != m_graph.end() && it->second.m_ptr) {
                        return it->second.m_ptr.get();
                    }
                }

                // check for it via policies
                ElementPtr ret = SerializationPolicy::parseIndividul(PersistencePolicy::loadElementData(id), *this);

                if (ret) {
                    // TODO restore

                }

                return ret;
            }

            // TODO the rest
    };
}

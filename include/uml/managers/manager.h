#pragma once

#include <mutex>
#include <tuple>
#include "abstractManager.h"
#include "typeID.h"
#include "uml/managers/baseElement.h"
#include "uml/managers/filePersistencePolicy.h"
#include "uml/managers/serialization/uml-cafe/openUmlSerializationPolicy.h"
#include "uml/managers/typeInfo.h"
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

    template <class Tlist>
    class AbstractFactory : public CreatorHierarchy<Tlist, Creator> {
        public:
            template <class T>
            T* factoryCreate(std::size_t elementType, AbstractManager& manager) {
                // TODO
                return Creator<T>::createPtr(elementType, manager);
            }
    };


    // manager
    template <class Tlist, class SerializationPolicy = UmlCafeSerializationPolicy, class PersistencePolicy = FilePersistencePolicy> 
    class Manager : public ManagerTypes<Tlist>, public AbstractFactory<Tlist>, public AbstractManager , public SerializationPolicy, public PersistencePolicy {
        protected:
            std::mutex m_graphMutex;
            std::unordered_map<ID, ManagerNode> m_graph;
            AbstractElementPtr m_root;
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
            void destroyNode(ID id, ManagerNode& node) override {
                // copied this from old api, says "remove any trailing references", not sure if we still need it
                for (auto& referencePair : node.m_references) {
                    if (referencePair.second.m_node.m_references.contains(id)) {
                        referencePair.second.m_node.m_references.erase(id);
                    }
                }

                // remove from memory
                std::lock_guard<std::mutex> graphLock(m_graphMutex);
                m_graph.erase(id);
 
            }
            void reindex(ID oldID, ID newID) override {
                auto el = m_graph.at(oldID).m_ptr.get();
                auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*el));
                for (auto& pair : sets) {
                    if (pair.second->size() > 0) {
                        throw ManagerStateException("Bad reindex, element must be newly created to reindex!");
                    }
                }

                el->m_id = newID;
                {
                    std::lock_guard<std::mutex> graphLock(m_graphMutex);
                    m_graph.erase(oldID);
                }
                if (m_graph.count(newID)) {
                    // erase node we are overwriting
                    erase(*m_graph.at(newID).m_ptr);
                }
                registerPtr(el);
            }
        public:
            // create by type id
            // AbstractElementPtr create(std::size_t elementType) override {
            //     return registerPtr(AbstractFactory<Tlist>::template factoryCreate<ManagerTypes<Tlist>::template GetType<elementType>::type>(elementType, *this));
            // }

            // create by type
            template <class T>
            UmlPtr<T> create() {
                return registerPtr(this->template factoryCreate<T>(ManagerTypes<Tlist>::template TypeIdOf<T>::value, *this));
            }

            // create Ptr
            AbstractElementPtr createPtr(ID id) override {
                auto ret = get(id);
                if (ret) {
                    return ret;
                }
                auto pair = m_graph.emplace(id, *this);
                ret.m_node = std::make_shared<NodeReference>(pair.first->second);
                ret.m_id = id;
                return ret;
            }

            // get by id
            AbstractElementPtr get(ID id) override {

                // local memory look up
                {
                    std::lock_guard<std::mutex> graphLock(m_graphMutex);
                    auto it = m_graph.find(id);
                    if (it != m_graph.end() && it->second.m_ptr) {
                        return it->second.m_ptr.get();
                    }
                }

                // check for it via policies
                AbstractElementPtr ret = SerializationPolicy::parseIndividual(PersistencePolicy::loadElementData(id), *this);


                if (ret) {
                    // run add policies we skipped over
                    auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*ret));
                    for (auto& pair : sets) {
                        auto set = pair.second;
                        auto endPtr = set->endPtr();                        
                        for (auto itPtr = set->beginPtr(); *itPtr != *endPtr; itPtr->next()) {
                            auto elRestore = itPtr->getCurr();
                            set->runAddPolicy(*elRestore);
                        } 
                    }
                }

                return ret;
            }

            void setRoot(AbstractElementPtr root) {
                m_root = root;
            }

            AbstractElementPtr getRoot() const {
                return m_root;
            }

            void release(AbstractElement& el) override {
                PersistencePolicy::saveElementData(SerializationPolicy::emitIndividual(el, *this), el.getID());
                ManagerNode& node = el.m_node->m_node;
                ID id = node.m_ptr->getID();

                // clear shared_ptrs
                for (auto ptr : node.m_ptrs) {
                    ptr->releasePtr();    
                }
                node.m_ptr = 0;

                // check if there are any ptrs to this node,
                // if there are none we can get rid of this node
                if (node.m_ptrs.empty() && !node.m_ptr) {
                    destroyNode(id, node);
                }
            }

            void erase(AbstractElement& el) {
                auto id = el.getID();
                PersistencePolicy::eraseEl(id);
                for (auto referencePair : el.m_node->m_node.m_references) {
                    AbstractElementPtr referencedEl;
                    if (!referencePair.second.m_node.m_ptr) {
                        // element has been released
                        referencedEl = get(referencePair.first);
                    } else {
                        referencedEl = referencePair.second.m_node.m_ptr.get();
                    }

                    // remove all el from all of referencedEl's sets with el in it
                    auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*referencedEl));
                    for (auto& pair : sets) {
                        auto set = pair.second;
                        while (set->contains(&el)) {
                            set->innerRemove(&el);
                        } 
                    }
                } 
    
                std::lock_guard<std::mutex> graphLock(m_graphMutex);
                m_graph.erase(id);
            }

            bool loaded(ID id) {
                // todo thread safety
                return m_graph.count(id);
            }

            // TODO the rest
    };
}

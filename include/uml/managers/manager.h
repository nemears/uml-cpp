#pragma once

#include <cstddef>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "uml/managers/filePersistencePolicy.h"
#include "uml/managers/serialization/uml-cafe/umlCafeSerializationPolicy.h"
#include "umlPtr.h"
#include "abstractManager.h"
#include "comparableElement.h"
#include "templateTypeList.h"

namespace UML {
    // Storage Policy, how the manager accesses the element data stored
    // usually this involves serializing data and saving to persistent storage
    // for that reason the SerializedStoragePolicy is defined to provide the link between
    // persistent data and serialization
    // SerializationPolicy should have the following methods defined:
    //      - UmlPtr parseIndividual(string data)
    //      - string emitIndividual(AbstractElement& el)
    //      - Collection<UmlPtr> parseWhole(string data)
    //      - string emitWhole(AbstractElement& el)
    // Persistence Policy should have the following methods defined:
    //      string loadElementData(ID id)
    //      void saveElementData(string data, ID id)
    //      string getProjectData(string path)
    //      string getProjectData()
    //      void saveProjectData(string data, string path)
    template <class SerializationPolicy, class PersistencePolicy>
    class SerializedStoragePolicy : virtual public SerializationPolicy, public PersistencePolicy {
        protected:
            AbstractElementPtr loadElement(ID id) {
                return SerializationPolicy::parseIndividual(PersistencePolicy::loadElementData(id));
            }
            void saveElement(AbstractElement& el) {
                PersistencePolicy::saveElementData(SerializationPolicy::emitIndividual(el), el.getID());
            }
            AbstractElementPtr loadAll(std::string path) {
                return SerializationPolicy::parseWhole(PersistencePolicy::getProjectData(path))[0];
            }
            AbstractElementPtr loadAll() {
                return SerializationPolicy::parseWhole(PersistencePolicy::getProjectData())[0];
            }
            void saveAll(AbstractElement& root, std::string location) {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(root), location);
            }
            void saveAll(AbstractElement& root) {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(root));
            }
            void eraseEl(ID id) {
                PersistencePolicy::eraseEl(id);
            }
    };
   
    // manager
    template <class TypePolicyList, class StoragePolicy = SerializedStoragePolicy<UmlCafeSerializationPolicy<TypePolicyList>, FilePersistencePolicy>>
    class Manager : public StoragePolicy, virtual public AbstractManager {
        public:
            // Base element for all types created by manager, the policy classes provided will be filled out with
            // this BaseElement as part of their policy
            class BaseElement : public ComparableElement<TypePolicyList> {
                protected:
                    // constructor
                    BaseElement(std::size_t elementType, AbstractManager& manager) : ComparableElement<TypePolicyList>(elementType, manager) {}
                public:
                    using Manager = Manager;
            };
        private:
            // data
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            std::mutex m_graphMtx;
            bool m_destructionFlag = false;
            UmlPtr<BaseElement> m_root;

        public:
            template <template <class> class T, std::size_t I = 0, bool HasBases = (TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result > 0)>
            struct GenBaseHierarchy;

            template <template <class> class T, std::size_t I>
            struct GenBaseHierarchy<T, I, false> : virtual public BaseElement {};

            template <template <class> class T, std::size_t I>
            struct GenBaseHierarchy<T, I, true> :
                virtual public TemplateTypeListType<I, typename T<BaseElement>::Info::BaseList>::template result<GenBaseHierarchy<TemplateTypeListType<I, typename T<BaseElement>::Info::BaseList>::template result>>,
                virtual public GenBaseHierarchy<T, I + 1, I + 1 < TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result>
            {};
        protected:
            template<template <class> class T>
            UmlPtr<T<GenBaseHierarchy<T>>> registerPtr(std::shared_ptr<T<GenBaseHierarchy<T>>>& ptr) {
                // create Node by emplace in graph
                std::lock_guard<std::mutex> graphLock(this->m_graphMutex);
                ptr->m_node = m_graph.emplace(ptr->getID(), std::make_shared<ManagerNode>(ptr)).first->second;
                ptr->m_node.lock()->m_myPtr = ptr->m_node;
                // initialize ptr through copy
                return ptr.get();
            }
            template <template <class> class CurrentType, std::size_t I = 0>
            bool setsAreEmptyHelper(BaseElement& el) {
                if constexpr (I < TemplateTypeListSize<typename CurrentType<BaseElement>::Info::BaseList>::result) {
                    if (!setsAreEmptyHelper<TemplateTypeListType<I, typename CurrentType<BaseElement>::Info::BaseList>::template result>(el)) {
                        return false;
                    }
                    return setsAreEmptyHelper<CurrentType, I + 1>(el);
                } else {
                    for (auto setPair : CurrentType<BaseElement>::Info::sets(el)) {
                        if (!setPair.second->empty()) {
                            return false;
                        }
                    }
                    return true;
                }
            }
            template <std::size_t I = 0>
            bool setsAreEmpty(BaseElement& el) {
                if constexpr (I < TemplateTypeListSize<TypePolicyList>::result) {
                    if (el.getElementType() == I) {
                        return setsAreEmpty(el);
                    } else {
                        return setsAreEmpty<I + 1>(el);
                    }
                } else {
                    throw ManagerStateException("Cannot find type!");
                }
            }
            void reindex(ID oldID, ID newID) override {
                auto oldNode = m_graph.at(oldID);
                auto el = oldNode->m_ptr;

                // check valid
                if (!setsAreEmpty(el)) {
                    throw ManagerStateException("Bad reindex, element must be newly created to reindex!");
                }

                // clear shared_ptrs
                std::vector<AbstractPtr*> ptrs(oldNode->m_ptrs.size());
                auto i = 0;
                for (auto ptr : oldNode->m_ptrs) {
                    ptrs[i] = ptr;
                    i++;
                }
                {
                    std::lock_guard<std::mutex> graphLock(this->m_graphMutex);
                    m_graph.erase(oldID);
                }

                // handle new node
                el->m_id = newID;

                std::shared_ptr<ManagerNode> newNode;
                if (m_graph.count(newID)) {
                    // erase node we are overwriting
                    newNode = m_graph.at(newID);
                    el->m_node = newNode;
                    newNode->m_ptr = el;
                    ptrs.resize(ptrs.size() + newNode->m_ptrs.size());
                    for (auto ptr : newNode->m_ptrs) {
                        ptrs[i] = ptr;
                        i++;
                    }
                } else {
                    registerPtr(el);
                    newNode = el->m_node.lock();
                }
                for (auto ptr : ptrs) {
                    ptr->m_node = newNode;
                    ptr->m_id = newID;
                    ptr->setPtr(el);
                    newNode->m_ptrs.insert(ptr);
                }
            }
            void destroy(ID id) override {
                if (m_destructionFlag) {
                    return;
                }
                m_graph.erase(id);
            }
            void addToSet(AbstractSet& set, AbstractElement& el) override {
                set.innerAdd(&el);
            }
            void addToSet(AbstractSet& set, ID id) override {
                set.nonPolicyAdd(createPtr(id));
            }
            void runAllAddPolicies(AbstractSet& set, AbstractElement& el) const override {
                std::list<AbstractSet*> queue = {&set};
                std::unordered_set<AbstractSet*> visited;
                while (!queue.empty()) {
                    auto front = queue.front();
                    queue.pop_front();
                    if (visited.count(front)) {
                        continue;
                    }
                    visited.insert(front);
                    front->runAddPolicy(el);
                    for (auto superSet : front->m_structure->m_superSets) {
                        queue.push_back(&superSet->m_set);
                    }
                }
            }
            void restoreElAndOpposites(UmlPtr<BaseElement> ptr) {
                // TODO replace
                auto sets = getAllSets(*ptr);
                for (auto& pair : sets) {
                    auto set = pair.second;
                    std::vector<AbstractElementPtr> els(set->size());
                    auto i = 0;
                    for (auto itPtr = set->beginPtr(); *itPtr != *set->endPtr(); itPtr->next()) {
                        auto elRestore = itPtr->getCurr();
                        els[i] = elRestore;
                        i++;
                    }
                    for (auto el: els) {
                        if (!el.loaded()) {
                            continue;
                        }
                        
                        set->runAddPolicy(*el);
                    }
                }
                for (auto& pair : sets) {
                    auto set = pair.second;
                    std::vector<AbstractElementPtr> els(set->size());
                    auto i = 0;
                    for (auto itPtr = set->beginPtr(); *itPtr != *set->endPtr(); itPtr->next()) {
                        auto elRestore = itPtr->getCurr();
                        els[i] = elRestore;
                        i++;
                    }
                    for (auto el: els) {
                        if (!el.loaded()) {
                            continue;
                        }
                        
                        if (!set->subSetContains(el.id())) {
                            set->addToOpposite(el);
                        }
                    }
                }
            }
            void restoreEl(BaseElement& el) {
                // run add policies we skipped over
                // TODO remake
                auto sets = getAllSets(el);
                for (auto& pair : sets) {
                    auto set = pair.second;
                    std::vector<AbstractElementPtr> els(set->size());
                    auto i = 0;
                    for (auto itPtr = set->beginPtr(); *itPtr != *set->endPtr(); itPtr->next()) {
                        auto elRestore = itPtr->getCurr();
                        els[i] = elRestore;
                        i++;
                    }
                    for (auto el: els) {
                        if (!el.loaded()) {
                            continue;
                        }
                        
                        set->runAddPolicy(*el);
                    }
                }
            }
        public:
            // create factory function
            template <template <class> class T>
            UmlPtr<T<GenBaseHierarchy<T>>> create() {
                auto ptr = std::make_shared<T<GenBaseHierarchy<T>>>(TemplateTypeListIndex<T, TypePolicyList>::result, *this);
                return registerPtr<T>(ptr);
            }
        private:
            template <std::size_t I = 0>
            UmlPtr<AbstractElement> createHelper(std::size_t type) {
                if (type == I) {
                    return create<TemplateTypeListType<I, TypePolicyList>::template result<BaseElement>>();
                }
                return createHelper<I + 1>(type);
            }
        public:
            // create by type id
            UmlPtr<AbstractElement> create(std::size_t type) override {
                return createHelper(type);
            }

            // create Ptr
            AbstractElementPtr createPtr(ID id) override {
                AbstractElementPtr ret;
                auto nodeIt = m_graph.find(id);
                if (nodeIt != m_graph.end()){
                    if (nodeIt->second->m_ptr) {
                        ret = AbstractElementPtr(nodeIt->second->m_ptr.get());
                    } else {
                        ret.m_id = id;
                        ret.m_node = nodeIt->second;
                        nodeIt->second->addPtr(&ret);
                    }
                    return ret;
                }
                auto pair = m_graph.emplace(id, std::make_shared<ManagerNode>(id, *this));
                ret.m_node = pair.first->second;
                ret.m_node.lock()->m_myPtr = ret.m_node.lock();
                ret.m_id = id;
                pair.first->second->addPtr(&ret);
                return ret;
            }

            // get by id
            AbstractElementPtr abstractGet(ID id) override {

                // local memory look up
                {
                    std::lock_guard<std::mutex> graphLock(this->m_graphMutex);
                    auto it = m_graph.find(id);
                    if (it != m_graph.end() && it->second->m_ptr) {
                        return it->second->m_ptr.get();
                    }
                }

                // check for it via policies
                AbstractElementPtr ret = StoragePolicy::loadElement(id);

                if (ret) {
                    restoreEl(dynamic_cast<BaseElement&>(*ret));
                }

                return ret;
            }

            UmlPtr<BaseElement> get(ID id) {
                return abstractGet(id);
            }

            void setRoot(AbstractElementPtr root) override {
                m_root = root;
            }

            UmlPtr<BaseElement> getRoot() const {
                return m_root;
            }

            AbstractElementPtr getAbstractRoot() const override {
                return m_root;
            }

            void release(AbstractElement& el) override {
                StoragePolicy::saveElement(el);
                // PersistencePolicy::saveElementData(SerializationPolicy::emitIndividual(dynamic_cast<BaseElement<Tlist>&>(el), *this), el.getID());
                auto node = el.m_node.lock();
                ID id = node->m_ptr->getID();
                m_destructionFlag = true;
                node->m_ptr = 0;
                m_destructionFlag = false;

                // check if there are any ptrs to this node,
                // if there are none we can get rid of this node
                if (node->m_ptrs.empty() && !node->m_ptr) {
                    // get rid of our reference in empty nodes
                    std::list<std::weak_ptr<ManagerNode>> nodesToErase;
                    for (auto& referencePair : node->m_references) {
                        auto& referenceReferences = referencePair.second.m_node.lock()->m_references;
                        auto referenceMatch = referenceReferences.find(id);
                        if (referenceMatch != referenceReferences.end()) {
                            nodesToErase.push_back(referencePair.second.m_node);
                        }
                    }
                    for (auto& referencedNode : nodesToErase) {
                        referencedNode.lock()->m_references.erase(id);
                    }

                    // get rid of node from graph
                    std::lock_guard<std::mutex> lockGuard(this->m_graphMutex);
                    m_graph.erase(id);
                }
            }

            template <class ... Ts>
            void release(AbstractElement& el, Ts&... els) {
                release(el);
                release(els...);
            }

            void erase(AbstractElement& el) override {
                auto id = el.getID();
                StoragePolicy::eraseEl(id);
                // PersistencePolicy::eraseEl(id);
                std::list<UmlPtr<BaseElement>> elsToDeleteReference;
                for (auto& referencePair : el.m_node.lock()->m_references) {
                    AbstractElementPtr referencedEl;
                    auto referencedNode = referencePair.second.m_node.lock();
                    if (!referencedNode->m_ptr) {
                        // element has been released
                        referencedEl = get(referencedNode->m_id);
                    } else {
                        referencedEl = referencedNode->m_ptr.get();
                    }
                    elsToDeleteReference.push_back(referencedEl);
                }
                for (auto referencedEl : elsToDeleteReference) {
                    // TODO
                    auto sets = getAllSets(dynamic_cast<BaseElement&>(*referencedEl));
                    for (auto& pair : sets) {
                        auto set = pair.second;
                        while (set->contains(&el)) {
                            set->innerRemove(&el);
                        } 
                    }

                    // remove reference
                    auto& referenceReferences = referencedEl.m_node.lock()->m_references;
                    auto referenceReferencesIt = referenceReferences.find(id);
                    if (referenceReferencesIt != referenceReferences.end()) {
                        referenceReferences.erase(referenceReferencesIt);
                    }
                }
    
                std::lock_guard<std::mutex> graphLock(this->m_graphMutex);
                m_graph.erase(id);
            }

            bool loaded(ID id) {
                // todo thread safety
                auto it = m_graph.find(id);
                if (it == m_graph.end()) {
                    return false;
                }
                return it->second->m_ptr != 0;
            }

            UmlPtr<BaseElement> open(std::string dataPath) {
                auto root = StoragePolicy::loadAll(dataPath);
                setRoot(root);
                return root;
            }
            UmlPtr<BaseElement> open() {
                auto root = StoragePolicy::loadAll();
                setRoot(root);
                return root;
            }
            void save(std::string location) {
                StoragePolicy::saveAll(*getRoot(), location);
            }
            void save() {
                StoragePolicy::saveAll(*getRoot());
            }
    };
}

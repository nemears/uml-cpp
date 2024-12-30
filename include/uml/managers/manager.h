#pragma once

#include <cstddef>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include "uml/managers/abstractElement.h"
#include "uml/managers/filePersistencePolicy.h"
#include "uml/managers/integerList.h"
#include "uml/managers/serialization/uml-cafe/umlCafeSerializationPolicy.h"
#include "uml/set/abstractSet.h"
#include "umlPtr.h"
#include "abstractManager.h"
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
            class BaseElement;
        protected:
            struct AbstractManagerTypeInfo {
                virtual ~AbstractManagerTypeInfo() {}
                virtual void forEachSet(BaseElement& el, std::function<void(std::string, AbstractSet&)>)  = 0;
                virtual bool is(std::size_t type) = 0;
                virtual AbstractElementPtr create(Manager&) = 0;
            };

            std::unordered_map<std::size_t, std::unique_ptr<AbstractManagerTypeInfo>> m_types;

        public:
            using Types = TypePolicyList;
            template <template <class> class Type>
            using ElementType = TemplateTypeListIndex<Type, TypePolicyList>;
            // Base element for all types created by manager, the policy classes provided will be filled out with
            // this BaseElement as part of their policy
            class BaseElement : public AbstractElement {
                protected:    
                    using AbstractElement::AbstractElement;
                    BaseElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
                public:
                    using manager = Manager;
                    // is function to compare types compile time O(1)
                    template <template <class> class T>
                    bool is() const {
                        return dynamic_cast<Manager&>(m_manager).m_types.at(m_elementType)->is(ElementType<T>::result); 
                    }

                    // as to cast to other managed types
                    template <template<class> class T>
                    T<typename Manager::template GenBaseHierarchy<T>>& as() {
                        if (is<T>()) {
                            return dynamic_cast<T<typename Manager::template GenBaseHierarchy<T>>&>(*this);
                        }
                        throw ManagerStateException("Can not convert element to that type!");
                    }
            };

            template <template <class> class Type>
            struct ManagerTypeInfo : public AbstractManagerTypeInfo {
                private:
                    using Function = std::function<void(std::string, AbstractSet&)>;
                    template <template <class> class CurrType = Type, std::size_t I = 0>
                    void helper(BaseElement& el, Function f, std::unordered_set<std::size_t>& visited) {
                        if (!visited.contains(ElementType<CurrType>::result)) {
                            visited.insert(ElementType<CurrType>::result);
                        }
                        using Bases = typename CurrType<BaseElement>::Info::BaseList; 
                        if constexpr (I < TemplateTypeListSize<Bases>::result) {
                            helper<TemplateTypeListType<I, Bases>::template result, 0>(el, f, visited);
                            helper<CurrType, I + 1>(el, f, visited);
                        } else {
                            for (auto& setPair : CurrType<BaseElement>::Info::sets(dynamic_cast<CurrType<GenBaseHierarchy<CurrType>>&>(el))) {
                                f(setPair.first, *setPair.second);
                            } 
                        }
                    }
                    template <template <class> class CurrType = Type, std::size_t I = 0>
                    bool isHelper(std::size_t type) {
                        using BaseList = typename CurrType<BaseElement>::Info::BaseList;
                        if constexpr (I < TemplateTypeListSize<BaseList>::result) {
                            if (isHelper<TemplateTypeListType<I, BaseList>::template result>(type)) {
                                return true;
                            }
                            return isHelper<CurrType, I + 1>(type);
                        } else {
                            if (ElementType<CurrType>::result == type) {
                                return true;
                            }
                            return false;
                        }
                    }
                public:
                    void forEachSet(BaseElement& el, Function f) override {
                        std::unordered_set<std::size_t> visited; 
                        helper(el, f, visited);
                    }
                    bool is(std::size_t type) override {
                        return isHelper(type);                        
                    }
                    AbstractElementPtr create(Manager& manager) override {
                        return manager.create<Type>();
                    }
            };
        private:
            // data
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            std::mutex m_graphMtx;
            bool m_destructionFlag = false;
            UmlPtr<BaseElement> m_root;

        public:
            // Gen Base Hierarchy is a class that will be used to create dynamic objects that inherit from and can correspond to their bases
            // This is used mostly internally but all types created by the manager will be of this policy
            template <template <class> class T, class TypeList = T<BaseElement>::Info::BaseList>
            struct GenBaseHierarchy;

            template <template <class> class T>
            struct GenBaseHierarchy<T, TemplateTypeList<>> : virtual public BaseElement {
                protected:
                    GenBaseHierarchy(std::size_t elementType, AbstractManager& manager) : BaseElement(elementType, manager) {}
            };

            template <template <class> class T, template <class> class First, template <class> class ... Types>
            struct GenBaseHierarchy<T, TemplateTypeList<First, Types...>> :
                virtual public First<GenBaseHierarchy<First>>,
                public GenBaseHierarchy<T, TemplateTypeList<Types...>>
            {
                protected:
                    GenBaseHierarchy(std::size_t elementType, AbstractManager& manager) : 
                        BaseElement(elementType, manager),
                        First<GenBaseHierarchy<First>>(elementType, manager),
                        GenBaseHierarchy<T, TemplateTypeList<Types...>>(elementType, manager)
                    {}
            }; 
        protected:
            UmlPtr<BaseElement> registerPtr(std::shared_ptr<AbstractElement> ptr) {
                // create Node by emplace in graph
                std::lock_guard<std::mutex> graphLock(this->m_graphMtx);
                ptr->m_node = m_graph.emplace(ptr->getID(), std::make_shared<ManagerNode>(ptr)).first->second;
                ptr->m_node.lock()->m_myPtr = ptr->m_node;
                // initialize ptr through copy
                return dynamic_cast<BaseElement*>(ptr.get());
            }


            template <template <class> class CurrType>
            void populateTypes() {
                m_types.emplace(ElementType<CurrType>::result, std::make_unique<ManagerTypeInfo<CurrType>>());
                constexpr int next_type = TemplateTypeListIndex<CurrType, Types>::result + 1; 
                if constexpr (next_type < TemplateTypeListSize<Types>::result) { 
                    populateTypes<TemplateTypeListType<next_type, Types>::template result>();
                }
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
                        return setsAreEmptyHelper<TemplateTypeListType<I, Types>::template result>(el);
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
                if (!setsAreEmpty(dynamic_cast<BaseElement&>(*el))) {
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
                    std::lock_guard<std::mutex> graphLock(this->m_graphMtx);
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
                m_types.at(el.getElementType())->forEachSet(el, [](std::string, AbstractSet& set) {
                    std::vector<AbstractElementPtr> els(set.size());
                    auto i = 0;
                    for (auto itPtr = set.beginPtr(); *itPtr != *set.endPtr(); itPtr->next()) {
                        auto elRestore = itPtr->getCurr();
                        els[i] = elRestore;
                        i++;
                    }
                    for (auto el: els) {
                        if (!el.loaded()) {
                            continue;
                        }
                        
                        set.runAddPolicy(*el);
                    }    
                });
            }
        public:
            // create factory function
            template <template <class> class T>
            UmlPtr<T<GenBaseHierarchy<T>>> create() {
                if constexpr (T<BaseElement>::Info::abstract) {
                    throw ManagerStateException("Trying to instantiate and abstract type!");
                }
                auto ptr = std::make_shared<T<GenBaseHierarchy<T>>>(ElementType<T>::result, *this);
                return registerPtr(ptr);
            }
            // create by type id
            UmlPtr<AbstractElement> create(std::size_t type) override {
                return m_types.at(type)->create(*this);
            }
            
            // constructor
            Manager() {
                populateTypes<TemplateTypeListType<0, Types>::template result>();
            }

            virtual ~Manager() {
                m_destructionFlag = true;
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
                    std::lock_guard<std::mutex> graphLock(this->m_graphMtx);
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
                    std::lock_guard<std::mutex> lockGuard(this->m_graphMtx);
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
                    m_types.at(referencedEl->getElementType())->forEachSet(dynamic_cast<BaseElement&>(*referencedEl), [&el](std::string, AbstractSet& set) {
                        while (set.contains(&el)) {
                            set.innerRemove(&el);
                        }    
                    });

                    // remove reference
                    auto& referenceReferences = referencedEl.m_node.lock()->m_references;
                    auto referenceReferencesIt = referenceReferences.find(id);
                    if (referenceReferencesIt != referenceReferences.end()) {
                        referenceReferences.erase(referenceReferencesIt);
                    }
                }
    
                std::lock_guard<std::mutex> graphLock(this->m_graphMtx);
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


#pragma once

#include <memory>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include "abstractManager.h"
#include "uml/managers/baseElement.h"
#include "uml/managers/filePersistencePolicy.h"
#include "uml/managers/managerTypes.h"
#include "uml/managers/typeInfo.h"
#include "uml/managers/serialization/uml-cafe/umlCafeSerializationPolicy.h"
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
        template <class Tlist>
        friend class AbstractFactory;
        protected:
            std::shared_ptr<T> createPtr(std::size_t elementType, AbstractManager& manager) {
                return std::make_shared<T>(elementType, manager);
            }
    };

    template <class Tlist>
    class AbstractFactory : public CreatorHierarchy<Tlist, Creator> {
        public:
            template <class T>
            std::shared_ptr<T> factoryCreate(std::size_t elementType, AbstractManager& manager) {
                return Creator<T>::createPtr(elementType, manager);
            }
    };


    // helper types for typeInfo navigation
    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;

    template <class T>
    void addSetsToQueue(SetList& queue, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
        if (!visited.count(T::template idOf<T>())) {
            auto sets = T::Info::Info::sets(dynamic_cast<T&>(el));
            for (auto& pair : sets) {
                queue.emplace_back(pair.first, pair.second);
            }
            visited.insert(T::template idOf<T>());
        }
    }

    template <std::size_t I = 0, class BaseList>
    void addSetsToQueueHelper(SetList& queue, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
        if constexpr (std::tuple_size<BaseList>{} > I) {
            using ThisElement = std::tuple_element_t<I, BaseList>;
            addSetsToQueue<ThisElement>(queue, visited, el);
            addSetsToQueueHelper<I + 1, BaseList>(queue, visited, el);
            addSetsToQueueHelper<0, typename ThisElement::Info::BaseList>(queue, visited, el);
        }
    }
   
    template <class Tlist> 
    struct AbstractStaticSetFunctor {
        virtual SetList operator()(BaseElement<Tlist>& el) = 0;
    };

    template <class T, class Tlist>
    struct StaticSetFunctor : public AbstractStaticSetFunctor<Tlist> {
        SetList operator()(BaseElement<Tlist>& el) override {
            SetList ret = T::Info::Info::sets(el.template as<T>());
            std::unordered_set<std::size_t> visited = { el.getElementType() };
            addSetsToQueueHelper<0, typename T::Info::BaseList>(ret, visited, el);
            return ret;
        }
    };

    template <std::size_t I = 0, class Tlist>
    void populateTypes(std::unordered_map<std::size_t, std::unique_ptr<AbstractStaticSetFunctor<Tlist>>>& types) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            types.emplace(I, new StaticSetFunctor<std::tuple_element_t<I, Tlist>, Tlist>());
            populateTypes<I+1, Tlist>(types);
        }
    }

    // manager
    template <class Tlist, class SerializationPolicy = UmlCafeSerializationPolicy<Tlist>, class PersistencePolicy = FilePersistencePolicy> 
    class Manager : public ManagerTypes<Tlist>, public AbstractFactory<Tlist>, virtual public AbstractManager , virtual public SerializationPolicy, public PersistencePolicy {
        protected:

            std::unordered_map<std::size_t, std::unique_ptr<AbstractStaticSetFunctor<Tlist>>> m_types;
            std::mutex m_graphMutex;
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            UmlPtr<BaseElement<Tlist>> m_root;

            SetList getAllSets(BaseElement<Tlist>& el) {
                return (*m_types.at(el.m_elementType))(el);
            }

            template<class T>
            UmlPtr<T> registerPtr(std::shared_ptr<T>& ptr) {
                // create Node by emplace in graph
                std::lock_guard<std::mutex> graphLock(m_graphMutex);
                ptr->m_node = m_graph.emplace(ptr->getID(), std::make_shared<ManagerNode>(ptr)).first->second;

                // initialize ptr through copy
                return ptr.get();
            }
            void reindex(ID oldID, ID newID) override {
                auto oldNode = m_graph.at(oldID);
                auto el = oldNode->m_ptr;

                // check valid
                auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*el));
                for (auto& pair : sets) {
                    if (pair.second->size() > 0) {
                        throw ManagerStateException("Bad reindex, element must be newly created to reindex!");
                    }
                }

                // clear shared_ptrs
                std::vector<AbstractPtr*> ptrs(oldNode->m_ptrs.size());
                auto i = 0;
                for (auto ptr : oldNode->m_ptrs) {
                    ptrs[i] = ptr;
                    i++;
                }
                {
                    std::lock_guard<std::mutex> graphLock(m_graphMutex);
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
        private:
            bool m_destructionFlag = false;
        public:
            Manager() {
                populateTypes<0,Tlist>(m_types);
            }
            ~Manager() {
                m_destructionFlag = true;
            }

            // create by type
            template <class T>
            UmlPtr<T> create() {
                auto ptr = this->template factoryCreate<T>(ManagerTypes<Tlist>::template idOf<T>(), *this); 
                return registerPtr(ptr);
            }
        private:
            // create by elementType helper
            template <std::size_t I = 0>
            AbstractElementPtr create(std::size_t elementType) {
                if constexpr (std::tuple_size<Tlist>{} > I) {
                    using ElementType = std::tuple_element_t<I, Tlist>;
                    if constexpr (!ElementType::Info::Info::abstract) {
                        if (I == elementType) {
                            if constexpr (ElementType::Info::Info::abstract) {
                                throw ManagerStateException("Tried to instantiate abstract element");
                            }         
                            return create<std::tuple_element_t<I, Tlist>>();
                        }
                    }
                    return create<I + 1>(elementType);
                }
                return 0;
            }
        public:
            // create by elementType
            AbstractElementPtr create(std::size_t elementType) override {
                return create<0>(elementType);
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
                ret.m_id = id;
                pair.first->second->addPtr(&ret);
                return ret;
            }

            // get by id
            AbstractElementPtr abstractGet(ID id) override {

                // local memory look up
                {
                    std::lock_guard<std::mutex> graphLock(m_graphMutex);
                    auto it = m_graph.find(id);
                    if (it != m_graph.end() && it->second->m_ptr) {
                        return it->second->m_ptr.get();
                    }
                }

                // check for it via policies
                AbstractElementPtr ret = SerializationPolicy::parseIndividual(PersistencePolicy::loadElementData(id), *this);


                if (ret) {
                    // run add policies we skipped over
                    auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*ret));
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
                            set->runAddPolicy(*el);
                        }
                    }
                }

                return ret;
            }

            UmlPtr<BaseElement<Tlist>> get(ID id) {
                return abstractGet(id);
            }

            void setRoot(UmlPtr<BaseElement<Tlist>> root) {
                m_root = root;
            }

            UmlPtr<BaseElement<Tlist>> getRoot() const {
                return m_root;
            }

            AbstractElementPtr getAbstractRoot() const override {
                return m_root;
            }

            void release(AbstractElement& el) override {
                PersistencePolicy::saveElementData(SerializationPolicy::emitIndividual(dynamic_cast<BaseElement<Tlist>&>(el), *this), el.getID());
                auto node = el.m_node.lock();
                ID id = node->m_ptr->getID();
                m_destructionFlag = true;
                node->m_ptr = 0;
                m_destructionFlag = false;

                // check if there are any ptrs to this node,
                // if there are none we can get rid of this node
                if (node->m_ptrs.empty() && !node->m_ptr) {
                    // get rid of our reference in empty nodes
                    for (auto& reference : node->m_references) {
                        auto& referenceReferences = reference.m_node.lock()->m_references;
                        for (auto referenceReferenceIt = referenceReferences.begin(); referenceReferenceIt != referenceReferences.end(); ++referenceReferenceIt) {
                            if (referenceReferenceIt->m_node.lock()->m_id == id) {
                                referenceReferences.erase(referenceReferenceIt);
                                break;
                            }
                        }
                    }

                    // get rid of node from graph
                    std::lock_guard<std::mutex> lockGuard(m_graphMutex);
                    m_graph.erase(id);
                }
            }

            template <class ... Ts>
            void release(AbstractElement& el, Ts&... els) {
                release(el);
                release(els...);
            }

            void erase(BaseElement<Tlist>& el) {
                auto id = el.getID();
                PersistencePolicy::eraseEl(id);
                for (auto& referencePair : el.m_node.lock()->m_references) {
                    AbstractElementPtr referencedEl;
                    auto referencedNode = referencePair.m_node.lock();
                    if (!referencedNode->m_ptr) {
                        // element has been released
                        referencedEl = get(referencedNode->m_id);
                    } else {
                        referencedEl = referencedNode->m_ptr.get();
                    }

                    // remove all el from all of referencedEl's sets with el in it
                    auto sets = getAllSets(dynamic_cast<BaseElement<Tlist>&>(*referencedEl));
                    for (auto& pair : sets) {
                        auto set = pair.second;
                        while (set->contains(&el)) {
                            set->innerRemove(&el);
                        } 
                    }

                    // remove reference
                    auto referenceReferencesIt = referencedNode->m_references.begin();
                    auto referenceReferencesEnd = referencedNode->m_references.end();
                    while (referenceReferencesIt != referenceReferencesEnd) {
                        if (referenceReferencesIt->m_node.lock()->m_id == id) {
                            break;
                        }
                        referenceReferencesIt++;
                    }
                    if (referenceReferencesIt != referenceReferencesEnd) {
                        referencedNode->m_references.erase(referenceReferencesIt);
                    }
                } 
    
                std::lock_guard<std::mutex> graphLock(m_graphMutex);
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

            UmlPtr<BaseElement<Tlist>> open(std::string dataPath) {
                auto roots = SerializationPolicy::parseWhole(PersistencePolicy::getProjectData(dataPath), *this);
                if (roots.size() == 0) {
                    throw ManagerStateException("No elements parsed!");
                }

                // first element will be root
                setRoot(roots[0]);
                return roots[0];
            }
            UmlPtr<BaseElement<Tlist>> open() {
                auto roots = SerializationPolicy::parseWhole(PersistencePolicy::getProjectData(), *this);
                if (roots.size() == 0) {
                    throw ManagerStateException("No elements parsed!");
                }

                // first element will be root
                setRoot(roots[0]);
                return roots[0];
            }
            void save(std::string location) {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(*getRoot(), *this), location);
            }
            void save() {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(*getRoot(), *this));
            }

            // std::string dump() {
            //     return SerializationPolicy::emitWhole(*getRoot(), *this);
            // }

            // std::string dump(BaseElement<Tlist>& el) {
            //     return SerializationPolicy::emitWhole(el, *this);
            // }
            // TODO the rest
    };
}

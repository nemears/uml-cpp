#pragma once
#include "uml/managers/baseElement.h"
#include "uml/managers/managerTypes.h"
#include "uml/set/privateSet.h"
#include "uml/umlPtr.h"
#include "uml/managers/abstractManager.h"
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/node/parse.h>
#include "yaml-cpp/yaml.h"
#include "uml/set/abstractSet.h"

namespace UML {
    class SerializationError : public std::exception {
        private:
            std::string m_msg;
        public:
            SerializationError(std::string msg) : m_msg(msg) {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };



    template <class Tlist>
    class UmlCafeSerializationPolicy : virtual public AbstractManager {
        private:
            struct AbstractEmitterFunctor;
            struct AbstractParserFunctor;

            std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor>> m_emitterFunctors;
            std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor>> m_parserfunctors;

            template <class, class>
            struct TupleCat;

            template <class ... Left, class ... Right>
            struct TupleCat<std::tuple<Left...>, std::tuple<Right...>> {
                using type = std::tuple<Left..., Right...>;
            };

            template<std::size_t ... Is>
            struct IntList {};

            template <std::size_t I, typename Ints>
            struct HasType;

            template <std::size_t I, std::size_t J, std::size_t... Is>
            struct HasType<I, IntList<J, Is...>> {
                static const bool value = I == J ? true : HasType<I, IntList<Is...>>::value;
            };

            template<std::size_t I>
            struct HasType<I, IntList<>> {
                static const bool value = false;
            };

            template <class, std::size_t>
            struct IntAppend;

            template <std::size_t ... Is, std::size_t Right>
            struct IntAppend<IntList<Is...>, Right> {
                using type = IntList<Is..., Right>;
            };

            template <class T>
            static std::unique_ptr<std::pair<std::string, AbstractSet*>> findScopeHelper(T& el) {
                for (auto& setPair : T::Info::Info::sets(el)) {
                    if (setPair.second->getComposition() == CompositionType::ANTI_COMPOSITE) {
                        return std::make_unique<std::pair<std::string, AbstractSet*>>(setPair);
                    }
                }
                return 0;
            }

            // TODO compile time visited instead of runtime like below

            template <std::size_t I, class EmitTypes, class Visited = IntList<>>
            static std::unique_ptr<std::pair<std::string, AbstractSet*>> findScope(AbstractElement& el) {
                if constexpr (std::tuple_size<EmitTypes>{} > I) {
                    using CurrentType = std::tuple_element_t<I, EmitTypes>;
                    if constexpr (!HasType<CurrentType::template idOf<CurrentType>(), Visited>::value) {
                        auto possibleScope = findScopeHelper<CurrentType>(dynamic_cast<CurrentType&>(el));
                        using NewVisited = IntAppend<Visited, CurrentType::template idOf<CurrentType>()>::type;
                        auto basePossibleScope = findScope<I + 1, typename TupleCat<EmitTypes, typename CurrentType::Info::BaseList>::type, NewVisited>(el);
                        
                        // compare and choose best option
                        if (!possibleScope || possibleScope->second->size() == 0) {
                            return basePossibleScope;
                        }
                        if (possibleScope->second->readonly() && basePossibleScope && !basePossibleScope->second->readonly() && basePossibleScope->second->size() != 0) {
                            return basePossibleScope;
                        }
                        return possibleScope;
                    }
                    // if it's already visited we move onto the next one
                    return findScope<I + 1, EmitTypes, Visited>(el);
                }
                return 0;
            }

            template <class T>
            static void emitData(YAML::Emitter& emitter, AbstractElement& el) {
                if constexpr (T::Info::extraData) {
                    auto& typedEl = dynamic_cast<T&>(el);
                    for (auto& dataPair : T::Info::Info::data(typedEl)) {
                        auto data = dataPair.second->getData();
                        if (!data.empty()) {
                            emitter << YAML::Key << dataPair.first << YAML::Value << data;
                        }
                    }
                }
            }

            template <class T>
            static void emitIndividualDataHelper(YAML::Emitter& emitter, AbstractElement& el) {
                emitData<T>(emitter, el);
                for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
                    auto set = setPair.second;
                    if (set->readonly() || set->empty() || set->getComposition() == CompositionType::ANTI_COMPOSITE || !set->rootSet()) {
                        continue;
                    }

                    // check if subsets have any of our elements
                    std::size_t numElsInSet = set->size();
                    for (auto id : set->ids()) {
                        auto subSetWithEl = set->subSetContains(id);
                        if (subSetWithEl && !subSetWithEl->readonly()) {
                            numElsInSet--;
                        }
                    }

                    // all in subsets continue
                    if (numElsInSet == 0) {
                        continue;
                    }


                    emitter << YAML::Key << setPair.first;
                    switch (set->setType()) {
                        case SetType::SET:
                        case SetType::ORDERED_SET:
                        {
                            emitter << YAML::BeginSeq;
                            for (auto id : set->ids()) {
                                auto subSetWithEl = set->subSetContains(id);
                                if (subSetWithEl && !subSetWithEl->readonly()) {
                                    continue;
                                }
                                emitter << id.string();
                            }
                            emitter << YAML::EndSeq;
                            break;
                        }
                        case SetType::SINGLETON : {
                            emitter << YAML::Value << set->ids().front().string();
                            break;
                        }
                        default:
                            throw SerializationError("Could not emit, cannot handle set type!");
                    }
                }
            }

            template <class T>
            static void emitWholeDataHelper(YAML::Emitter& emitter, AbstractElement& el, UmlCafeSerializationPolicy& self) {
                emitData<T>(emitter, el);
                for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
                    auto set = setPair.second;
                    if (set->readonly() || set->empty() || !set->rootSet() || set->getComposition() == CompositionType::ANTI_COMPOSITE) {
                        continue;
                    }

                    // check if subsets have any of our elements
                    std::size_t numElsInSet = set->size();
                    for (auto id : set->ids()) {
                        auto subSetWithEl = set->subSetContains(id);
                        if (subSetWithEl && !subSetWithEl->readonly()) {
                            numElsInSet--;
                        }
                    }

                    // all in subsets continue
                    if (numElsInSet == 0) {
                        continue;
                    }

                    emitter << YAML::Key << setPair.first;
                    switch (set->setType()) {
                        case SetType::SET:
                        case SetType::ORDERED_SET:
                        {
                            emitter << YAML::BeginSeq;
                            if (set->getComposition() == CompositionType::NONE) {
                                for (auto id : set->ids()) {
                                    auto subSetWithEl = set->subSetContains(id);
                                    if (subSetWithEl) {
                                        if (!subSetWithEl->readonly()) {
                                            continue;
                                        }
                                    }
                                    emitter << id.string();
                                }
                                
                            } else if (set->getComposition() == CompositionType::COMPOSITE) {
                                for (auto it = set->beginPtr(); *it != *set->endPtr(); it->next()) {
                                    auto child = UmlPtr<BaseElement<Tlist>>(it->getCurr());
                                    auto subSetWithEl = set->subSetContains(child.id());
                                    if (subSetWithEl) {
                                        if (!subSetWithEl->readonly()) {
                                            continue;
                                        }
                                    }
                                    self.m_emitterFunctors.at(child->getElementType())->emitWhole(emitter, *child);
                                }
                            }
                            emitter << YAML::EndSeq;
                            break;
                        }
                        case SetType::SINGLETON : {
                            if (set->getComposition() == CompositionType::NONE) {
                                emitter << YAML::Value << set->ids().front().string();
                            } else if (set->getComposition() == CompositionType::COMPOSITE) {
                                auto child = UmlPtr<BaseElement<Tlist>>(set->beginPtr()->getCurr());
                                self.m_emitterFunctors.at(child->getElementType())->emitWhole(emitter, *child);
                            }
                            break;
                        }
                        default:
                            throw SerializationError("Could not emit, cannot handle set type!");
                    }
                }
            }

            template <std::size_t I = 0, class EmitTypes>
            static void emitIndividualData(YAML::Emitter& emitter, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
                if constexpr (std::tuple_size<EmitTypes>{} > I) {
                    using ElementType = std::tuple_element_t<I, EmitTypes>;
                    // dfs
                    if (!visited.count(ElementType::template idOf<ElementType>())) {
                        visited.insert(ElementType::template idOf<ElementType>());
                        emitIndividualData<0, typename ElementType::Info::BaseList>(emitter, visited, el);
                        emitIndividualData<I + 1, EmitTypes>(emitter, visited, el);
                        emitIndividualDataHelper<ElementType>(emitter, el);
                    }
                }
            }

            template <std::size_t I = 0, class EmitTypes>
            static void emitWholeData(YAML::Emitter& emitter, std::unordered_set<std::size_t>& visited, AbstractElement& el, UmlCafeSerializationPolicy& self) {
                if constexpr (std::tuple_size<EmitTypes>{} > I) {
                    using ElementType = std::tuple_element_t<I, EmitTypes>;
                    // dfs
                    if (!visited.count(ElementType::template idOf<ElementType>())) {
                        visited.insert(ElementType::template idOf<ElementType>());
                        emitWholeData<0, typename ElementType::Info::BaseList>(emitter, visited, el, self);
                        emitWholeData<I + 1, EmitTypes>(emitter, visited, el, self);
                        emitWholeDataHelper<ElementType>(emitter, el, self);
                    }
                }
            }

            struct AbstractEmitterFunctor {
                virtual ~AbstractEmitterFunctor() {}
                virtual std::string operator()(BaseElement<Tlist>& el) = 0;
                virtual void emitWhole(YAML::Emitter& emitter, BaseElement<Tlist>& el) = 0;
            };

            template <class T>
            struct EmitterFunctor : public AbstractEmitterFunctor {
                UmlCafeSerializationPolicy& m_self;
                EmitterFunctor(UmlCafeSerializationPolicy& self) : m_self(self) {}
                std::string operator()(BaseElement<Tlist>& el) override {
                    YAML::Emitter emitter;
                    emitter << YAML::BeginMap;
                    auto possibleScope = findScope<0, std::tuple<T>>(el);
                    if (possibleScope && !possibleScope->second->empty()) {
                        emitter << YAML::Key << possibleScope->first << YAML::Value << possibleScope->second->ids().front().string();
                    }
                    std::unordered_set<std::size_t> visited;
                    std::string elementName = T::Info::Info::name;
                    emitter << YAML::Key << elementName << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
                    emitIndividualData<0, std::tuple<T>>(emitter, visited, el);
                    emitter << YAML::EndMap;
                    emitter << YAML::EndMap;

                    return emitter.c_str();
                }
                void emitWhole(YAML::Emitter& emitter, BaseElement<Tlist>& el) override {
                    std::unordered_set<std::size_t> visited;
                    emitter << YAML::BeginMap << T::Info::Info::name << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
                    emitWholeData<0, std::tuple<T>>(emitter, visited, el, m_self);
                    emitter << YAML::EndMap << YAML::EndMap;    
                }
            };

            struct NodeHandlers {
                YAML::Node innerData;
                YAML::Node outerData;
                AbstractParserFunctor& functor;
            };

            NodeHandlers getFunctor(const YAML::Node& node) {
                auto it = node.begin();
                while (it != node.end()) {
                    const auto& keyNode = it->first;
                    const auto& valNode = it->second;
                    if (valNode.IsMap()) {
                        // look up key
                        try {
                            return NodeHandlers {
                                valNode,
                                node,
                                *m_parserfunctors.at(keyNode.as<std::string>())
                            };
                        } catch (std::exception& e) {
                            throw SerializationError("Could not find proper type to parse! line number " + std::to_string(keyNode.Mark().line));
                        }
                    }
                    it++;
                } 
                throw SerializationError("Could not identify type to parse relevant to this manager!");
            }

            std::unordered_map<AbstractReadableSet*, ID> m_setsToRunPolicies;

            void addByID(AbstractReadableSet& set, ID id) {
                set.add(id);
                m_setsToRunPolicies[&set] = id;
            }

            template <class T>
            void parseData(T& el, const YAML::Node& node) {
                if constexpr (T::Info::extraData) {
                    for (auto& dataPair : T::Info::data(el)) {
                        if (node[dataPair.first]) {
                            dataPair.second->setData(node[dataPair.first].template as<std::string>());
                        }
                    }
                }
            }

            template <class T>
            void parseIndividualHelper(T& el, const YAML::Node& node) {
                parseData<T>(el, node);
                for (auto& setPair : T::Info::sets(dynamic_cast<T&>(el))) {
                    if (setPair.second->readonly() || !setPair.second->rootSet()) {
                        continue;
                    }
                    auto set = dynamic_cast<AbstractReadableSet*>(setPair.second);
                    if (node[setPair.first]) {
                        auto& setNode = node[setPair.first];
                        if (setNode.IsScalar()) {
                            if (set->setType() != SetType::SINGLETON) {
                                throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
                            }
                            // TODO if parsing compositely, check to make sure the set is not composite
                            set->add(ID::fromString(setNode.template as<std::string>()));
                        } else if (setNode.IsSequence()) {
                            for (const auto& valNode : setNode) {
                                set->add(ID::fromString(valNode.template as<std::string>()));
                            }
                        } else {
                            throw SetStateException("Invalid set formatting for individual parsing! line number " + std::to_string(setNode.Mark().line));
                        }
                    }
                }
            }

            template <class T>
            bool parseScopeHelper(const YAML::Node& node, T& el) {
                for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
                    if (setPair.second->readonly()) {
                        continue;
                    }
                    auto set = dynamic_cast<AbstractReadableSet*>(setPair.second);
                    if (set->getComposition() != CompositionType::ANTI_COMPOSITE || !set->rootSet()) {
                        continue;
                    }
                    if (node[setPair.first]) {
                        auto& setNode = node[setPair.first];
                        if (setNode.IsScalar()) {
                            if (set->setType() != SetType::SINGLETON) {
                                throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
                            }
                            // TODO if parsing compositely, check to make sure the set is not composite
                            set->add(ID::fromString(setNode.template as<std::string>()));
                            return true;
                        }
                    }
                }
                return false;
            }

            template <class T>
            void parseWholeHelper(T& el, const YAML::Node& node) {
                parseData<T>(el, node);
                for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
                    if (setPair.second->readonly() || !setPair.second->rootSet()) {
                        continue;
                    }
                    auto set = dynamic_cast<AbstractReadableSet*>(setPair.second);
                    if (node[setPair.first]) {
                        auto& setNode = node[setPair.first];
                        if (setNode.IsScalar()) {
                            if (set->setType() != SetType::SINGLETON) {
                                throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
                            }
                            // TODO if parsing compositely, check to make sure the set is not composite
                            // TODO ensure policy is always ran?
                            this->addByID(*set, ID::fromString(setNode.template as<std::string>()));
                            // set->add(ID::fromString(setNode.template as<std::string>()));
                        } else if (setNode.IsSequence()) {
                            for (const auto& valNode : setNode) {
                                if (valNode.IsMap()) {
                                    // composite parsing
                                    auto match = this->getFunctor(valNode);
                                    auto parsedChild = match.functor.parseWhole(match.innerData);
                                    this->addToSet(*set, *parsedChild);
                                } else if (valNode.IsScalar()) {
                                    // TODO run policy
                                    this->addByID(*set, ID::fromString(valNode.template as<std::string>()));
                                    // set->add(ID::fromString(valNode.template as<std::string>()));
                                } else {
                                    throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(valNode.Mark().line));
                                }
                            }
                        } else {
                            if (set->setType() != SetType::SINGLETON) {
                                throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
                            }
                            auto match = this->getFunctor(setNode);
                            auto parsedChild = match.functor.parseWhole(match.innerData);
                            this->addToSet(*set, *parsedChild);
                        }
                    }
                }
            }

            template <std::size_t I, class ParseTypes>
            void parseIndividual(std::tuple_element_t<I, ParseTypes>& el, const YAML::Node& node, std::unordered_set<std::size_t>& visited) {
                using ElementType = std::tuple_element_t<I, ParseTypes>;
                if (!visited.count(ElementType::template idOf<ElementType>())) {
                    visited.insert(ElementType::template idOf<ElementType>());
                    parseIndividualHelper<ElementType>(el, node);
                    if constexpr (std::tuple_size<ParseTypes>{} > I + 1) {
                        parseIndividual<I + 1, ParseTypes>(dynamic_cast<std::tuple_element_t<I + 1, ParseTypes>&>(el), node, visited);
                    }
                    using ElBases = ElementType::Info::BaseList;
                    if constexpr (std::tuple_size<ElBases>{} > 0) {
                        parseIndividual<0, ElBases>(el, node, visited);
                    }
                }
            }

            template <std::size_t I, class ParseTypes>
            void parseScope(const YAML::Node& node, std::tuple_element_t<I, ParseTypes>& el) {
                using ElementType = std::tuple_element_t<I, ParseTypes>;
                if (parseScopeHelper<ElementType>(node, el)) {
                    return;
                }
                if constexpr (std::tuple_size<ParseTypes>{} > I + 1) {
                    parseScope<I + 1, ParseTypes>(node, el.template as<std::tuple_element_t<I + 1, ParseTypes>>());
                }
                using ElBases = ElementType::Info::BaseList;
                if constexpr (std::tuple_size<ElBases>{} > 0) {
                    parseScope<0, ElBases>(node, el);
                }
            }
            
            template <std::size_t I, class ParseTypes>
            void parseWhole(std::tuple_element_t<I, ParseTypes>& el, const YAML::Node& node, std::unordered_set<std::size_t>& visited) {
                using ElementType = std::tuple_element_t<I, ParseTypes>;
                if (!visited.count(ElementType::template idOf<ElementType>())) {
                    visited.insert(ElementType::template idOf<ElementType>());
                    parseWholeHelper<ElementType>(el, node);
                    if constexpr (std::tuple_size<ParseTypes>{} > I + 1) {
                        parseWhole<I + 1, ParseTypes>(dynamic_cast<std::tuple_element_t<I + 1, ParseTypes>&>(el), node, visited);
                    }
                    using ElBases = ElementType::Info::BaseList;
                    if constexpr (std::tuple_size<ElBases>{} > 0) {
                        parseWhole<0, ElBases>(el, node, visited);
                    }
                }
            }

            struct AbstractParserFunctor {
                UmlCafeSerializationPolicy& m_self;
                AbstractParserFunctor(UmlCafeSerializationPolicy& self) : m_self(self) {}
                virtual ~AbstractParserFunctor() {}
                virtual UmlPtr<BaseElement<Tlist>> operator()(const YAML::Node node) = 0; // rename?
                virtual void parseScope(YAML::Node node, BaseElement<Tlist>& el) = 0;
                virtual UmlPtr<BaseElement<Tlist>> parseWhole(const YAML::Node node) = 0;
            };

            template <class T>
            struct ParserFunctor : public AbstractParserFunctor {
                typedef T Type;
                ParserFunctor(UmlCafeSerializationPolicy& self) : AbstractParserFunctor(self) {}
                UmlPtr<BaseElement<Tlist>> operator()(const YAML::Node node) override {
                    UmlPtr<T> ret = this->m_self.create(ManagerTypes<Tlist>::template idOf<T>());
                    if (node["id"]) {
                        ret->setID(ID::fromString(node["id"].as<std::string>()));
                    }
                    std::unordered_set<std::size_t> visited;
                    this->m_self.template parseIndividual<0, std::tuple<T>>(*ret, node, visited);

                    return ret;
                }
                void parseScope(YAML::Node node, BaseElement<Tlist>& el) override {
                    this->m_self.template parseScope<0, std::tuple<T>>(node, el.template as<T>());
                }
                UmlPtr<BaseElement<Tlist>> parseWhole(const YAML::Node node) {
                    UmlPtr<T> ret = this->m_self.create(T::Info::elementType);
                    if (node["id"]) {
                        if (!node["id"].IsScalar()) {
                            throw SerializationError("Improper ID formatting, should be a Scalar value! line number " + std::to_string(node["id"].Mark().line));
                        }
                        ret->setID(ID::fromString(node["id"].as<std::string>()));
                    }
                    std::unordered_set<std::size_t> visited;
                    this->m_self.template parseWhole<0, std::tuple<T>>(*ret, node, visited);
                    return ret;
                }
            };

            template <std::size_t I = 0>
            void populateEmitterFunctors(std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor>>& functors) {
                if constexpr (std::tuple_size<Tlist>{} > I) {
                    functors.emplace(I, std::make_unique<EmitterFunctor<std::tuple_element_t<I, Tlist>>>(*this));
                    populateEmitterFunctors<I + 1>(functors);
                }
            }

            template <std::size_t I = 0>
            void populateParserFunctors(std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor>>& functors) {
                if constexpr (std::tuple_size<Tlist>{} > I) {
                    using ElementType = std::tuple_element_t<I, Tlist>;
                    if constexpr (!ElementType::Info::Info::abstract) {
                        functors.emplace(ElementType::Info::Info::name, std::make_unique<ParserFunctor<ElementType>>(*this));
                    }
                    populateParserFunctors<I + 1>(functors);
                }
            }
        protected:
            UmlCafeSerializationPolicy() {
                populateEmitterFunctors<0>(m_emitterFunctors);
                populateParserFunctors<0>(m_parserfunctors);
            }
            AbstractElementPtr parseIndividual(std::string data, __attribute__((unused)) AbstractManager& manager) {
                std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
                if (rootNodes.empty()) {
                    throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
                }
                auto match = getFunctor(rootNodes[0]);
                auto parsedEl = match.functor(match.innerData);
                match.functor.parseScope(match.outerData, *parsedEl);
                return parsedEl;
            }
            std::vector<UmlPtr<BaseElement<Tlist>>> parseWhole(std::string data, __attribute__((unused)) AbstractManager& manager) {
                std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
                if (rootNodes.empty()) {
                    throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
                }
                std::vector<UmlPtr<BaseElement<Tlist>>> ret(rootNodes.size());
                auto i = 0;
                for (auto& node : rootNodes) {
                    auto match = getFunctor(node);
                    ret[i] = match.functor.parseWhole(match.innerData);
                    i++;
                }
                for (auto& pair : m_setsToRunPolicies) {
                    auto& set = *pair.first;
                    auto el = this->abstractGet(pair.second);
                    this->runAllAddPolicies(set, *el);
                }
                m_setsToRunPolicies.clear();
                return ret;
            }
            std::string emitIndividual(BaseElement<Tlist>& el, __attribute__((unused)) AbstractManager& manager) {
                return (*m_emitterFunctors.at(el.getElementType()))(el);
            }
            std::string emitWhole(AbstractElement& el, __attribute__((unused)) AbstractManager& manager) {
                YAML::Emitter emitter;
                // emitter << YAML::BeginDoc;
                m_emitterFunctors.at(el.getElementType())->emitWhole(emitter, dynamic_cast<BaseElement<Tlist>&>(el));
                // emitter << YAML::EndDoc;
                return emitter.c_str();
            }
        public:
            std::string dump() {
                return  this->emitWhole(*getAbstractRoot(), *this);
            }
            std::string dump(BaseElement<Tlist>& el) {
                return this->emitWhole(el, *this);
            }
    };

    // TODO
    // class UmlCafeJsonSerializationPolicy {
    //     protected:
    //         AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager);
    //         AbstractElementPtr parseWhole(std::string data, AbstractManager& manager);
    //         std::string emitIndividual(AbstractElement& el, AbstractManager& manager);
    //         std::string emitWhole(AbstractElement& el, AbstractManager& manager);
    // };

}

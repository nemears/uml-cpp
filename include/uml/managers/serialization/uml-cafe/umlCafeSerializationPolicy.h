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

    template <class T>
    std::unique_ptr<std::pair<std::string, AbstractSet*>> findScopeHelper(T& el) {
        for (auto& setPair : T::Info::Info::sets(el)) {
            if (setPair.second->getComposition() == CompositionType::ANTI_COMPOSITE) {
                return std::make_unique<std::pair<std::string, AbstractSet*>>(setPair);
            }
        }
        return 0;
    }

    template <std::size_t I, class Tlist>
    std::unique_ptr<std::pair<std::string, AbstractSet*>> findScope(AbstractElement& el) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            using CurrentType = std::tuple_element_t<I, Tlist>; 
            auto possibleScope = findScopeHelper<CurrentType>(dynamic_cast<CurrentType&>(el));
            if (possibleScope) {
                return possibleScope;
            }
            possibleScope = findScope<I + 1, Tlist>(el);
            if (possibleScope) {
                return possibleScope;
            }
            return findScope<0, typename CurrentType::Info::BaseList>(el);
            
        }
        return 0;
    }

    template <class T>
    void emitIndividualDataHelper(YAML::Emitter& emitter, AbstractElement& el) {

        // TODO other features besides sets
        
        for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
            auto set = setPair.second;
            if (set->readonly() || set->empty()) {
                continue;
            }
            emitter << YAML::Key << setPair.first;
            switch (set->setType()) {
                case SetType::SET:
                case SetType::ORDERED_SET:
                {
                    emitter << YAML::BeginSeq;
                    for (auto id : set->ids()) {
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

    template <std::size_t I = 0, class Tlist>
    void emitIndividualData(YAML::Emitter& emitter, std::unordered_set<std::size_t> visited, AbstractElement& el) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            using ElementType = std::tuple_element_t<I, Tlist>;
            // dfs
            if (!visited.count(ElementType::template idOf<ElementType>())) {
                visited.insert(ElementType::template idOf<ElementType>());
                emitIndividualData<0, typename ElementType::Info::BaseList>(emitter, visited, el);
                emitIndividualData<I + 1, Tlist>(emitter, visited, el);
                emitIndividualDataHelper<ElementType>(emitter, el);
            }
        }
    }

    template <class Tlist>
    struct AbstractEmitterFunctor {
        virtual std::string operator()(BaseElement<Tlist>& el) = 0;
    };

    template <class T, class Tlist>
    struct EmitterFunctor : public AbstractEmitterFunctor<Tlist> {
        std::string operator()(BaseElement<Tlist>& el) override {
            T& typedEl = el.template as<T>();
            YAML::Emitter emitter;
            emitter << YAML::BeginMap;
            std::string elementName = T::Info::Info::name;
            auto possibleScope = findScope<0, std::tuple<T>>(el);
            if (possibleScope && !possibleScope->second->empty()) {
                emitter << YAML::Key << possibleScope->first << YAML::Value << possibleScope->second->ids().front().string();
            }
            std::unordered_set<std::size_t> visited;
            emitter << YAML::Key << elementName << YAML::Value << YAML::BeginMap;
            emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
            emitIndividualData<0, std::tuple<T>>(emitter, visited, el);
            emitter << YAML::EndMap;
            emitter << YAML::EndMap;

            return emitter.c_str();
        }
    };

    template <class T>
    void parseIndividualHelper(T& el, const YAML::Node& node) {
        for (auto& setPair : T::Info::Info::sets(dynamic_cast<T&>(el))) {
            auto set = setPair.second;
            if (set->readonly()) {
                continue;
            }
            if (node[setPair.first]) {
                auto& setNode = node[setPair.first];
                if (setNode.IsScalar()) {
                    if (set->setType() != SetType::SINGLETON) {
                        throw SerializationError("Error while parsing: Singleton node is not a scalar value!");
                    }
                    set->add(ID::fromString(setNode.template as<std::string>()));
                } else if (setNode.IsSequence()) {
                    for (const auto& valNode : setNode) {
                        set->add(ID::fromString(valNode.template as<std::string>()));
                    }
                } else {
                    throw SetStateException("Invalid set formatting for individual parsing!");
                }
            }
        } 
    }

    template <std::size_t I, class Tlist>
    void parseIndividual(std::tuple_element_t<I, Tlist>& el, const YAML::Node& node) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            using ElementType = std::tuple_element_t<I, Tlist>;
            parseIndividualHelper<ElementType>(el, node);
            parseIndividualHelper<I + 1, Tlist>(el, node);
            parseIndividualHelper<0, ElementType::Info::BaseList>(el, node);
        }
    }

    template <class Tlist>
    struct AbstractParserFunctor {
        virtual UmlPtr<BaseElement<Tlist>> operator()(const YAML::Node& node, AbstractManager& manager) = 0;
    };

    template <class T, class Tlist>
    struct ParserFunctor : public AbstractParserFunctor<Tlist> {
        UmlPtr<BaseElement<Tlist>> operator()(const YAML::Node& node, AbstractManager& manager) {
            UmlPtr<T> ret = manager.create(ManagerTypes<Tlist>::template idOf<T>());
            if (node["id"]) {
                ret->setID(ID::fromString(node["id"].as<std::string>()));
            }

            // TODO parse the rest
            //

            return ret;
        }
    };

    template <std::size_t I = 0, class Tlist>
    void populateEmitterFunctors(std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor<Tlist>>>& functors) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            functors.emplace(I, std::make_unique<EmitterFunctor<std::tuple_element_t<I, Tlist>, Tlist>>());
            populateEmitterFunctors<I + 1, Tlist>(functors);
        }
    }

    template <std::size_t I = 0, class Tlist>
    void populateParserFunctors(std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor<Tlist>>>& functors) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            using ElementType = std::tuple_element_t<I, Tlist>;
            if constexpr (!ElementType::Info::Info::abstract) {
                functors.emplace(ElementType::Info::Info::name, std::make_unique<ParserFunctor<ElementType, Tlist>>());
            }
            populateParserFunctors<I + 1, Tlist>(functors);
        }
    }

    template <class Tlist>
    class UmlCafeSerializationPolicy {
        private:
            std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor<Tlist>>> m_emitterFunctors;
            std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor<Tlist>>> m_parserfunctors;
        protected:
            UmlCafeSerializationPolicy() {
                populateEmitterFunctors<0, Tlist>(m_emitterFunctors);
                populateParserFunctors<0, Tlist>(m_parserfunctors);
            }
            AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager) {
                std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
                if (rootNodes.empty()) {
                    throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
                }
                auto it = rootNodes[0].begin();
                while (it != rootNodes[0].end()) {
                    const auto& keyNode = it->first;
                    const auto& node = it->second;
                    if (node.IsMap()) {
                        // look up key
                        return (*m_parserfunctors.at(keyNode.as<std::string>()))(node, manager);
                    }
                }
                throw SerializationError("Could not identify type to parse releant to this manager!");
            }
            AbstractElementPtr parseWhole(std::string data, AbstractManager& manager) {
                throw ManagerStateException("TODO");
            }
            std::string emitIndividual(AbstractElement& el, __attribute__((unused)) AbstractManager& manager) {
                return (*m_emitterFunctors.at(el.getElementType()))(dynamic_cast<BaseElement<Tlist>&>(el));
            }
            std::string emitWhole(AbstractElement& el, AbstractManager& manager) {
                throw ManagerStateException("TODO");
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

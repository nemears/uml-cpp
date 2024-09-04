#pragma once
#include "uml/managers/baseElement.h"
#include "uml/umlPtr.h"
#include "uml/managers/abstractManager.h"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <yaml-cpp/emittermanip.h>
#include "yaml-cpp/yaml.h"
#include "uml/set/abstractSet.h"

namespace UML {

    enum class SerializationMode {
        INDIVIDUAL,
        WHOLE
    };

    class EmitterData {
        public:
            SerializationMode mode = SerializationMode::INDIVIDUAL;
            bool isJSON = false;
    };

    class ParserData {
        public:
            SerializationMode mode = SerializationMode::INDIVIDUAL;
            AbstractManager* manager = 0;
            bool update = false; // update flag
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
                    throw ManagerStateException("Could not emit, cannot handle set type!");
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
    struct AbstractSerializationFunctor {
        virtual std::string emitIndividual(BaseElement<Tlist>& el) = 0;
    };

    template <class T, class Tlist>
    struct SerializationFunctor : public AbstractSerializationFunctor<Tlist> {
        std::string emitIndividual(BaseElement<Tlist>& el) override {
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
        // UmlPtr<T> parseIndividual(std::string& data, AbstractManager& m) {
        //     std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
        //     if (rootNodes.empty()) {
        //         throw ManagerStateException("could not parse data supplied to manager! Is it JSON or YAML?");
        //     }
        //     
        //     auto ret = m.create(T::template idOf<T>());
        //     
        // }
    };

    template <std::size_t I = 0, class Tlist>
    void populateSerializationFunctors(std::unordered_map<std::size_t, std::unique_ptr<AbstractSerializationFunctor<Tlist>>>& functors) {
        if constexpr (std::tuple_size<Tlist>{} > I) {
            functors.emplace(I, std::make_unique<SerializationFunctor<std::tuple_element_t<I, Tlist>, Tlist>>());
            populateSerializationFunctors<I + 1, Tlist>(functors);
        }
    }

    template <class Tlist>
    class UmlCafeSerializationPolicy {
        private:
            std::unordered_map<std::size_t, std::unique_ptr<AbstractSerializationFunctor<Tlist>>> m_serializationFunctors;
        protected:
            UmlCafeSerializationPolicy() {
                populateSerializationFunctors<0, Tlist>(m_serializationFunctors);
            }
            AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager) {
                throw ManagerStateException("TODO");
            }
            AbstractElementPtr parseWhole(std::string data, AbstractManager& manager) {
                throw ManagerStateException("TODO");
            }
            std::string emitIndividual(AbstractElement& el, __attribute__((unused)) AbstractManager& manager) {
                return m_serializationFunctors.at(el.getElementType())->emitIndividual(dynamic_cast<BaseElement<Tlist>&>(el));
            }
            std::string emitWhole(AbstractElement& el, AbstractManager& manager) {
                throw ManagerStateException("TODO");
            }
    };

    class UmlCafeJsonSerializationPolicy {
        protected:
            std::string extensionName();
            AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager);
            AbstractElementPtr parseWhole(std::string data, AbstractManager& manager);
            std::string emitIndividual(AbstractElement& el, AbstractManager& manager);
            std::string emitWhole(AbstractElement& el, AbstractManager& manager);
    };
    class EmitterData;
    class ParserData;

    std::string emit(AbstractElement& el, EmitterData& data);
    AbstractElementPtr parse(std::string data, ParserData& metaData);

    class SerializationError : public std::exception {
        private:
            std::string m_msg;
        public:
            SerializationError(std::string msg) : m_msg(msg) {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };
}

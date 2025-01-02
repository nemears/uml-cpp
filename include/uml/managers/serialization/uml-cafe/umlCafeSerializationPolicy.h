#pragma once
#include "uml/managers/abstractElement.h"
#include "uml/managers/managerTypes.h"
#include "uml/set/privateSet.h"
#include "uml/managers/umlPtr.h"
#include "uml/managers/abstractManager.h"
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/node/parse.h>
#include "yaml-cpp/yaml.h"
#include "uml/set/abstractSet.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"

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
        // private:
        //     struct AbstractEmitterFunctor;
        //     struct AbstractParserFunctor;

        //     std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor>> m_emitterFunctors;
        //     std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor>> m_parserfunctors;

        //     template<std::size_t ... Is>
        //     struct IntList {};

        //     template <std::size_t I, typename Ints>
        //     struct HasType;

        //     template <std::size_t I, std::size_t J, std::size_t... Is>
        //     struct HasType<I, IntList<J, Is...>> {
        //         static const bool value = I == J ? true : HasType<I, IntList<Is...>>::value;
        //     };

        //     template<std::size_t I>
        //     struct HasType<I, IntList<>> {
        //         static const bool value = false;
        //     };

        //     template <class, std::size_t>
        //     struct IntAppend;

        //     template <std::size_t ... Is, std::size_t Right>
        //     struct IntAppend<IntList<Is...>, Right> {
        //         using type = IntList<Is..., Right>;
        //     };

        //     template <class T>
        //     static std::unique_ptr<std::pair<std::string, AbstractSet*>> findScopeHelper(T& el) {
        //         for (auto& setPair : T::Info::sets(el)) {
        //             if (setPair.second->getComposition() == CompositionType::ANTI_COMPOSITE && setPair.second->size() > 0) {
        //                 return std::make_unique<std::pair<std::string, AbstractSet*>>(setPair);
        //             }
        //         }
        //         return 0;
        //     }

        //     // TODO compile time visited instead of runtime like below

        //     template <std::size_t I, class EmitTypes, class Visited = IntList<>>
        //     static std::unique_ptr<std::pair<std::string, AbstractSet*>> findScope(AbstractElement& el) {
        //         if constexpr (TemplateTypeListSize<EmitTypes>::result > I) {
        //             // using CurrentType = std::tuple_element_t<I, EmitTypes>;
        //             const std::size_t typeID = TemplateTypeListIndex<TemplateTypeListType<I, EmitTypes>::template result, Tlist>::result;
        //             if constexpr (!HasType<typeID, Visited>::value) {
        //                 using CurrentType = TemplateTypeListType<I, EmitTypes>::template result<ComparableElement<Tlist>>;
        //                 auto possibleScope = findScopeHelper<CurrentType>(dynamic_cast<CurrentType&>(el));
        //                 using NewVisited = typename IntAppend<Visited, typeID>::type;
        //                 auto basePossibleScope = findScope<I + 1, typename TemplateTypeListCat<EmitTypes, typename CurrentType::Info::BaseList>::result, NewVisited>(el);
        //                 
        //                 // compare and choose best option
        //                 if (!possibleScope || possibleScope->second->size() == 0) {
        //                     return basePossibleScope;
        //                 }
        //                 if (basePossibleScope && basePossibleScope->second->size() != 0) {
        //                     if (possibleScope->second->readonly() && !basePossibleScope->second->readonly()) {
        //                         return basePossibleScope;
        //                     }
        //                     if (basePossibleScope->second->isSubSetOf(*possibleScope->second)) {
        //                         return basePossibleScope;
        //                     }
        //                 }
        //                 return possibleScope;
        //             }
        //             // if it's already visited we move onto the next one
        //             return findScope<I + 1, EmitTypes, Visited>(el);
        //         }
        //         return 0;
        //     }

        //     template <template <class> class T>
        //     static void emitData(YAML::Emitter& emitter, AbstractElement& el) {
        //         if constexpr (T<ComparableElement<Tlist>>::Info::extraData) {
        //             auto& typedEl = dynamic_cast<T<ComparableElement<Tlist>>&>(el);
        //             for (auto& dataPair : T<ComparableElement<Tlist>>::Info::data(typedEl)) {
        //                 auto data = dataPair.second->getData();
        //                 if (!data.empty()) {
        //                     emitter << YAML::Key << dataPair.first << YAML::Value << data;
        //                 }
        //             }
        //         }
        //     }

        //     template <template <class> class T>
        //     static void emitIndividualDataHelper(YAML::Emitter& emitter, AbstractElement& el) {
        //         emitData<T>(emitter, el);
        //         for (auto& setPair : T<ComparableElement<Tlist>>::Info::sets(dynamic_cast<T<ComparableElement<Tlist>>&>(el))) {
        //             auto set = setPair.second;
        //             if (/*set->readonly() ||*/ set->empty() || set->getComposition() == CompositionType::ANTI_COMPOSITE || !set->rootSet()) {
        //                 continue;
        //             }

        //             // check if subsets have any of our elements
        //             std::size_t numElsInSet = set->size();
        //             for (auto id : set->ids()) {
        //                 auto subSetWithEl = set->subSetContains(id);
        //                 if (subSetWithEl /*&& !subSetWithEl->readonly()*/) {
        //                     numElsInSet--;
        //                 }
        //             }

        //             // all in subsets continue
        //             if (numElsInSet == 0) {
        //                 continue;
        //             }


        //             emitter << YAML::Key << setPair.first;
        //             switch (set->setType()) {
        //                 case SetType::SET:
        //                 case SetType::ORDERED_SET:
        //                 {
        //                     emitter << YAML::BeginSeq;
        //                     for (auto id : set->ids()) {
        //                         auto subSetWithEl = set->subSetContains(id);
        //                         if (subSetWithEl && !subSetWithEl->readonly()) {
        //                             continue;
        //                         }
        //                         emitter << id.string();
        //                     }
        //                     emitter << YAML::EndSeq;
        //                     break;
        //                 }
        //                 case SetType::SINGLETON : {
        //                     emitter << YAML::Value << set->ids().front().string();
        //                     break;
        //                 }
        //                 default:
        //                     throw SerializationError("Could not emit, cannot handle set type!");
        //             }
        //         }
        //     }

        //     template <template <class> class T>
        //     static void emitWholeDataHelper(YAML::Emitter& emitter, AbstractElement& el, UmlCafeSerializationPolicy& self) {
        //         emitData<T>(emitter, el);
        //         for (auto& setPair : T<ComparableElement<Tlist>>::Info::sets(dynamic_cast<T<ComparableElement<Tlist>>&>(el))) {
        //             auto set = setPair.second;
        //             if (set->readonly() || set->empty() || !set->rootSet() || set->getComposition() == CompositionType::ANTI_COMPOSITE) {
        //                 continue;
        //             }

        //             // check if subsets have any of our elements
        //             std::size_t numElsInSet = set->size();
        //             for (auto id : set->ids()) {
        //                 auto subSetWithEl = set->subSetContains(id);
        //                 if (subSetWithEl && !subSetWithEl->readonly()) {
        //                     numElsInSet--;
        //                 }
        //             }

        //             // all in subsets continue
        //             if (numElsInSet == 0) {
        //                 continue;
        //             }

        //             emitter << YAML::Key << setPair.first;
        //             switch (set->setType()) {
        //                 case SetType::SET:
        //                 case SetType::ORDERED_SET:
        //                 {
        //                     emitter << YAML::BeginSeq;
        //                     if (set->getComposition() == CompositionType::NONE) {
        //                         for (auto id : set->ids()) {
        //                             auto subSetWithEl = set->subSetContains(id);
        //                             if (subSetWithEl) {
        //                                 if (!subSetWithEl->readonly()) {
        //                                     continue;
        //                                 }
        //                             }
        //                             emitter << id.string();
        //                         }
        //                         
        //                     } else if (set->getComposition() == CompositionType::COMPOSITE) {
        //                         for (auto it = set->beginPtr(); *it != *set->endPtr(); it->next()) {
        //                             auto child = UmlPtr<AbstractElement>(it->getCurr());
        //                             auto subSetWithEl = set->subSetContains(child.id());
        //                             if (subSetWithEl) {
        //                                 if (!subSetWithEl->readonly()) {
        //                                     continue;
        //                                 }
        //                             }
        //                             self.m_emitterFunctors.at(child->getElementType())->emitWhole(emitter, *child);
        //                         }
        //                     }
        //                     emitter << YAML::EndSeq;
        //                     break;
        //                 }
        //                 case SetType::SINGLETON : {
        //                     if (set->getComposition() == CompositionType::NONE) {
        //                         emitter << YAML::Value << set->ids().front().string();
        //                     } else if (set->getComposition() == CompositionType::COMPOSITE) {
        //                         auto child = UmlPtr<AbstractElement>(set->beginPtr()->getCurr());
        //                         self.m_emitterFunctors.at(child->getElementType())->emitWhole(emitter, *child);
        //                     }
        //                     break;
        //                 }
        //                 default:
        //                     throw SerializationError("Could not emit, cannot handle set type!");
        //             }
        //         }
        //     }

        //     template <std::size_t I = 0, class EmitTypes>
        //     static void emitIndividualData(YAML::Emitter& emitter, std::unordered_set<std::size_t>& visited, AbstractElement& el) {
        //         if constexpr (TemplateTypeListSize<EmitTypes>::result > I) {
        //             const std::size_t typeID = TemplateTypeListIndex<TemplateTypeListType<I, EmitTypes>::template result, Tlist>::result;
        //             // dfs
        //             if (!visited.count(typeID)) {
        //                 visited.insert(typeID);
        //                 using ElementType = TemplateTypeListType<I, EmitTypes>::template result<ComparableElement<Tlist>>;
        //                 emitIndividualData<0, typename ElementType::Info::BaseList>(emitter, visited, el);
        //                 emitIndividualData<I + 1, EmitTypes>(emitter, visited, el);
        //                 emitIndividualDataHelper<TemplateTypeListType<I, EmitTypes>::template result>(emitter, el);
        //             } else {
        //                 emitIndividualData<I + 1, EmitTypes>(emitter, visited, el);
        //             }
        //         }
        //     }

        //     template <std::size_t I = 0, class EmitTypes>
        //     static void emitWholeData(YAML::Emitter& emitter, std::unordered_set<std::size_t>& visited, AbstractElement& el, UmlCafeSerializationPolicy& self) {
        //         if constexpr (TemplateTypeListSize<EmitTypes>::result > I) {
        //             // dfs
        //             const std::size_t typeID = TemplateTypeListIndex<TemplateTypeListType<I, EmitTypes>::template result, Tlist>::result;
        //             if (!visited.count(typeID)) {
        //                 visited.insert(typeID);
        //                 using ElementType = TemplateTypeListType<I, EmitTypes>::template result<ComparableElement<Tlist>>;
        //                 emitWholeData<0, typename ElementType::Info::BaseList>(emitter, visited, el, self);
        //                 emitWholeData<I + 1, EmitTypes>(emitter, visited, el, self);
        //                 emitWholeDataHelper<TemplateTypeListType<I, EmitTypes>::template result>(emitter, el, self);
        //             } else {
        //                 emitWholeData<I + 1, EmitTypes>(emitter, visited, el, self);
        //             }
        //         }
        //     }

        //     struct AbstractEmitterFunctor {
        //         virtual ~AbstractEmitterFunctor() {}
        //         virtual std::string operator()(ComparableElement<Tlist>& el) = 0;
        //         virtual void emitWhole(YAML::Emitter& emitter, ComparableElement<Tlist>& el) = 0;
        //     };

        //     template <template <class> class T, class ManagerPolicy>
        //     struct EmitterFunctor : public AbstractEmitterFunctor {
        //         UmlCafeSerializationPolicy& m_self;
        //         EmitterFunctor(UmlCafeSerializationPolicy& self) : m_self(self) {}
        //         std::string operator()(ComparableElement<Tlist>& el) override {
        //             YAML::Emitter emitter;
        //             m_self.primeEmitter(emitter);
        //             emitter << YAML::BeginMap;
        //             auto possibleScope = findScope<0, TemplateTypeList<T>>(el);
        //             if (possibleScope && !possibleScope->second->empty()) {
        //                 emitter << YAML::Key << possibleScope->first << YAML::Value << possibleScope->second->ids().front().string();
        //             }
        //             std::unordered_set<std::size_t> visited;
        //             std::string elementName = std::string(T<ComparableElement<Tlist>>::Info::name(dynamic_cast<T<ManagerPolicy>&>(el)));
        //             emitter << YAML::Key << elementName << YAML::Value << YAML::BeginMap;
        //             emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
        //             emitIndividualData<0, TemplateTypeList<T>>(emitter, visited, el);
        //             emitter << YAML::EndMap;
        //             emitter << YAML::EndMap;

        //             return emitter.c_str();
        //         }
        //         void emitWhole(YAML::Emitter& emitter, ComparableElement<Tlist>& el) override {
        //             std::unordered_set<std::size_t> visited;
        //             emitter << YAML::BeginMap << T<ComparableElement<Tlist>>::Info::name(dynamic_cast<T<ManagerPolicy>&>(el)) << YAML::Value << YAML::BeginMap;
        //             emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
        //             emitWholeData<0, TemplateTypeList<T>>(emitter, visited, el, m_self);
        //             emitter << YAML::EndMap << YAML::EndMap;    
        //         }
        //     };

        //     struct NodeHandlers {
        //         YAML::Node innerData;
        //         YAML::Node outerData;
        //         AbstractParserFunctor& functor;
        //     };

        //     NodeHandlers getFunctor(const YAML::Node& node) {
        //         auto it = node.begin();
        //         while (it != node.end()) {
        //             const auto& keyNode = it->first;
        //             const auto& valNode = it->second;
        //             if (valNode.IsMap()) {
        //                 // look up key
        //                 try {
        //                     return NodeHandlers {
        //                         valNode,
        //                         node,
        //                         *m_parserfunctors.at(keyNode.as<std::string>())
        //                     };
        //                 } catch (std::exception& e) {
        //                     throw SerializationError("Could not find proper type to parse! line number " + std::to_string(keyNode.Mark().line));
        //                 }
        //             }
        //             it++;
        //         } 
        //         throw SerializationError("Could not identify type to parse relevant to this manager!");
        //     }

        //     std::unordered_map<AbstractSet*, ID> m_setsToRunPolicies;

        //     void addByID(AbstractSet& set, ID id) {
        //         addToSet(set, id);
        //         m_setsToRunPolicies[&set] = id;
        //     }

        //     template <template <class> class T, class ManagerPolicy>
        //     void parseData(T<ManagerPolicy>& el, const YAML::Node& node) {
        //         if constexpr (T<ManagerPolicy>::Info::extraData) {
        //             for (auto& dataPair : T<ManagerPolicy>::Info::data(el)) {
        //                 if (node[dataPair.first]) {
        //                     dataPair.second->setData(node[dataPair.first].template as<std::string>());
        //                 }
        //             }
        //         }
        //     }

        //     template <template <class> class T, class ManagerPolicy>
        //     void parseIndividualHelper(T<ManagerPolicy>& el, const YAML::Node& node) {
        //         parseData<T>(el, node);
        //         for (auto& setPair : T<ManagerPolicy>::Info::sets(el)) {
        //             if (!setPair.second->rootSet()) {
        //                 continue;
        //             }
        //             auto set = setPair.second;
        //             if (node[setPair.first]) {
        //                 auto& setNode = node[setPair.first];
        //                 if (setNode.IsScalar()) {
        //                     if (set->setType() != SetType::SINGLETON) {
        //                         throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                     }
        //                     addToSet(*set, ID::fromString(setNode.template as<std::string>()));
        //                 } else if (setNode.IsSequence()) {
        //                     for (const auto& valNode : setNode) {
        //                         addToSet(*set, ID::fromString(valNode.template as<std::string>()));
        //                     }
        //                 } else {
        //                     throw SetStateException("Invalid set formatting for individual parsing! line number " + std::to_string(setNode.Mark().line));
        //                 }
        //             }
        //         }
        //     }

        //     template <template <class> class T, class ManagerPolicy>
        //     bool parseReadOnlyScopeHelper(const YAML::Node& node, T<ManagerPolicy>& el) {
        //         for (auto& setPair : T<ManagerPolicy>::Info::sets(el)) {
        //             auto set = setPair.second;
        //             if (set->getComposition() != CompositionType::ANTI_COMPOSITE || !set->rootSet()) {
        //                 continue;
        //             }
        //             if (node[setPair.first]) {
        //                 auto& setNode = node[setPair.first];
        //                 if (setNode.IsScalar()) {
        //                     if (set->setType() != SetType::SINGLETON) {
        //                         throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                     }
        //                     addToSet(*set, ID::fromString(setNode.template as<std::string>()));
        //                     return true;
        //                 }
        //             }
        //         }
        //         return false;
        //     }

        //     template <template <class> class T, class ManagerPolicy>
        //     bool parseScopeHelper(const YAML::Node& node, T<ManagerPolicy>& el) {
        //         for (auto& setPair : T<ManagerPolicy>::Info::sets(el)) {
        //             if (setPair.second->readonly()) {
        //                 continue;
        //             }
        //             auto set = dynamic_cast<AbstractReadableSet*>(setPair.second);
        //             if (set->getComposition() != CompositionType::ANTI_COMPOSITE || !set->rootSet()) {
        //                 continue;
        //             }
        //             if (node[setPair.first]) {
        //                 auto& setNode = node[setPair.first];
        //                 if (setNode.IsScalar()) {
        //                     if (set->setType() != SetType::SINGLETON) {
        //                         throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                     }
        //                     addToSet(*set, ID::fromString(setNode.template as<std::string>()));
        //                     return true;
        //                 }
        //             }
        //         }
        //         return false;
        //     }

        //     template <template <class> class T, class ManagerPolicy>
        //     void parseWholeHelper(T<ManagerPolicy>& el, const YAML::Node& node) {
        //         parseData<T>(el, node);
        //         for (auto& setPair : T<ManagerPolicy>::Info::sets(el)) {
        //             if (!setPair.second->rootSet()) {
        //                 continue;
        //             }
        //             auto set = setPair.second;
        //             if (node[setPair.first]) {
        //                 auto& setNode = node[setPair.first];
        //                 if (setNode.IsScalar()) {
        //                     if (set->setType() != SetType::SINGLETON) {
        //                         throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                     }
        //                     addByID(*set, ID::fromString(setNode.template as<std::string>()));
        //                 } else if (setNode.IsSequence()) {
        //                     for (const auto& valNode : setNode) {
        //                         if (valNode.IsMap()) {
        //                             // composite parsing
        //                             if (set->getComposition() != CompositionType::COMPOSITE) {
        //                                 throw SerializationError("bad format for non composite set " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                             }
        //                             auto match = this->getFunctor(valNode);
        //                             auto parsedChild = match.functor.parseWhole(match.innerData);
        //                             this->addToSet(*set, *parsedChild);
        //                         } else if (valNode.IsScalar()) {
        //                             if (set->getComposition() != CompositionType::NONE) {
        //                                 throw SerializationError("bad format for set " + setPair.first + ", it should be composite, line number " + std::to_string(setNode.Mark().line));
        //                             }
        //                             this->addByID(*set, ID::fromString(valNode.template as<std::string>()));
        //                         } else {
        //                             throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(valNode.Mark().line));
        //                         }
        //                     }
        //                 } else {
        //                     if (set->setType() != SetType::SINGLETON) {
        //                         throw SerializationError("bad format for " + setPair.first + ", line number " + std::to_string(setNode.Mark().line));
        //                     }
        //                     auto match = this->getFunctor(setNode);
        //                     auto parsedChild = match.functor.parseWhole(match.innerData);
        //                     this->addToSet(*set, *parsedChild);
        //                 }
        //             }
        //         }
        //     }

        //     template <std::size_t I, class ParseTypes, class ManagerPolicy>
        //     void parseIndividual(TemplateTypeListType<I, ParseTypes>::template result<ManagerPolicy>& el, const YAML::Node& node, std::unordered_set<std::size_t>& visited) {
        //         const std::size_t typeID = TemplateTypeListIndex<TemplateTypeListType<I, ParseTypes>::template result, Tlist>::result;
        //         if (!visited.count(typeID)) {
        //             visited.insert(typeID);
        //             parseIndividualHelper<TemplateTypeListType<I, ParseTypes>::template result>(el, node);
        //             if constexpr (TemplateTypeListSize<ParseTypes>::result > I + 1) {
        //                 parseIndividual<I + 1, ParseTypes>(
        //                         dynamic_cast<TemplateTypeListType<I + 1, ParseTypes>::template result<ManagerPolicy::Manager::template GenBaseHierarchy<TemplateTypeListType<I + 1, ParseTypes>::template result>>&>(el), 
        //                         node, 
        //                         visited
        //                 );
        //             }
        //             using ElementType = TemplateTypeListType<I, ParseTypes>::result;
        //             using ElBases = typename ElementType::Info::BaseList;
        //             if constexpr (TemplateTypeListSize<ElBases>::result > 0) {
        //                 parseIndividual<0, ElBases>(el, node, visited);
        //             }
        //         } else {
        //             if constexpr (TemplateTypeListSize<ParseTypes>::result > I + 1) {
        //                 parseIndividual<I + 1, ParseTypes>(
        //                         dynamic_cast<TemplateTypeListType<I + 1, ParseTypes>::template result<ManagerPolicy::Manager::template GenBaseHierarchy<TemplateTypeListType<I + 1, ParseTypes>::template result>>&>(el), 
        //                         node, 
        //                         visited
        //                 );
        //             }
        //         }
        //     }

        //     template <std::size_t I, class ParseTypes, class ManagerPolicy>
        //     bool parseReadOnlyScope(const YAML::Node node, TemplateTypeListType<I, ParseTypes>::template result<ManagerPolicy>& el) {
        //         if (parseReadOnlyScopeHelper<TemplateTypeListType<I, ParseTypes>::template result>(node, el)) {
        //             return true;
        //         }
        //         if constexpr (TemplateTypeListSize<ParseTypes>::result > I + 1) {
        //             if (parseReadOnlyScope<I + 1, ParseTypes>(node, el.template as<TemplateTypeListType<I + 1, ParseTypes>::template result>())) {
        //                 return true;
        //             }
        //         }
        //         using ElementType = typename TemplateTypeListType<I, ParseTypes>::template result<ComparableElement<Tlist>>;
        //         using ElBases = typename ElementType::Info::BaseList;
        //         if constexpr (TemplateTypeListSize<ElBases>{} > 0) {
        //             if (parseReadOnlyScope<0, ElBases>(node, el)) {
        //                 return true;
        //             }
        //         }
        //         return false;
        //     }

        //     template <std::size_t I, class ParseTypes, class ManagerPolicy>
        //     bool parseScope(const YAML::Node& node, TemplateTypeListType<I, ParseTypes>::template result<ManagerPolicy>& el) {
        //         if (parseScopeHelper<TemplateTypeListType<I, ParseTypes>::template result>(node, el)) {
        //             return true;
        //         }
        //         if constexpr (std::tuple_size<ParseTypes>{} > I + 1) {
        //             if (parseScope<I + 1, ParseTypes>(node, el.template as<TemplateTypeListType<I + 1, ParseTypes>::template result>())) {
        //                 return true;
        //             }
        //         }
        //         using ElementType = typename TemplateTypeListType<I, ParseTypes>::template result<ComparableElement<Tlist>>;
        //         using ElBases = typename ElementType::Info::BaseList;
        //         if constexpr (TemplateTypeListSize<ElBases>::result > 0) {
        //             if (parseScope<0, ElBases>(node, el)) {
        //                 return true;
        //             }
        //         }
        //         return false;
        //     }
        //     
        //     template <std::size_t I, class ParseTypes, class ManagerPolicy>
        //     void parseWhole(TemplateTypeListType<I, ParseTypes>::template result<ManagerPolicy>& el, const YAML::Node& node, std::unordered_set<std::size_t>& visited) {
        //         using ElementType = TemplateTypeListType<I, ParseTypes>::template result<ManagerPolicy>;
        //         const std::size_t typeID = TemplateTypeListIndex<TemplateTypeListType<I, ParseTypes>::template result, Tlist>::result;
        //         if (!visited.count(typeID)) {
        //             visited.insert(typeID);
        //             parseWholeHelper<TemplateTypeListType<I, ParseTypes>::template result>(el, node);
        //             if constexpr (TemplateTypeListSize<ParseTypes>::result > I + 1) {
        //                 parseWhole<I + 1, ParseTypes>(
        //                         dynamic_cast<TemplateTypeListType<I + 1, ParseTypes>::template result<ManagerPolicy::Manager::template GenBaseHierarchy<TemplateTypeListType<I + 1, ParseTypes>::template result>>>(el), 
        //                         node, 
        //                         visited
        //                 );
        //             }
        //             using ElBases = typename ElementType::Info::BaseList;
        //             if constexpr (TemplateTypeListSize<ElBases>::result > 0) {
        //                 parseWhole<0, ElBases>(el, node, visited);
        //             }
        //         } else {
        //             if constexpr (TemplateTypeListSize<ParseTypes>::result > I + 1) {
        //                 parseWhole<I + 1, ParseTypes>(
        //                         dynamic_cast<TemplateTypeListType<I + 1, ParseTypes>::template result<ManagerPolicy::Manager::template GenBaseHierarchy<TemplateTypeListType<I + 1, ParseTypes>::template result>>&>(el), 
        //                         node, 
        //                         visited
        //                 );
        //             }
        //         }
        //     }

        //     struct AbstractParserFunctor {
        //         UmlCafeSerializationPolicy& m_self;
        //         AbstractParserFunctor(UmlCafeSerializationPolicy& self) : m_self(self) {}
        //         virtual ~AbstractParserFunctor() {}
        //         virtual UmlPtr<AbstractElement> operator()(const YAML::Node node) = 0; // rename?
        //         virtual void parseScope(YAML::Node node, ComparableElement<Tlist>& el) = 0;
        //         virtual UmlPtr<AbstractElement> parseWhole(const YAML::Node node) = 0;
        //     };

        //     template <template <class> class T>
        //     struct ParserFunctor : public AbstractParserFunctor {
        //         // typedef T Type;
        //         ParserFunctor(UmlCafeSerializationPolicy& self) : AbstractParserFunctor(self) {}
        //         auto operator()(const YAML::Node node) override {
        //             auto ret = this->m_self.create(TemplateTypeListIndex<T, Tlist>::result);
        //             if (node["id"]) {
        //                 ret->setID(ID::fromString(node["id"].as<std::string>()));
        //             }
        //             std::unordered_set<std::size_t> visited;
        //             this->m_self.template parseIndividual<0, TemplateTypeList<T>>(*ret, node, visited);

        //             return ret;
        //         }
        //         void parseScope(YAML::Node node, ComparableElement<Tlist>& el) override {
        //             auto found = this->m_self.template parseScope<0, TemplateTypeList<T>>(node, el.template as<T>());
        //             if (!found) {
        //                 this->m_self.template parseReadOnlyScope<0, TemplateTypeList<T>>(node, el.template as<T>());
        //             }
        //         }
        //         auto parseWhole(const YAML::Node node) {
        //             auto ret = this->m_self.create(TemplateTypeListIndex<T, Tlist>::result);
        //             if (node["id"]) {
        //                 if (!node["id"].IsScalar()) {
        //                     throw SerializationError("Improper ID formatting, should be a Scalar value! line number " + std::to_string(node["id"].Mark().line));
        //                 }
        //                 ret->setID(ID::fromString(node["id"].as<std::string>()));
        //             }
        //             std::unordered_set<std::size_t> visited;
        //             this->m_self.template parseWhole<0, TemplateTypeList<T>>(*ret, node, visited);
        //             return ret;
        //         }
        //     };

        //     template <std::size_t I = 0>
        //     void populateEmitterFunctors(std::unordered_map<std::size_t, std::unique_ptr<AbstractEmitterFunctor>>& functors) {
        //         if constexpr (TemplateTypeListSize<Tlist>::result > I) {
        //             functors.emplace(I, std::make_unique<EmitterFunctor<TemplateTypeListType<I, Tlist>::template result>>(*this));
        //             populateEmitterFunctors<I + 1>(functors);
        //         }
        //     }

        //     template <std::size_t I = 0>
        //     void populateParserFunctors(std::unordered_map<std::string, std::unique_ptr<AbstractParserFunctor>>& functors) {
        //         if constexpr (TemplateTypeListSize<Tlist>::result > I) {
        //             using ElementType = TemplateTypeListType<I, Tlist>::template result<ComparableElement<Tlist>>;
        //             if constexpr (!ElementType::Info::abstract) {
        //                 UmlPtr<ElementType> placeholderEl = this->create(I);
        //                 functors.emplace(ElementType::Info::name(*placeholderEl), std::make_unique<ParserFunctor<TemplateTypeListType<I, Tlist>::template result>>(*this));
        //                 this->erase(*placeholderEl);
        //             }
        //             populateParserFunctors<I + 1>(functors);
        //         }
        //     }
        // protected:
        //     UmlCafeSerializationPolicy() {
        //         populateEmitterFunctors<0>(m_emitterFunctors);
        //         populateParserFunctors<0>(m_parserfunctors);
        //     }
        //     AbstractElementPtr parseNode(YAML::Node node) {
        //         auto match = getFunctor(node);
        //         auto parsedEl = match.functor(match.innerData);
        //         match.functor.parseScope(match.outerData, *parsedEl);
        //         return parsedEl;
        //     }
        private:
            struct AbstractSerializationPolicy {
                AbstractManager& m_manager;
                AbstractSerializationPolicy(AbstractManager* manager) : m_manager(*manager) {}
                virtual ~AbstractSerializationPolicy() {}
                virtual AbstractElementPtr create() const  = 0;
                virtual void parseBody(YAML::Node, AbstractElementPtr) const = 0;
                virtual void parseScope(YAML::Node, AbstractElementPtr) const = 0;
                virtual std::string emit(AbstractElementPtr) const = 0;
                // TODO
            };
            template <template <class> class Type>
            struct SerializationPolicy : public AbstractSerializationPolicy {
                AbstractElementPtr create() const override {
                    return this->m_manager.create(TemplateTypeListIndex<Type, Tlist>::result);
                }
                private:
                    template <class List, template <class> class Curr, class Bases = Curr<AbstractElement>::Info::BaseList>
                    struct AddChildrenTypes;

                    template <class List, template <class> class Curr, template <class> class Front, template <class> class ... Bases>
                    struct AddChildrenTypes<List, Curr, TemplateTypeList<Front, Bases...>> {
                        static const std::size_t front_type = TemplateTypeListIndex<Front, Tlist>::result;
                        template <bool IsFirst, class First, class Second>
                        struct Choose;
                        template <class First, class Second>
                        struct Choose<true, First, Second> {
                            using result = First;
                        };
                        template <class First, class Second>
                        struct Choose<false, First, Second> {
                            using result = Second;
                        };
                        using ListAndFront = Choose<HasInt<front_type, List>::value, List ,typename IntAppend<List, front_type>::type>::result;
                        using result = typename AddChildrenTypes<
                                typename AddChildrenTypes<
                                    ListAndFront, 
                                    Front
                                >::result, 
                                Curr, 
                                TemplateTypeList<Bases...>
                            >::result;
                    };

                    template <class List, template <class> class Curr>
                    struct AddChildrenTypes<List, Curr, TemplateTypeList<>> {
                        using result = List;
                    };

                    template <template <class> class Curr, class Visited = IntList<>, std::size_t BaseIndex = 0>
                    void parseBodyHelper(YAML::Node node, AbstractElementPtr el) {
                        if constexpr (BaseIndex == 0) {
                            // parse all data and sets TODO
                            for (auto& setPair : ElementInfo<Curr>::sets(*el)) {
                                // TODO
                            }
                        }
                        using Bases = Curr<AbstractElement>::Info::BaseList;
                        if constexpr (BaseIndex < TemplateTypeListSize<Bases>::result) {
                            constexpr std::size_t BaseType = TemplateTypeListIndex<TemplateTypeListType<BaseIndex, Bases>::template result, Tlist>::result;
                            if constexpr (!HasInt<BaseType, Visited>::value) {
                                using NewVisited = AddChildrenTypes<typename IntAppend<Visited, BaseType>::type, TemplateTypeListType<BaseIndex, Bases>::template result>::result;
                                parseBodyHelper<TemplateTypeListType<BaseIndex, Bases>::result, NewVisited>(node, el);
                                parseBodyHelper<Curr, NewVisited, BaseIndex + 1>(node, el);
                            } else {
                                parseBodyHelper<Curr, Visited, BaseIndex + 1>(node, el);
                            }
                        }
                    }
                public:
                void parseBody(YAML::Node bodyNode, AbstractElementPtr el) const override {
                    // TODO
                }
                void parseScope(YAML::Node elNode, AbstractElementPtr el) const override {
                    // TODO
                }
                std::string emit(AbstractElementPtr el) const override {
                    // TODO
                }
            };
            std::unordered_map<std::string, std::shared_ptr<AbstractSerializationPolicy>> m_serializationByName;
            std::unordered_map<std::size_t, std::shared_ptr<AbstractSerializationPolicy>> m_serializationByType;
            template <std::size_t I = 0>
            void populatePolicies() {
                if constexpr (I < TemplateTypeListSize<Tlist>::result) {
                    auto serialization_policy = std::make_shared<SerializationPolicy<TemplateTypeListType<I, Tlist>::template result>>(this);
                    m_serializationByName.emplace(ElementInfo<TemplateTypeListType<I, Tlist>::template result>::name(), serialization_policy);
                    m_serializationByType.emplace(I, serialization_policy);
                    populatePolicies<I + 1>();
                }
            }
        public:
            UmlCafeSerializationPolicy() {
                populatePolicies();
            }
        protected:
            AbstractElementPtr parseIndividual(std::string data) {
                std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
                if (rootNodes.empty()) {
                    throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
                }
                auto node = rootNodes[0];
                auto it = node.begin();
                while (it != node.end()) {
                    const auto& keyNode = it->first;
                    const auto& valNode = it->second;
                    if (valNode.IsMap()) {
                        // look up key
                        try {
                            auto serialization_policy = m_serializationByName.at(keyNode.as<std::string>());
                            AbstractElementPtr el = serialization_policy->create();
                            serialization_policy->parseBody(valNode, el);
                            serialization_policy->parseScope(node, el);
                            return el;
                        } catch (std::exception& e) {
                            throw SerializationError("Could not find proper type to parse! line number " + std::to_string(keyNode.Mark().line));
                        }
                    }
                    it++;
                } 
                throw SerializationError("Could not identify type to parse relevant to this manager!");
            }
            std::vector<UmlPtr<AbstractElement>> parseWhole(std::string data) {
                // std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
                // if (rootNodes.empty()) {
                //     throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
                // }
                // std::vector<UmlPtr<AbstractElement>> ret(rootNodes.size());
                // auto i = 0;
                // for (auto& node : rootNodes) {
                //     auto match = getFunctor(node);
                //     ret[i] = match.functor.parseWhole(match.innerData);
                //     i++;
                // }
                // for (auto& pair : m_setsToRunPolicies) {
                //     auto& set = *pair.first;
                //     auto el = this->abstractGet(pair.second);
                //     this->runAllAddPolicies(set, *el);
                // }
                // m_setsToRunPolicies.clear();
                // return ret;
                return std::vector<UmlPtr<AbstractElement>>{};
            }
            std::string emitIndividual(AbstractElement& el) {
                return m_serializationByType.at(el.getElementType()).emit(el);
            }
            std::string emitWhole(AbstractElement& el) {
                // YAML::Emitter emitter;
                // primeEmitter(emitter);
                // m_emitterFunctors.at(el.getElementType())->emitWhole(emitter, dynamic_cast<AbstractElement&>(el));
                // return emitter.c_str();
                return "";
            }
            virtual void primeEmitter(__attribute__((unused)) YAML::Emitter& emitter) {}
        public:
            std::string dump() {
                return  this->emitWhole(*getAbstractRoot());
            }
            std::string dump(AbstractElement& el) {
                return this->emitWhole(el);
            }
    };

    template <class Tlist>
    class UmlCafeJsonSerializationPolicy : public UmlCafeSerializationPolicy<Tlist> {
        private:
            bool emitYaml = false;
        protected:
            void primeEmitter(YAML::Emitter& emitter) override {
                if (!emitYaml) {
                    emitter << YAML::DoubleQuoted << YAML::Flow;
                }
            }
            std::string dumpYaml() {
                emitYaml = true;
                auto ret = this->emitWhole(*this->getAbstractRoot());
                emitYaml = false;
                return ret;
            }
            std::string dumpYaml(AbstractElement& el) {
                emitYaml = true;
                auto ret = this->emitWhole(el);
                emitYaml = false;
                return ret;
            }
    };
}

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
        private:
            // data
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            std::mutex m_graphMtx;
        public:
            // Base element for all types created by manager, the policy classes provided will be filled out with
            // this BaseElement as their policy
            class BaseElement : public ComparableElement<TypePolicyList> {
                protected:
                    // constructor
                    BaseElement(std::size_t elementType, AbstractManager& manager) : ComparableElement<TypePolicyList>(elementType, manager) {}
                public:
                    using Manager = Manager;
            };

            template <template <class> class T, std::size_t I = 0, bool HasBases = I < TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result>
            struct GenBaseHierarchy;

            template <template <class> class T, std::size_t I>
            struct GenBaseHierarchy<T, I, false> : public T<BaseElement> {};

            template <template <class> class T, std::size_t I>
            struct GenBaseHierarchy<T, I, true> :
                public GenBaseHierarchy<TemplateTypeListType<I, typename T<BaseElement>::Info::BaseList>::template result, 0, TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result != 0>,
                public GenBaseHierarchy<T, I + 1, I + 1 < TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result>
            {};

            // create factory function
            template <template <class> class T>
            UmlPtr<T<GenBaseHierarchy<T>>> create() {
                auto ret = std::make_shared<T<GenBaseHierarchy<T>>>(TemplateTypeListIndex<T, TypePolicyList>::result, *this);
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                ret->m_node = m_graph.emplace(ret->getID(), std::make_shared<ManagerNode>(ret)).first->second;
                ret->m_node.lock()->m_myPtr = ret->m_node;
                return ret.get();
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
            UmlPtr<BaseElement> create(std::size_t type) {
                return createHelper(type);
            }
    };
}

#pragma once

#include <cstddef>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "umlPtr.h"
#include "abstractManager.h"
#include "abstractElement.h"
#include "templateTypeList.h"

namespace UML {
    

    // manager
    template <class TypePolicyList, class StoragePolicy>
    class Manager : public StoragePolicy, virtual public AbstractManager {
        private:
            // data
            std::unordered_map<ID, std::shared_ptr<ManagerNode>> m_graph;
            std::mutex m_graphMtx;
        public:
            // Base element for all types created by manager, the policy classes provided will be filled out with
            // this BaseElement as their policy
            class BaseElement : public AbstractElement {
                protected:
                    // constructor
                    BaseElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
                public:
                    // is function to compare types compile time O(1)
                    template <template <class> class T, std::size_t I = 0>
                    constexpr bool is() const {
                        if constexpr (TemplateTypeListIndex<T, TypePolicyList>::result == m_elementType) {
                            return true;
                        }
                        if constexpr (I < TemplateTypeListSize<typename T<BaseElement>::Info::BaseList>::result) {
                            if constexpr (TemplateTypeListType<I, typename T<BaseElement>::Info::BaseList, BaseElement>::result::template is<T>()) {
                                return true;
                            }
                        }
                        return is<T, I + 1>();
                    }

                    // as to cast to other managed types
                    template <template<class> class T>
                    constexpr T<BaseElement>& as() const {
                        if constexpr (is<T>()) {
                            return dynamic_cast<T<BaseElement>&>(*this);
                        }
                        throw ManagerStateException("Can not convert element to that type!");
                    }
            };

            // create factory function
            template <template <class> class T>
            UmlPtr<T<BaseElement>> create() {
                auto ret = std::make_shared<T<BaseElement>>(TemplateTypeListIndex<T, TypePolicyList>::result, *this);
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                ret->m_node = m_graph.emplace(ret->getID(), std::make_shared<ManagerNode>(ret)).first->second;
                ret->m_node.lock()->m_myPtr = ret->m_node;
                return ret.get();
            }
        private:
            template <std::size_t I = 0>
            UmlPtr<AbstractElement> createHelper(std::size_t type) {
                if (type == I) {
                    return create<TemplateTypeListType<I, TypePolicyList, BaseElement>::result>();
                }
                return createHelper<I + 1>(type);
            }
        public:
            // create by type id
            UmlPtr<AbstractElement> create(std::size_t type) {
                return createHelper(type);
            }
    };
}

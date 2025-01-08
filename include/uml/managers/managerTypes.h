#pragma once

#include "uml/managers/abstractManager.h"
#include <functional>
#include <string>
#include <memory>
#include "templateTypeList.h"
#include "abstractElement.h"
#include "umlPtr.h"

namespace UML {
    template <class TypeList>
    class ManagerTypes : virtual public AbstractManager {
        public:
            class BaseElement;
            
            // Gen Base Hierarchy is a class that will be used to create dynamic objects that inherit from and can correspond to their bases
            // This is used mostly internally but all types created by the manager will be of this policy
            template <template <class> class T, class Bases = T<BaseElement>::Info::BaseList>
            struct GenBaseHierarchy;
        protected:
            struct AbstractManagerTypeInfo {
                virtual ~AbstractManagerTypeInfo() {}
                virtual void forEachSet(BaseElement& el, std::function<void(std::string, AbstractSet&)>)  = 0;
                virtual bool is(std::size_t type) = 0;
                virtual AbstractElementPtr create(ManagerTypes&) = 0;
            };

            std::unordered_map<std::size_t, std::unique_ptr<AbstractManagerTypeInfo>> m_types;

        public:
            using Types = TypeList;
            template <template <class> class Type>
            using ElementType = TemplateTypeListIndex<Type, Types>;
            // Base element for all types created by manager, the policy classes provided will be filled out with
            // this BaseElement as part of their policy
            class BaseElement : public AbstractElement {
                protected:    
                    using AbstractElement::AbstractElement;
                    BaseElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
                public:
                    using manager = ManagerTypes;
                    // is function to compare types compile time O(1)
                    template <template <class> class T>
                    bool is() const {
                        return dynamic_cast<ManagerTypes&>(m_manager).m_types.at(m_elementType)->is(ElementType<T>::result); 
                    }

                    // as to cast to other managed types
                    template <template<class> class T>
                    T<typename ManagerTypes::template GenBaseHierarchy<T>>& as() {
                        if (is<T>()) {
                            return dynamic_cast<T<typename ManagerTypes::template GenBaseHierarchy<T>>&>(*this);
                        }
                        throw ManagerStateException("Can not convert element to that type!");
                    }
            };

            template <template <class> class Type>
            struct ManagerTypeInfo : public AbstractManagerTypeInfo {
                private:
                    using Function = std::function<void(std::string, AbstractSet&)>;
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
                    struct ForEachSetVisitor {
                        BaseElement& el;
                        Function f;
                        template <template <class> class Curr>
                        void visit() {
                            for (auto& setPair : Curr<BaseElement>::Info::sets(dynamic_cast<Curr<GenBaseHierarchy<Curr>>&>(el))) {
                                f(setPair.first, *setPair.second);
                            }
                        }
                    };
                public:
                    void forEachSet(BaseElement& el, Function f) override {
                        ForEachSetVisitor visitor { el, f };
                        visitAllTypesDFS<ForEachSetVisitor, Type, Types>(visitor);
                    }
                    bool is(std::size_t type) override {
                        return isHelper(type);                        
                    }
                    AbstractElementPtr create(ManagerTypes& manager) override {
                        return manager.template create<Type>();
                    }
            };

        public:
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
        private:
            template <class TypesToPopulate, class Dummy = void>
            struct PopulateTypes;

            template <template <class> class First, template <class> class ... Rest, class Dummy>
            struct PopulateTypes<TemplateTypeList<First, Rest ...>, Dummy> {
                static void populate(ManagerTypes& manager) {
                    manager.m_types.emplace(ElementType<First>::result, std::make_unique<ManagerTypeInfo<First>>());
                    PopulateTypes<TemplateTypeList<Rest ...>>::populate(manager);
                }
            };

            template <class Dummy>
            struct PopulateTypes<TemplateTypeList<>, Dummy> {
                static void populate(__attribute__((unused)) ManagerTypes& manager) {}
            };
        protected:
            virtual UmlPtr<BaseElement> registerPtr(std::shared_ptr<AbstractElement> ptr) = 0;
        public:
            ManagerTypes() {
                PopulateTypes<Types>::populate(*this);
            }
            // create factory function
            template <template <class> class T>
            UmlPtr<T<GenBaseHierarchy<T>>> create() {
                if constexpr (T<BaseElement>::Info::abstract) {
                    throw ManagerStateException("Trying to instantiate and abstract type!");
                }
                auto ptr = std::make_shared<T<GenBaseHierarchy<T>>>(ElementType<T>::result, *this);
                return registerPtr(ptr);
            }
    };
}

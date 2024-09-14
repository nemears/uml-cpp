#pragma once

#include "property.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"

namespace UML {

    class Interface;
    class BehavioredClassifier;

    class Port : public Property {

        friend class BehavioredClassifier;
        friend class InterfaceRealization;
        friend struct ElementInfo<Port>;

        private:
            void setPortInterfaces(BehavioredClassifier& clazz);
            void removePortInterfaces(BehavioredClassifier& clazz);
        protected:
            struct TypePolicy {
                void elementAdded(Type& el, Port& me);
                void elementRemoved(Type& el, Port& me);
            };
            ReadOnlyIndexableSet<Interface, Port> m_required = ReadOnlyIndexableSet<Interface, Port>(this);
            ReadOnlyIndexableSet<Interface, Port> m_provided = ReadOnlyIndexableSet<Interface, Port>(this);
            Singleton<Type, Port, TypePolicy> m_portType = Singleton<Type, Port, TypePolicy>(this);
            bool m_isBehavior = false;
            bool m_isConjugated = false;
            bool m_isService = true;
        public:
            Port(std::size_t elementType, AbstractManager& manager);
            bool isBehavior() const;
            void setIsBehavior(bool isBehavior);
            bool isConjugated() const;
            void setIsConjugated(bool isConjugated);
            bool isService() const;
            void setIsService(bool isService);
            ReadOnlyIndexableSet<Interface, Port>& getRequired();
            ReadOnlyIndexableSet<Interface, Port>& getProvided();
            typedef TypeInfo<std::tuple<Property>, Port> Info;
    };

    template <>
    struct ElementInfo<Port> : DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Port"};
        static SetList sets(Port& port) {
            return SetList {
                makeSetPair("required", port.m_required),
                makeSetPair("provided", port.m_provided),
                makeSetPair("portType", port.m_portType)
            };
        }
    };
}

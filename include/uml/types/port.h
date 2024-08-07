#pragma once

#include "property.h"

namespace UML {

    class Interface;
    class BehavioredClassifier;

    class Port : public Property {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class BehavioredClassifier;
        friend class InterfaceRealization;

        private:
            void setPortInterfaces(BehavioredClassifier& clazz);
            void removePortInterfaces(BehavioredClassifier& clazz);
        protected:
            struct TypePolicy {
                void elementAdded(Type& el, Port& me);
                void elementRemoved(Type& el, Port& me);
            };
            ReadOnlySet<Interface, Port> m_required = ReadOnlySet<Interface, Port>(this);
            ReadOnlySet<Interface, Port> m_provided = ReadOnlySet<Interface, Port>(this);
            Singleton<Type, Port, TypePolicy> m_portType = Singleton<Type, Port, TypePolicy>(this);
            bool m_isBehavior = false;
            bool m_isConjugated = false;
            bool m_isService = true;
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Port();
        public:
            virtual ~Port();
            bool isBehavior() const;
            void setIsBehavior(bool isBehavior);
            bool isConjugated() const;
            void setIsConjugated(bool isConjugated);
            bool isService() const;
            void setIsService(bool isService);
            ReadOnlySet<Interface, Port>& getRequired();
            ReadOnlySet<Interface, Port>& getProvided();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PORT;
            }
    };
}

#pragma once

#include "property.h"

namespace UML {

    class Interface;

    class Port : public Property {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class BehavioredClassifier;
        friend class InterfaceRealization;

        private:
            void setPortInterfaces(BehavioredClassifier& clazz);
            void removePortInterfaces(BehavioredClassifier& clazz);
        protected:
            class SetTypePolicy {
                public:
                    void apply(Type& el, Port& me);
            };
            class RemoveTypePolicy {
                public:
                    void apply(Type& el, Port& me);
            };
            CustomReadOnlySet<Interface, Port> m_required = CustomReadOnlySet<Interface, Port>(this);
            CustomReadOnlySet<Interface, Port> m_provided = CustomReadOnlySet<Interface, Port>(this);
            CustomSingleton<Type, Port, SetTypePolicy, RemoveTypePolicy> m_portType = CustomSingleton<Type, Port, SetTypePolicy, RemoveTypePolicy>(this);
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PORT;
            }
    };
}
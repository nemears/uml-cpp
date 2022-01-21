#ifndef _UML_PORT_H_
#define _UML_PORT_H_

#include "property.h"

namespace UML {
    class Port : public Property {

        friend UmlManager;

        private:
            void setPortInterfaces(BehavioredClassifier& clazz);
            void removePortInterfaces(BehavioredClassifier& clazz);
        protected:
            Set<Interface, Port> m_required = Set<Interface, Port>(this);
            Set<Interface, Port> m_provided = Set<Interface, Port>(this);
            class SetTypeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    SetTypeFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveTypeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveTypeFunctor(Element* el) : SetFunctor(el) {};
            };
            bool m_isBehavior = false;
            bool m_isConjugated = false;
            bool m_isService = true;
            void referencingReleased(ID id) override;
            void restoreReference(Element* el) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Port& rhs);
            Port();
        public:
            virtual ~Port();
            Port(const Port& rhs);
            bool isBehavior() const;
            void setIsBehavior(bool isBehavior);
            bool isConjugated() const;
            void setIsConjugated(bool isConjugated);
            bool isService() const;
            void setIsService(bool isService);
            Set<Interface, Port>& getRequired();
            Set<Interface, Port>& getProvided();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PORT;
            }
    };
}

#endif
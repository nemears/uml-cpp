#ifndef OPERATIONH
#define OPERATIONH

#include "behavioralFeature.h"
#include "behavior.h"

namespace UML {

    class Class;

    class Operation : public BehavioralFeature {
        protected:
            Type* m_type;
            Sequence<Behavior>* m_methods;
            Class* m_class;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
            class AddMethodFunctor : public AbstractSequenceFunctor {
                public:
                    AddMethodFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Operation();
            ~Operation();
            Sequence<Behavior>& getMethods();
            Type* getType();
            void setType(Type* type);
            Class* getClass();
            void setClass(Class* clazz);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
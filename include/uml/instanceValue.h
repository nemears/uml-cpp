#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    namespace Parsers {
        class SetInstance;
    }

    class InstanceValue : public ValueSpecification {

        friend class UmlManager;
        friend class Parsers::SetInstance;

        protected:
            Singleton<InstanceSpecification, InstanceValue> m_instance = Singleton<InstanceSpecification, InstanceValue>(this);
            class RemoveInstanceProcedure : public AbstractSingletonProcedure<InstanceSpecification, InstanceValue> {
                public:
                    RemoveInstanceProcedure(InstanceValue* me) : AbstractSingletonProcedure<InstanceSpecification, InstanceValue>(me) {};
                    void operator()(InstanceSpecification* el) const override;
            };
            class AddInstanceProcedure : public AbstractSingletonProcedure<InstanceSpecification, InstanceValue> {
                public:
                    AddInstanceProcedure(InstanceValue* me) : AbstractSingletonProcedure<InstanceSpecification, InstanceValue>(me) {};
                    void operator()(InstanceSpecification* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            InstanceValue();
        public:
            InstanceValue(const InstanceValue& rhs);
            InstanceSpecification* getInstance();
            InstanceSpecification& getInstanceRef();
            bool hasInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecification* inst);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}

#endif
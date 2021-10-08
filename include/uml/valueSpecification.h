#ifndef VALUE_SPECIFICATION_H
#define VALUE_SPECIFICATION_H

#include "typedElement.h"
#include "sequence.h"
#include "packageableElement.h"

namespace UML {

    class OpaqueBehavior;
    class Property;
    namespace Parsers {
        class SetOwningSlot;
    }

    class ValueSpecification : public TypedElement , public PackageableElement {

        friend class UmlManager;
        friend class OpaqueBehavior;
        friend class Property;
        friend class Parsers::SetOwningSlot;

        protected:
            Singleton<Slot, ValueSpecification> m_owningSlot = Singleton<Slot, ValueSpecification>(this);
            class RemoveOwningSlotProcedure : public AbstractSingletonProcedure<Slot, ValueSpecification> {
                public:
                    RemoveOwningSlotProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Slot, ValueSpecification>(me) {};
                    void operator()(Slot* el) const override;
            };
            class AddOwningSlotProcedure : public AbstractSingletonProcedure<Slot, ValueSpecification> {
                public:
                    AddOwningSlotProcedure(ValueSpecification* me) : AbstractSingletonProcedure<Slot, ValueSpecification>(me) {};
                    void operator()(Slot* el) const override;
                    void operator()(ID id) const override;
            };
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            ValueSpecification();
        public:
            ValueSpecification(const ValueSpecification& rhs);
            Slot* getOwningSlot();
            Slot& getOwningSlotRef();
            ID getOwningSlotID() const;
            bool hasOwningSlot() const;
            void setOwningSlot(Slot* slot);
            void setOwningSlot(Slot& slot);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_SPECIFICATION;
            };
    };
}
#endif
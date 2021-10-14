#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        friend class UmlManager;

        protected:
            Sequence<LiteralString> m_bodies = Sequence<LiteralString>(this);
            class AddBodyFunctor : public TemplateAbstractSequenceFunctor<LiteralString, OpaqueBehavior> {
                public:
                    AddBodyFunctor(OpaqueBehavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(LiteralString& el) const override;
            };
            class RemoveBodyFunctor : public TemplateAbstractSequenceFunctor<LiteralString, OpaqueBehavior> {
                public:
                    RemoveBodyFunctor(OpaqueBehavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(LiteralString& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            OpaqueBehavior();
        public:
            OpaqueBehavior(const OpaqueBehavior& rhs);
            virtual ~OpaqueBehavior();
            Sequence<LiteralString>& getBodies();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_BEHAVIOR;
            };
    };
}

#endif
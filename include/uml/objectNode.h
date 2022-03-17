#ifndef _UML_OBJECT_NODE_H_
#define _UML_OBJECT_NODE_H_

#include "activityNode.h"
#include "typedElement.h"
#include "valueSpecification.h"

namespace UML {

    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;
    typedef UmlPtr<Behavior> BehaviorPtr;

    enum class ObjectNodeOrderingKind {
        UNORDERED,
        ORDERED,
        LIFO,
        FIFO
    };

    class ObjectNode : public ActivityNode, public TypedElement {

        friend class UmlManager;

        protected:
            Singleton<ValueSpecification, ObjectNode> m_upperBound = Singleton<ValueSpecification, ObjectNode>(this);
            Singleton<Behavior, ObjectNode> m_selection = Singleton<Behavior, ObjectNode>(this);
            bool m_controlType = false;
            ObjectNodeOrderingKind m_ordering = ObjectNodeOrderingKind::FIFO;
            Set<ValueSpecification, ObjectNode>& getUpperBoundSingleton();
            Set<Behavior, ObjectNode>& getSelectionSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void reindexName(ID id, std::string newName) override;
            void init();
            ObjectNode();
        public:
            virtual ~ObjectNode();
            bool isControlType() const;
            void setControlType(bool controlType);
            ObjectNodeOrderingKind getOrdering() const;
            void setOrdering(ObjectNodeOrderingKind ordering);
            ValueSpecificationPtr getUpperBound() const;
            void setUpperBound(ValueSpecification* upperBound);
            void setUpperBound(ValueSpecification& upperBound);
            void setUpperBound(ValueSpecificationPtr upperBound);
            void setUpperBound(ID id);
            BehaviorPtr getSelection() const;
            void setSelection(Behavior* selection);
            void setSelection(Behavior& selection);
            void setSelection(BehaviorPtr selection);
            void setSelection(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_NODE;
            };
    };
}

#endif
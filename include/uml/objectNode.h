#ifndef _UML_OBJECT_NODE_H_
#define _UML_OBJECT_NODE_H_

#include "activityNode.h"
#include "typedElement.h"
#include "valueSpecification.h"

namespace UML {

    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class ObjectNode : public ActivityNode, public TypedElement {

        friend class UmlManager;

        protected:
            Singleton<ValueSpecification, ObjectNode> m_upperBound = Singleton<ValueSpecification, ObjectNode>(this);
            Set<ValueSpecification, ObjectNode>& getUpperBoundSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override {}; // TODO
            void referenceErased(ID id) override {}; // TODO
            void reindexName(ID id, std::string newName) override;
            void init();
            ObjectNode();
        public:
            virtual ~ObjectNode();
            ValueSpecificationPtr getUpperBound() const;
            void setUpperBound(ValueSpecification* upperBound);
            void setUpperBound(ValueSpecification& upperBound);
            void setUpperBound(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_NODE;
            };
    };
}

#endif
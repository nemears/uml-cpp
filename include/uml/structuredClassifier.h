#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"

namespace UML {

    class ConnectableElement;

    class StructuredClassifier : virtual public Classifier {
        protected:
            Set<ConnectableElement, StructuredClassifier> m_roles = Set<ConnectableElement, StructuredClassifier>(this);
            Set<Property, StructuredClassifier> m_ownedAttributes = Set<Property, StructuredClassifier>(this);
            Set<Property, StructuredClassifier> m_parts = Set<Property, StructuredClassifier>(this);
            class AddPartFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddPartFunctor(Element* el) : SetFunctor(el) {};
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const StructuredClassifier& rhs);
            StructuredClassifier();
        public:
            StructuredClassifier(const StructuredClassifier& clazz);
            virtual ~StructuredClassifier();
            Set<Property, StructuredClassifier>& getOwnedAttributes();
            Set<ConnectableElement, StructuredClassifier>& getRoles();
            Set<Property, StructuredClassifier>& getParts();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}

#endif
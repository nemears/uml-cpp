#ifndef _UML_STRUCTURED_CLASSIFIER_H_
#define _UML_STRUCTURED_CLASSIFIER_H_

#include "classifier.h"
#include "uml/connector.h"

namespace UML {

    class ConnectableElement;
    class Connector;

    class StructuredClassifier : virtual public Classifier {

        friend class Property;

        protected:
            Set<ConnectableElement, StructuredClassifier> m_roles = Set<ConnectableElement, StructuredClassifier>(this);
            Set<Property, StructuredClassifier> m_ownedAttributes = Set<Property, StructuredClassifier>(this);
            Set<Property, StructuredClassifier> m_parts = Set<Property, StructuredClassifier>(this);
            Set<Connector, StructuredClassifier> m_ownedConnectors = Set<Connector, StructuredClassifier>(this);
            class AddPartFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddPartFunctor(Element* el) : SetFunctor(el) {};
            };
            void restoreReferences() override;
            void init();
            StructuredClassifier();
        public:
            virtual ~StructuredClassifier();
            Set<Property, StructuredClassifier>& getOwnedAttributes();
            Set<ConnectableElement, StructuredClassifier>& getRoles();
            Set<Property, StructuredClassifier>& getParts();
            Set<Connector, StructuredClassifier>& getOwnedConnectors();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}

#endif
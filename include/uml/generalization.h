#ifndef GENERALIZATION_H
#define GENERALIZATION_H

#include "directedRelationship.h"
#include "classifier.h"

namespace UML {
    class Generalization : public DirectedRelationship {
        protected:
            Classifier* m_general;
            Classifier* m_specific;
        public:
            Generalization();
            Classifier* getGeneral();
            void setGeneral(Classifier* general);
            Classifier* getSpecific();
            void setSpecific(Classifier* specific);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION;
            };
    };
}

#endif
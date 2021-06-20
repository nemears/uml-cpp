#ifndef GENERALIZATION_H
#define GENERALIZATION_H

#include "directedRelationship.h"
#include "classifier.h"

namespace UML {
    class Generalization : public DirectedRelationship {
        protected:
            ID m_generalID;
            ID m_specificID;
            Classifier* m_generalPtr;
            Classifier* m_specificPtr;
        public:
            Generalization();
            Classifier* getGeneral();
            void setGeneral(Classifier* general);
            Classifier* getSpecific();
            void setSpecific(Classifier* specific);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION;
            };
    };
}

#endif
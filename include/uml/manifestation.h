#ifndef MANIFESTATION_H
#define MANIFESTATION_H

#include "abstraction.h"

namespace UML {
    class Manifestation : public Abstraction {
        private:
            ID m_utilizedElementID;
            PackageableElement* m_utilizedElementPtr;
        public:
            Manifestation();
            Manifestation(const Manifestation& manifestation);
            virtual ~Manifestation();
            PackageableElement* getUtilizedElement();
            void setUtilizedElement(PackageableElement* utilizedElements);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MANIFESTATION;
            };
    };
}

#endif
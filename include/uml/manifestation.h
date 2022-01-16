#ifndef MANIFESTATION_H
#define MANIFESTATION_H

#include "abstraction.h"

namespace UML {

    class Manifestation : public Abstraction {

        friend class UmlManager;
        
        protected:
            Singleton<PackageableElement, Manifestation> m_utilizedElement = Singleton<PackageableElement, Manifestation>(this);
            Set<PackageableElement, Manifestation>& getUtilizedElementSingleton();
            void init();
            void copy(const Manifestation& rhs);
            Manifestation();
        public:
            Manifestation(const Manifestation& rhs);
            virtual ~Manifestation();
            PackageableElement* getUtilizedElement();
            PackageableElement& getUtilizedElementRef();
            bool hasUtilizedElement() const;
            void setUtilizedElement(PackageableElement& utilizedElement);
            void setUtilizedElement(PackageableElement* utilizedElement);
            void setUtilizedElement(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MANIFESTATION;
            };
    };
}

#endif
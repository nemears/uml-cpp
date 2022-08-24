#ifndef _UML_MANIFESTATION_H_
#define _UML_MANIFESTATION_H_

#include "abstraction.h"

namespace UML {

    typedef UmlPtr<PackageableElement> PackageableElementPtr;

    class Manifestation : public Abstraction {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        
        protected:
            Singleton<PackageableElement, Manifestation> m_utilizedElement = Singleton<PackageableElement, Manifestation>(this);
            Set<PackageableElement, Manifestation>& getUtilizedElementSingleton();
            void init();
            Manifestation();
        public:
            virtual ~Manifestation();
            PackageableElementPtr getUtilizedElement() const;
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
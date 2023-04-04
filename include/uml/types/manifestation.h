#ifndef _UML_MANIFESTATION_H_
#define _UML_MANIFESTATION_H_

#include "abstraction.h"

namespace UML {

    typedef UmlPtr<PackageableElement> PackageableElementPtr;

    class Manifestation : public Abstraction {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        
        protected:
            CustomSingleton<PackageableElement, Manifestation> m_utilizedElement = CustomSingleton<PackageableElement, Manifestation>(this);
            TypedSet<PackageableElement, Manifestation>& getUtilizedElementSingleton();
            void restoreReferences() override;
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
#pragma once

#include "abstraction.h"

namespace UML {

    typedef UmlPtr<PackageableElement> PackageableElementPtr;

    class Manifestation : public Abstraction {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        
        protected:
            Singleton<PackageableElement, Manifestation> m_utilizedElement = Singleton<PackageableElement, Manifestation>(this);
            Singleton<PackageableElement, Manifestation>& getUtilizedElementSingleton();
            void restoreReferences() override;
            Manifestation();
        public:
            virtual ~Manifestation();
            PackageableElementPtr getUtilizedElement() const;
            void setUtilizedElement(PackageableElement& utilizedElement);
            void setUtilizedElement(PackageableElement* utilizedElement);
            void setUtilizedElement(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MANIFESTATION;
            };
    };
}

#pragma once

#include "packageableElement.h"
#include "uml/set/set.h"

namespace UML {

    class Classifier;
    class Generalization;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class GeneralizationSet : public PackageableElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;
        
        protected:
            bool m_covering = false;
            bool m_disjoint = false;
            Singleton<Classifier, GeneralizationSet> m_powerType = Singleton<Classifier, GeneralizationSet>(this);
            Set<Generalization, GeneralizationSet> m_generalizations = Set<Generalization, GeneralizationSet>(this);
            void referenceErased(ID id) override;
            Singleton<Classifier, GeneralizationSet>& getPowerTypeSingleton();
            GeneralizationSet();
        public:
            virtual ~GeneralizationSet();
            bool isCovering() const;
            bool isDisjoint() const;
            void setCovering(bool covering);
            void setDisjoint(bool disjoint);
            ClassifierPtr getPowerType() const;
            void setPowerType(ClassifierPtr powerType);
            void setPowerType(Classifier& powerType);
            void setPowerType(ID id);
            Set<Generalization, GeneralizationSet>& getGeneralizations();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION_SET;
            };
    };
}

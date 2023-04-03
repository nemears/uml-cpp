#ifndef _UML_GENERALIZATION_SET_H_
#define _UML_GENERALIZATION_SET_H_

#include "packageableElement.h"
#include "uml/set/set.h"

namespace UML {

    class Classifier;
    class Generalization;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class GeneralizationSet : public PackageableElement {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;
        
        protected:
            bool m_covering = false;
            bool m_disjoint = false;
            CustomSingleton<Classifier, GeneralizationSet> m_powerType = CustomSingleton<Classifier, GeneralizationSet>(this);
            CustomSet<Generalization, GeneralizationSet> m_generalizations = CustomSet<Generalization, GeneralizationSet>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<Classifier, GeneralizationSet>& getPowerTypeSingleton();
            GeneralizationSet();
        public:
            virtual ~GeneralizationSet();
            bool isCovering() const;
            bool isDisjoint() const;
            void setCovering(bool covering);
            void setDisjoint(bool disjoint);
            ClassifierPtr getPowerType() const;
            void setPowerType(Classifier* powerType);
            void setPowerType(Classifier& powerType);
            void setPowerType(ID id);
            Set<Generalization, GeneralizationSet>& getGeneralizations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION_SET;
            };
    };
}

#endif
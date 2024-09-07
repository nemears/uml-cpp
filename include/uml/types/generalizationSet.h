#pragma once

#include "packageableElement.h"
#include "uml/managers/abstractManager.h"
#include "uml/set/set.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;
    class Generalization;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class GeneralizationSet : public PackageableElement {

        friend class Classifier;
        friend struct ElementInfo<GeneralizationSet>;
        
        protected:
            bool m_covering = false;
            bool m_disjoint = false;
            Singleton<Classifier, GeneralizationSet> m_powerType = Singleton<Classifier, GeneralizationSet>(this);
            Set<Generalization, GeneralizationSet> m_generalizations = Set<Generalization, GeneralizationSet>(this);
            Singleton<Classifier, GeneralizationSet>& getPowerTypeSingleton();
            GeneralizationSet(std::size_t elementType, AbstractManager& manager);
        public:
            bool isCovering() const;
            bool isDisjoint() const;
            void setCovering(bool covering);
            void setDisjoint(bool disjoint);
            ClassifierPtr getPowerType() const;
            void setPowerType(ClassifierPtr powerType);
            void setPowerType(Classifier& powerType);
            void setPowerType(ID id);
            Set<Generalization, GeneralizationSet>& getGeneralizations();
            typedef TypeInfo<std::tuple<PackageableElement>, GeneralizationSet> Info;
    };

    template <>
    struct ElementInfo<GeneralizationSet> {
        static const bool abstract = false;
        inline static const std::string name {"GeneralizationSet"};
        static SetList sets(GeneralizationSet& el) {
            return SetList {
                makeSetPair("powerType", el.m_powerType),
                makeSetPair("generalizations", el.m_generalizations),
            };
        }
    };
}

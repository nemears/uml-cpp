#pragma once

#include "directedRelationship.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;
    class GeneralizationSet;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Generalization : public DirectedRelationship {

        friend class Classifier;
        friend struct ElementInfo<Generalization>;

        protected:
            struct GeneralPolicy {
                void elementAdded(Classifier& el, Generalization& me);
                void elementRemoved(Classifier& el, Generalization& me);
            };
            Singleton<Classifier, Generalization, GeneralPolicy> m_general = Singleton<Classifier, Generalization, GeneralPolicy>(this);
            Singleton<Classifier, Generalization> m_specific = Singleton<Classifier, Generalization>(this);
            Set<GeneralizationSet, Generalization> m_generalizationSets = Set<GeneralizationSet, Generalization>(this);
            Singleton<Classifier, Generalization, GeneralPolicy>& getGeneralSingleton();
            Singleton<Classifier, Generalization>& getSpecificSingleton();
        public:
            Generalization(std::size_t elementType, AbstractManager& manager);
            ClassifierPtr getGeneral() const;
            void setGeneral(ClassifierPtr general);
            void setGeneral(Classifier& general);
            void setGeneral(ID id);
            ClassifierPtr getSpecific() const;
            void setSpecific(Classifier& specific);
            void setSpecific(ClassifierPtr specific);
            void setSpecific(ID id);
            Set<GeneralizationSet, Generalization>& getGeneralizationSets();
            typedef TypeInfo<std::tuple<DirectedRelationship>, Generalization> Info;
    };

    template <>
    struct ElementInfo<Generalization> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"Generalization"};
        static SetList sets(Generalization& el) {
            return SetList {
                makeSetPair("general", el.m_general),
                makeSetPair("specific", el.m_specific),
                makeSetPair("generalizationSets", el.m_generalizationSets)
            };
        }
    };
}

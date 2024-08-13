#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"

namespace UML {

    class Classifier;
    class GeneralizationSet;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Generalization : public DirectedRelationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;

        protected:
            struct GeneralPolicy {
                void elementAdded(Classifier& el, Generalization& me);
                void elementRemoved(Classifier& el, Generalization& me);
            };
            Singleton<Classifier, Generalization, GeneralPolicy> m_general = Singleton<Classifier, Generalization, GeneralPolicy>(this);
            Singleton<Classifier, Generalization> m_specific = Singleton<Classifier, Generalization>(this);
            Set<GeneralizationSet, Generalization> m_generalizationSets = Set<GeneralizationSet, Generalization>(this);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Singleton<Classifier, Generalization, GeneralPolicy>& getGeneralSingleton();
            Singleton<Classifier, Generalization>& getSpecificSingleton();
            Generalization();
        public:
            virtual ~Generalization();
            ClassifierPtr getGeneral() const;
            void setGeneral(ClassifierPtr general);
            void setGeneral(Classifier& general);
            void setGeneral(ID id);
            ClassifierPtr getSpecific() const;
            void setSpecific(Classifier& specific);
            void setSpecific(ClassifierPtr specific);
            void setSpecific(ID id);
            Set<GeneralizationSet, Generalization>& getGeneralizationSets();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION;
            };
    };
}

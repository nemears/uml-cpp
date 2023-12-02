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
            class AddGeneralPolicy {
                public:
                    static void apply(Classifier& el, Generalization& me);
            };
            class RemoveGeneralPolicy {
                public:
                    static void apply(Classifier& el, Generalization& me);
            };
            CustomSingleton<Classifier, Generalization, AddGeneralPolicy, RemoveGeneralPolicy> m_general = CustomSingleton<Classifier, Generalization, AddGeneralPolicy, RemoveGeneralPolicy>(this);
            CustomSingleton<Classifier, Generalization> m_specific = CustomSingleton<Classifier, Generalization>(this);
            CustomSet<GeneralizationSet, Generalization> m_generalizationSets = CustomSet<GeneralizationSet, Generalization>(this);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            TypedSet<Classifier, Generalization>& getGeneralSingleton();
            TypedSet<Classifier, Generalization>& getSpecificSingleton();
            Generalization();
        public:
            virtual ~Generalization();
            ClassifierPtr getGeneral() const;
            void setGeneral(Classifier* general);
            void setGeneral(Classifier& general);
            void setGeneral(ID id);
            ClassifierPtr getSpecific() const;
            void setSpecific(Classifier& specific);
            void setSpecific(Classifier* specific);
            void setSpecific(ID id);
            Set<GeneralizationSet, Generalization>& getGeneralizationSets();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION;
            };
    };
}
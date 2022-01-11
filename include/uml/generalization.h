#ifndef GENERALIZATION_H
#define GENERALIZATION_H

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {

    class Classifier;
    class GeneralizationSet;

    class Generalization : public DirectedRelationship {

        friend class UmlManager;
        friend class Classifier;

        protected:
            Singleton<Classifier, Generalization> m_general = Singleton<Classifier, Generalization>(this);
            Singleton<Classifier, Generalization> m_specific = Singleton<Classifier, Generalization>(this);
            Set<GeneralizationSet, Generalization> m_generalizationSets = Set<GeneralizationSet, Generalization>(this);
            class AddGeneralFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddGeneralFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveGeneralFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveGeneralFunctor(Element* el) : SetFunctor(el) {};
            };
            void referenceReindexed(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Set<Classifier, Generalization>& getGeneralSingleton();
            Set<Classifier, Generalization>& getSpecificSingleton();
            void init();
            void copy(const Generalization& rhs);
            Generalization();
        public:
            Generalization(const Generalization& rhs);
            virtual ~Generalization();
            Classifier* getGeneral();
            Classifier& getGeneralRef();
            ID getGeneralID() const;
            bool hasGeneral() const;
            void setGeneral(Classifier* general);
            void setGeneral(Classifier& general);
            void setGeneral(ID id);
            Classifier* getSpecific();
            Classifier& getSpecificRef();
            ID getSpecificID() const;
            bool hasSpecific() const;
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

#endif
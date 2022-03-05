#ifndef _UML_GENERALIZATION_H_
#define _UML_GENERALIZATION_H_

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {

    class Classifier;
    class GeneralizationSet;
    typedef UmlPtr<Classifier> ClassifierPtr;

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
            void restoreReference(Element* el) override;
            Set<Classifier, Generalization>& getGeneralSingleton();
            Set<Classifier, Generalization>& getSpecificSingleton();
            void init();
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

#endif
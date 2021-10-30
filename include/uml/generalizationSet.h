#ifndef UML_GENERALIZATION_SET_H
#define UML_GENERALIZATION_SET_H

#include "uml/packageableElement.h"
#include "uml/sequence.h"

namespace UML {

    class Classifier;
    class Generalization;

    class GeneralizationSet : public PackageableElement {

        friend class UmlManager;

        protected:
            Singleton<Classifier, GeneralizationSet> m_powerType = Singleton<Classifier, GeneralizationSet>(this);
            class RemovePowerTypeProcedure : public AbstractSingletonProcedure<Classifier, GeneralizationSet> {
                public:
                    RemovePowerTypeProcedure(GeneralizationSet* me) : AbstractSingletonProcedure<Classifier, GeneralizationSet>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class AddPowerTypeProcedure : public AbstractSingletonProcedure<Classifier, GeneralizationSet> {
                public:
                    AddPowerTypeProcedure(GeneralizationSet* me) : AbstractSingletonProcedure<Classifier, GeneralizationSet>(me) {};
                    void operator()(Classifier* el) const override;
            };
            Sequence<Generalization> m_generalizations = Sequence<Generalization>(this);
            class AddGeneralizationFunctor : public TemplateAbstractSequenceFunctor<Generalization,GeneralizationSet> {
                public:
                    AddGeneralizationFunctor(GeneralizationSet* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Generalization& el) const override;
            };
            class RemoveGeneralizationFunctor : public TemplateAbstractSequenceFunctor<Generalization,GeneralizationSet> {
                public:
                    RemoveGeneralizationFunctor(GeneralizationSet* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Generalization& el) const override;
            };
            void referenceReindexed(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            GeneralizationSet();
        public:
            GeneralizationSet(const GeneralizationSet& rhs);
            Classifier* getPowerType();
            Classifier& getPowerTypeRef();
            ID getPowerTypeID() const;
            bool hasPowerType() const;
            void setPowerType(Classifier* powerType);
            void setPowerType(Classifier& powerType);
            Sequence<Generalization>& getGeneralizations();
            bool isSubClassOf(ElementType eType) const override;
    };
}

#endif
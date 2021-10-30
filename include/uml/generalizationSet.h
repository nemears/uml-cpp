#ifndef UML_GENERALIZATION_SET_H
#define UML_GENERALIZATION_SET_H

#include "uml/packageableElement.h"

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
            GeneralizationSet();
        public:
            GeneralizationSet(const GeneralizationSet& rhs);
            Classifier* getPowerType();
            Classifier& getPowerTypeRef();
            ID getPowerTypeID() const;
            bool hasPowerType() const;
            void setPowerType(Classifier* powerType);
            void setPowerType(Classifier& powerType);
            bool isSubClassOf(ElementType eType) const override;
    };
}

#endif
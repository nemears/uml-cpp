#ifndef GENERALIZATION_H
#define GENERALIZATION_H

#include "directedRelationship.h"

namespace UML {

    class Classifier;

    class Generalization : public DirectedRelationship {

        friend class UmlManager;

        protected:
            Singleton<Classifier, Generalization> m_general = Singleton<Classifier, Generalization>(this);
            class RemoveGeneralProcedure : public AbstractSingletonProcedure<Classifier, Generalization> {
                public:
                    RemoveGeneralProcedure(Generalization* me) : AbstractSingletonProcedure<Classifier, Generalization>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class AddGeneralProcedure : public AbstractSingletonProcedure<Classifier, Generalization> {
                public:
                    AddGeneralProcedure(Generalization* me) : AbstractSingletonProcedure<Classifier, Generalization>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class RemoveSpecificProcedure : public AbstractSingletonProcedure<Classifier, Generalization> {
                public:
                    RemoveSpecificProcedure(Generalization* me) : AbstractSingletonProcedure<Classifier, Generalization>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class AddSpecificProcedure : public AbstractSingletonProcedure<Classifier, Generalization> {
                public:
                    AddSpecificProcedure(Generalization* me) : AbstractSingletonProcedure<Classifier, Generalization>(me) {};
                    void operator()(Classifier* el) const override;
            };
            Singleton<Classifier, Generalization> m_specific = Singleton<Classifier, Generalization>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
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
            Classifier* getSpecific();
            Classifier& getSpecificRef();
            ID getSpecificID() const;
            bool hasSpecific() const;
            void setSpecific(Classifier& specific);
            void setSpecific(Classifier* specific);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION;
            };
    };
}

#endif
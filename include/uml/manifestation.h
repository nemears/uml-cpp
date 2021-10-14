#ifndef MANIFESTATION_H
#define MANIFESTATION_H

#include "abstraction.h"

namespace UML {

    namespace Parsers {
        class ManifestationSetArtifact;
    }

    class Manifestation : public Abstraction {

        friend class UmlManager;
        friend class Parsers::ManifestationSetArtifact;

        protected:
            Singleton<PackageableElement, Manifestation> m_utilizedElement = Singleton<PackageableElement, Manifestation>(this);
            class RemoveUtilizedElementProcedure : public AbstractSingletonProcedure<PackageableElement, Manifestation> {
                public:
                    RemoveUtilizedElementProcedure(Manifestation* me) : AbstractSingletonProcedure<PackageableElement, Manifestation>(me) {};
                    void operator()(PackageableElement* el) const override;
            };
            class AddUtilizedElementProcedure : public AbstractSingletonProcedure<PackageableElement, Manifestation> {
                public:
                    AddUtilizedElementProcedure(Manifestation* me) : AbstractSingletonProcedure<PackageableElement, Manifestation>(me) {};
                    void operator()(PackageableElement* el) const override;
            };
            Singleton<Artifact, Manifestation> m_artifact = Singleton<Artifact, Manifestation>(this);
            class RemoveArtifactProcedure : public AbstractSingletonProcedure<Artifact, Manifestation> {
                public:
                    RemoveArtifactProcedure(Manifestation* me) : AbstractSingletonProcedure<Artifact, Manifestation>(me) {};
                    void operator()(Artifact* el) const override;
             };
            class AddArtifactProcedure : public AbstractSingletonProcedure<Artifact, Manifestation> {
                public:
                    AddArtifactProcedure(Manifestation* me) : AbstractSingletonProcedure<Artifact, Manifestation>(me) {};
                    void operator()(Artifact* el) const override;
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Manifestation();
        public:
            Manifestation(const Manifestation& manifestation);
            virtual ~Manifestation();
            PackageableElement* getUtilizedElement();
            PackageableElement& getUtilizedElementRef();
            bool hasUtilizedElement() const;
            void setUtilizedElement(PackageableElement& utilizedElement);
            void setUtilizedElement(PackageableElement* utilizedElement);
            Artifact* getArtifact();
            Artifact& getArtifactRef();
            ID getArtifactID() const;
            bool hasArtifact() const;
            void setArtifact(Artifact& artifact);
            void setArtifact(Artifact* artifact);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MANIFESTATION;
            };
    };
}

#endif
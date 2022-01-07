#ifndef UML_GENERALIZATION_SET_H
#define UML_GENERALIZATION_SET_H

#include "uml/packageableElement.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class Classifier;
    class Generalization;
    namespace Parsers {
        void parseGeneralizationSet(YAML::Node node, GeneralizationSet& generalizationSet, Parsers::ParserMetaData& data);
    }

    class GeneralizationSet : public PackageableElement {

        friend class UmlManager;
        friend class Classifier;
        friend void Parsers::parseGeneralizationSet(YAML::Node node, GeneralizationSet& generalizationSet, Parsers::ParserMetaData& data);

        protected:
            bool m_covering = false;
            bool m_disjoint = false;
            Singleton<Classifier, GeneralizationSet> m_powerType = Singleton<Classifier, GeneralizationSet>(this);
            Set<Generalization, GeneralizationSet> m_generalizations = Set<Generalization, GeneralizationSet>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Classifier, GeneralizationSet>& getPowerTypeSingleton();
            void init();
            void copy(const GeneralizationSet& rhs);
            GeneralizationSet();
        public:
            GeneralizationSet(const GeneralizationSet& rhs);
            bool isCovering() const;
            bool isDisjoint() const;
            void setCovering(bool covering);
            void setDisjoint(bool disjoint);
            Classifier* getPowerType();
            Classifier& getPowerTypeRef();
            ID getPowerTypeID() const;
            bool hasPowerType() const;
            void setPowerType(Classifier* powerType);
            void setPowerType(Classifier& powerType);
            Set<Generalization, GeneralizationSet>& getGeneralizations();
            bool isSubClassOf(ElementType eType) const override;
    };
}

#endif
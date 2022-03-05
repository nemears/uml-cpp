#ifndef _UML_GENERALIZATION_SET_H_
#define _UML_GENERALIZATION_SET_H_

#include "uml/packageableElement.h"

namespace UML {

    class Classifier;
    class Generalization;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class GeneralizationSet : public PackageableElement {

        friend class UmlManager;
        friend class Classifier;
        
        protected:
            bool m_covering = false;
            bool m_disjoint = false;
            Singleton<Classifier, GeneralizationSet> m_powerType = Singleton<Classifier, GeneralizationSet>(this);
            Set<Generalization, GeneralizationSet> m_generalizations = Set<Generalization, GeneralizationSet>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceErased(ID id) override;
            Set<Classifier, GeneralizationSet>& getPowerTypeSingleton();
            void init();
            GeneralizationSet();
        public:
            virtual ~GeneralizationSet();
            bool isCovering() const;
            bool isDisjoint() const;
            void setCovering(bool covering);
            void setDisjoint(bool disjoint);
            ClassifierPtr getPowerType() const;
            void setPowerType(Classifier* powerType);
            void setPowerType(Classifier& powerType);
            void setPowerType(ID id);
            Set<Generalization, GeneralizationSet>& getGeneralizations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::GENERALIZATION_SET;
            };
    };
}

#endif
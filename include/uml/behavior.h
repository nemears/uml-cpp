#ifndef _UML_BEHAVIOR_H_
#define _UML_BEHAVIOR_H_

#include "class.h"

namespace UML {

    class BehavioralFeature;
    typedef UmlPtr<BehavioralFeature> BehavioralFeaturePtr;

    class Behavior : public Class {

        friend class UmlManager;
        friend class BehavioralFeature;

        protected:
            Set<Parameter, Behavior> m_ownedParameters = Set<Parameter, Behavior>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            void referencingReleased(ID id) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<BehavioralFeature, Behavior>& getSpecificationSingleton();
            void init();
            Behavior();
        public:
            virtual ~Behavior();
            Set<Parameter, Behavior>& getOwnedParameters();
            BehavioralFeaturePtr getSpecification() const;
            void setSpecification(BehavioralFeature& specification);
            void setSpecification(BehavioralFeature* specification);
            void setSpecification(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

#endif
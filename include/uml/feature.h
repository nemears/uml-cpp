#ifndef _UML_FEATURE_H_
#define _UML_FEATURE_H_

#include "redefinableElement.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;

        protected:
            Singleton<Classifier, Feature> m_featuringClassifier = Singleton<Classifier, Feature>(this);
            bool m_static = false;
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<Classifier, Feature>& getFeaturingClassifierSingleton();
            void init();
            Feature();
        public:
            ClassifierPtr getFeaturingClassifier();
            void setFeaturingClassifier(Classifier* clazz);
            void setFeaturingClassifier(Classifier& clazz);
            void setFeaturingClassifier(ID id);
            bool isStatic();
            void setStatic(bool isStatic);
            bool isSubClassOf(ElementType eType) const override;
            Feature& operator=(Feature&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::FEATURE;
            };
    };
}

#endif
#ifndef FEATURE_H
#define FEATURE_H

#include "redefinableElement.h"

namespace UML {

    class Classifier;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;

        protected:
            Singleton<Classifier, Feature> m_featuringClassifier = Singleton<Classifier, Feature>(this);
            bool m_static = false;
            // void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Classifier, Feature>& getFeaturingClassifierSingleton();
            void init();
            void copy(const Feature& rhs);
            Feature();
        public:
            Feature(const Feature& feature);
            Classifier* getFeaturingClassifier();
            Classifier& getFeaturingClassifierRef();
            ID getFeaturingClassifierID() const;
            bool hasFeaturingClassifier() const;
            void setFeaturingClassifier(Classifier* clazz);
            void setFeaturingClassifier(Classifier& clazz);
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
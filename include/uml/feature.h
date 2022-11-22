#ifndef _UML_FEATURE_H_
#define _UML_FEATURE_H_

#include "redefinableElement.h"
#include "set/singleton.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;

        protected:
            CustomSingleton<Classifier, Feature> m_featuringClassifier = CustomSingleton<Classifier, Feature>(this);
            bool m_static = false;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<Classifier, Feature>& getFeaturingClassifierSingleton();
            void init();
            Feature();
        public:
            ClassifierPtr getFeaturingClassifier() const;
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
#ifndef FEATURE_H
#define FEATURE_H

#include "redefinableElement.h"

namespace UML {

    class Classifier;

    class Feature : virtual public RedefinableElement {
        protected:
            Classifier* m_featuringClassifier;
            bool m_static;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Feature();
            Feature(const Feature& feature);
            Classifier* getFeaturingClassifier();
            void setFeaturingClassifier(Classifier* clazz);
            bool isStatic();
            void setStatic(bool isStatic);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            Feature& operator=(Feature&&) {
                return *this;
            };
    };
}

#endif
#ifndef FEATURE_H
#define FEATURE_H

#include "redefinableElement.h"

namespace UML {

    class Classifier;

    class Feature : virtual public RedefinableElement {
        protected:
            ID m_featuringClassifierID;
            Classifier* m_featuringClassifierPtr;
            bool m_static;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            Feature();
            Feature(const Feature& feature);
            Classifier* getFeaturingClassifier();
            void setFeaturingClassifier(Classifier* clazz);
            bool isStatic();
            void setStatic(bool isStatic);
            ElementType getElementType() const override;
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
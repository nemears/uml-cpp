#ifndef FEATURE_H
#define FEATURE_H

#include "redefinableElement.h"

namespace UML {

    class Classifier;

    class Feature : virtual public RedefinableElement {
        protected:
            Singleton<Classifier, Feature> m_featuringClassifier = Singleton<Classifier, Feature>(this);
            class RemoveFeaturingClassifierProcedure : public AbstractSingletonProcedure<Classifier, Feature> {
                public:
                    RemoveFeaturingClassifierProcedure(Feature* me) : AbstractSingletonProcedure<Classifier, Feature>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class AddFeaturingClassifierProcedure : public AbstractSingletonProcedure<Classifier, Feature> {
                public:
                    AddFeaturingClassifierProcedure(Feature* me) : AbstractSingletonProcedure<Classifier, Feature>(me) {};
                    void operator()(Classifier* el) const override;
            };
            bool m_static;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID);
            Feature();
        public:
            Feature(const Feature& feature);
            Classifier* getFeaturingClassifier();
            Classifier& getFeaturingClassifierRef();
            bool hasFeaturingClassifier() const;
            void setFeaturingClassifier(Classifier* clazz);
            void setFeaturingClassifier(Classifier& clazz);
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
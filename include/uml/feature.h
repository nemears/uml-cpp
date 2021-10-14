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
                    void operator()(ID id) const override;
            };
            bool m_static;
            void reindexName(std::string oldName, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
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
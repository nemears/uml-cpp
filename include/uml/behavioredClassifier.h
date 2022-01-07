#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class Behavior;
    namespace Parsers {
        void parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, Parsers::ParserMetaData& data);
    }

    class BehavioredClassifier : virtual public Classifier {

        friend void Parsers::parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, Parsers::ParserMetaData& data);

        protected:
            Set<Behavior, BehavioredClassifier> m_ownedBehaviors = Set<Behavior, BehavioredClassifier>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Behavior, BehavioredClassifier>& getClassifierBehaviorSingleton();
            void init();
            void copy(const BehavioredClassifier& rhs);
            BehavioredClassifier();
        public:
            BehavioredClassifier(const BehavioredClassifier& rhs);
            virtual ~BehavioredClassifier();
            Set<Behavior, BehavioredClassifier>& getOwnedBehaviors();
            Behavior* getClassifierBehavior();
            Behavior& getClassifierBehaviorRef();
            ID getClassifierBehaviorID() const;
            bool hasClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(Behavior* behavior);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif
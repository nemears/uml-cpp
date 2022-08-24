#ifndef CREATE_OBJECT_ACTION_H
#define CREATE_OBJECT_ACTION_H

#include "action.h"
#include "classifier.h"

namespace UML {
    class CreateObjectAction : public Action {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Classifier* classifier;
            CreateObjectAction();
        public:
            CreateObjectAction(const CreateObjectAction& rhs);
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CREATE_OBJECT_ACTION;
            };
    };
}

#endif
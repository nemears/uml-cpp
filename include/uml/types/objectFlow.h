#pragma once

#include "activityEdge.h"

namespace UML {

    class Behavior;

    typedef UmlPtr<Behavior> BehaviorPtr;

    class ObjectFlow : public ActivityEdge {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Behavior, ObjectFlow> m_transformation = CustomSingleton<Behavior, ObjectFlow>(this);
            CustomSingleton<Behavior, ObjectFlow> m_selection = CustomSingleton<Behavior, ObjectFlow>(this);
            TypedSet<Behavior, ObjectFlow>& getTransformationSingleton();
            TypedSet<Behavior, ObjectFlow>& getSelectionSingleton();
            void referenceErased(ID id) override;
            ObjectFlow();
        public:
            virtual ~ObjectFlow();
            BehaviorPtr getTransformation() const;
            void setTransformation(Behavior* transformation);
            void setTransformation(Behavior& transformation);
            void setTransformation(BehaviorPtr transformation);
            void setTransformation(ID id);
            BehaviorPtr getSelection() const;
            void setSelection(Behavior* selection);
            void setSelection(Behavior& selection);
            void setSelection(ID id);
            void setSelection(BehaviorPtr selection);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_FLOW;
            };
    };
}
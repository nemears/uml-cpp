#ifndef _UML_OBJECT_FLOW_H_
#define _UML_OBJECT_FLOW_H_

#include "activityEdge.h"

namespace UML {

    class Behavior;

    typedef UmlPtr<Behavior> BehaviorPtr;

    class ObjectFlow : public ActivityEdge {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Behavior, ObjectFlow> m_transformation = CustomSingleton<Behavior, ObjectFlow>(this);
            CustomSingleton<Behavior, ObjectFlow> m_selection = CustomSingleton<Behavior, ObjectFlow>(this);
            TypedSet<Behavior, ObjectFlow>& getTransformationSingleton();
            TypedSet<Behavior, ObjectFlow>& getSelectionSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
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

#endif
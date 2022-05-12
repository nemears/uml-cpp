#ifndef _UML_OBJECT_FLOW_H_
#define _UML_OBJECT_FLOW_H_

#include "activityEdge.h"

namespace UML {

    class Behavior;

    typedef UmlPtr<Behavior> BehaviorPtr;

    class ObjectFlow : public ActivityEdge {

        friend class UmlManager;

        protected:
            Singleton<Behavior, ObjectFlow> m_transformation = Singleton<Behavior, ObjectFlow>(this);
            Singleton<Behavior, ObjectFlow> m_selection = Singleton<Behavior, ObjectFlow>(this);
            Set<Behavior, ObjectFlow>& getTransformationSingleton();
            Set<Behavior, ObjectFlow>& getSelectionSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            void init();
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
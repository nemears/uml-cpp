#ifndef _UML_ACTIVITY_PARAMETER_NODE_H_
#define _UML_ACTIVITY_PARAMETER_NODE_H_

#include "objectNode.h"
#include "parameter.h"

namespace UML {

    typedef UmlPtr<Parameter> ParameterPtr;

    class ActivityParameterNode : public ObjectNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Parameter, ActivityParameterNode> m_parameter = CustomSingleton<Parameter, ActivityParameterNode>(this);
            TypedSet<Parameter, ActivityParameterNode>& getParameterSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            ActivityParameterNode();
        public:
            virtual ~ActivityParameterNode();
            ParameterPtr getParameter() const;
            void setParameter(Parameter* parameter);
            void setParameter(Parameter& parameter);
            void setParameter(ParameterPtr parameter);
            void setParameter(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_PARAMETER_NODE;
            };
    };
}

#endif
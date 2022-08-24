#ifndef _UML_ACTIVITY_PARAMETER_NODE_H_
#define _UML_ACTIVITY_PARAMETER_NODE_H_

#include "objectNode.h"
#include "parameter.h"

namespace UML {

    typedef UmlPtr<Parameter> ParameterPtr;

    class ActivityParameterNode : public ObjectNode {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Parameter, ActivityParameterNode> m_parameter = Singleton<Parameter, ActivityParameterNode>(this);
            Set<Parameter, ActivityParameterNode>& getParameterSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void reindexName(ID id, std::string newName) override;
            void init();
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
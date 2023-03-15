#ifndef _UML_PARAMETER_SET_H_
#define _UML_PARAMETER_SET_H_

#include "namedElement.h"
#include "constraint.h"

namespace UML {

    class Parameter;

    class ParameterSet : public NamedElement {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

        protected:
            CustomSet<Constraint, ParameterSet> m_conditions = CustomSet<Constraint, ParameterSet>(this);
            CustomSet<Parameter, ParameterSet> m_parameters = CustomSet<Parameter, ParameterSet>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            ParameterSet();
        public:
            virtual ~ParameterSet();
            Set<Constraint, ParameterSet>& getConditions();
            Set<Parameter, ParameterSet>& getParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER_SET;
            };
    };

}

#endif
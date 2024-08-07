#pragma once

#include "namedElement.h"
#include "constraint.h"

namespace UML {

    class Parameter;

    class ParameterSet : public NamedElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Constraint, ParameterSet> m_conditions = Set<Constraint, ParameterSet>(this);
            Set<Parameter, ParameterSet> m_parameters = Set<Parameter, ParameterSet>(this);
            void referenceErased(ID id) override;
            ParameterSet();
        public:
            virtual ~ParameterSet();
            Set<Constraint, ParameterSet>& getConditions();
            Set<Parameter, ParameterSet>& getParameters();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER_SET;
            };
    };

}

#pragma once

#include "namedElement.h"
#include "constraint.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class Parameter;

    class ParameterSet : public NamedElement {

        friend struct ElementInfo<ParameterSet>;

        protected:
            Set<Constraint, ParameterSet> m_conditions = Set<Constraint, ParameterSet>(this);
            Set<Parameter, ParameterSet> m_parameters = Set<Parameter, ParameterSet>(this);
        public:
            ParameterSet(std::size_t elementType, AbstractManager& manager);
            Set<Constraint, ParameterSet>& getConditions();
            Set<Parameter, ParameterSet>& getParameters();
            typedef TypeInfo<std::tuple<NamedElement>, ParameterSet> Info;
    };

    template <>
    struct ElementInfo<ParameterSet> {
        static const bool abstract = false;
        inline static const std::string name { "ParameterSet" };
        static SetList sets(ParameterSet& el) {
            return SetList {
                makeSetPair("conditions", el.m_conditions),
                makeSetPair("parameters", el.m_parameters)
            };
        }
    };
}

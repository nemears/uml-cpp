#ifndef _UML_PARAMETER_SET_H_
#define _UML_PARAMETER_SET_H_

#include "namedElement.h"
#include "constraint.h"

namespace UML {

    class Parameter;

    class ParameterSet : public NamedElement {

        friend class UmlManager;

        protected:
            Set<Constraint, ParameterSet> m_conditions = Set<Constraint, ParameterSet>(this);
            Set<Parameter, ParameterSet> m_parameters = Set<Parameter, ParameterSet>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
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
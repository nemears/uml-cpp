#ifndef _UML_PORT_H_
#define _UML_PORT_H_

#include "property.h"

namespace UML {
    class Port : public Property {
        protected:
            bool m_isBehavior = false;
            bool m_isConjugated = false;
            bool m_isService = true;
            void copy(const Port& rhs);
            Port();
        public:
            virtual ~Port();
            Port(const Port& rhs);
            bool isBehavior() const;
            void setIsBehavior(bool isBehavior);
            bool isConjugated() const;
            void setIsConjugated(bool isConjugated);
            bool isService() const;
            void setIsService(bool isService);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PORT;
            }
    };
}

#endif
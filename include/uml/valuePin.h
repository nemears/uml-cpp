#ifndef _UML_VALUE_PIN_H_
#define _UML_VALUE_PIN_H_

#include "inputPin.h"

namespace UML {
    class ValuePin : public InputPin {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<ValueSpecification, ValuePin> m_value = CustomSingleton<ValueSpecification, ValuePin>(this);
            TypedSet<ValueSpecification, ValuePin>& getValueSingleton();
            ValuePin();
        public:
            virtual ~ValuePin();
            ValueSpecificationPtr getValue() const;
            void setValue(ValueSpecification* value);
            void setValue(ValueSpecification& value);
            void setValue(ValueSpecificationPtr value);
            void setValue(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::VALUE_PIN;
            };
    };
}

#endif
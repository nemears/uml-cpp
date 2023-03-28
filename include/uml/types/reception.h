#ifndef _UML_RECEPTION_H_
#define _UML_RECEPTION_H_

#include "behavioralFeature.h"
#include "signal.h"

namespace UML {

    class Signal;
    typedef UmlPtr<Signal> SignalPtr;

    class Reception : public BehavioralFeature {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Class;

        protected:
            CustomSingleton<Signal, Reception> m_signal = CustomSingleton<Signal, Reception>(this);
            TypedSet<Signal, Reception>& getSignalSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            Reception();
        public:
            virtual ~Reception();
            SignalPtr getSignal() const;
            void setSignal(Signal* signal);
            void setSignal(Signal& signal);
            void setSignal(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::RECEPTION;
            };
    };
}

#endif
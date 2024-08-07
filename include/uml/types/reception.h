#pragma once

#include "behavioralFeature.h"

namespace UML {

    class Signal;
    typedef UmlPtr<Signal> SignalPtr;

    class Reception : public BehavioralFeature {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Class;

        protected:
            Singleton<Signal, Reception> m_signal = Singleton<Signal, Reception>(this);
            Singleton<Signal, Reception>& getSignalSingleton();
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Reception();
        public:
            virtual ~Reception();
            SignalPtr getSignal() const;
            void setSignal(Signal* signal);
            void setSignal(Signal& signal);
            void setSignal(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::RECEPTION;
            };
    };
}

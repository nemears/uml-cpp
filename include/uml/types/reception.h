#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class BehavioralFeature;

    template <class>
    class Signal;

    template <class ManagerPolicy>
    class Reception : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Reception, EGM::TemplateTypeList<BehavioralFeature>>;
        protected:
            using SignalSingleton = EGM::Singleton<Signal, Reception>;
            SignalSingleton m_signal = SignalSingleton(this);
        private:
            void init() {
                m_signal.opposite(&SignalSingleton::ManagedType::getReceptions);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Reception);

            SignalSingleton& getSignalSingleton() { return m_signal; }
            using SignalImpl = typename SignalSingleton::ManagedType;
            using SignalPtr = EGM::ManagedPtr<SignalImpl>;
            SignalPtr getSignal() const { return m_signal.get(); }
            void setSignal(SignalImpl& el) { m_signal.set(el); }
            void setSignal(SignalPtr el) { m_signal.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Reception> {
        static std::string name() { return "Reception"; }
        template <class Policy>
        static SetList sets(UML::Reception<Policy>& el) {
            return SetList {
                make_set_pair("signal", el.getSignalSingleton())
            };
        }
    };
}

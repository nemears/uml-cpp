#pragma once

#include "behavioralFeature.h"
#include "uml/types/element.h"

namespace UML {

    class Signal;
    typedef UmlPtr<Signal> SignalPtr;

    class Reception : public BehavioralFeature {

        friend class Class;
        friend struct ElementInfo<Reception>;

        protected:
            Singleton<Signal, Reception> m_signal = Singleton<Signal, Reception>(this);
            Singleton<Signal, Reception>& getSignalSingleton();
        public:
            Reception(std::size_t elementType, AbstractManager& manager);
            SignalPtr getSignal() const;
            void setSignal(SignalPtr signal);
            void setSignal(Signal& signal);
            void setSignal(ID id);
            typedef TypeInfo<std::tuple<BehavioralFeature>, Reception> Info;
    };

    template <>
    struct ElementInfo<Reception> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"Reception"};
        static SetList sets(Reception& el) {
            return SetList {
                makeSetPair("signal", el.m_signal),
            };
        }
    };
}

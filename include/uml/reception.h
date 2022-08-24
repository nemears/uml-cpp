#ifndef _UML_RECEPTION_H_
#define _UML_RECEPTION_H_

#include "behavioralFeature.h"
#include "signal.h"

namespace UML {

    class Signal;
    typedef UmlPtr<Signal> SignalPtr;

    class Reception : public BehavioralFeature {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Signal, Reception> m_signal = Singleton<Signal, Reception>(this);
            Set<Signal, Reception>& getSignalSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
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
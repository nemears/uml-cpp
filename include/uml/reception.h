#ifndef _UML_RECEPTION_H_
#define _UML_RECEPTION_H_

#include "behavioralFeature.h"

namespace UML {

    class Signal;

    class Reception : public BehavioralFeature {

        friend class UmlManager;

        protected:
            Singleton<Signal, Reception> m_signal = Singleton<Signal, Reception>(this);
            Set<Signal, Reception>& getSignalSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Reception& rhs);
            Reception();
        public:
            Reception(const Reception& rhs);
            virtual ~Reception();
            Signal* getSignal();
            Signal& getSignalRef();
            bool hasSignal() const;
            ID getSignalID() const;
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
#include "uml/set/abstractSet.h"
#include "uml/set/privateSet.h"
#include <iostream>

namespace UML {
    std::ostream& operator<<(std::ostream& stream, const SetType& setType) {
        switch (setType) {
            case SetType::SET : return stream << "Set";
            case SetType::SINGLETON : return stream << "Singleton";
            case SetType::ORDERED_SET : return stream << "Ordered Set";
            case SetType::LIST : return stream << "List";
            case SetType::BAG : return stream << "Bag";
        }
        throw SetStateException("impossible");
    }
    AbstractSet::AbstractSet() {
        m_structure = std::make_shared<SetStructure>(*this);
        m_structure->m_rootRedefinedSet = m_structure;
    }
    AbstractSet::~AbstractSet() {
        for (auto subSet : m_structure->m_subSets) {
            auto superSetsIt = subSet->m_superSets.begin();
            while (
                    superSetsIt != subSet->m_superSets.end() &&
                    (*superSetsIt).get() != m_structure->m_rootRedefinedSet.get()) 
            {
                superSetsIt++;
            }
            if (superSetsIt != subSet->m_superSets.end()) {
                subSet->m_superSets.erase(superSetsIt);
            }
        }
        for (auto superSet : m_structure->m_superSets) {
            auto subSetsIt = superSet->m_subSets.begin();
            while (
                subSetsIt != superSet->m_subSets.end() &&
                (*subSetsIt).get() != m_structure->m_rootRedefinedSet.get()        
            ) {
                subSetsIt++;
            }
            if (subSetsIt != superSet->m_subSets.end()) {
                superSet->m_subSets.erase(subSetsIt);
            }
            
            auto subSetsWithDataIt = superSet->m_subSetsWithData.begin();
            while (
                subSetsWithDataIt != superSet->m_subSetsWithData.end() &&
                (*subSetsWithDataIt).get() != m_structure.get()
            ) {
                subSetsWithDataIt++;
            }
            if (subSetsWithDataIt != superSet->m_subSetsWithData.end()) {
                superSet->m_subSetsWithData.erase(subSetsWithDataIt);
            }
        }
        for (auto redefinedSet : m_structure->m_redefinedSets) {
            redefinedSet->m_rootRedefinedSet.reset();
        }
        m_structure->m_subSets.clear();
        m_structure->m_superSets.clear();
        m_structure->m_redefinedSets.clear();
        m_structure->m_subSetsWithData.clear();
        m_structure->m_rootRedefinedSet.reset();
        m_structure.reset();
    }
    void AbstractSet::subsets(AbstractSet& superSet) {
        auto rootRedefinedSet = m_structure->m_rootRedefinedSet;
        auto superSetRootRedefinedSet = superSet.m_structure->m_rootRedefinedSet;
        superSetRootRedefinedSet->m_subSets.insert(rootRedefinedSet);
        rootRedefinedSet->m_superSets.insert(superSetRootRedefinedSet);
        if (superSetRootRedefinedSet->m_composition != CompositionType::NONE) {
            rootRedefinedSet->m_composition = superSetRootRedefinedSet->m_composition;
        }
    }
    void AbstractSet::redefines(AbstractSet& redefinedSet) {
        for (std::shared_ptr<SetStructure> superSet : m_structure->m_rootRedefinedSet->m_superSets) {
            redefinedSet.m_structure->m_rootRedefinedSet->m_superSets.insert(superSet);
        }
        for (std::shared_ptr<SetStructure> subSet : m_structure->m_rootRedefinedSet->m_subSets) {
            redefinedSet.m_structure->m_rootRedefinedSet->m_subSets.insert(subSet);
        }
        for (std::shared_ptr<SetStructure> alreadyRedefinedSet : m_structure->m_rootRedefinedSet->m_redefinedSets) {
            redefinedSet.m_structure->m_rootRedefinedSet->m_redefinedSets.insert(alreadyRedefinedSet);
        }
        m_structure->m_rootRedefinedSet->m_superSets.clear();
        m_structure->m_rootRedefinedSet->m_subSets.clear();
        m_structure->m_rootRedefinedSet->m_redefinedSets.clear();
        redefinedSet.m_structure->m_rootRedefinedSet->m_redefinedSets.insert(m_structure->m_rootRedefinedSet);
        m_structure->m_rootRedefinedSet = redefinedSet.m_structure->m_rootRedefinedSet;
    }
    void AbstractSet::setComposition(CompositionType composition) {
        m_structure->m_composition = composition;
    }
    CompositionType AbstractSet::getComposition() const {
        return m_structure->m_rootRedefinedSet->m_composition;
    }
    size_t AbstractSet::size() const {
        return m_structure->m_size;
    }
    bool AbstractSet::empty() const {
        return m_structure->m_size == 0;
    }
    IDSet AbstractSet::ids() const {
        return IDSet(this);
    }
}

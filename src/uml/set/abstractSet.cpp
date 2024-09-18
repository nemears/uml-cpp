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
        auto redefinedStructure = redefinedSet.m_structure->m_rootRedefinedSet;
        for (auto superSet : redefinedStructure->m_superSets) {
            m_structure->m_superSets.insert(superSet);
            superSet->m_subSets.erase(redefinedStructure);
            superSet->m_subSets.insert(m_structure);
        }
        for (auto subSet : redefinedStructure->m_subSets) {
            m_structure->m_subSets.insert(subSet);
            subSet->m_superSets.erase(redefinedStructure);
            subSet->m_superSets.insert(m_structure);
        }
        for (auto redefinedSetRedefinedSet : redefinedStructure->m_redefinedSets) {
            m_structure->m_redefinedSets.insert(redefinedSetRedefinedSet);
            redefinedSetRedefinedSet->m_rootRedefinedSet = m_structure;
            redefinedSetRedefinedSet->m_redefinedSets.erase(redefinedStructure);
            redefinedSetRedefinedSet->m_redefinedSets.insert(m_structure);
        }
        m_structure->m_redefinedSets.insert(redefinedStructure);
        redefinedStructure->m_superSets.clear();
        redefinedStructure->m_subSets.clear();
        redefinedStructure->m_redefinedSets.clear();
        redefinedStructure->m_rootRedefinedSet = m_structure;
        m_structure->m_rootRedefinedSet = m_structure;
        if (m_structure->m_composition != CompositionType::NONE) {
            redefinedSet.setComposition(m_structure->m_composition);
        } else if (redefinedStructure->m_composition != CompositionType::NONE) {
            this->setComposition(redefinedStructure->m_composition);
        }
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
    bool AbstractSet::rootSet() const {
        return m_structure->m_rootRedefinedSet == m_structure;
    }
    AbstractSet* AbstractSet::subSetContains(ID id) const {
        for (auto subSet : m_structure->m_rootRedefinedSet->m_subSets) {
            if (subSet->m_set.contains(id)) {
                return &subSet->m_set;
            }
        }
        return 0;
    }
    IDSet AbstractSet::ids() const {
        return IDSet(this);
    }
}

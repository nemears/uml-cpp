#pragma once

#include "abstractSet.h"
#include "wrapperSet.h"
#include "uml/set/doNothingPolicy.h"
#include "uml/types/element.h"
#include "uml/umlPtr.h"
#include <memory>
#include <mutex>
#include <unordered_set>

namespace UML {
    class SetStateException : public std::exception {
        std::string m_msg;
        public:
            SetStateException(){};
            SetStateException(std::string msg) : m_msg("Error in Uml Set: " + msg) {};
            const char* what() const throw() override {
                return m_msg.c_str();
            };
    };
    template <class T>
    class OppositeInterface {
        public:
            virtual ~OppositeInterface() = default;
            virtual bool enabled() = 0;
            virtual void addOpposite(T& el) = 0;
            virtual void removeOpposite(T& el) = 0;
            bool skip = false; // unused rn
    };

    template <class T>
    class NoOpposite : public OppositeInterface<T> {
        public:
            bool enabled() override {
                return false;
            }
            void addOpposite(__attribute__((unused)) T& el) override {}
            void removeOpposite(__attribute__((unused)) T& el) override {}
    };


    template <class T,  class U, class DataTypePolicy, class ApiPolicy>
    class PrivateSet : virtual public AbstractSet , virtual public DataTypePolicy, protected ApiPolicy {

        friend class Element;
        friend class NamedElement;
        // TODO do we need these friends? we can do it without by moving the sets around and redefinition
        friend class Package;
        friend class Property;
        friend class Class;
        friend class Operation;
        friend class BehavioredClassifier;
        friend class Usage;
        friend class Connector;
        friend class InterfaceRealization;
        friend class Reception;
        friend class Manifestation;
        friend class ElementImport;
        friend class PackageImport;
        friend T;
        friend U;
        friend ApiPolicy;
        template <class V, class W, class OtherDataTypePolicy, class OtherApiPolicy>
        friend class PrivateSet;

        private:
            std::mutex m_mutex;
        protected:
            U& m_el;
            std::unique_ptr<OppositeInterface<T>> m_opposite = std::unique_ptr<NoOpposite<T>>(new NoOpposite<T>());

            void runAddPolicy(Element& el) override {
                ApiPolicy::elementAdded(el.as<T>(), m_el);
            }
            void runRemovePolicy(Element& el) override {
                ApiPolicy::elementRemoved(el.as<T>(), m_el);
            }
            bool oppositeEnabled() const override {
                return m_opposite->enabled();
            }
            void oppositeAdd(Element& el) override {
                m_opposite->addOpposite(el.as<T>());
            }
            void oppositeRemove(Element& el) override {
                m_opposite->removeOpposite(el.as<T>());
            }
            bool hasData() const override {
                return DataTypePolicy::hasData();
            }
            bool containsData(ElementPtr ptr) const override {
                return DataTypePolicy::containsData(ptr);
            }
            bool removeData(ElementPtr ptr) override {
                return DataTypePolicy::removeData(ptr);
            }
            void innerAdd(ElementPtr ptr) override {
                auto rootRedefinedSet = m_structure->m_rootRedefinedSet;
                if (rootRedefinedSet.get() != m_structure.get()) {
                    return rootRedefinedSet->m_set.innerAdd(ptr);
                }

                std::lock_guard<std::mutex> setLock(m_mutex);

                nonOppositeAdd(ptr);
               
                // increase size, run policies, and opposite
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        bool oppositeRan = false;
                        if (ptr.loaded()) {
                            if (!oppositeRan && front->m_set.oppositeEnabled()) {
                                front->m_set.oppositeAdd(*ptr);
                                oppositeRan = true;
                            }
                        }
                        for (std::shared_ptr<SetStructure> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                if (!oppositeRan && front->m_set.oppositeEnabled()) {
                                    front->m_set.oppositeAdd(*ptr);
                                    oppositeRan = true;
                                }
                            }
                        }
                        if (!oppositeRan) {
                            for (std::shared_ptr<SetStructure> superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }
                m_el.m_node->setReference(&ptr);
                ptr.m_node->setReference(m_el.getID(), m_el.m_node);
            }
            void nonOppositeAdd(ElementPtr ptr) override {

                auto rootRedefinedSet = m_structure->m_rootRedefinedSet;

                // look at supersets to allocate data
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->m_set.allocatePtr(ptr, *rootRedefinedSet);
                        for (auto superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }

                // add data via policy
                DataTypePolicy::addData(ptr);

                // mark superSets that we have data
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    for (auto superSet : rootRedefinedSet->m_superSets) {
                        queue.push_back(superSet);
                    }
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->m_subSetsWithData.insert(rootRedefinedSet);
                        if (!front->m_set.hasData()) {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }
                
                // increase size, run policies 
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->m_size++;
                        if (ptr.loaded()) {
                            front->m_set.runAddPolicy(*ptr);
                        }
                        for (auto redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                redefinedSet->m_set.runAddPolicy(*ptr);
                            }
                            redefinedSet->m_size++;
                        }
                        for (auto superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
            }
            void innerRemove(ElementPtr ptr) override {
                auto rootRedefinedSet = m_structure->m_rootRedefinedSet;
                if (rootRedefinedSet.get() != m_structure.get()) {
                    return rootRedefinedSet->m_set.innerRemove(ptr);
                }

                std::lock_guard<std::mutex> setLock(m_mutex);
                nonOppositeRemove(ptr);

                // run opposite
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        bool oppositeRan = false;
                        if (ptr.loaded()) {
                            if (!oppositeRan && front->m_set.oppositeEnabled()) {
                                front->m_set.oppositeRemove(*ptr);
                                oppositeRan = true;
                            }
                        }
                        for (auto redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                if (!oppositeRan && front->m_set.oppositeEnabled()) {
                                    redefinedSet->m_set.oppositeRemove(*ptr);
                                    oppositeRan = true;
                                }
                            }
                        }
                        if (!oppositeRan) {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }

                m_el.m_node->removeReference(&ptr);
            }
            void weakRemove(ElementPtr ptr) override {
                if (contains(ptr)) {
                    innerRemove(ptr);
                }
            }
            void nonOppositeRemove(ElementPtr ptr) override {
                auto rootRedefinedSet = m_structure->m_rootRedefinedSet;
                // look at supersets to deallocate data
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->m_set.deAllocatePtr(ptr);
                        for (auto superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
                
                // remove
                std::shared_ptr<SetStructure> setwithEl;
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    queue.push_back(rootRedefinedSet);
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (front->m_set.containsData(ptr)) {
                            setwithEl = front;
                            front->m_set.removeData(ptr);
                            if (!front->m_set.hasData()) {
                                // remove us from all super sets m_subSetsWithData unordered_set
                                std::list<std::shared_ptr<SetStructure>> adjustQueue;
                                for (auto superSet : front->m_superSets) {
                                    adjustQueue.push_back(superSet);
                                }
                                while (!adjustQueue.empty()) {
                                    auto adjustFront = adjustQueue.front();
                                    adjustQueue.pop_front();
                                    adjustFront->m_subSetsWithData.erase(front);
                                    if (!adjustFront->m_set.hasData()) {
                                        for (auto adjustSuperSet : adjustFront->m_superSets) {
                                            adjustQueue.push_back(adjustSuperSet);
                                        }
                                    }
                                }
                            }
                        } else {
                            for (auto subSet : front->m_subSets) {
                                queue.push_back(subSet);
                            }
                        }
                    }
                }

                // run removal policies adjust size
                {
                    std::list<std::shared_ptr<SetStructure>> queue;
                    std::unordered_set<std::shared_ptr<SetStructure>> visited;
                    queue.push_back(setwithEl);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (ptr.loaded()) {
                            front->m_set.runRemovePolicy(*ptr);
                        }
                        front->m_size--;
                        for (auto redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                redefinedSet->m_set.runRemovePolicy(*ptr);
                            }
                            redefinedSet->m_size--;
                        }
                        for (auto superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }

            }
            struct PtrPolicy {
                UmlPtr<T> get(ElementPtr ptr) {
                    return UmlPtr<T>(ptr);    
                }
            };
            typedef WrapperSet<UmlPtr<T>, PtrPolicy> PtrSet;
        public:
            PrivateSet(U* el) : m_el(*el) {}
            PrivateSet& operator=(PrivateSet& rhs) = delete;
            PrivateSet(PrivateSet& rhs) = delete;
            virtual ~PrivateSet() {}
            bool contains(ElementPtr ptr) const override {
                if (m_structure->m_rootRedefinedSet.get() != m_structure.get()) {
                    return m_structure->m_rootRedefinedSet->m_set.contains(ptr);
                }
                if (containsData(ptr)) {
                    return true;
                }
                for (auto setWithData : m_structure->m_subSetsWithData) {
                    if (setWithData->m_set.contains(ptr)) {
                        return true;
                    }
                }
                return false;
            }
            bool contains(ID id) const {
                return contains(m_el.m_manager->createPtr(id));
            }
            bool contains(T& el) const {
                return contains(UmlPtr<T>(&el));
            }
            template <class S>
            void opposite(S& (T::*oppositeSignature)()) {
                class OppositeInterfaceAdapter : public OppositeInterface<T> {
                    public:
                        S& (T::*signature)() = 0;
                        U& me;
                        OppositeInterfaceAdapter(U& u, S& (T::*sig)()) : me(u) {
                            signature = sig;
                        }
                        bool enabled() override {
                            return true;
                        }
                        void addOpposite(T& el) override {
                            (el.*signature)().nonOppositeAdd(UmlPtr<U>(&me));
                        }
                        void removeOpposite(T& el) override {
                            (el.*signature)().nonOppositeRemove(UmlPtr<U>(&me));
                        }
                };
                m_opposite = std::unique_ptr<OppositeInterfaceAdapter>(new OppositeInterfaceAdapter(m_el, oppositeSignature));
            }
            PtrSet ptrs() const {
                return PtrSet(this);
            }
            IDSet ids() const {
                return IDSet(this);
            }
    };
}

#pragma once

#include "abstractSet.h"
#include "uml/set/doNothingPolicy.h"
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
        template <class V, class W, class OtherDataTypePolicy, class OtherApiPolicy>
        friend class PrivateSet;

        private:
            std::mutex m_mutex;
        protected:
            U& m_el;
            std::unique_ptr<OppositeInterface<T>> m_opposite = std::unique_ptr<NoOpposite<T>>(new NoOpposite<T>());

            void allocatePtr(__attribute__((unused)) ElementPtr ptr) override {}            
            void deAllocatePtr(__attribute__((unused)) ElementPtr ptr) override {}            
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
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->innerAdd(ptr);
                }

                std::lock_guard<std::mutex> setLock(m_mutex);

                nonOppositeAdd(ptr);
               
                // increase size, run policies, and opposite
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        std::shared_ptr<AbstractSet> front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        bool oppositeRan = false;
                        if (ptr.loaded()) {
                            if (!oppositeRan && front->oppositeEnabled()) {
                                front->oppositeAdd(*ptr);
                                oppositeRan = true;
                            }
                        }
                        for (std::shared_ptr<AbstractSet> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                if (!oppositeRan && front->oppositeEnabled()) {
                                    front->oppositeAdd(*ptr);
                                    oppositeRan = true;
                                }
                            }
                        }
                        if (!oppositeRan) {
                            for (std::shared_ptr<AbstractSet> superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }
            }
            void nonOppositeAdd(ElementPtr ptr) override {

                // look at supersets to allocate data
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->allocatePtr(ptr);
                        for (auto superSet : m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }

                // add data via policy
                DataTypePolicy::addData(ptr);

                // mark superSets that we have data
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    for (auto superSet : m_superSets) {
                        queue.push_back(superSet);
                    }
                    while (!queue.empty()) {
                        std::shared_ptr<AbstractSet> front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->m_subSetsWithData.insert(m_rootRedefinedSet);
                        if (!front->hasData()) {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }
                
                // increase size, run policies 
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        std::shared_ptr<AbstractSet> front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (ptr.loaded()) {
                            front->runAddPolicy(*ptr);
                        }
                        front->m_size++;
                        for (std::shared_ptr<AbstractSet> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                redefinedSet->runAddPolicy(*ptr);
                            }
                            redefinedSet->m_size++;
                        }
                        for (std::shared_ptr<AbstractSet> superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
            }
            void innerRemove(ElementPtr ptr) override {
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->innerRemove(ptr);
                }

                std::lock_guard<std::mutex> setLock(m_mutex);

                nonOppositeRemove(ptr);

                // run opposite
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        bool oppositeRan = false;
                        if (ptr.loaded()) {
                            if (!oppositeRan && front->oppositeEnabled()) {
                                front->oppositeRemove(*ptr);
                                oppositeRan = true;
                            }
                        }
                        for (std::shared_ptr<AbstractSet> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                if (!oppositeRan && front->oppositeEnabled()) {
                                    redefinedSet->oppositeRemove(*ptr);
                                    oppositeRan = true;
                                }
                            }
                        }
                        if (!oppositeRan) {
                            for (std::shared_ptr<AbstractSet> superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }
            }
            void weakRemove(UmlPtr<T> ptr) {
                if (contains(ptr)) {
                    innerRemove(ptr);
                }
            }
            void nonOppositeRemove(ElementPtr ptr) override {
                // look at supersets to deallocate data
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        front->deAllocatePtr(ptr);
                        for (auto superSet : m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
                
                // remove
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    queue.push_back(m_rootRedefinedSet);
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (front->containsData(ptr)) {
                            front->removeData(ptr);
                            if (!front->hasData()) {
                                // remove us from all super sets m_subSetsWithData unordered_set
                                std::list<std::shared_ptr<AbstractSet>> adjustQueue;
                                for (auto superSet : front->m_superSets) {
                                    adjustQueue.push_back(superSet);
                                }
                                while (!adjustQueue.empty()) {
                                    auto adjustFront = adjustQueue.front();
                                    adjustQueue.pop_front();
                                    adjustFront->m_subSetsWithData.erase(front);
                                    if (!adjustFront->hasData()) {
                                        for (auto adjustSuperSet : front->m_superSets) {
                                            adjustQueue.push_back(adjustSuperSet);
                                        }
                                    }
                                }
                            }
                        } else {
                            for (auto superSet : front->m_superSets) {
                                queue.push_back(superSet);
                            }
                        }
                    }
                }

                // run removal policies adjust size
                {
                    std::list<std::shared_ptr<AbstractSet>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    while (!queue.empty()) {
                        auto front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (ptr.loaded()) {
                            front->runRemovePolicy(*ptr);
                        }
                        front->m_size--;
                        for (std::shared_ptr<AbstractSet> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                redefinedSet->runRemovePolicy(*ptr);
                            }
                            redefinedSet->m_size--;
                        }
                        for (std::shared_ptr<AbstractSet> superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
            }
        public:
            PrivateSet(U* el) : m_el(*el) {}
            PrivateSet& operator=(PrivateSet& rhs) = delete;
            PrivateSet(PrivateSet& rhs) = delete;
            virtual ~PrivateSet() {}
            bool contains(ElementPtr ptr) const override {
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->contains(ptr);
                }
                if (containsData(ptr)) {
                    return true;
                }
                for (std::shared_ptr<AbstractSet> setWithData : m_subSetsWithData) {
                    if (setWithData->contains(ptr)) {
                        return true;
                    }
                }
                return false;
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
    };
}

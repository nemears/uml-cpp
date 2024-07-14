#pragma once

#include "abstractSet2.h"
#include <memory>
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
            bool skip = false;
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

    template <class T, class DataTypePolicy, class ApiPolicy, class U>
    class PrivateSet2 : public AbstractSet2 , public DataTypePolicy, protected ApiPolicy {
        protected:
            U& m_el;
            OppositeInterface<T> m_opposite = NoOpposite<T>();

            void runAddPolicy(Element& el) override {
                ApiPolicy::elementAdded(el.as<T>(), m_el);
            }
            void runRemovePolicy(Element& el) override {
                ApiPolicy::elementRemoved(el.as<T>(), m_el);
            }
            bool oppositeEnabled() override {
                return m_opposite.enabled();
            }
            void oppositeAdd(Element& el) override {
                m_opposite.addOpposite(el.as<T>());
            }
            void oppositeRemove(Element& el) override {
                m_opposite.removeOpposite(el.as<T>());
            }
            bool hasData() override {
                return DataTypePolicy::hasData();
            }
            bool containsData(UmlPtr<T> ptr) override {
                return DataTypePolicy::containsData(ptr);
            }
            bool removeData(UmlPtr<T> ptr) override {
                return DataTypePolicy::removeData(ptr);
            }
            void innerAdd(UmlPtr<T> ptr) override {
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->innerAdd(ptr);
                }
                if (contains(ptr.id())) {
                    throw SetStateException("Element already in set!");
                }

                // add data via policy
                DataTypePolicy::addData(ptr);

                // mark superSets that we have data
                {
                    std::list<std::shared_ptr<AbstractSet2>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet2>> visited;
                    for (auto superSet : m_superSets) {
                        queue.push_back(superSet);
                    }
                    while (!queue.empty()) {
                        std::shared_ptr<AbstractSet2> front = queue.front();
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
                
                // increase size, run policies, and opposite
                {
                    std::list<std::shared_ptr<AbstractSet2>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet2>> visited;
                    queue.push_back(m_rootRedefinedSet);
                    bool oppositeRan = false;
                    while (!queue.empty()) {
                        std::shared_ptr<AbstractSet2> front = queue.front();
                        queue.pop_front();
                        if (visited.count(front)) {
                            continue;
                        }
                        visited.insert(front);
                        if (ptr.loaded()) {
                            front->runAddPolicy(*ptr);
                            if (!oppositeRan && front->oppositeEnabled()) {
                                front->oppositeAdd(*ptr);
                                oppositeRan = true;
                            }
                        }
                        front->m_size++;
                        for (std::shared_ptr<AbstractSet2> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                front->runAddPolicy(*ptr);
                                if (!oppositeRan && front->oppositeEnabled()) {
                                    front->oppositeAdd(*ptr);
                                    oppositeRan = true;
                                }
                            }
                            front->m_size++;
                        }
                        for (std::shared_ptr<AbstractSet2> superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
            }
            void innerRemove(UmlPtr<T> ptr) override {
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->innerRemove(ptr);
                }

                // remove
                {
                    std::list<std::shared_ptr<AbstractSet2>> queue;
                    queue.push_back(m_rootRedefinedSet);
                    std::unordered_set<std::shared_ptr<AbstractSet2>> visited;
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
                                std::list<std::shared_ptr<AbstractSet2>> adjustQueue;
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

                // run removal policies adjust size and opposite
                {
                    std::list<std::shared_ptr<AbstractSet2>> queue;
                    std::unordered_set<std::shared_ptr<AbstractSet2>> visited;
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
                            front->runRemovePolicy(*ptr);
                            if (!oppositeRan && front->oppositeEnabled()) {
                                front->oppositeRemove(*ptr);
                                oppositeRan = true;
                            }
                        }
                        front->m_size--;
                        for (std::shared_ptr<AbstractSet2> redefinedSet : front->m_redefinedSets) {
                            if (ptr.loaded()) {
                                front->runRemovePolicy(*ptr);
                                if (!oppositeRan && front->oppositeEnabled()) {
                                    front->oppositeRemove(*ptr);
                                    oppositeRan = true;
                                }
                            }
                            front->m_size--;
                        }
                        for (std::shared_ptr<AbstractSet2> superSet : front->m_superSets) {
                            queue.push_back(superSet);
                        }
                    }
                }
            }
        public:
            PrivateSet2(U& el) : m_el(el) {}
            bool contains(UmlPtr<T> ptr) override {
                if (m_rootRedefinedSet.get() != this) {
                    return m_rootRedefinedSet->contains(ptr);
                }
                if (containsData(ptr)) {
                    return true;
                }
                for (std::shared_ptr<AbstractSet2> setWithData : m_subSetsWithData) {
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
                            (el.*signature)().innerAdd(me);
                        }
                        void removeOpposite(T& el) override {
                            (el.*signature)().innerRemove(me.getID());
                        }
                };
                m_opposite = OppositeInterfaceAdapter(m_el, oppositeSignature);
            }
    };
}

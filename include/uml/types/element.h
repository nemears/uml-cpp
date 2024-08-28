#pragma once

#include <algorithm>
#include <string>
#include <exception>
#include <memory>
#include <list>
#include <unordered_set>
#include "uml/id.h"
#include "uml/set/doNothingPolicy.h"

namespace YAML {
    class Node;
}

namespace UML {

    // Helper function to assess possible ids
    bool isValidID(std::string strn);

    class Element;
    class AbstractSet;
    template <class T, class U, class DataTypePolicy, class ApiPolicy = DoNothingPolicy>
    class PrivateSet;
    template <class T>
    class SetDataPolicy;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySet = PrivateSet<T, U, SetDataPolicy<T>, ApiPolicy>;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class Set;
    template<class T>
    class SingletonDataPolicy;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class Singleton;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySingleton = PrivateSet<T, U, SingletonDataPolicy<T>, ApiPolicy>;
    template <class T> class UmlPtr;
    typedef UmlPtr<Element> ElementPtr;
    class EmitterData;
    class ParserData;
    class AbstractManager;
    struct ManagerNode;
    class InstanceSpecification;
    class Comment;
    class Classifier; // TODO remove this is because of weird policy in Classifier members set
    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {

        friend class AbstractManager;
        friend struct ManagerNode;
//        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        template <class T, class U, class DataTypePolicy, class ApiPolicy> friend class PrivateSet;
        template <class T> friend class UmlPtr;
        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        template <class T, class U, class ApiPolicy>
        friend class Set;
        template <class T, class U, class ApiPolicy>
        friend class OrderedSet;
        friend std::string emit(Element& el, EmitterData& data);
        friend ElementPtr parse(std::string data, ParserData& metaData);
        friend bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
        friend class Classifier; // TODO remove
        friend class BehavioredClassifier;
        friend class Usage;
        friend class InterfaceRealization;

        private:
        protected:
            // struct AbstractTypeInfo {
            // 
            // };

            

            // struct TypeInfo : public AbstractTypeInfo {
            //     // struct TypeInfoNode {
            //     //     TypeInfo& m_typeInfo;
            //     //     TypeInfoNode(TypeInfo* typeInfo) : m_typeInfo(*typeInfo) {};
            //     // };
            //     // std::shared_ptr<TypeInfoNode> m_node = std::make_shared<TypeInfoNode>(this);
            //     // std::unordered_set<std::shared_ptr<TypeInfoNode>> m_base;
            //     // std::unordered_set<std::shared_ptr<TypeInfoNode>> m_derived;
            //     TypeInfo() {}
            //     // ~TypeInfo() {
            //     //     for (auto base : m_base) {
            //     //         auto baseDerivedIt = base->m_typeInfo.m_derived.begin();
            //     //         while (baseDerivedIt != base->m_typeInfo.m_derived.end() && *baseDerivedIt != m_node) {
            //     //             baseDerivedIt++;
            //     //         }
            //     //         if (baseDerivedIt == base->m_typeInfo.m_derived.end()) {
            //     //             // TODO error;
            //     //         }
            //     //         base->m_typeInfo.m_derived.erase(baseDerivedIt);
            //     //     }
            //     //     for (auto derived : m_derived) {
            //     //         auto derivedBaseIt = derived->m_typeInfo.m_base.begin();
            //     //         while (derivedBaseIt != derived->m_typeInfo.m_base.end() && *derivedBaseIt != m_node) {
            //     //             derivedBaseIt++;
            //     //         }
            //     //         if (derivedBaseIt == derived->m_typeInfo.m_base.end()) {
            //     //             // TODO error;
            //     //         }
            //     //         derived->m_typeInfo.m_base.erase(derivedBaseIt);
            //     //     }
            //     //     m_base.clear();
            //     //     m_derived.clear();
            //     // }
            //     // void setBase(TypeInfo& base) {
            //     //     m_base.insert(base.m_node);
            //     //     base.m_derived.insert(m_node);
            //     // } 
            // };
            // 
            // // type info for meta details
            // TypeInfo<"Element"> Info;
            // Info m_elementTypeInfo;
            // AbstractTypeInfo& m_typeInfo;

            const std::size_t m_elementType;

            // TODO replace these ptrs as references
            AbstractManager* m_manager = 0;
            ManagerNode* m_node = 0;

            // ID
            ID m_id;
            
            std::unique_ptr<ReadOnlySingleton<Element, Element>> m_owner;
            std::unique_ptr<ReadOnlySet<Element, Element>> m_ownedElements;
            std::unique_ptr<Set<Comment, Element, DoNothingPolicy>> m_ownedComments;
            std::unique_ptr<Set<InstanceSpecification, Element, DoNothingPolicy>> m_appliedStereotypes;
            ReadOnlySingleton<Element, Element>& getOwnerSingleton();
            void setOwner(ElementPtr el);
            virtual void restoreReferences();
            virtual void referenceErased(ID id);
            Element(std::size_t elementType);
            void eraseFromSet(ID id, AbstractSet& set);
        public:
            Element(const Element&) = delete;
            Element& operator=(const Element&) = delete;
            ID getID() const;
            ElementPtr getOwner() const;
            ReadOnlySet<Element, Element>& getOwnedElements();
            Set<Comment, Element, DoNothingPolicy>& getOwnedComments();
            /**
             * TODO: I am keeping it simple for now, instance specification of stereotype to
             *       hold tags and operations, but I think it would be cool to dynamically map
             *       methods if we load the stereotype before runtime. Also would be cool to have
             *       stereotype tags as keyword in yaml config for disk storage (not necessarily useful though?)
             **/
            Set<InstanceSpecification, Element, DoNothingPolicy>& getAppliedStereotypes();
            virtual void setID(std::string id);
            void setID(ID id);
            bool is(std::size_t type) const;
            std::size_t elementType() const {
                return m_elementType;
            }
            template <class T = Element> T& as() {
                T& ret = dynamic_cast<T&>(*this);
                // if (!ret) {
                //     throw InvalidElementCastException(getElementTypeString().c_str() , elementTypeToString(T::elementType()).c_str());
                // }
                return ret;
            }

            inline friend bool operator==(const Element& lhs, const Element& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const Element& lhs, const Element& rhs) {
                return lhs.m_id != rhs.m_id;
            };
        private:
            void setOwner(ID id);
    };

    //Exceptions
    class InvalidID_Exception: public std::exception {
        public:
        virtual const char* what() const throw() {
            return "String of id is not a valid UUID4";
        }
    };
}

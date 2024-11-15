#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include <vector>

namespace YAML {
    class Node;
}

namespace UML {

    template <class>
    class Comment;
    template <class>
    class Dependency;
    template <class>
    class Element;
    template <class>
    class ElementImport;
    template <class>
    class InstanceSpecification;
    template <class>
    class NamedElement;
    template <class>
    class Namespace;
    template <class>
    class Package;
    template <class>
    class PackageableElement;
    template <class>
    class PackageImport;
    template <class>
    class PackageMerge;
    template <class>
    class ParameterableElement;
    template <class>
    class ProfileApplication;
    template <class>
    class Stereotype;

    using UmlTypes = TemplateTypeList<
        Comment,
        Dependency,
        Element,
        ElementImport,
        InstanceSpecification,
        NamedElement,
        Namespace,
        Package,
        PackageableElement,
        PackageImport,
        PackageMerge,
        ParameterableElement,
        ProfileApplication,
        Stereotype
    >;
    // typedef UmlPtr<Element> ElementPtr;

    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    template <class ManagerPolicy>
    class Element : public ManagerPolicy {

        friend struct ElementInfo<Element>;

        protected:
            ReadOnlySingleton<Element, Element> m_owner = ReadOnlySingleton<Element, Element>(this);
            ReadOnlySet<Element, Element> m_ownedElements = ReadOnlySet<Element, Element>(this);
            // Set<Comment, Element> m_ownedComments = Set<Comment, Element>(this);
            // Set<InstanceSpecification, Element> m_appliedStereotypes = Set<InstanceSpecification, Element>(this);
            ReadOnlySingleton<Element, Element>& getOwnerSingleton() {
                return m_owner;
            }
            Element(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_owner.opposite(&Element::getOwnedElements);
                m_owner.setComposition(CompositionType::ANTI_COMPOSITE);
                m_ownedElements.opposite(&Element::getOwnerSingleton);
                m_ownedElements.setComposition(CompositionType::COMPOSITE);
                // m_ownedComments.subsets(m_ownedElements);
            }
        public:
            UmlPtr<Element> getOwner() const {
                return m_owner.get();
            }
            ReadOnlySet<Element, Element>& getOwnedElements() {
                return m_ownedElements;
            }
            // Set<Comment, Element, DoNothingPolicy>& getOwnedComments() {
            //     return m_ownedComments;
            // }
            /**
             * TODO: I am keeping it simple for now, instance specification of stereotype to
             *       hold tags and operations, but I think it would be cool to dynamically map
             *       methods if we load the stereotype before runtime. Also would be cool to have
             *       stereotype tags as keyword in yaml config for disk storage (not necessarily useful though?)
             **/
            // Set<InstanceSpecification, Element, DoNothingPolicy>& getAppliedStereotypes() {
            //     return m_appliedStereotypes;
            // }

            typedef TypeInfo<TemplateTypeList<>, Element> Info;
        
        private:
            void setOwner(ID id) {
                m_owner.nonOppositeAdd(ManagerPolicy::m_manager.createPtr(id));
            }
    };

    inline std::pair<std::string, AbstractSet*> makeSetPair(const char* name, AbstractSet& set) {
        return std::make_pair<std::string, AbstractSet*>(name, &set);
    }

    // template specialization for ElementInfo<Element> (required for all implementing types)
    template <class ManagerType>
    struct ElementInfo<Element<ManagerType>> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Element"; }
        static SetList sets(Element<ManagerType>& el) {
            return std::vector {
                makeSetPair("owner", el.m_owner),
                makeSetPair("ownedElements", el.m_ownedElements)
                // makeSetPair("ownedComments", el.m_ownedComments),
                // makeSetPair("appliedStereotypes", el.m_appliedStereotypes)
            };
        }
    };
}

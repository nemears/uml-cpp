#pragma once

#include "association.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"

namespace UML {

    class ExtensionEnd;
    class Class;
    typedef UmlPtr<ExtensionEnd> ExtensionEndPtr;
    typedef UmlPtr<Class> ClassPtr;

    class Extension : public Association {

        friend class ExtensionEnd;
        friend class Property;
        friend struct ElementInfo<Extension>;

        public:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            class MemberEndPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(Property& el, Extension& me);
                    void elementRemoved(Property& el, Extension& me);
            };
            IndexableOrderedSet<Property, Extension, MemberEndPolicy> m_extensionMemberEnds = 
                    IndexableOrderedSet<Property, Extension, MemberEndPolicy>(this);
            Singleton<ExtensionEnd, Extension> m_ownedEnd = Singleton<ExtensionEnd, Extension>(this);
            ReadOnlySingleton<Class, Extension> m_metaClass = ReadOnlySingleton<Class, Extension>(this);
            bool m_setFlag = false;
            Singleton<ExtensionEnd, Extension>& getOwnedEndSingleton();
        public:
            Extension(std::size_t elementType, AbstractManager& manager);
            ExtensionEndPtr getOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEndPtr end);
            void setOwnedEnd(ID id);
            ClassPtr getMetaClass() const;
            typedef TypeInfo<std::tuple<Association>, Extension> Info;
    };

    template <>
    struct ElementInfo<Extension> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"Extension"};
        static SetList sets(Extension& el) {
            return SetList {
                makeSetPair("memberEnds", el.m_extensionMemberEnds),
                makeSetPair("ownedEnd", el.m_ownedEnd),
                makeSetPair("metaClass", el.m_metaClass)
            };
        }
    };
}

#pragma once

#include "association.h"
#include "uml/set/indexableSet.h"

namespace UML {

    class ExtensionEnd;
    class Class;
    typedef UmlPtr<ExtensionEnd> ExtensionEndPtr;
    typedef UmlPtr<Class> ClassPtr;

    class Extension : public Association {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class ExtensionEnd;
        friend class Property;

        private:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            // ElementType m_metaClass = ElementType::NOT_SET;
            struct MemberEndPolicy : public IndexablePolicy {
                void elementAdded(Property& el, Extension& me);
                void elementRemoved(Property& el, Extension& me);
            };
            IndexableOrderedSet<Property, Extension, MemberEndPolicy> m_extensionMemberEnds = 
                    IndexableOrderedSet<Property, Extension, MemberEndPolicy>(this);
            Singleton<ExtensionEnd, Extension> m_ownedEnd = Singleton<ExtensionEnd, Extension>(this);
            Singleton<Class, Extension> m_metaClass = Singleton<Class, Extension>(this);
            bool m_setFlag = false;
            Singleton<ExtensionEnd, Extension>& getOwnedEndSingleton();
            void restoreReferences() override;
            Extension();
        public:
            virtual ~Extension();
            // void setMetaClass(ElementType metaClass);
            // ElementType getMetaClass();
            ExtensionEndPtr getOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEndPtr end);
            void setOwnedEnd(ID id);
            ClassPtr getMetaClass() const;
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

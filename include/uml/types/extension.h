#ifndef _UML_EXTENSION_H_
#define _UML_EXTENSION_H_

#include "association.h"

namespace UML {

    class ExtensionEnd;
    typedef UmlPtr<ExtensionEnd> ExtensionEndPtr;

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
            class ExtensionAddMemberEndPolicy {
                public:
                    void apply(Property& el, Extension& me);
            };
            class ExtensionRemoveMemberEndPolicy {
                public:
                    void apply(Property& el, Extension& me);
            };
            CustomOrderedSet<Property, Extension, ExtensionAddMemberEndPolicy, ExtensionRemoveMemberEndPolicy> m_extensionMemberEnds = 
                    CustomOrderedSet<Property, Extension, ExtensionAddMemberEndPolicy, ExtensionRemoveMemberEndPolicy>(this);
            CustomSingleton<ExtensionEnd, Extension> m_ownedEnd = CustomSingleton<ExtensionEnd, Extension>(this);
            CustomSingleton<Class, Extension> m_metaClass = CustomSingleton<Class, Extension>(this);
            bool m_setFlag = false;
            TypedSet<ExtensionEnd, Extension>& getOwnedEndSingleton();
            void restoreReferences() override;
            Extension();
        public:
            virtual ~Extension();
            // void setMetaClass(ElementType metaClass);
            // ElementType getMetaClass();
            ExtensionEndPtr getOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEnd* end);
            void setOwnedEnd(ID id);
            ClassPtr getMetaClass() const;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif
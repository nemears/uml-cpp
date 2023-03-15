#ifndef _UML_EXTENSION_H_
#define _UML_EXTENSION_H_

#include "association.h"

namespace UML {

    class ExtensionEnd;
    typedef UmlPtr<ExtensionEnd> ExtensionEndPtr;

    class Extension : public Association {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class ExtensionEnd;

        private:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            ElementType m_metaClass = ElementType::NOT_SET;
            CustomSingleton<ExtensionEnd, Extension> m_ownedEnd = CustomSingleton<ExtensionEnd, Extension>(this);
            bool m_setFlag = false;
            TypedSet<ExtensionEnd, Extension>& getOwnedEndSingleton();
            Extension();
        public:
            virtual ~Extension();
            void setMetaClass(ElementType metaClass);
            ElementType getMetaClass();
            ExtensionEndPtr getOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEnd* end);
            void setOwnedEnd(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif